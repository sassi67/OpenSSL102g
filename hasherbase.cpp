/**************************************************************************
**    CFDEM Workbench
**
**    File: hasherbase.cpp
**    Author: Alessandro Sacilotto
**    Created on: 2015-08-12
**
**    Copyright (C) 2015 DCS Computing GmbH
**************************************************************************/

#include "hasherbase.h"
#include <QTextStream>

/**
 * @author Alessandro Sacilotto
 * @brief HasherBase::HasherBase
 * @param algoName
 *
 * Check whether the required hashing algorithm exists
 */
HasherBase::HasherBase(const QString &algoName)
{
    if (algoName.isEmpty())
    {
        QTextStream out(stdout);
        out << "HasherBase: invalid hashing algorithm name!" << endl;
        exit(1);
    }
}

