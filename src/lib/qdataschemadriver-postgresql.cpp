/****************************************************************************
** $Id: qdataschemadriver-postgresql.cpp,v 1.9 2009/06/11 14:59:25 app Exp $
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
#include "qdataschemadriver-postgresql.h"

/*!
 * \class QDataSchemaDriverPostgreSQL qdataschemadriver-postgresql.h
 * \en
 * \brief Database schema PostgreSQL driver object.
 *
 * \_en \ru
 * \brief Класс для работы с сервером PostgreSQL.
 *
 * Используется для работы с сервером PostgreSQL.
 * \_ru
 */
QDataSchemaDriverPostgreSQL::QDataSchemaDriverPostgreSQL() : QDataSchemaDriver()
{
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QDataSchemaDriverPostgreSQL::~QDataSchemaDriverPostgreSQL()
{
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriverPostgreSQL::sqlDriverName()
{
    return "QPSQL7";
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriverPostgreSQL::fieldtype( const QString &qdstype,
        int width, int decimals,
        bool notnull, bool primarykey, bool serial )
{
    QString dt="";
    char t=' ';

    t = toAscii( toLower( qdstype[0] ) );
    switch ( t ) {
    case 'i':
        if ( serial ) dt = " serial";
        else dt = QString("integer");
        break;
    case 'l':
        if ( serial ) dt = " bigserial";
        else dt = "bigint";
        break;
    case 'c':
        dt = QString("character(%1)").arg( width );
        break;
    case 's':
        dt = QString("character varying(%1)").arg( width );
        break;
    case 'n':
        dt = QString("numeric(%1,%2)").arg( width ).arg( decimals );
        break;
    case 'd':
        dt = QString("timestamp");
        break;
    case 'o':
        dt = QString("bigint");
        break;
    default:
        dt = "";
        break;
    }
    if ( notnull ) dt.append(" not null" );
    if ( primarykey ) dt.append(" PRIMARY KEY" );
    return dt;
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
int
QDataSchemaDriverPostgreSQL::fieldWidth( QSqlFieldInfo *ftypedef )
{
    return ( ftypedef->length() & 0xFF0000 ) >> 16;
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
int
QDataSchemaDriverPostgreSQL::fieldDecimals( QSqlFieldInfo *ftypedef )
{
    return ftypedef->length() & 0xFF;
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriverPostgreSQL::queryCreateDatabase( const QString &name )
{
    return QString("create database %1 with encoding='UTF-8'").arg( name );
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriverPostgreSQL::queryAlterTableModifyField( const QString &tname, const QString &field, const QString &/*oldfield*/ )
{
    QString query;
    query.append( queryAlterTableAddField( tname, "__"+field ) );
    query.append( QString("update %1 set __%2=%3\n").arg( tname ).arg( field ).arg( field ) );
    query.append( queryAlterTableDropField( tname, field ) );
    query.append( QString("alter table %1 rename column __%2 to %3\n").arg( tname ).arg( field ).arg( field ) );
    return query;
}


/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriverPostgreSQL::queryDropIndex( const QString &/*tname*/, const QString &iname )
{
    return QString("drop index %1").arg( iname );
}
