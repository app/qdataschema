/****************************************************************************
** $Id: qdataschemadriver-sqlite.cpp,v 1.12 2008/11/09 20:03:16 leader Exp $
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
#include <qdataschemadriver-sqlite.h>

/*!
 * \class QDataSchemaDriverSqlite qdataschemadriver-sqlite.h
 * \en
 * \brief Database schema SQLite drive object.
 *
 * \_en \ru
 * \brief Класс для работы с сервером SQLite.
 *
 * Используется для работы с сервером SQLite.
 * \_ru
 */


QDataSchemaDriverSqlite::QDataSchemaDriverSqlite() : QDataSchemaDriver()
{
}



/*!

*/
QDataSchemaDriverSqlite::~QDataSchemaDriverSqlite()
{
}



/*!

*/
QString
QDataSchemaDriverSqlite::sqlDriverName()
{
	return "QSQLITE";
}


/*!
  Возвращает список системных таблиц, разделенных "\n".
*/
QString
QDataSchemaDriverSqlite::systemTables()
{
    QString list;
    list.append("sqlite_sequence\n"); 
    list.append("sqlite_master\n"); 
    return list;
};



/*!

*/
QString
QDataSchemaDriverSqlite::fieldtype( const QString &qdstype,
		int width, int decimals,
		bool notnull, bool primarykey, bool serial )

{
        QString dt="";
	char t=' ';

//	t = qdstype[0].toLower().toAscii();
        t = toAscii( toLower( qdstype[0] ) );
	if ( serial && t=='l' ) t = 'i';

	switch ( t ) {
        case 'i':
            dt = QString("integer");
            break;
        case 'l':
            dt = QString("integer");;
            break;
        case 'c':
            dt = QString("char(%1)").arg( width );
            break;
        case 's':
            dt = QString("varchar(%1)").arg( width );
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
        }
        if ( !serial && !primarykey && notnull ) dt.append(" not null" );
        if ( primarykey ) dt.append(" PRIMARY KEY");
        if ( serial ) dt.append(" autoincrement");
        return dt;
}



/*!
 * \en
 * \brief
 *
 * \_en \ru
 * \brief Формирует запрос на обновление структуры таблицы.
 *
 * \_ru
 */
QString
QDataSchemaDriverSqlite::queryAlterTable( const QString &tname,
                                          const QString &fieldsnew, const QString &/*fieldsold*/,
                                    const QString &fieldsadd,const QString &fieldsdrop,
                                    const QString &/*fieldsmodifynew*/,const QString &/*fieldsmodifyold*/ )
{
    QString query = "";
    QString trfields="";
    int fldn;
//    query = QDataSchemaDriver::queryAlterTable( tname,fieldsnew,fieldsold,fieldsadd,fieldsdrop,
//            fieldsmodifynew,fieldsmodifyold );

    QString temptblname = "n_"+tname;
    query.append( QString("create table %1 (%2)\n").arg(temptblname).arg( QString(fieldsnew).replace("\n",",")));
    // We have to transfer data from old table to new one.
    // To do this we'll make something like "INSERT INTO newtable SELECT field1,field2,'def1',def2 FROM oldtable"
    // Thank to typelessness of SQLITE we do not bother about changes in fields' types.
    fldn=0;
    QString fname=fieldsnew.section("\n",fldn,fldn).section(" ",0,0);
    while(!fname.isEmpty())
    {
        if ( indexOf( fieldsdrop, fname) == -1 )
        { // field was not dropped
            if ( indexOf( fieldsadd, fname ) == -1 )
            {
                if (!trfields.isEmpty() ) trfields.append(",");
                trfields.append(fname);
            }
            else
            { // field was added. We must supply default value. I think NULL as default value will do well.
                if (!trfields.isEmpty() ) trfields.append(",");
                trfields.append("null");
            };
        };
        fldn++;
        fname=fieldsnew.section("\n",fldn,fldn).section(" ",0,0);
    };
    query.append(QString("INSERT INTO %1 SELECT %2 FROM %3\n").arg(temptblname).arg(trfields).arg(tname));
    query.append( queryDropTable( tname ) );
    query.append("\n");
    query.append( queryCreateTable( tname, fieldsnew ));
    query.append("\n");
    query.append(QString("INSERT INTO %1 SELECT * FROM %2\n").arg(tname).arg(temptblname));
    query.append( queryDropTable( temptblname ) );
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
QDataSchemaDriverSqlite::queryInitConnection( bool unicode )
{
    QString query="";

    if ( unicode ){
        query.append("PRAGMA encoding=\"UTF-8\"\n");
    }
    query.append(
//        "PRAGMA temp_store=MEMORY\n"
        "PRAGMA cache_size=8000\n"
        "PRAGMA synchronous=OFF\n"
        );

    return query;
}
