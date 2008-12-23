/****************************************************************************
** $Id: qdataschemadriver.cpp,v 1.12 2008/11/09 20:03:16 leader Exp $
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
#include <qdataschemadriver.h>
#include <qdataschemaglobal.h>
/*!
 * \class QDataSchemaDriver qdataschemadriver.h
 * \en
 * \brief Database schema driver base object.
 *
 * \_en \ru
 * \brief Класс для работы с сервером БД.
 *
 * Используется для работы с сервером БД.
 * \_ru
 */





/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */

QDataSchemaDriver::QDataSchemaDriver() : QObject()
{
}


/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriver::sqlDriverName()
{
	return "";
}



/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriver::fieldtype( const QString &/*qdstype*/,
		int /*width*/, int /*decimals*/,
		bool /*notnull*/, bool /*primarykey*/, bool /*serial*/ )
{
	return "";
}



/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriver::fieldSQLToDS( QSqlFieldInfo *ftypedef )
{
        QString t;
	int w, d;

	w = fieldWidth( ftypedef );
        d = fieldDecimals( ftypedef );

        switch ( ftypedef->type() ) {
        case QVariant::DateTime:
            t.sprintf("D 0 0");
            break;
        case QVariant::Int:
            t.sprintf("I 0 0");
            break;
        case QVariant::LongLong:
            t.sprintf("L 0 0");
            break;
        case QVariant::String:
#if QT_VERSION<0x040000
        case QVariant::CString:
#endif
            t.sprintf("S %d 0", w);
            break;
        case QVariant::Double:
/*
            if ( drv == "QPSQL7" ) {
                d = w & 0xFF;
                w = ( w & 0xFF0000 ) >> 16;
            }
            if ( drv == "QMYSQL3" ) {
                w = w - d;
                if ( d==0 ) w--;
            }
*/
            t.sprintf("N %d %d", w, d);
            break;
	default:
		t = "";
        }
#if QT_VERSION<0x040000
        if ( ftypedef->isRequired() )
#else
        if ( ftypedef->requiredStatus() )
#endif
            t = t.section(" ",0,0)+"N "+t.section(" ",1);
        t = ftypedef->name()+" "+t;
	return t;
}



/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriver::fieldName( QSqlFieldInfo *ftypedef )
{
	return ftypedef->name();
}



/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
int
QDataSchemaDriver::fieldWidth( QSqlFieldInfo *ftypedef )
{
	return ftypedef->length();
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
QDataSchemaDriver::fieldDecimals( QSqlFieldInfo *ftypedef )
{
	return ftypedef->precision();
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
QDataSchemaDriver::queryInitConnection( bool /*unicode*/ )
{
    return "";
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
QDataSchemaDriver::queryCreateDatabase( const QString &name )
{
    return QString("create database %1").arg( name );
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
QDataSchemaDriver::queryDropDatabase( const QString &name )
{
    return QString("drop database %1").arg( name );
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
QDataSchemaDriver::queryCreateTable( const QString &tname, const QString &fields )
{
    return QString("create table %1 (%2)").arg( tname ).arg( QString( fields ).replace("\n",",") );
}


/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriver::queryDropTable( const QString &tname )
{
    return QString("drop table %1").arg( tname );
}



/*!
 * \fn QDataSchemaDriver::
 * \en
 * \brief
 *
 * \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchemaDriver::queryCreateIndex( const QString &tname, const QString &iname, const QString &fields )
{
    return QString("create index %1 on %2 (%3)").arg( iname ).arg( tname ).arg( fields );
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
QDataSchemaDriver::queryDropIndex( const QString &/*tname*/, const QString &iname )
{

    return QString("drop index %1").arg( iname );
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief Формирует запрос на обновление структуры таблицы.
 *
 * Вызывает последовательность функций
 * queryAlterTableAddField(), queryAlterTableDropField() и
 * queryAlterTableModifyField() для каждого поля из списка
 * полей fieldsadd, fieldsdrop, fieldsmodifynew
 *
 * \arg tname (in) имя таблицы в базе данных
 * \arg fieldsnew (in)
 * \arg fieldsold (in)
 * \arg fieldsadd (in)
 * \arg fieldsdrop (in)
 * \arg fieldsmodifynew (in)
 * \arg fieldsmodifyold (in)
 * \return Последовательность SQL запросов для обновления структуры таблицы,
 * разделенная "\n"
 * \_ru
 */
QString
QDataSchemaDriver::queryAlterTable( const QString &tname,
                                    const QString &/*fieldsnew*/, const QString &/*fieldsold*/,
                                    const QString &fieldsadd,const QString &fieldsdrop,
                                    const QString &fieldsmodifynew,const QString &fieldsmodifyold )
{
    QDS_int i;
    QStringList fl;
    QString query = "";
    QString f;

    // Do add fields
    fl = split( "\n", fieldsadd );
    for (i = 0; i < fl.count(); i++ ){
        if ( !query.isEmpty()) query.append("\n");
        f = trimm( fl[i] );
        if ( !f.isEmpty() ) query.append( queryAlterTableAddField( tname, f ) );
    }
    // Do drop fields
    fl = split( "\n", fieldsdrop );
    for (i = 0; i < fl.count(); i++ ){
        if ( !query.isEmpty()) query.append("\n");
        f = trimm( fl[i] );
        if ( !f.isEmpty() ) query.append( queryAlterTableDropField( tname, fl[i] ) );
    }
    // Do modify fields
    fl = split( "\n", fieldsmodifynew );
    for (i = 0; i < fl.count(); i++ ){
        if ( !query.isEmpty()) query.append("\n");
        query.append( queryAlterTableModifyField( tname, fl[i], fieldsmodifyold.section("\n", i, i ) ) );
    }
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
QDataSchemaDriver::queryAlterTableDropField( const QString &tname, const QString &field )
{
    return QString("alter table %1 drop column %2\n").arg( tname ).arg( field.section(" ",0,0) );
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
QDataSchemaDriver::queryAlterTableAddField(const QString &tname, const QString &field )
{
    return QString("alter table %1 add column %2\n").arg( tname ).arg( field );
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
QDataSchemaDriver::queryAlterTableModifyField( const QString &tname, const QString &field, const QString &/*oldfield*/ )
{
    return QString("alter table %1 modify column %2\n").arg( tname ).arg( field );
}

