/**************************************************************************
**    CFDEM Workbench
**
**    File: hasherbase.h
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#ifndef HASHERBASE_H
#define HASHERBASE_H

#include <QByteArray>
#include <QString>

class HasherBase
{
public:
    HasherBase(const QString& algoName);
    virtual ~HasherBase() {}
    virtual QByteArray getHashKey(const QString& key) = 0;

protected:

private:
    //Pre C++2011 hide by making private and don't implement
    explicit HasherBase(const HasherBase& rhs);
    HasherBase& operator= (const HasherBase& rhs);
};
#endif // HASHERBASE_H



