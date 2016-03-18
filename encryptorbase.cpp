/**************************************************************************
**    CFDEM Workbench
**
**    File: encryptorbase.cpp
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#include "encryptorbase.h"
#include <QTextStream>

/**
 * @author Alessandro Sacilotto
 * @brief EncryptorBase::EncryptorBase
 * @param algoName
 * @param hashKey
 */
EncryptorBase::EncryptorBase(const QString &algoName, const QByteArray &hashKey):
    algoName_(algoName),
    hashKey_(hashKey)
{
    QTextStream out(stdout);

    if (algoName.isEmpty())
    {
        out << "EncryptorBase: invalid hashing algorithm name!" << endl;
        exit(1);
    }
    if (hashKey.isEmpty())
    {
        out << "EncryptorBase: invalid hashing key!" << endl;
        exit(1);
    }

}

