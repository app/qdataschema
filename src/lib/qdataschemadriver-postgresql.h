/****************************************************************************
** $Id: qdataschemadriver-postgresql.h,v 1.7 2009/06/11 14:59:25 app Exp $
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

#ifndef QDATASCHEMADRIVER_POSTGRESQL_H
#define QDATASCHEMADRIVER_POSTGRESQL_H
#include <qobject.h>
#include "qdataschemaglobal.h"
#include "qdataschemadriver.h"
//#include <qdict.h>
//#include "config.h"
//class QSqlCursor;

/*!
\~english

\~russian

\~
*/
class QDataSchemaDriverPostgreSQL: public QDataSchemaDriver
{
Q_OBJECT

public:
    QDataSchemaDriverPostgreSQL();
    ~QDataSchemaDriverPostgreSQL();

    QString sqlDriverName();
    QString fieldtype( const QString &qdstype,
            int width, int decimals = 0,
            bool notnull = false, bool primarykey = false, bool serial = false );

    int fieldWidth( QSqlFieldInfo *ftypedef );
    int fieldDecimals( QSqlFieldInfo *ftypedef );
    QString defaultDatabaseName(){ return "template1";};

protected:
    QString queryCreateDatabase( const QString &name );
    QString queryAlterTableModifyField( const QString &tname, const QString &field, const QString &oldfield = "" );
    QString queryDropIndex( const QString &tname, const QString &iname );
};

#endif
