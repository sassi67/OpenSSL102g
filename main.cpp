/**************************************************************************
** Qt Creator license header template
**   Special keywords: alessandro 11.03.2016 2016
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/
#include <QCoreApplication>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/applink.c>

#include <string.h>
#include <stdio.h>

/* Print Encrypted and Decrypted data packets */
void print_data(const char *title, const void* data, int len);

void handleErrors(void);
void createHashKey(const char *message, unsigned char **digest, unsigned int *digest_len);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    /* Initialise the library */
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);

    /* Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */

    /* A 256 bit key */
    // 1. key encryption
    QByteArray hashKey;
    {
        unsigned char *key = (unsigned char *)argv[1];
        unsigned char digest[SHA512_DIGEST_LENGTH];
        unsigned int len_digest = 0;
        createHashKey((const char*)key, (unsigned char**)digest, &len_digest);

        for (unsigned int i = 0; i < len_digest; ++i) {
            hashKey.append(digest[i]);
        }
    }
    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0000000000000000";

    /* Message to be encrypted */
    unsigned char *plaintext = (unsigned char *)argv[2];
//    printf("%d\n", strlen((char *)plaintext));
    /* Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, dependant on the
     * algorithm and mode
     */
//    unsigned char ciphertext[128];
    unsigned char *ciphertext = (unsigned char *)malloc(strlen((char *)plaintext));

    /* Buffer for the decrypted text */
//    unsigned char decryptedtext[128];
    unsigned char *decryptedtext = (unsigned char *)malloc(strlen((char *)plaintext) + 1);

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
    {
        unsigned char key_hash[SHA512_DIGEST_LENGTH];

        for (int i = 0; i < hashKey.size(); ++i) {
            key_hash[i] = hashKey.at(i);
        }
        ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key_hash, iv,
                              ciphertext);
    }
    /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    {
        unsigned char key_hash[SHA512_DIGEST_LENGTH];

        for (int i = 0; i < hashKey.size(); ++i) {
            key_hash[i] = hashKey.at(i);
        }

        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key_hash, iv,
                                decryptedtext);
    }
    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    /* Show the decrypted text */
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);


    /* Clean up */
    free(ciphertext);
    free(decryptedtext);
    EVP_cleanup();
    ERR_free_strings();
    return a.exec();
}

void print_data(const char *title, const void* data, int len)
{
    printf("%s : ",title);
    const unsigned char * p = (const unsigned char*)data;
    int i = 0;

    for (; i<len; ++i)
        printf("%02X ", *p++);

    printf("\n");
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    exit(1);
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void createHashKey(const char *message, unsigned char **digest, unsigned int *digest_len)
{
    EVP_MD_CTX *mdctx;

    if((mdctx = EVP_MD_CTX_create()) == NULL)
        handleErrors();

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha512(), NULL))
        handleErrors();

    if(1 != EVP_DigestUpdate(mdctx, message, strlen(message)))
        handleErrors();

    if((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha512()))) == NULL)
        handleErrors();

    if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
        handleErrors();

    EVP_MD_CTX_destroy(mdctx);
}
