/**************************************************************************
**    CFDEM Workbench
**
**    File: hashsha512.h
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#ifndef HASHSHA512_H
#define HASHSHA512_H

#include "hasherbase.h"

class HashSHA512 : public HasherBase
{
public:
    HashSHA512();
    virtual ~HashSHA512();
    virtual QByteArray getHashKey(const QString& keyToHash);

protected:

private:
    //Pre C++2011 hide by making private and don't implement
    explicit HashSHA512(const HashSHA512& rhs);
    HashSHA512& operator= (const HashSHA512& rhs);
};
#endif // HASHSHA512_H



