/****************************************************************************
** $Id: qdataschema.cpp,v 1.52 2008/11/09 20:03:16 leader Exp $
**
** QT Data Schema project
**
** Copyright (C) 1999-2006 Leader InfoTech.  All rights reserved.
** Copyright (C) 1999-2006 Valery Grazdankin.  All rights reserved.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/ or email sales@leaderit.ru
**********************************************************************/

/*
for mysql server set variable in /var/lib/mysql/my.cnf
default-character-set=koi8_ru
*/
#include "qdataschema.h"
#include "qdataschemadriver.h"
#include "qdataschemadriver-mysql.h"
#include "qdataschemadriver-postgresql.h"
#include "qdataschemadriver-sqlite.h"

/*!
 * \class QDataSchema qdataschema.h
 * \en
 * \brief Class for works with data schema.
 *
 * Used for support syncronous data structure in application
 * and database server. Application data structure describes in
 * database server independed context. In depends of the
 * database server, chose method for structure updates if
 * structure of the application data was changed ( for
 * example, we wer updated application version ) and we need
 * updates data base structure with save information, that
 * was collected earlier.
 *
 * In addition to tables, described in the application,
 * QDataSchema creates additional table of data dictionary
 * with name dictionaryTableName() ( default "dd" ) for storing
 * information about current structure of the data base for
 * maximum correct updates of the information structure.
 *
 * Dictionary creates automaticaly with folowing parameters:
 *\verbatim
   T=dd|Data dictionary|S
   F=id|Line number|I|0|0|
   F=value|Data value|C|250|0|\endverbatim
 * Object usage:
 * \_en \ru
 * \brief Класс для работы со схемой базы данных.
 *
 * Используется для поддержки синхронной структуры данных
 * приложения и сервера базы данных. Структура данных
 * приложения описывается в серверо-независимом контексте.
 * В зависимости от конкретного сервера базы данных
 * выбирается способ обновления структуры в том случае, если
 * структура информации в приложении изменилась ( например,
 * обновлена версия приложения ) и нам требуется обновить
 * структуру базы данных, сохранив ранее введенную информацию.
 *
 * в дополнение в таблицам, описанным в приложении,
 * QDataSchema создает дополнительную таблицу словаря данных
 * с именем dictionaryTableName() ( по умолчанию "dd" )
 * в которой хранится служебная информация о текущей структуре
 * базы данных для максимально корректного обновления структуры.
 *
 * Словарь создается автоматически со следующими параметрами:
 *\verbatim
   T=dd|Data dictionary|S
   F=id|Line number|I|0|0|
   F=value|Data value|C|250|0|\endverbatim
 * Последовательность работы с объектом:
 * \_ru
 * \code
 *
 *  QDataSchema *qds;
 *  QString dbname = "qds",
 *          dbuser = "",
 *          dbpasswd = "",
 *          dbserver = "localhost";
 *  int dbport = 0;
 *  QStringList dd;
 *
 *  qds = new QDataSchema( QDataSchema::CT_INTERNAL );
 *  // Application data schema description
 *  dd<<"D=QDS|Description|UTF-8|UID";
 *  dd<<"T=T1|Description|U";
 *  dd<<"F=F1|Description|I|0|0|PSN";
 *  dd<<"F=F2|Description|D|0|0|";
 *  dd<<"F=F3|Description|C|200|0|";
 *  dd<<"F=F3|Description|S|200|0|";
 *  dd<<"F=F4|Description|N|10|2|";
 *  dd<<"F=F5|Description|I|0|0|";
 *  dd<<"F=F2|Description|L|0|0|";
 *  dd<<"I=I1|Description|F2,F3|";
 *  dd<<"T=T2|Description|U";
 *  dd<<"F=F1|Description|I|0|0|PSN";
 *  dd<<"F=F2|Description|C|10|0|";
 *
 *  qds->setDataDictionary( dd );
 *  if ( qds->init( dbname,dbuser,dbpasswd,dbserver,dbport ) )
 *  {
 *       if ( !qds->verifyStructure() ){
 *          // need to update
 *          printf("verify log:\n%s\n", ( const char * ) qds->verifyLog().join("\n") );
 *          printf("update structure query:\n%s\n", ( const char * ) qds->updateStructureQuery().join("\n") );
 *          qds->updateStructure();
 *       }
 *  } else printf("ERROR INIT DATABASE\n");
 *
 * \endcode
 * \en
 * Objectives of the QDataSchema class is support actualy SQL database
 * server tables structure automaticaly if database structure
 * changed in applications, using QDataSchema.
 * QDataSchema uses tables structure description.
 *
 * Theare are tow tables structure descriptions:
 * 1) Tables structure on database server
 * 2) Tables structure, loaded by application on
 * class initialisation.
 *
 * varifyStructure() method check correspondence betwin that structures
 * and if there different, prepare 3 differents lists:
 * \li deletes from database list
 * \li additions to database list
 * \li updates in database list
 *
 * Database server tables structure modification sased on that 3 lists.
 * Structures are syncronised in result.
 *
 * Tables structure is a strings list in format:
 *
 * \_en \ru
 * Класс QDataSchema нацелен на автоматическую поддержку актуальной
 * структуры таблиц базы данных на SQL сервере при изменении
 * в структуре базы данных приложения, использующего QDataSchema.
 * QDataSchema использует описание структуры таблиц.
 *
 * Существуют два описания структуры таблиц:
 * 1) Структура таблиц на сервере базы данных
 * 2) структура таблиц, загруженная приложением при инициализации
 * класса.
 *
 * Метод verifyStructure() проверяет соответствие этих двух
 * структур и в случае расхождения составляет 3 списка различий:
 *
 * \li список удалений из БД
 * \li список добавлений в БД
 * \li список изменений в БД
 *
 * На основе этих 3-х списков производится модификация структуры
 * таблиц на сервере БД, в результате которой структуры синхронизируются.
 *
 * структура таблиц представляет список строк следующего формата:
 * \_ru
 *
 * \verbatim
 * # Commentaries in (UTF-8)
 * # --- Data dictionary
 * # Name|Description|Charset|UID
 * D=DATASCHEMA|Description of the data schema|UTF-8|XXXCCCWWW
 * # --- Table
 * # Name | Descr | Type[A/S/U]| DBName|Flags
 * T=TableName|Table description|A|DBTable|
 * # --- Fields
 * # Name| Description|Type [D/C/N/I]|Length|Precision|
 * F=CURDATE|Current date|D|8|0|
 * #--- Indexes
 * # Name| Description |Unique [0/1]|Index fields|DBName
 * I=IDD|Index of ID|0|ID,DATE,DESCR(UPPER)|IDD
 * # END
 * \endverbatim
 *
 */



