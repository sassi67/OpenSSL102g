/**************************************************************************
** Qt Creator license header template
**   Special keywords: alessandro 11.03.2016 2016
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/
#include <QCoreApplication>

#include <openssl/sha.h>
#include <openssl/aes.h>

#include <stdio.h>
/* AES key for Encryption and Decryption */
//const static unsigned char aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};

/* Print Encrypted and Decrypted data packets */
void print_data(const char *title, const void* data, int len);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 1. key encryption
    QByteArray hashKey;
    unsigned char digest[SHA512_DIGEST_LENGTH];
    {
        const char* s = "Giovanni da Udine";

        SHA512_CTX ctx;
        SHA512_Init(&ctx);
        SHA512_Update(&ctx, s, strlen(s));
        SHA512_Final(digest, &ctx);

//        hashKey = QByteArray::fromRawData(reinterpret_cast<const char*>(digest), static_cast<int>(sizeof(digest)));
        hashKey.clear();
        for (size_t i = 0; i < sizeof(digest); ++i)
            hashKey.append(digest[i]);
    }

    // 2. encryption
    QByteArray encResult;
    QByteArray arr("Ma andate a cagare!!123456789012y");
    unsigned char aes_input[arr.size()];
    {
        /* Input data to encrypt */
        for (int i = 0; i < arr.size(); ++i)
            aes_input[i] = arr.at(i);

        unsigned char digest[SHA512_DIGEST_LENGTH];
        for (int i = 0; i < hashKey.size(); ++i)
            digest[i] = hashKey.at(i);


        /* Init vector */
        unsigned char iv[AES_BLOCK_SIZE];
        memset(iv, 0x00, AES_BLOCK_SIZE);

        /* Buffer for Encryption */
        unsigned char enc_out[sizeof(aes_input)];

        /* AES-128 bit CBC Encryption */
        AES_KEY enc_key;
        AES_set_encrypt_key(digest, sizeof(digest)*2, &enc_key);
        AES_cbc_encrypt(aes_input, enc_out, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);

//        encResult = QByteArray::fromRawData(reinterpret_cast<const char*>(enc_out), static_cast<int>(sizeof(aes_input)));
        encResult.clear();
        for (size_t i = 0; i < sizeof(enc_out); ++i)
            encResult.append(enc_out[i]);
    }

    // 3. decryption
    QByteArray decResult;
    unsigned char enc_out[encResult.size()];
    unsigned char dec_out[encResult.size()];
    {
        unsigned char digest[SHA512_DIGEST_LENGTH];
        for (int i = 0; i < hashKey.size(); ++i)
            digest[i] = hashKey.at(i);


        /* Init vector */
        unsigned char iv[AES_BLOCK_SIZE];
        memset(iv, 0x00, AES_BLOCK_SIZE);

        /* Buffer for decryption */
        for(int i = 0; i < encResult.size(); ++i)
            enc_out[i] = encResult.at(i);

        /* AES-128 bit CBC Encryption */
        AES_KEY dec_key;
        AES_set_decrypt_key(digest, sizeof(digest)*2, &dec_key); // Size of key is in bits
        AES_cbc_encrypt(enc_out, dec_out, sizeof(dec_out), &dec_key, iv, AES_DECRYPT);

//        decResult = QByteArray::fromRawData(reinterpret_cast<const char*>(dec_out), static_cast<int>(sizeof(dec_out)));
        decResult.clear();
        for (size_t i = 0; i < sizeof(dec_out); ++i)
            decResult.append(dec_out[i]);
    }

    /* Printing and Verifying */
    print_data("\n Key ", digest, sizeof(digest)); // you can not print data as a string, because after Encryption its not ASCII

    print_data("\n Original ",aes_input, sizeof(aes_input)); // you can not print data as a string, because after Encryption its not ASCII

    print_data("\n Encrypted",enc_out, sizeof(enc_out));

    print_data("\n Decrypted",dec_out, sizeof(dec_out));
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
