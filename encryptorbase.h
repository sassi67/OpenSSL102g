/**************************************************************************
**    CFDEM Workbench
**
**    File: encryptorbase.h
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#ifndef ENCRYPTORBASE_H
#define ENCRYPTORBASE_H

#include <QByteArray>
#include <QString>

class EncryptorBase
{
public:
    EncryptorBase(const QString& algoName, const QByteArray& hashKey);
    virtual ~EncryptorBase() {}
    virtual QByteArray encode(const QByteArray& content) = 0;
    virtual QByteArray decode(const QByteArray& content) = 0;
protected:

    QString     algoName_;
    QByteArray  hashKey_;

private:
    //Pre C++2011 hide by making private and don't implement
    explicit EncryptorBase(const EncryptorBase& rhs);
    EncryptorBase& operator= (const EncryptorBase& rhs);

};
#endif // ENCRYPTORBASE_H



