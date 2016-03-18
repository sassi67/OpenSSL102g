/**************************************************************************
**    CFDEM Workbench
**
**    File: hashsha512.cpp
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#include "hashsha512.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
//#include <openssl/applink.c>
#include <string.h>

#include <QTextStream>

void hashInitializeLib();
void hashCleanUp();
void hashHandleErrors();
void createHashKey(const char *message, unsigned char **digest, unsigned int *digest_len);

/**
 * @author Alessandro Sacilotto
 * @brief HashSHA512::HashSHA512
 *
 * Constructor
 */
HashSHA512::HashSHA512() :
    HasherBase("sha512")
{
    hashInitializeLib();
}
/**
 * @author Alessandro Sacilotto
 * @brief HashSHA512::~HashSHA512
 *
 * Destructor
 */
HashSHA512::~HashSHA512()
{
    hashCleanUp();
}
/**
 * @author Alessandro Sacilotto
 * @brief HashSHA512::getHashKey
 * @param keyToHash
 * @return a QByteArray representing the hashed key
 */
QByteArray HashSHA512::getHashKey(const QString &keyToHash)
{
    if (keyToHash.isEmpty() || keyToHash.isNull()) {
        QTextStream out(stderr);
        out << "HashSHA512: invalid string to hash!" << endl;
        return QByteArray();
    }
    unsigned char *key = (unsigned char *)(keyToHash.toStdString().c_str()) ;
    unsigned char digest[SHA512_DIGEST_LENGTH];
    unsigned int len_digest = 0;
    createHashKey((const char*)key, (unsigned char**)digest, &len_digest);

    QByteArray hashKey;
    for (unsigned int i = 0; i < len_digest; ++i) {
        hashKey.append(digest[i]);
    }

    return hashKey;
}
/**
 * @brief initializeLib
 */
void hashInitializeLib()
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
}
/**
 * @brief cleanUp
 */
void hashCleanUp()
{
    EVP_cleanup();
    ERR_free_strings();
}
/**
 * @brief handleErrors
 */
void hashHandleErrors()
{
    ERR_print_errors_fp(stderr);
    exit(1);
}
/**
 * @brief createHashKey
 * @param message
 * @param digest
 * @param digest_len
 */
void createHashKey(const char *message, unsigned char **digest, unsigned int *digest_len)
{
    EVP_MD_CTX *mdctx;

    if ((mdctx = EVP_MD_CTX_create()) == NULL)
        hashHandleErrors();

    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha512(), NULL))
        hashHandleErrors();

    if (1 != EVP_DigestUpdate(mdctx, message, strlen(message)))
        hashHandleErrors();

    if ((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha512()))) == NULL)
        hashHandleErrors();

    if (1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
        hashHandleErrors();

    EVP_MD_CTX_destroy(mdctx);
}
