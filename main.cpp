/**************************************************************************
** Qt Creator license header template
**   Special keywords: alessandro 11.03.2016 2016
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/
#include <QCoreApplication>
#include <stdio.h>
#include <memory>

#include "hashsha512.h"
#include "encryptoraes256.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray hashKey;
    {
        QString key(argv[1]);
        std::auto_ptr<HasherBase> hasher(new HashSHA512());
        hashKey = hasher->getHashKey(key);
        if (hashKey.isEmpty()) {
            printf("Error while hashing the key.\n");
            exit(1);
        }
    }
    QByteArray plainText(argv[2]);
    QByteArray encodedContent;
    {
        // create the encryptor based on generated hash key
        std::auto_ptr<EncryptorBase> encrypt(new EncryptorAES256(hashKey));
        encodedContent = encrypt->encode(plainText);
    }
    QByteArray decodedContent;
    {
        // create the encryptor based on generated hash key
        std::auto_ptr<EncryptorBase> encrypt(new EncryptorAES256(hashKey));
        decodedContent = encrypt->decode(encodedContent);
    }
    if (decodedContent != plainText) {
        printf("Error while hashing the key.\n");
        exit(1);
    } else {
        printf("OK!!\n");
    }
    return a.exec();
}
