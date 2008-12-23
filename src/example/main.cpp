#include <qapplication.h>
#include <qtextcodec.h>
#include <qdom.h>
#include <qfile.h>
#include <qvariant.h>

#include <qdataschema.h>

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QStringList l;
    QString dbtype="QSQLITE";

    QString ddname="qds.dd";
    QString dtype="internal";
    QString dbuser="",dbpasswd="",dbname="exampledb",
            dbserver="localhost";
    QString s1, s2;
    QString cmd="help";
    int dbport = 0;

    printf("QDataSchema example. (c) 2004-2007 LeaderIT, Orenburg, Russia\n"
           "use --help for arguments description\n");
    for (int i=1;i<argc; i++){
        s1=QString(argv[i]);
        if (s1=="--help"){
            printf(
                    "u=dbuser (default:empty)\n"
                    "p=dbpassword (default:empty)\n"
                    "t=dbtype [internal/mysql/postgresql] (default:internal)\n"
                    "d=data-dictionary-name (default:qds.dd)\n"
                    "s=dbserver (default:localhost)\n"
                    "n=dbname (default:exampledb)\n"
                    "o=dbport (default:0 = use default port)\n"
                  );
            return 0;
        }
        s2=s1.section("=",1,1);
        s1=s1.section("=",0,0).lower();
        if (s1=="u") dbuser=s2;
        if (s1=="p") dbpasswd=s2;
        if (s1=="t") dtype=s2;
        if (s1=="d") ddname=s2;
        if (s1=="s") dbserver=s2;
        if (s1=="n") dbname=s2;
        if (s1=="o") dbport=s2.toInt();
    }

    QFile buf( ddname );
    if ( !buf.open( IO_ReadOnly ) ){
        printf("ERROR: Open dictionary file\n");
        return 0;
    };
    QTextStream ts( &buf );
    l = QStringList::split("\n",ts.read());
    buf.close();
    if ( dtype=="internal") dbtype="QSQLITE";
    if ( dtype=="mysql") dbtype="QMYSQL";
    if ( dtype=="postgresql") dbtype="QPOSTGRESQL";

    QDataSchema adb( dbtype );
    adb.setDataDictionary( l );
//    adb.setNameSpace("example1_");
    if ( adb.open( dbname,dbuser,dbpasswd,dbserver,dbport ) )
    {
        adb.verifyStructure();
        printf("verify log:\n%s\n", ( const char * ) adb.verifyLog().join("\n") );
        printf("update structure query:\n%s\n", ( const char * ) adb.updateStructureQuery().join("\n") );
        adb.updateStructure();

        adb.databaseExport("export.txt");
        adb.close();
        adb.setNameSpace("example2_");
        adb.open();
        adb.databaseImport("export.txt", true );
        adb.close();
    } else printf("ERROR INIT DATABASE\n");
    return 0;
}
