/****************************************************************************
** $Id: qdataschemaglobal.cpp,v 1.4 2008/11/09 20:03:16 leader Exp $
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
**
**********************************************************************/

/*!
 * \en
 * \mainpage QDataSchema - class for updates application and database server data structure actualy.
 *
 * \section intro Introduce
 *
 * Used for support syncronous data structure in application
 * and database server. Application data structure describes in
 * database server independed context. In depends of the
 * database server, chose method for structure updates if
 * structure of the application information was changed ( for
 * example, we wer updated application version ) and we need
 * updates data base structure with save information, that
 * was collected earlier.
 *
 * \section sql_servers Database servers supports:
 *
 * \li \b SQLite
 * \li \b MySQL
 * \li \b PostgreSQL
 *
 * \section futures Futures:
 *
 * \li (in translate)
 * \li (in translate)
 *
 * \section copyright Developers
 *
 * \_en \ru
 * \mainpage QDataSchema - класс для поддержки синхронной структуры базы данных
 *
 * \section intro Введение
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
 * \section sql_servers Поддерживаемые базы данных:
 *
 * \li \b SQLite
 * \li \b MySQL
 * \li \b PostgreSQL
 *
 * \section futures Возможности:
 *
 * \li Поддерживает работу с пространствами имен. Это позволяет
 * в одной базе данных иметь несколько схем в разных пространствах
 * имен, то есть фактически несколько виртуальных баз данных внутри
 * одной физической.
 * \li Поддерживает экспорт содержимого базы данных в серверонезависимом
 * формате, что позволяет легко переносить данные между поддерживаемыми
 * серверами баз данных.
 *
 *
 * \section copyright Разработчики
 *
 * \_ru
 *
 * (c) 2004-2007 Leader IT, Orenburg, Russia
 *
 * (c) 2004-2007 Valery Grazdankin, Orenburg, Russia
 *
 */

#include "qdataschemaglobal.h"

// Шаблон для вставки версионно-зависимого кода.
#if QT_VERSION>=0x040000
// QT4 code
#else
// QT3 code

#endif

/*!
 *\class QDS qdsglobal.h
 *\en
 * \brief The QDS class is a namespace for miscellaneous
 * identifiers that need to be global-like in QDataSchema
 * objects.
 *
 *
 *\_en \ru
 * \brief Пространство имен QDataSchema.
 *
 *
 * \_ru
 *
 */
int
QDS::version()
{
    return QDS_VERSION;
}


QChar
toLower(QChar c)
{
#if QT_VERSION>=0x040000
// QT4 code
    return c.toLower();
#else
// QT3 code
    return c.lower();
#endif   
}


QString
toLower(const QString &s)
{
#if QT_VERSION>=0x040000
// QT4 code
    return s.toLower();
#else
// QT3 code
    return s.lower();
#endif   
}


char
toAscii(QChar c)
{
#if QT_VERSION>=0x040000
// QT4 code
    return c.toAscii();
#else
// QT3 code
    return c.latin1();
#endif   
}


int
indexOf( const QString &str, const QString &find_str )
{
#if QT_VERSION>=0x040000
// QT4 code
    return str.indexOf( find_str );
#else
// QT3 code
    return str.find( find_str );
#endif
}
   
const char *
toLocal8Bit(const QString &s)
{
#if QT_VERSION>=0x040000
// QT4 code
    return s.toLocal8Bit().data();
#else
// QT3 code
    return s.local8Bit();
#endif   
}


/*!
 * \en
 * \_en \ru
 * \_ru
 */
QStringList
split(const QString &div, const QString &str )
{
#if QT_VERSION>=0x040000
    return str.split( div );
#else
    return QStringList::split( div, str );
#endif
}


/*!
 * \en
 * \_en \ru
 * \_ru
 */
QString trimm(const QString &str ) {
#if QT_VERSION<0x040000
    return str.stripWhiteSpace();
#else
    return str.trimmed();
#endif
}
