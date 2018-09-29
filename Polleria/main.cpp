#include "mylibrary/mylibrary_global.h"
#include "mylibrary/_startprogram.h"

#include <QtXml>
#include "SoapSunatCall.h"
//#include <openssl/applink.c>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString response = "0100";
    cout<<response.toInt()<<endl;
/*
    SoapSunatCall obj;
    obj.set_str_keyfile((char *)"mykey.pem");
    obj.set_str_password((char *)"4t48gWXqdd358Msx");
    obj.set_str_cafile((char *)"myca.pem");
    obj.set_str_certfile((char *)"mycert.pem");
    obj.set_str_sec_username((char *)"20498590587ELBIBAM6");
    obj.set_str_sec_password((char *)"nArputo-");
    obj.set_str_zip_xml_filename((char *)"20498590587-03-B004-15.ZIP");
    obj.set_str_zip_xml_out_filename((char *)"res.zip");
    obj.set_str_xml_file((char *)"20498590587-03-B004-15.XML");


    if(obj.xmlDSig() < 0){
        qDebug() << "No se pudo firmar el doc" << endl;
    }else{
        qDebug() << "Se firmo el doc" << endl;
    }*/
/*
    obj.set_str_keyfile((char *)"mycertificate.pem");
    obj.set_str_password((char *)"4t48gWXqdd358Msx");
    obj.set_str_cafile((char *)"myca.pem");
    obj.set_str_certfile((char *)"mycert.pem");
    obj.set_str_sec_username((char *)"20498590587ELBIBAM6");
    obj.set_str_sec_password((char *)"nArputo-");
    obj.set_str_zip_xml_filename((char *)"20498590587-07-B011-1.ZIP");
    obj.set_str_zip_xml_out_filename((char *)"res.zip");
    obj.set_str_xml_file((char *)"20498590587-07-B011-1.XML");

    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    QString str_zip_dir = "\"" + app_dir + "\\20498590587-07-B011-1.zip\"";
    //QString str_xml_dir = "\"" + app_dir + "\\20498590587-07-B011-1.xml\"";
    QString command = "\"c:\\Program Files\\WinRAR\\WinRAR.exe\" a -afzip"
                      " "+str_zip_dir+" 20498590587-07-B011-1.xml";

    qDebug()<<command<<endl;
    QProcess::execute(command);

    if(obj.sendXML()<0){
        cout << "NOOO" << endl;
    }else{
        cout << "ha sido enviado a la sunat" <<endl;
    }
*/
    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    //QWidget w;
    //w.show();

    _StartProgram* s = new _StartProgram;
    s->run();

    return a.exec();
}



/*
    QString str_digestValue;
    QString str_signatureValue;
    QString str_X509SubjectName;
    QString str_X509Certificate;
    {
        QDomDocument dom;
        QFile file("firma.xml");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "no se pudo abrir el xml" << endl;
        }else{
            if(!dom.setContent(&file)){
                qDebug()<<"no se pudo cargar el documento"<<endl;
            }
            file.close();
        }
        QDomElement root = dom.firstChildElement("Invoice");
        QDomNodeList items = root.elementsByTagName("DigestValue");
        QDomNode itemnode = items.at(0);

        if(itemnode.isElement()){
            QDomElement iteme = itemnode.toElement();
            str_digestValue = iteme.text();
        }

        items = root.elementsByTagName("SignatureValue");
        itemnode = items.at(0);

        if(itemnode.isElement()){
            QDomElement iteme = itemnode.toElement();
            str_signatureValue = iteme.text();
        }
        items = root.elementsByTagName("X509SubjectName");
        itemnode = items.at(0);

        if(itemnode.isElement()){
            QDomElement iteme = itemnode.toElement();
            str_X509SubjectName = iteme.text();
        }
        items = root.elementsByTagName("X509Certificate");
        itemnode = items.at(0);

        if(itemnode.isElement()){
            QDomElement iteme = itemnode.toElement();
            str_X509Certificate = iteme.text();
        }
    }
    // write
    {
        //QDomDocument dom;
        QFile file("20498590587-03-B004-1430.xml");
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
            qDebug() << "no se pudo abrir el xml" << endl;
        }else{
            /*
            if(!dom.setContent(&file)){
                qDebug()<<"no se pudo cargar el documento"<<endl;
            }*/
            //file.close();
        /*}
        QString str_extContent;
        str_extContent  = "\n<ds:Signature Id=\"SignatureKG\">\n";
        str_extContent += "<ds:SignedInfo>\n";
        str_extContent += "<ds:CanonicalizationMethod Algorithm=\"http://www.w3.org/2001/10/xml-exc-c14n#\"/>\n";
        str_extContent += "<ds:SignatureMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#rsa-sha1\"/>\n";
        str_extContent += "<ds:Reference URI=\"\">\n";
        str_extContent += "<ds:Transforms>\n";
        str_extContent += "<ds:Transform Algorithm=\"http://www.w3.org/2000/09/xmldsig#envelopedsignature\"/>\n";
        str_extContent += "</ds:Transforms>\n";
        str_extContent += "<ds:DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/>\n";
        str_extContent += "<ds:DigestValue>"+str_digestValue+"=</ds:DigestValue>\n";
        str_extContent += "</ds:Reference>\n";
        str_extContent += "</ds:SignedInfo>\n";
        str_extContent += "<ds:SignatureValue>"+str_signatureValue+"</ds:SignatureValue>\n";
        str_extContent += "<ds:KeyInfo>\n";
        str_extContent += "<ds:X509Data>\n";
        str_extContent += "<ds:X509SubjectName>"+str_X509SubjectName+"</ds:X509SubjectName>\n";
        str_extContent += "<ds:X509Certificate>"+str_X509Certificate+"</ds:X509Certificate>\n";
        str_extContent += "</ds:X509Data>\n";
        str_extContent += "</ds:KeyInfo>\n";
        str_extContent += "</ds:Signature>";

        QString str = file.readAll();
        int index = str.indexOf("ext:ExtensionContent");
        str.insert(index + QString("ext:ExtensionContent>").length(), str_extContent);

        file.remove();
        file.setFileName("20498590587-03-B004-1430.xml");
        file.open(QIODevice::ReadWrite | QIODevice::Text);

        QTextStream out(&file);
        out << str;
        //file.write(str.toStdString().c_str(), str.length());


        file.close();
    }*/


