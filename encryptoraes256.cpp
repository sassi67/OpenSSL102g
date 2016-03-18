/**************************************************************************
**    CFDEM Workbench
**
**    File: encryptoraes256.cpp
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#include "encryptoraes256.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
//#include <openssl/applink.c>
#include <string.h>

#include <QTextStream>

void encryptInitializeLib();
void encryptCleanUp();
void encryptHandleErrors();
int encryption(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);
int decryption(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

/**
 * @author Alessandro Sacilotto
 * @brief EncryptorAES256::EncryptorAES256
 * @param hashKey
 *
 * Constructor
 */
EncryptorAES256::EncryptorAES256(const QByteArray &hashKey) :
    EncryptorBase("aes256", hashKey)
{
    encryptInitializeLib();
}
/**
 * @author Alessandro Sacilotto
 * @brief EncryptorAES256::~EncryptorAES256
 *
 * Destructor
 */
EncryptorAES256::~EncryptorAES256()
{
    encryptCleanUp();
}
/**
 * @author Alessandro Sacilotto
 * @brief EncryptorAES256::encode
 * @param content
 * @return a QByteArray which represents the encoded content
 */
QByteArray EncryptorAES256::encode(const QByteArray &content)
{
    if (content.isEmpty() || content.isNull())
        return QByteArray();

    // get the key (hashed)
    unsigned char key_hash[SHA512_DIGEST_LENGTH];

    for (int i = 0; i < hashKey_.size(); ++i) {
        key_hash[i] = hashKey_.at(i);
    }

    unsigned char *plaintext = (unsigned char *)malloc(content.size() * 2 + 1);
    for (int i = 0; i < content.size(); ++i) {
        plaintext[i] = content.at(i);
    }
    unsigned char *ciphertext = (unsigned char *)malloc(content.size() * 2 + 1);
    //TODO: a 128 bit IV
    unsigned char *iv = (unsigned char *)"0000000000000000";
    // encryption
    int ciphertext_len = encryption(plaintext, content.size(),
                                    key_hash, iv,
                                    ciphertext);
    QByteArray ret;
    for (int i = 0; i < ciphertext_len; ++i) {
        ret.append(ciphertext[i]);
    }

    free(plaintext);
    free(ciphertext);

    return ret;
}
/**
 * @author Alessandro Sacilotto
 * @brief EncryptorAES256::decode
 * @param content
 * @return a QByteArray which represents the decoded content
 */
QByteArray EncryptorAES256::decode(const QByteArray &content)
{
    if (content.isEmpty() || content.isNull())
        return QByteArray();

    // get the key (hashed)
    unsigned char key_hash[SHA512_DIGEST_LENGTH];

    for (int i = 0; i < hashKey_.size(); ++i) {
        key_hash[i] = hashKey_.at(i);
    }

    unsigned char *ciphertext = (unsigned char *)malloc(content.size() * 2 + 1);
    for (int i = 0; i < content.size(); ++i) {
        ciphertext[i] = content.at(i);
    }

    unsigned char *decryptedtext = (unsigned char *)malloc(content.size() * 2 + 1);
    //TODO: a 128 bit IV
    unsigned char *iv = (unsigned char *)"0000000000000000";
    int decryptedtext_len = decryption(ciphertext, content.size(),
                                       key_hash, iv,
                                       decryptedtext);

    QByteArray ret;
    for (int i = 0; i < decryptedtext_len; ++i) {
        ret.append(decryptedtext[i]);
    }

    free(ciphertext);
    free(decryptedtext);

    return ret;
}
/**
 * @brief initializeLib
 */
void encryptInitializeLib()
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
}
/**
 * @brief cleanUp
 */
void encryptCleanUp()
{
    EVP_cleanup();
    ERR_free_strings();
}
/**
 * @brief handleErrors
 */
void encryptHandleErrors()
{
    ERR_print_errors_fp(stderr);
    exit(1);
}
/**
 * @brief encrypt
 * @param plaintext
 * @param plaintext_len
 * @param key
 * @param iv
 * @param ciphertext
 * @return
 */
int encryption(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // create and initialise the context
    if (!(ctx = EVP_CIPHER_CTX_new()))
        encryptHandleErrors();

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        encryptHandleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        encryptHandleErrors();
    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) encryptHandleErrors();
        ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}
/**
 * @brief decrypt
 * @param ciphertext
 * @param ciphertext_len
 * @param key
 * @param iv
 * @param plaintext
 * @return
 */
int decryption(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        encryptHandleErrors();

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        encryptHandleErrors();

    /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        encryptHandleErrors();
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) encryptHandleErrors();
        plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
