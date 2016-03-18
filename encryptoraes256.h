/**************************************************************************
**    CFDEM Workbench
**
**    File: encryptoraes256.h
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#ifndef ENCRYPTORAES256_H
#define ENCRYPTORAES256_H

#include "encryptorbase.h"

class EncryptorAES256 : public EncryptorBase
{
public:
    EncryptorAES256(const QByteArray &hashKey);
    virtual ~EncryptorAES256();
    virtual QByteArray encode(const QByteArray &content);
    virtual QByteArray decode(const QByteArray& content);
protected:

private:
    //Pre C++2011 hide by making private and don't implement
    explicit EncryptorAES256(const EncryptorAES256& rhs);
    EncryptorAES256& operator= (const EncryptorAES256& rhs);
};
#endif // ENCRYPTORAES256_H



