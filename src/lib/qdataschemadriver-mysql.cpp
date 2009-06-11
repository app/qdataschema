/****************************************************************************
** $Id: qdataschemadriver-mysql.cpp,v 1.8 2009/06/11 14:59:25 app Exp $
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
#include "qdataschemadriver-mysql.h"

/*!
 * \class QDataSchemaDriverMySQL qdataschemadriver-mysql.h
 * \en
 * \brief Database schema MySQL drive object.
 *
 * \_en \ru
 * \brief Класс для работы с сервером MySQL.
 *
 * Используется для работы с сервером MySQL.
 * \_ru
 */

QDataSchemaDriverMySQL::QDataSchemaDriverMySQL() : QDataSchemaDriver()
{
}



/*!

*/
QDataSchemaDriverMySQL::~QDataSchemaDriverMySQL()
{
}


/*!

*/
QString
QDataSchemaDriverMySQL::sqlDriverName()
{
	return "QMYSQL3";
}



/*!

*/
QString
QDataSchemaDriverMySQL::fieldtype( const QString &qdstype,
		int width, int decimals,
		bool notnull, bool primarykey, bool serial )
{
	QString dt="";
	char t=' ';

	t = toAscii( toLower( qdstype[0] ) );
	switch ( t ) {
	case 'i':
            dt = QString("int");
            break;
        case 'l':
            dt = QString("bigint");
            break;
        case 'c':
            dt = QString("char(%1)").arg( width );
            break;
        case 's':
            dt = QString("varchar(%1)").arg( width );
            break;
        case 'n':
            dt = QString("decimal(%1,%2)").arg( width ).arg( decimals );
            break;
        case 'd':
            dt = QString("datetime");
            break;
        case 'o':
            dt = QString("bigint");
            break;
        default:
            dt = "";
            break;
        }
        if ( notnull ) dt.append(" not null" );
        if ( serial ) dt.append(" auto_increment");
        if ( primarykey ) dt.append(" PRIMARY KEY" );
        return dt;
}



/*!

*/
int
QDataSchemaDriverMySQL::fieldWidth( QSqlFieldInfo *ftypedef )
{
	int w;

	w = ftypedef->length() - ftypedef->precision();
	if ( ftypedef->precision() == 0 ) w--;
	return w;
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
QDataSchemaDriverMySQL::queryCreateDatabase( const QString &name )
{
    return QString("create database %1 character set utf8").arg( name );
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
QDataSchemaDriverMySQL::queryDropIndex( const QString &tname, const QString &iname )
{

    return QString("drop index %1 on %2").arg( iname ).arg(tname);
}



