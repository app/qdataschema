#include <qapplication.h>
#include <qtextcodec.h>
#include <qdom.h>
#include <qfile.h>
#include <qvariant.h>

#include <qdataschema.h>

void cfg_message(int, char const *, ...) {};

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));
    QDataSchema adb;
    adb.init("internal","odbx","root","","localhost",0);
    ObjectDB odb(&adb,"red75");
    QFile in("db.xml");
    in.open(IO_ReadOnly);
    QDomDocument dbsch;
    dbsch.setContent(&in);
    deserializeFromXML(odb,dbsch);
    OType * pt;
    pt=odb.addType("����","people");
    pt->addField("�������","string");
    pt->addField("���","string");
    pt->addField("��������","string");
    pt->addField("������������","date");
    pt->addField("�������","int");
    pt=odb.addType("�����","accounts");
    pt->addField("����","string");
    pt->addField("����������","string");
    pt=odb.addType("�����������","eprize");
    pt->addField("��������","string");
    pt->addField("�����","string");
    pt->addField("�������","string");
    pt->addField("���","string(30)");
    pt->addField("���","string(30)");
    pt->addField("����","�����");
    pt=odb.addType("����������","employee");
    pt->addField("���","�����������");
    pt->addField("���","����");
    pt->addField("���������","string");
    odb.updateDatabase();
    OList olist=odb.list("����������");
    olist.and_c(olist.chain().link("���").link("�������"),OC_EQ,1);
    olist.and_c(olist.chain().link("���").link("������������"),OC_BETWEEN,2,3);
    olist.and_c(olist.chain().link("���").link("�������"),OC_BETWEEN,4,5);
    olist.and_c(olist.chain().link("���").link("�����"),OC_EQ,6);
    olist.and_c(olist.chain().link("���").link("����"),OC_EQ,7);
    QString sqlq=olist.query();
    printf("Query string: %s\n",sqlq.ascii());
    QDomDocument ddc("DataSchema");
    serializeToXML(odb,ddc);
    QFile out("db.xml");
    out.open(IO_WriteOnly);
    QTextStream ots(&out);
    ddc.save(ots,4);
    out.close();
	char buf[sizeof(QString)];
	QString * str=new (buf) QString;
	*str="Allocated";
	printf("%s",str->ascii());
	OVariant ov[1024];
	int i;
	for(i=0;i<1024;i++)
		ov[i]=QString().fill('2',2048);
	for(i=0;i<1024;i++)
		ov[i]=0;
	printf("Size of QVariant is %d\nSize of OVariant is %d\n",sizeof(QVariant),sizeof(OVariant));
}
