/****************************************************************************
** $Id: qdataschemadriver.h,v 1.13 2009/06/11 14:59:25 app Exp $
**
** Header file of the QDataSchemaDriver
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
**
** This file is part of the Library of the Ananas
** automation accounting system.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/page=ananas or email sales@leaderit.ru
** See http://www.leaderit.ru/gpl/ for GPL licensing information.
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QDATASCHEMADRIVER_H
#define QDATASCHEMADRIVER_H
#include "qdataschemaglobal.h"

class QDataSchemaDriver: public QObject
{
Q_OBJECT

public:
    QDataSchemaDriver();
    virtual QString sqlDriverName();
    virtual QString fieldtype( const QString &qdstype,
            int width, int decimals = 0,
            bool notnull = false, bool primarykey = false, bool serial = false );
    virtual QString systemTables(){ return "";};
    virtual QString fieldSQLToDS( QSqlFieldInfo *ftypedef );
    virtual QString fieldName( QSqlFieldInfo *ftypedef );
    virtual int fieldWidth( QSqlFieldInfo *ftypedef );
    virtual int fieldDecimals( QSqlFieldInfo *ftypedef );
    virtual QString defaultDatabaseName(){ return "";};
    // SQL queries generate functions
    virtual QString queryInitConnection( bool unicode );
    virtual QString queryCreateDatabase( const QString &name );
    virtual QString queryDropDatabase( const QString &name );
    virtual QString queryCreateTable( const QString &tname, const QString &fields );
    virtual QString queryDropTable( const QString &tname );
    virtual QString queryCreateIndex( const QString &tname, const QString &iname, const QString &fields );
    virtual QString queryDropIndex( const QString &tname, const QString &iname );
    virtual QString queryAlterTable( const QString &tname,
                                     const QString &fieldsnew, const QString &fieldsold,
                                     const QString &fieldsadd,const QString &fieldsdrop,
                                     const QString &fieldsmodifynew,const QString &fieldsmodifyold );
protected:
    virtual QString queryAlterTableDropField( const QString &tname, const QString &field );
    virtual QString queryAlterTableAddField( const QString &tname, const QString &field );
    virtual QString queryAlterTableModifyField( const QString &tname, const QString &field, const QString &oldfield = "" );
};

#endif
