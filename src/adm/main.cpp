#if QT_VERSION<0x040000
    #include <qapplication.h>
#else
    #include <qcoreapplication.h>
#endif
#include <qtextcodec.h>
#include <qfile.h>
#include <qvariant.h>

#include <qdataschema.h>

void help()
{
            printf( "Usage: qdsadm [command] [options]\n"
		    "Commands are:\n"
		    "help, verify, update, export, import\n"    
		    "Options are:\n"
                    "u=dbuser (default:empty)\n"
                    "p=dbpassword (default:empty)\n"
                    "t=dbtype [internal/mysql/postgresql] (default:internal)\n"
                    "d=data-dictionary-name (default:qds.dd)\n"
                    "s=dbserver (default:localhost)\n"
                    "n=dbname (default:exampledb)\n"
                    "o=dbport (default:0 = use default port)\n"
		    "f=filename (default:dump.xml)\n"
                  );
            printf("\nDrivers avalable: %s\n", toLocal8Bit( QDataSchema::drivers().join(", ") ) );
}

int main(int argc, char * argv[])
{
#if QT_VERSION<0x040000
    QApplication app(argc,argv);
#else
    QCoreApplication app(argc,argv);
#endif
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QStringList l;
    QString dbtype="QSQLITE";

    QString ddname="qds.dd";
    QString dtype="internal";
    QString dbuser="",dbpasswd="",dbname="exampledb",
            dbserver="localhost";
    QString cmd = "help", filename="dump.xml";
    QString s1, s2;
    int dbport = 0;
    printf("QDataSchema administrator. (c) 2004-2007 LeaderIT, Orenburg, Russia\n"
           "use help for arguments description\n");
    for (int i=1;i<argc; i++){
        s1=QString(argv[i]);
//        if (s1=="--help") help();
        s2=s1.section("=",1,1);
        s1= toLower( s1.section("=",0,0) );
        if (s1=="u") dbuser=s2;
        if (s1=="p") dbpasswd=s2;
        if (s1=="t") dtype=s2;
        if (s1=="d") ddname=s2;
        if (s1=="s") dbserver=s2;
        if (s1=="n") dbname=s2;
        if (s1=="o") dbport=s2.toInt();
	if (s2.isEmpty()) cmd = s1;
    }

    if (cmd=="help"){
	help();
	return 0;
    }
    QFile buf( ddname );
    if ( !buf.open( QDS_IO_ReadOnly ) ) {
	printf("ERROR: Open dictionary file\n");
	return 1;
    }
    QTextStream ts( &buf );
    l =  split( "\n", QTextStream_readAll(ts) );
    buf.close();
    if ( dtype=="internal") dbtype="QSQLITE";
    if ( dtype=="mysql") dbtype="QMYSQL";
    if ( dtype=="postgresql") dbtype="QPOSTGRESQL";


    QDataSchema adb( dbtype );
    adb.setDataDictionary( l );
    if ( adb.open( dbname,dbuser,dbpasswd,dbserver,dbport ) )
    {
	if (cmd=="verify") adb.verifyStructure();
	if (cmd=="update"){
	    adb.verifyStructure();
	    adb.updateStructure();
	}
	if (cmd=="export"){
    	    adb.databaseExport( filename );
	}
	if (cmd=="import"){
    	    adb.databaseImport( filename, true );
	}
	adb.close();	
    } else printf("ERROR INIT DATABASE\n");
    return 0;
}