/*!
 * \en
 * \brief Create and initialise new object.
 *
 * \arg ct (in) Type of the server connection.
 * \arg objname (in) Object name. Uses as QSqlDatabase connection name.
 *
 * \_en \ru
 * \brief Создает и инициализирует новый объект.
 *
 * \arg ct (in) Тип соединения с сервером данных.
 * \arg objname (in) Имя объекта. Используется как имя соединения QSqldataBase.
 *
 * \_ru
 */
QDataSchema::QDataSchema( const QString ct, const char *objname, QObject *owner )
: QObject( owner )
{
    if ( !objname ) setObjectName("QDataSchema");
    p_drv=0;
#if QT_VERSION<0x040000
    p_db=0;
#endif
    if ( isDriverAvailable( ct ) ) {
        p_drv = createDriver( ct );
        if ( p_drv ) p_db = QSqlDatabase::addDatabase( p_drv->sqlDriverName(), objectName() );
    }
    dd_name = "dd";
    tablename_prefix = "";
}



/*!
 * \en
 * \brief Close connection and release object resources.
 *
 * \_en \ru
 * \brief Закрывает соединение и освобождает ресурсы объекта.
 *
 * \_ru
 */
QDataSchema::~QDataSchema()
{
    close();
#if QT_VERSION<0x040000
    if ( p_db ) QSqlDatabase::removeDatabase( p_db );
#else
    QSqlDatabase::removeDatabase( objectName() );
#endif
}



/*!
 * \en
 * \brief Open and initialise database server connection.
 *
 * \param dbname (in) - Database name.
 * \param dbuser (in) - Database user name.
 * \param dbpass (in) - Database password.
 * \param dbhost (in) - Database host.
 * \param dbport (in) - Database connection port.
 * \return true if no error or false if error rised
 * \_en \ru
 * \brief Открывает и инициализирует соединение с сервером базы данных.
 *
 * \param dbname (in) - Имя базы данных.
 * \param dbuser (in) - Имя пользователя сервера базы данныхю
 * \param dbpass (in) - Пароль пользователя сервера базы данныхю.
 * \param dbhost (in) - Адрес хоста сервера базы данныхю.
 * \param dbport (in) - Порт соединения сервера базы данныхю.
 * \return true если нет ошибок или false в случае возникновения ошибки.
 * \_ru
 */
bool
QDataSchema::open(  const QString &dbname,
        const QString &dbuser, const QString &dbpass,
        const QString &dbhost, int dbport )
{
//    QString qinit;

    if ( !db() || !driver() ) return false;
    db()->setDatabaseName( dbname );
    db()->setUserName( dbuser );
    db()->setPassword( dbpass );
    db()->setHostName( dbhost );
    if ( dbport ) db()->setPort( dbport );
    return open();
}



/*!
 *\en
 *\brief Open and init database connection.
 *
 * \return true if no error or false if error rised
 *\_en \ru
 *\brief Открывает и инициализирует соединение с сервером базы данных.
 *
 *
 * \return true если нет ошибок или false в случае возникновения ошибки.
 * \_ru
 */
bool
QDataSchema::open()
{
    bool res = false;

    if ( db() && driver() )
    {
        res = db()->open();
        if ( res ) {
            if ( execList( splitQString("\n",
                 driver()->queryInitConnection( true ) ) ) ) res = false;
        }
    }
    return res;
}



/*!
 * \en
 * \brief Closes database server connection.
 *
 *  \_en \ru
 * \brief Закрывает соединение с сервером базоы данных.
 *
 * \_ru
 */
void
QDataSchema::close()
{
    if ( db() ) {
        db()->close();
    }
}



/*!
 * \en
 * \brief Returns database connection.
 *
 * \return pointer to SQL database object.
 *
 *  \_en \ru
 * \brief
 *
 * \return Указатель на объект работы с сервером SQL.
 * \_ru
 */
QSqlDatabase*
QDataSchema::db()
{
#if QT_VERSION<0x040000
    return p_db;
#else
    return &p_db;
#endif
}



/*!
 * \en
 * \brief Creates QSqlCursor object for database table.
 *
 * Returned object can be used for browsing and
 * editing table in current database.
 *
 * \param name (in) - sql table name.
 * \return newly created object.
 * \_en \ru
 * \brief Создает объект QSqlCursor для таблицы базы данных.
 *
 * Возвращаемый объект может быть использован для
 * просмотра и редактирования таблицы в текущей базе данных.
 *
 * \param name (in) - имя таблицы в базе данных
 * \return вновь созданный объект.
 * \_ru
 */
QDS_SQLTable *
QDataSchema::table( const QString & name )
{
    QDS_SQLTable *t;
    
    if ( tableExists( name ) ) {
#if QT_VERSION<0x040000
        t = new QSqlCursor( name, this );
#else
        t = new QSqlTableModel( this, *db() );
        t->setTable( name );
#endif
    } else {
        t = 0;
    }
    return t;
}



#if 0
/*!
 *  Generates new unique id for system object.
 *  \param otype (in) - object type (e.g. document, catalog, journal etc.)
 *  \return newly generated unique id.
 */
/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief
 *
 * \_ru
 */
Q_ULLONG
QDataSchema::uid( int otype )
{
    Q_ULLONG uid = 0;
    QString query;

    QString drv = db()->driverName();
    query.sprintf("insert into uniques (otype) values (%d)", otype );
    QSqlQuery q = db()->exec( query );
    if ( drv == "QSQLITE") {
    // sqlite
        query.sprintf("select last_insert_rowid()");
    }
    if ( drv == "QMYSQL3") {
    // mysql
        query.sprintf("select last_insert_id()");
    }
    if ( drv == "QPSQL7" ) {
    // pgsql
        query.sprintf("select currval('uniques_id_seq')");
    }
    q = db()->exec( query );
    if ( q.first() ) uid = q.value( 0 ).toULongLong();
    return uid;
}



/*!
 *  Returns type of object identified by its uid.
 *  \param uid (in) - object's uid.
 *  \return object's type, e.g. catalog, document, iregister etc.
 */
int
QDataSchema::uidType ( Q_ULLONG uid )
{
    QSqlQuery q = db()->exec( "SELECT otype FROM uniques WHERE id="+QString("%1").arg(uid) );
    if ( q.first() )
        return q.value(0).toInt();
    return 0;
}

#endif

/*!
 * \en
 * \brief Checks table existence.
 *
 * \param name (in) - table name to check for.
 * \return true if table exists in current database or false if not.
 *  \_en \ru
 * \brief Проверка существования таблицы на сервере БД.
 *
 * \param name (in) - Иям таблицы для проверки.
 * \return true если таблица существует, false - не существует.
 * \_ru
 */
bool
QDataSchema::tableExists( const QString & name )
{
    QStringList list = db()->tables( QSql::Tables );
    QStringList::Iterator it = list.begin();
    while( it != list.end() ) {
        if ( lowerQString(*it) == lowerQString(name) ) return true;
        ++it;
    }
    return false;
}



