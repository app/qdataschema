/****************************************************************************
** $Id: qdataschema.h,v 1.30 2008/10/16 19:17:50 leader Exp $
**
** Header file of the ananas database of Ananas
** Designer and Engine applications
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

#ifndef QDATASCHEMA_H
#define QDATASCHEMA_H
#include "qdataschemaglobal.h"

#if defined(Q_OS_WIN32) && !defined(LIB_NO_DLL)
#    define DB_IMPORT __declspec(dllimport)
#else
#	define DB_IMPORT
#endif

class QSqlCursor;
class QDataSchemaDriver;

class QDataSchema: public QObject
{
Q_OBJECT
public:
    QDataSchema( const QString ctype, const char *objname = 0, QObject *owner = 0 );
    ~QDataSchema();
#if QT_VERSION<0x040000
    QString objectName() const { return name();};
    void setObjectName( const QString &newname ) { setName( newname );};
#endif

    bool open( const QString &dbname,
            const QString &dbuser = "", const QString &dbpass = "",
            const QString &dbhost = "localhost", int dbport = 0 );

    bool open();
    void close();

    QSqlDatabase* db();

    bool databaseCreate( const QString &dbadmuser, const QString &dbadmpass );
    bool databaseDrop( const QString &dbadmuser, const QString &dbadmpass );

    int execList( const QStringList &query, bool inTransaction = false );

    QDS_SQLTable *table( const QString &name = QString::null );
    QString tableName( const QString &name );
    QString tableNameStripPrefix( const QString &name );

    QString nameSpace() const;
    void setNameSpace( const QString &namesp );

    bool tableExists( const QString & name );

    static QStringList drivers();
    static bool isDriverAvailable ( const QString & name );
    static QDataSchemaDriver *createDriver(const QString ctype);
    QDataSchemaDriver *driver();
    void setDataDictionary( const QStringList &dd_new );
    QStringList dataDictionary();
    QStringList updateStructureQuery();
    QStringList verifyLog();
    int verifyStructure();
    int updateStructure();

    virtual int databaseExport( const QString &filename );
    virtual int databaseImport( const QString &filename, bool updateStruct = false );

public slots:

signals:
    void verifyMessage( QString op, QString value, QString oldvalue );

protected:
    void verifyLogRecord(QString op, QString value, QString oldvalue = "" );

    QString ddRecord( int idx, bool d_sql = false );
    int ddCount( const QString &etype, bool d_sql = false );
    int ddIndex( const QString &etype, int num, bool d_sql = false );
    int ddIndex( const QString &etype, const QString &name, bool d_sql = false );
    int ddCountSub( int idx, const QString &etype, bool d_sql = false );
    int ddIndexSub( int idx, const QString &etype, int num, bool d_sql = false );
    int ddIndexSub( int idx, const QString &etype, const QString &name, bool d_sql = false );
    QString ddRecSection( const QString &rec, int secnum );
    QString ddRecType( const QString &rec );
    QString ddRecName( const QString &rec );
    void joinLists( QStringList &list, const QStringList &add );
    QStringList splitQString(const QString &div, const QString &str );
    QString lowerQString(const QString &str );
    QString trimmedQString(const QString &str);
    int checkSqlError( QSqlQuery &query );
    QString fieldTypeSql( const QString &tdef );
    virtual int createSystables();

private:
    QStringList dd, dd_sql, dd_update, ql_update;
    QString dd_name, tablename_prefix;
    bool fVerified;

    QDataSchemaDriver *p_drv;

#if QT_VERSION<0x040000
    QSqlDatabase *p_db;
#else
    QSqlDatabase p_db;
#endif
    QString v_dbname, v_dbuser, v_dbpass, v_dbtype, v_dbhost;
    int	v_dbport;

    int readSqlDictionary();
    int writeSqlDictionary();
};

#endif