/*!
 * \en
 * \brief Set data dictionary for application.
 *
 * \arg dd_new (in) List of the data description.
 *
 *  \_en \ru
 * \brief Устанавливает словарь описания данных.
 *
 * \arg dd_new (in) Список описания структуры данных.
 *
 * \_ru
 */
void
QDataSchema::setDataDictionary( const QStringList &dd_new )
{
    dd = dd_new;
}



/*!
 * \en
 * \brief Return data dictionary.
 *
 * \return List of the data description.
 *
 *  \_en \ru
 * \brief Возвращает словарь описания данных.
 *
 * \return Список описания структуры данных.
 * \_ru
 */
QStringList
QDataSchema::dataDictionary()
{
    return dd;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Возвращает описание записи поля словаря для активного сервера БД.
 *
 * \arg tdef (in) Строка описание поля в терминах QDataSchema.
 * \return Описание типа поля в терминах активного сервера данных.
 *
 * \_ru
 */
QString
QDataSchema::fieldTypeSql( const QString &tdef )
{
    QString ftype, fname, flags, dt,idx;
    int w = 0, d = 0;
    bool    notnull = false,
            serial  = false,
            pkey    = false;

    if ( tdef.isEmpty() ) return "";
    fname = ddRecName( tdef );
    ftype = ddRecSection( tdef, 2 );
    w = ddRecSection( tdef, 3 ).toInt();
    d = ddRecSection( tdef, 4 ).toInt();
    flags = lowerQString(ddRecSection( tdef, 5 ));

    serial = ( flags.contains( 's' ) > 0 );
    notnull = ( flags.contains( 'n' ) > 0 );
    pkey = ( flags.contains( 'p' ) > 0 );

    return fname+" "+driver()->fieldtype( ftype, w, d, notnull, pkey, serial );
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Создание системных таблиц.
 *
 * Создает таблицу словаря со структурой
 *\verbatim
 * dd_name (id int, value char(250))
 *\endverbatim

 * для хранения дополнительной информации о типах полей
 * и индексов.
 *
 * \_ru
 */
int
QDataSchema::createSystables()
{
    int rc = 0;
    QStringList l;

    if ( !tableExists( tableName( dd_name ) ) ){
        l << driver()->queryCreateTable(tableName( dd_name ),
            fieldTypeSql("F=id||I|0|0|"     )+"\n"+
            fieldTypeSql("F=value||C|250|0|")
        );
        rc = execList(l);
    }
    return rc;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchema::tableName( const QString &name )
{
    return tablename_prefix + name;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief
 *
 * \_ru
 */
QString
QDataSchema::tableNameStripPrefix( const QString &name )
{
    if ( name.left( tablename_prefix.length() ) == tablename_prefix ) {
        return name.right( name.length()-tablename_prefix.length() );
    }
    return name;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Возвращает пространство имен таблиц базы данных.
 *
 * \_ru
 */
QString
QDataSchema::nameSpace() const
{
    return tablename_prefix;
}



/*!
 * \en
 * \brief
 *
 *\_en \ru
 *\brief Устанавливает пространство имен таблиц базы данных.
 *
 * Операция выполняется только над закрытой базой данных.
 *
 *\_ru
 */
void
QDataSchema::setNameSpace( const QString &namesp )
{
    if ( db() && !db()->isOpen() ) tablename_prefix = namesp;
}


#if 0
/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief
 *
 * \_ru
 */
void
QDataSchema::markDeleted(Q_ULLONG uid)
{
    db()->exec(QString("UPDATE uniques SET df='1' WHERE id=%1").arg(uid));
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief
 *
 * \_ru
 */
void
QDataSchema::deleteMarked()
{
    db()->exec("DELETE FROM uniques WHERE df='1'");
}

#endif


/*!
 *\en
 *\brief Check structure.
 *
 *
 *\_en \ru
 *\brief Проверяет структуру текущей БД на соответствие описанной в метаданных.
 *
 * Подготавливает информацию для последующего обновления.
 * \return 0 - обновление не требуется, 1 - обновление требуется.
 * \_ru
 */
int
QDataSchema::verifyStructure()
{
    QString lt = " "+lowerQString(db()->tables( QSql::Tables ).join("; "))+";";
    QString recType, recName;
    QStringList v;
    QString tname, tname_db, ename;
    QString s, es, es_sql, t_es, t_es_sql;
    int rc = 0;
    QDS_int i, ti, tcount, tidx, tsql_idx, ei, ecount, eidx;
    char umode = ' ';
    int tu_count;

//    printf("lt0 = %s\n",(const char *) lt.toLocal8Bit().data() );
#if QT_VERSION<0x040000
    QSqlRecordInfo recInfo;
    QSqlRecordInfo::Iterator it;
    QSqlFieldInfo f;
#else
    QSqlRecord recInfo;
    int it;
    QSqlField f;
#endif

    QString
            fields,   // Список новых полей БД с описанием типа, разделенных "\n"
            oldfields,// Список старых полей БД с описанием типа, разделенных "\n"
            ifields,
            fieldsdrop,// Список полей БД, требующих удаления, разделенных "\n"
            fieldsadd, // Список полей БД, требующих добавления, разделенных "\n"
            fieldsmodifynew,// Список полей БД, требующих обновления
                            // с описанием нового типа, разделенных "\n"
            fieldsmodifyold;// Список полей БД, требующих обновления
                            // с описанием старого типа, разделенных "\n"
    QString indexesadd;

    readSqlDictionary();
    dd_update.clear();
    ql_update.clear();

    // Проверим таблицы
    tcount = ddCount("T");
    for ( ti = 0; ti<tcount; ti++){
        tu_count = 0;
        tidx = ddIndex("T", ti );
        s = dd[tidx];
        tname = ddRecName( s ); // имя таблицы в словаре
        tname_db = tableName( tname ); //Имя таблицы в базе данных с префиксом имени

        fields = "";
        oldfields="";
        fieldsdrop="";
        fieldsadd="";
        fieldsmodifynew="";
        fieldsmodifyold="";
        indexesadd = "";
        // Подготовим список с описанием полей новой структуры
        ecount = ddCountSub( tidx, "F" );
        for ( ei = 0; ei < ecount; ei++ ){
            if ( !fields.isEmpty() ) fields.append("\n");
            fields.append( fieldTypeSql( ddRecord( ddIndexSub( tidx, "F", ei ) ) ) );
        }
        if ( tableExists( tname_db ) ) {
            umode='&';
            tsql_idx = ddIndex( "T", tname, true );
            // Сформируем список полей таблицы для проверки удаленных полей
//            recInfo = db()->recordInfo( tname_db );
            recInfo = db()->record( tname_db );
            // Сформируем список старых полей таблицы
#if QT_VERSION<0x040000
            it = recInfo.begin();
            while( it != recInfo.end() ) {
                t_es_sql = fieldTypeSql( ddRecord( ddIndexSub( tsql_idx, "F", (*it).name(), true ), true ) );
                if ( !oldfields.isEmpty() ) oldfields.append("\n");
                oldfields.append( t_es_sql );
                ++it;
            }
#else
            for( it=0; it<recInfo.count(); it++ ) {
                t_es_sql = fieldTypeSql( ddRecord( ddIndexSub( tsql_idx, "F", recInfo.field(it).name(), true ), true ) );
                if ( !oldfields.isEmpty() ) oldfields.append("\n");
                oldfields.append( t_es_sql );
            }
#endif
            // Проверим, требуется ли обновление структуры таблицы.
            if ( fields != oldfields ) {
                // Описания полей различны, требуется обновление структуры таблицы
                verifyLogRecord("&", s );
                // Сформируем список новых полей таблицы
                for ( ei = 0; ei < ecount; ei++ ){
                    es = ddRecord( ddIndexSub( tidx, "F", ei ) );
                    ename = ddRecName( es );
                    t_es = fieldTypeSql( es );
                    if ( recInfo.contains( ename ) > 0 ) {
                        // Если поле найдено в старой структуре
#if QT_VERSION<0x040000
                        f = recInfo.find( ename );
#else
                        f = recInfo.field( ename );
#endif
                        es_sql = ddRecord( ddIndexSub( ddIndex( "T", tname, true ), "F", ename, true ), true );
                        t_es_sql = fieldTypeSql( es_sql );
                        // Проверка, изменилось ли описание поля
                        if ( t_es_sql != t_es ) {
                            if ( !fieldsmodifynew.isEmpty() ) fieldsmodifynew.append("\n");
                            fieldsmodifynew.append( t_es );
                            if ( !fieldsmodifyold.isEmpty() ) fieldsmodifyold.append("\n");
                            fieldsmodifyold.append( t_es_sql );
                            // Зарегистрируем
                            verifyLogRecord("&", es, es_sql );
                        }
                    } else {
                        // Новое поле
                        if ( !fieldsadd.isEmpty() ) fieldsadd.append("\n");
                        fieldsadd.append( t_es );
                        // Зарегистрируем
                        verifyLogRecord("+", es );
                    }
                }
                // Сформируем список полей для удаления
#if QT_VERSION<0x040000
                it = recInfo.begin();
                while( it != recInfo.end() ) {
                    f = *it;
#else
                it = 0;
                while( it < recInfo.count() ) {
                    f = recInfo.field(it);
#endif
                    es_sql = ddRecord( ddIndexSub( tsql_idx, "F", f.name(), true ), true );
                    t_es_sql = fieldTypeSql( es_sql );
                    if ( ddIndexSub( tidx, "F", f.name() )==-1){
                        // Добавить имя поля в список удаленных полей
                        if ( !fieldsdrop.isEmpty() ) fieldsdrop.append("\n");
                        fieldsdrop.append( t_es_sql );
                        // зарегистрируем
                        if ( es_sql.isEmpty() ) es_sql = f.name();
                        verifyLogRecord("-", es_sql );
                    }
                    ++it;
                }
#ifdef DEBUG
                printf("%s\nnew:%s\nold:%s\n",(const char*)tname_db,(const char*)fields,(const char*)oldfields);
                printf("drop:%s\n",(const char*)fieldsdrop);
                printf("add:%s\n",(const char*)fieldsadd);
                printf("modifynew:%s\n",(const char*)fieldsmodifynew);
                printf("modifyold:%s\n",(const char*)fieldsmodifyold);
#endif
                joinLists( ql_update, splitQString("\n",
                    driver()->queryAlterTable( tname_db, fields, oldfields,
                    fieldsadd, fieldsdrop, fieldsmodifynew, fieldsmodifyold ) ) );
            }
        } else {
            umode = '+';
            verifyLogRecord("+", s );
            ql_update << driver()->queryCreateTable( tname_db, fields );
        }

        // Проверим индексы таблицы
        // Сначала определим, какие индексы надо удалить.
        // Удаляются индексы, которых нет в новой структуре или которые
        // изменили свои параметры.
        // Проверка удвления выполняется только при обновлении структуры таблицы,
        // так как при удалении таблицы все индексы, связанные с ней как правило
        // удаляются автоматическиЮ а при создании таблицы индексы отсутствуют.
        if (umode=='&') {
            ecount = ddCountSub( tidx, "I", true );
            for ( ei = 0; ei < ecount; ei++ ){

                es_sql = ddRecord( ddIndexSub( ddIndex( "T", tname, true ), "I", ei, true ), true );
                ename = ddRecName( es_sql );
                t_es_sql = ddRecSection( es_sql, 2 );
                es = ddRecord( ddIndexSub( ddIndex( "T", tname ), "I", ename ) );
                t_es = ddRecSection( es, 2 );
                // Если индекса нет в новой структуре или его описание изменилось
                // удалим его
                if ( es.isEmpty() || ( lowerQString(t_es_sql)!=lowerQString(t_es))) {
                    verifyLogRecord("-", es_sql );
                    ql_update << driver()->queryDropIndex( tname_db , tname_db+"_"+ename );
                    if ( !es.isEmpty() ) {
                        indexesadd.append(ename+"\n");
                    }
                }
            }
        }
        ecount = ddCountSub( tidx, "I" );
        for ( ei = 0; ei < ecount; ei++ ){
            eidx = ddIndexSub( tidx, "I", ei );
            es = dd[eidx];
            ename = ddRecName( es );
            ifields = ddRecSection( es, 2 );
            if ( umode=='+' ||
                 indexesadd.contains( ename+"\n")>0 ||
                 ddIndexSub( ddIndex( "T", tname, true ), "I", ename, true ) == -1 ) {
                verifyLogRecord("+", es );
                ql_update << driver()->queryCreateIndex( tname_db , tname_db+"_"+ename, ifields );

            }
        }

        // Удалим таблицу словаря из списка таблиц БД к удалению
        lt.remove( " "+lowerQString(tname_db)+";" );
    }
    // Удалим из списка к удалению системные таблицы
    lt.remove( " "+tableName( dd_name )+";" );
    v = splitQString( "\n", driver()->systemTables() );
    for ( i = 0; i < v.count(); i++ ){
        s = trimmedQString(v[i]);
        if (!s.isEmpty()) lt.remove( " "+s+";" );
    }
//    printf("lt = %s\n",(const char *) lt.toUtf8().data());
    // Добавим команду удаления таблиц, которых уже нет в словаре
    v = splitQString( ";", lt );
    for ( i = 0; i < v.count(); i++ ){
        s = trimmedQString(v[i]);
        if (!s.isEmpty()){
            tname = tableNameStripPrefix( s );
            // Удаляем таблицы только из нашего пространства имен
            // Или все отсуствующие в словаре, если пространство имен не задано
            if ( nameSpace().isEmpty() || s != tname ){
                es_sql = ddRecord( ddIndex( "T", tname, true ), true );
                if ( es_sql.isEmpty() ) es_sql = s;
                verifyLogRecord("-", es_sql );
                ql_update << driver()->queryDropTable( s );
            }
        }
    }
#ifdef DEBUG
    printf("dd_update=\n%s\n", ( const char *) dd_update.join("\n") );
    printf("ql_update=\n%s\n", ( const char *) ql_update.join(";\n") );
#endif
    if (ql_update.count()>0) rc = 1; else rc = 0;
    return rc;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Обновляет структуру таблиц на сервере.
 *
 * Обновление выполняется на основе информации, подготовленной
 * при вызове verifyStructure().
 * Обновление структуры производится в транзакции ( если сервер
 * базы данных поддерживает работу с транзакциями ).
 * \return 0 - нет ошибок, иначе - код ощибки.
 * \_ru
 */
int
QDataSchema::updateStructure()
{
    int rc = 0;
    QString recType, recName, s;
    QStringList ql;
    QStringList lfUpdate, lfRemove, lfNew;
    QString q;

    if ( db()->transaction() ){
        rc = createSystables();
        if ( !rc ) rc = execList( ql_update );
        if ( !rc ) {
            dd_sql = dd;
            rc = writeSqlDictionary();
        }
        if ( !rc ) db()->commit();
        else db()->rollback();
    } else rc = 1;
    return rc;
}



/*!
 * \en
 * \brief Return a drivers list.
 *
 * \return Drivers list.
 *
 *  \_en \ru
 * \brief Возвращает список доступных драйверов.
 *
 * \return Список драйверов
 * \_ru
 */
QStringList
QDataSchema::drivers()
{
    QDS_int i;
    QDataSchemaDriver *d;
    QStringList li, l;

    li << "QMYSQL";
    li << "QSQLITE";
    li << "QPOSTGRESQL";

    for (i=0; i<li.count(); i++) {
        d = createDriver( li[i] );
        if ( QSqlDatabase::isDriverAvailable( d->sqlDriverName() ) ) l << li[i];
        delete d;
    }
    return l;
}


/*!
 * \en
 * \brief Return a drivers list.
 *
 * \return Drivers list.
 *
 *  \_en \ru
 * \brief Возвращает список доступных драйверов.
 *
 * \return Список драйверов
 * \_ru
 */
QDataSchemaDriver *
QDataSchema::createDriver(const QString ctype){
    QDataSchemaDriver *drv = 0;

    if ( ctype == "QMYSQL" ) drv = new QDataSchemaDriverMySQL();
    if ( ctype == "QPOSTGRESQL" ) drv = new QDataSchemaDriverPostgreSQL();
    if ( ctype == "QSQLITE" ) drv = new QDataSchemaDriverSqlite();
    return drv;
}



/*!
 * \en
 * \brief Return a true if dataschema driver Name available.
 *
 * \arg name (in) driver name
 * \return true - driver avalable, false - unavalable.
 *
 *  \_en \ru
 * \brief Проверка доступности драйвера по имени.
 *
 * \arg name (in) имя драйвера
 * \return true - драйвер доступен, false - не доступен.
 *
 * \_ru
 */
bool
QDataSchema::isDriverAvailable ( const QString & name )
{
	return drivers().contains( name ) >0;
}



/*!
 * \en
 * \brief Return current QDataSchema driver.
 *
 * \_en \ru
 * \brief Возвращает текущий драйвер базы данных.
 *
 * \_ru
 */
QDataSchemaDriver *
QDataSchema::driver()
{
	return p_drv;
}



/*!
 *\en
 *\brief Create new database.
 *
 * Create new database with name was seted before.
 * For create database operation you are need to know
 * database server administrator name and password.
 *
 *\param dbadmuser (in) Database server administrator name
 *\param dbadmpass (in) Database server administrator password
 *\return true - if operation successfull, false - if operation unsuccessfull.
 *\_en \ru
 * \brief Создает новую базу данных.
 *
 * Создается новая база данных с ранее установленным именем.
 * Для создания новой базы данных необходимо знать имя и пароль
 * администратора сервера базы данных.
 *
 *\param dbadmuser (in) имя администратора сервера базы данных
 *\param dbadmpass (in) пароль администратора сервера базы данных
 *\return true - если операция выполнена успешно, false - не успешно.
 *\_ru
 */
bool
QDataSchema::databaseCreate( const QString &dbadmuser, const QString &dbadmpass )
{
    QString savename, savepass, savedb;
    bool res = false;

    if ( db() && driver() )
    {
        savename = db()->userName();
        savepass = db()->password();
        savedb = db()->databaseName();
        db()->setUserName( dbadmuser );
        db()->setPassword( dbadmpass );
        db()->setDatabaseName( driver()->defaultDatabaseName() );
        res = db()->open();
        if ( res ) {
            if ( execList( splitQString("\n", driver()->queryCreateDatabase( savedb ) ) ) ) res = false;
        }
        db()->setUserName( savename );
        db()->setPassword( savepass );
        db()->setDatabaseName( savedb );
    }
    return res;
}



/*!
 *\en
 *\brief Drop current database.
 *
 * Drop database with name was seted before.
 * For database drop operation you are need to know
 * database server administrator name and password.
 *
 *\param dbadmuser (in) Database server administrator name
 *\param dbadmpass (in) Database server administrator password
 *\return true - if operation successfull, false - if operation unsuccessfull.
 *\_en \ru
 *\brief Удаляет базу данных с сервера.
 *
 * Удаляется база данных с ранее установленным именем.
 * Для удаления базы данных необходимо знать имя и пароль
 * администратора сервера базы данных.
 *
 *\param dbadmuser (in) имя администратора сервера базы данных
 *\param dbadmpass (in) пароль администратора сервера базы данных
 *\return true - если операция выполнена успешно, false - не успешно.
 *\_ru
 */
bool
QDataSchema::databaseDrop( const QString &dbadmuser, const QString &dbadmpass )
{
    QString savename, savepass, savedb;
    bool res = false;

    if ( db() && driver() )
    {
        savename = db()->userName();
        savepass = db()->password();
        savedb = db()->databaseName();
        db()->setUserName( dbadmuser );
        db()->setPassword( dbadmpass );
        db()->setDatabaseName( driver()->defaultDatabaseName() );
        res = db()->open();
        if ( res ) {
            if ( execList( splitQString("\n", driver()->queryDropDatabase( savedb ) ) ) ) res = false;
        }
        db()->setUserName( savename );
        db()->setPassword( savepass );
        db()->setDatabaseName( savedb );
    }
    return res;
}



/*!
 *\en
 *\brief Check query result.
 *
 *\_en \ru
 *\brief Проверяет результат выполнения запроса.
 *
 * Если была ошибка, она выводится на стандартный вывод.
 *\arg query (in) запрос, выполненный ранее.
 *\return 0 - без ошибок, 1 - есть ошибка.
 *
 *\_ru
 */
int
QDataSchema::checkSqlError( QSqlQuery &query )
{
    QString err="";
#if QT_VERSION<0x040000
    if (query.lastError().type()!=QSqlError::None)
#else
    if (query.lastError().type()!=QSqlError::NoError)
#endif
    {
        err = QString("SQLError %1 %2\n").arg(query.lastError().databaseText()).arg(query.lastError().driverText());
#if QT_VERSION<0x040000
        fprintf(stderr, err.ascii());
#else
        fprintf(stderr, err.toAscii());
#endif
        return 1;
    }
    return 0;
}



/*!
 * \en
 *
 * \_en \ru
 * \brief Выполняет список последовательных запросов.
 *
 * \arg queryList (in) список строк запросов
 * \arg inTransaction (in) выполнять в транзакции. Если true - выполняет
 * список запросов в транзакции с автоматическим откатом в случае ошибки.
 * \return 0 - выполнено без ошибок, >0 - код ошибки Sql.
 * \_ru
 */
int
QDataSchema::execList( const QStringList &queryList, bool inTransaction )
{
    QSqlQuery query;
    QDS_int i, rc = 0;

        if ( inTransaction ) db()->transaction();
	for ( i=0; i< queryList.count(); i++ ) {
//		printf("QUERY %i:%s\n", i, (const char *) queryList[i] );
		query = db()->exec( queryList[ i ] );
		rc = checkSqlError( query );
                if ( rc ) {
#if QT_VERSION<0x040000
                    printf("ERROR ON QUERY %i:%s\n", i, (const char *) queryList[i] );
#else
                    printf("ERROR ON QUERY %i:%s\n", i, (const char *) queryList[i].toAscii() );
#endif
                    break;
                }
	}
        if ( rc && inTransaction ) db()->rollback();
        if ( !rc && inTransaction ) db()->commit();
	return rc;
}



/*!
 *\en
 *\brief
 *
 *\_en \ru
 *\brief
 *
 *\_ru
 */
int
QDataSchema::readSqlDictionary()
{
    QSqlQuery query;
    int rc = 0;

    dd_sql.clear();
    if ( tableExists( tableName( dd_name ) ) ){
        query = db()->exec( QString("select id,value from %1 order by id").arg( tableName( dd_name ) ) );
        rc = checkSqlError( query );
        if ( !rc ) {
            while ( query.next() ) {
                dd_sql << QString(trimmedQString(query.value(1).toString()));
            }
#ifdef DEBUG
            printf("sql_dd=\n%s\n", (const char *) dd_sql.join("\n"));
#endif
        }
    }
    return rc;
}



/*!
 *\en
 *\brief
 *
 *\_en \ru
 *\brief
 *
 *\_ru
 */
int
QDataSchema::writeSqlDictionary()
{
    QStringList l;
    QDS_int i, rc=0;

    if ( tableExists( tableName( dd_name ) ) ){
        l << QString("delete from %1").arg( tableName( dd_name ) );
        for (i=0; i<dd_sql.count(); i++){
            l << QString("insert into %1 (id, value) values (%2,'%3')").arg( tableName( dd_name ) ).arg(i+1).arg(dd_sql[i].left(250));
        }
        rc = execList( l );
    } else rc = 1;
    return rc;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Возвращает запись словаря по индексу.
 *
 * \param idx (in) индекс записи словаря"
 * \param s_sql (in) используемый словарь
 *          false - пользовательский словарь (по умолчанию)
 *          true - словарь, хранимый в базе данных
 * \return запись словаря.
 * \_ru
 */
QString
QDataSchema::ddRecord( int idx, bool d_sql )
{
    QStringList *pdd;
    if ( d_sql ) pdd = &dd_sql; else pdd = &dd;
    if ( idx>=0 && idx < (int)pdd->count() ) return (*pdd)[idx];
    return "";
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Количество записей выбранного вида в словаре.
 *
 * \param etype (in) тип элемента словаря в виде "[T/D]"
 * \param s_sql (in) используемый словарь
 *          false - пользовательский словарь (по умолчанию)
 *          true - словарь, хранимый в базе данных
 * \return количество записей выбранного типа в словаре.
 *
 * \_ru
 */
int
QDataSchema::ddCount( const QString &etype, bool d_sql )
{
    QDS_int i, ec=0;
    QString s;
    QStringList *cdd;

    if ( d_sql ) cdd = &dd_sql; else cdd = &dd;

    for ( i = 0; i< cdd->count(); i++ ){
        if ( ddRecType( (*cdd)[i] ) == etype ) ec++;
    }
    return ec;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Возвращает индекс элемента словаря.
 *
 * \param etype (in) тип элемента словаря в виде "[T/D]"
 * \param s_sql (in) используемый словарь
 *          false - пользовательский словарь (по умолчанию)
 *          true - словарь, хранимый в базе данных
 * \return Индекс строки элемента с словаре.
 *
 * \_ru
 */
int
QDataSchema::ddIndex( const QString &etype, int num, bool d_sql )
{
    QDS_int i;
    int ec=-1;
    QStringList *cdd;

    if ( d_sql ) cdd = &dd_sql; else cdd = &dd;

    for ( i = 0; i< cdd->count(); i++ ){
        if ( ddRecType( (*cdd)[i] ) == etype ) ec++;
        if ( ec == num ) return i;
    }
    return -1;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Возвращает индекс элемента словаря.
 *
 * \param etype (in) тип элемента словаря в виде "[T/D]"
 * \param s_sql (in) используемый словарь
 *          false - пользовательский словарь (по умолчанию)
 *          true - словарь, хранимый в базе данных
 * \return Индекс строки элемента с словаре.
 *
 * \_ru
 */
int
QDataSchema::ddIndex( const QString &etype, const QString &name, bool d_sql )
{
    int i, c, idx;

    c = ddCount( etype, d_sql );
    for ( i=0; i < c; i++ ){
        idx = ddIndex( etype, i, d_sql );
        if ( lowerQString(ddRecName( ddRecord( idx, d_sql ) )) == lowerQString(name) ) return idx;
    }
    return -1;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief  Количество подчиненных записей выбранного вида в словаре.
 *
 * \param idx (in) индекс главного элемента словаря, полученный
 *                  вызовом функции ddIndex()
 * \param etype (in) тип элемента словаря в виде "[T/D]"
 * \param s_sql (in) используемый словарь
 *          false - пользовательский словарь (по умолчанию)
 *          true - словарь, хранимый в базе данных
 * \return Количество подчиненных записей выбранного вида в словаре..
 *
 * \_ru
 */
int
QDataSchema::ddCountSub( int idx, const QString &etype, bool d_sql )
{
    QDS_int i, ec=0;
    QString s;
    QStringList *cdd;

    if ( d_sql ) cdd = &dd_sql; else cdd = &dd;

    for ( i = idx+1; i< cdd->count(); i++ ){
        s = ddRecType((*cdd)[i]);
        if ( s=="T" || s=="D" ) break;
        if ( s == etype ) ec++;
    }
    return ec;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Возвращает индекс подчиненного элемента словаря.
 *
 * \param idx (in) индекс главного элемента словаря, полученный
 *                  вызовом функции ddIndex()
 * \param etype (in) тип подчиненного элемента словаря в виде "[F/I]"
 * \param s_sql (in) используемый словарь
 *          false - пользовательский словарь (по умолчанию)
 *          true - словарь, хранимый в базе данных
 * \return Индекс строки подчиненного элемента с словаре.
 *
 * \_ru
 */
int
QDataSchema::ddIndexSub( int idx, const QString &etype, int num, bool d_sql )
{
    QDS_int i;
    QString s;
    int ec=-1;
    QStringList *cdd;

    if ( d_sql ) cdd = &dd_sql; else cdd = &dd;

    for ( i = idx+1; i< cdd->count(); i++ ){
        s = ddRecType((*cdd)[i]);
        if ( s=="T" || s=="D" ) break;
        if ( s == etype ) ec++;
        if ( ec == num ) return i;
    }
    return -1;
}



/*!
 * \en
 * \brief
 *
 *  \_en \ru
 * \brief Возвращает индекс подчиненного элемента словаря.
 *
 * \param idx (in) индекс главного элемента словаря, полученный
 *                  вызовом функции ddIndex()
 * \param etype (in) тип подчиненного элемента словаря в виде "[F/I]"
 * \param s_sql (in) используемый словарь
 *          false - пользовательский словарь (по умолчанию)
 *          true - словарь, хранимый в базе данных
 * \return Индекс строки подчиненного элемента с словаре.
 *
 * \_ru
 */
int
QDataSchema::ddIndexSub( int idx, const QString &etype, const QString &name, bool d_sql )
{
    int i, c, idxs;

    c = ddCountSub( idx, etype, d_sql );
    for ( i=0; i < c; i++ ){
        idxs = ddIndexSub( idx, etype, i, d_sql );
        if ( lowerQString(ddRecName( ddRecord( idxs, d_sql ) )) == lowerQString(name) ) return idxs;
    }
    return -1;
}



/*!
 * \en
 * \brief
 * \brief Return dictionary element record section.
 *
 * \param rec (in) record (line) of the dictionary
 * \param secnum (in) section number, starts from 0. Sections divide by "|" symbol.
 * \return string of the section without head and trale spaces.
 *  \_en \ru
 * \brief Возвращает секцию записи элемента словаря.
 *
 * \param rec (in) запись (строка) словаря
 * \param secnum (in) номер секции, начиная с 0. Секции разделены символом "|"
 * \return Строка секции без начальных и конейных пробелов.
 * \_ru
 */
QString
QDataSchema::ddRecSection( const QString &rec, int secnum )
{
    return trimmedQString(rec.section("|", secnum, secnum ));
}



/*!
 * \en
 * \brief Return type of dictionary element record.
 *
 * \param rec (in) record (line) of the dictionary
 * \return Type of dictionary element record.
 *  \_en \ru
 * \brief Возвращает тип записи элемента словаря.
 *
 * \param rec (in) запись (строка) словаря
 * \return Тип записи элемента словаря в виде [D/T/F/I/#].
 * \_ru
 */
QString
QDataSchema::ddRecType( const QString &rec )
{
#if QT_VERSION<0x040000
    if ( trimmedQString(rec)[0].latin1()=='#' ) return "#";
#else
    if ( trimmedQString(rec)[0].toLatin1()=='#' ) return "#";
#endif
    return trimmedQString(ddRecSection( rec, 0 ).section("=", 0, 0 ));
}



/*!
 * \en
 * \brief Return name of dictionary element record.
 *
 * \param rec (in) record (line) of the dictionary
 * \return Name of dictionary element record.
 *  \_en \ru
 * \brief Возвращает имя записи элемента словаря.
 *
 * \param rec (in) запись (строка) словаря
 * \return Имя записи элемента словаря.
 * \_ru
 */
QString
QDataSchema::ddRecName( const QString &rec )
{
    return trimmedQString(ddRecSection( rec, 0 ).section("=", 1, 1 ));
}



/*!
 * \en
 * \brief Append one string list to another.
 *
 * \param list (in) string list for expansion
 * \param add (in) string list for add to "list"
 * \_en \ru
 * \brief Добавляет один список строк к другому.
 *
 * \param list (in) список, в который добавляются строки
 * \param add (in) список для добавления
 * \_ru
 */
void
QDataSchema::joinLists( QStringList &list, const QStringList &add )
{
    QDS_int i;
    for (i=0; i<add.count(); i++ ) list << add[i];
}



/*!
 * \en
 * \brief Return queries list for database update.
 *
 * Queries list prepares while verifyStructure()
 * executes.
 *
 * \_en \ru
 * \brief Возвращает список запросов для обновления БД.
 *
 * Список запросов формируется при выполнении
 * verifyStructure().
 * \_ru
 */
QStringList
QDataSchema::updateStructureQuery()
{
    return ql_update;
}



/*!
 * \en
 * \brief Return list of differents.
 *
 * It is show differents in data base and application dictionaries.
 * \return list of differents
 *
 * \_en \ru
 * \brief Возвращает список изменений в БД.
 *
 * Список различий формируется при выполнении
 * verifyStructure().
 * \return Список различий.
 * \_ru
 */
QStringList
QDataSchema::verifyLog()
{
    return dd_update;
}



/*!
 * \en
 * \brief Register record of change in database.
 *
 * Calls when differenses list forms on execution
 * verifyStructure().
 * Generates verifyMessage() signal.
 *
 * \param op (in) operation:
 *      "+" - add,
 *      "-" - delete,
 *      "&" - change
 * \param value (in) new value of changed dictionary element
 * \param oldvalue (in) old value of changed dictionary element.
 * Empty for add and delete operations.
 *
 * \_en \ru
 * \brief Регистрирует запись изменений в БД.
 *
 * Вызывается при формировании списка различий при выполнении
 * verifyStructure().
 * генерирует сигнал verifyMessage().
 *
 * \param op (in) операция:
 *      "+" - добавление,
 *      "-" - удаление
 *      "&" - изменение
 * \param value (in) новое значение измененяемого элемента словаря
 * \param oldvalue (in) старое значение измененяемого элемента словаря.
 * Пустое для операций добавления и удаления.
 * \_ru
 */
void
QDataSchema::verifyLogRecord(QString op, QString value, QString oldvalue )
{
    dd_update << op+value;
    if ( op=="&" && ddRecType( value ) == "F" ) dd_update << "<"+oldvalue;
    emit verifyMessage( op, value, oldvalue );
}



/*!
 * \en
 * \brief Unload information from database.
 *
 * \param filename (in) name of file for data exchange
 * \_en \ru
 * \brief Выгружает информацию из базы данных.
 *
 * \param filename (in) имя файла обмена
 * \_ru
 */
int
QDataSchema::databaseExport( const QString &filename )
{
    QFile f( filename );
    QString s, ts, es;
    QString tname, tname_db, ename, etype;
    int res = 0;
    int ti, tcount, ei, ecount;
    int tidx;
    QSqlQuery query;
    QStringList fields, fieldtypes;

    QDomDocument doc( "qdataschema" );
    QDomElement root, docElem, e, et, er, ef;
    QDomText etext;

    if ( f.open( QDS_IO_WriteOnly ) ) {
        QTextStream t ( &f );
        t.setCodec(QTextCodec::codecForName("UTF-8"));
        if ( verifyStructure()==0 ) {
            root = doc.createElement("qdataschema");
            doc.appendChild( root );
            e = doc.createElement("datadictionary");
            root.appendChild( e );
            root.setAttribute("charset", "UTF-8");
            root.setAttribute("databaseName", db()->databaseName() );
            etext = doc.createTextNode( dataDictionary().join("\n") );
            e.appendChild( etext );
            e = doc.createElement("data");
            root.appendChild( e );

            // Выгрузим таблицы
            tcount = ddCount("T");
            for ( ti = 0; ti<tcount; ti++){
                tidx = ddIndex("T", ti );
                ts = dd[tidx];
                tname = ddRecName( ts ); // имя таблицы в словаре
                tname_db = tableName( tname ); //Имя таблицы в базе данных с префиксом имени
                et = doc.createElement("table");
                e.appendChild( et );
                et.setAttribute("name", tname );
                fields.clear();
                fieldtypes.clear();
                // Подготовим список полей
                ecount = ddCountSub( tidx, "F" );
                for ( ei = 0; ei < ecount; ei++ ){
                    es = ddRecord( ddIndexSub( tidx, "F", ei ) );
                    ename = ddRecName( es );
                    etype = ddRecSection( es, 2 )+"|"+ddRecSection( es, 3 )+"|"+ddRecSection( es, 4 );
                    fields << ename;
                    fieldtypes << etype;
                }
                query = db()->exec(QString("select %1 from %2").arg(fields.join(",")).arg(tname_db));
                if ( checkSqlError( query ) == 0) {
                    while ( query.next() ) {
                        er = doc.createElement("record");
                        et.appendChild( er );
                        for ( ei = 0; ei < ecount; ei++ ){
                            ef = doc.createElement("field");
                            er.appendChild( ef );
                            ef.setAttribute("name", fields[ei]);
                            ef.setAttribute("type", fieldtypes[ei]);
                            ef.setAttribute("value", query.value(ei).toString());
                        }
                    }
                }
            }
            t << doc.toString( 4 );
        } else res = 1;
        f.close();
    }
    return res;
}



/*!
 * \en
 * \brief Load information into data base.
 *
 * \param filename (in) name of file for data exchange
 * \param updateStruct (in) true - update structure, if it is different from
 * existence, false - to return error code.
 * \_en \ru
 * \brief Загружает информацию в базу данных.
 *
 * \param filename (in) имя файла обмена
 * \param updateStruct (in) true - надо обновлять структуру, если она отличается от
 * существующей, false - возвращать код ошибки
 * \_ru
 */
int
QDataSchema::databaseImport( const QString &filename, bool updateStruct )
{
    QFile f( filename );
    QString s, ts, es;
    QString tname, tname_db, ename, edata, etype, etypec;
    int res = 0;
    QSqlQuery query;
    QStringList fields, values, dd, savedd, querylist;
    QDomDocument doc( "qdataschema" );
    QDomElement docElem, e, et, er, ef;
    QDomNode n, nt, nr, nf;
    QString data;

    savedd = dataDictionary();
    if ( f.open( QDS_IO_ReadOnly ) ) {
        QTextStream t ( &f );
        t.setCodec(QTextCodec::codecForName("UTF-8"));
        data = QTextStream_readAll( t );
        if ( doc.setContent( data ) ) {
            docElem = doc.documentElement();
            n = docElem.firstChild();
            while( !n.isNull() ) {
                e = n.toElement();
                if( !e.isNull() ) {
                    if ( e.tagName() == "datadictionary") {
                        dd = splitQString("\n",e.text());
                        setDataDictionary( dd );
                        if ( verifyStructure() ) {
                            if ( updateStruct ) res = updateStructure();
                            else res = 1;
                        }
                        if ( res ) break;
                    }
                    if ( e.tagName() == "data") {
                        nt = e.firstChild();
                        while( !nt.isNull() ) {
                            et = nt.toElement();
                            if ( et.tagName() == "table") {
                                // Подготовим добавление в таблицу
                                tname = et.attribute("name");
                                tname_db = tableName( tname );
                                // Очистим таблицу от старых данных
                                querylist << QString("delete from %1").arg(tname_db);
                                //cout << QString("delete from %1").arg(tname_db) <<endl;
                                nr = et.firstChild();
                                while( !nr.isNull() ) {
                                    er = nr.toElement();
                                    if ( er.tagName() == "record") {
                                        // Загрузим значения полей записи
                                        fields.clear();
                                        values.clear();
                                        nf = nr.firstChild();
                                        while( !nf.isNull() ) {
                                            ef = nf.toElement();
                                            if ( ef.tagName() == "field") {
                                                ename = ef.attribute("name");
                                                edata = ef.attribute("value");
                                                etype = ef.attribute("type");
//                                                cout << ef.tagName() << endl;
                                                etypec = etype.section("|",0, 0);
                                                if ( etypec == "C" || etypec=="D" ) {
                                                    edata = "'"+edata+"'";
                                                } else {
                                                    if (edata.isEmpty()) edata="null";
                                                }
                                                fields << ename;
                                                values << edata;
                                            }
                                            nf = nf.nextSibling();
                                        }
                                        // Добавим запись
                                        querylist << QString("insert into %1 (%2) values (%3)").arg(tname_db).arg( fields.join(",") ).arg( values.join(","));
                                    }
                                    nr = nr.nextSibling();
                                } // records
                            }
                            nt = nt.nextSibling();
                        } // tables
                    }
                }
                n = n.nextSibling();
            }
        }
        f.close();
    }
    // Выполним список запросов импорта информации
    if (!res) {
        res = execList( querylist, true );
    }
    // Восстановим старый словарь в случае ошибки импорта
    if ( res ) setDataDictionary( savedd );
    return res;
}



/*!
 * \en
 * \_en \ru
 * \_ru
 */
QStringList QDataSchema::splitQString(const QString &div, const QString &str )
{
#if QT_VERSION<0x040000
    return QStringList::split( div, str );
#else
    return str.split( div );
#endif
}



/*!
 * \en
 * \_en \ru
 * \_ru
 */
QString QDataSchema::lowerQString(const QString &str )
{
#if QT_VERSION<0x040000
    return str.lower();
#else
    return str.toLower();
#endif
}



/*!
 * \en
 * \_en \ru
 * \_ru
 */
QString QDataSchema::trimmedQString(const QString &str ) {
#if QT_VERSION<0x040000
    return str.stripWhiteSpace();
#else
    return str.trimmed();
#endif
}
