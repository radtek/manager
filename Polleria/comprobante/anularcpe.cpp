#include "anularcpe.h"
#include "ui_anularcpe.h"

AnularCPE::AnularCPE(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnularCPE)
{
    ui->setupUi(this);
}

AnularCPE::~AnularCPE()
{
    delete ui;
}

void AnularCPE::on_pushButton_ver_clicked()
{

}

void AnularCPE::on_lineEdit_motivo_textChanged(const QString &arg1)
{

}

void AnularCPE::on_pushButton_ok_clicked()
{

}

void AnularCPE::on_pushButton_cancelar_clicked()
{

}

bool AnularCPE::create_ncredito()
{
    QString str_ruc = "20498590587";
    QString str_razon = "POLLERIA EL POLLO LEAL E.I.R.L.";
    //QString str_razon = "20498590587";
    QString str_serie = ui->lineEdit_serie->text();
    QString str_numero = ui->lineEdit_numero->text();

    QString fileName = str_ruc+"-"+"07"+"-"+str_serie+"-"+str_numero+".XML";

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
      QMessageBox::warning(this, "Advertencia", "No se puede crear file.", "Aceptar");
      return false;
    }
    QSqlQuery query;
    QString str_query;
    str_query += "(SELECT id, nombre, serie, numero, fecha_emision";
    str_query += ", direccion, delivery, pago";
    str_query += ", delivery_estado_item_nombre, anulado, pago_item_nombre";
    str_query += ", estado_item_nombre, operacion_item_nombre, tipo_item_nombre";
    str_query += " FROM comprobante";
    str_query += " WHERE serie = '"+ui->lineEdit_serie_doc->text()+"'";
    str_query += " AND numero = '"+ui->lineEdit_numero_doc->text()+"')";
    str_query += " UNION ALL";
    str_query += "(SELECT per.cod, per.nombre, per.direccion";
    str_query += " FROM comprobante_has_persona com_h_per";
    str_query += " JOIN comprobante com ON com_h_per.comprobante_id = com.id";
    str_query += " JOIN persona per ON per.cod = com_h_per.persona_cod";
    str_query += " WHERE serie = '"+ui->lineEdit_serie_doc->text()+"'";
    str_query += " AND numero = '"+ui->lineEdit_numero_doc->text()+"')";
    str_query += " UNION ALL";
    str_query += "(SELECT SUM(com_h_prod.precio)";
    str_query += " FROM comprobante_has_producto com_h_prod";
    str_query += " JOIN comprobante com ON com_h_prod.comprobante_id = com.id";
    str_query += " WHERE serie = '"+ui->lineEdit_serie_doc->text()+"'";
    str_query += " AND numero = '"+ui->lineEdit_numero_doc->text()+"')";

    QString str_ruc_cus;
    QString str_razon_cus;
    QString str_direccion_cus;
    QString str_tipo_doc;
    double total = 0.0;
    if(query.exec(str_query)){
        query.next();
        str_tipo_doc = query.value(13).toString();
        query.next();
        str_ruc_cus = query.value(0).toString();
        str_razon_cus = query.value(0).toString();
        str_direccion_cus = query.value(0).toString();
        query.next();
        total = query.value(0).toDouble();

    }else{
        return false;
    }
    QString textXML;
    //textXML += "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\"?>\n";
    textXML += "<CreditNote xmlns=\"urn:oasis:names:specification:ubl:schema:xsd:CreditNote-2\"\n";
    textXML += "xmlns:cac=\"urn:oasis:names:specification:ubl:schema:xsd:CommonAggregateComponents-2\"\n";
    textXML += "xmlns:cbc=\"urn:oasis:names:specification:ubl:schema:xsd:CommonBasicComponents-2\"\n";
    textXML += "xmlns:ccts=\"urn:un:unece:uncefact:documentation:2\"\n";
    textXML += "xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\"\n";
    textXML += "xmlns:ext=\"urn:oasis:names:specification:ubl:schema:xsd:CommonExtensionComponents-2\"\n";
    textXML += "xmlns:qdt=\"urn:oasis:names:specification:ubl:schema:xsd:QualifiedDatatypes-2\"\n";
    textXML += "xmlns:sac=\"urn:sunat:names:specification:ubl:peru:schema:xsd:SunatAggregateComponents-1\"\n";
    textXML += "xmlns:udt=\"urn:un:unece:uncefact:data:specification:UnqualifiedDataTypesSchemaModule:2\"\n";
    textXML += "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n";
    textXML += "<ext:UBLExtensions>\n";
    textXML += "<ext:UBLExtension>\n";
    textXML += "<ext:ExtensionContent>\n";
    textXML += "<sac:AdditionalInformation>\n";
    textXML += "<sac:AdditionalMonetaryTotal>\n";
    textXML += "<cbc:ID>1001</cbc:ID>\n";
    textXML += "<cbc:PayableAmount currencyID=\"PEN\">"+QString().setNum(total/1.18, ' ', 2)+"</cbc:PayableAmount>\n";
    textXML += "</sac:AdditionalMonetaryTotal>\n";
    textXML += "</sac:AdditionalInformation>\n";
    textXML += "</ext:ExtensionContent>\n";
    textXML += "</ext:UBLExtension>\n";
    textXML += "<ext:UBLExtension>\n";
    textXML += "<ext:ExtensionContent>\n";
    textXML += "</ext:ExtensionContent>\n";
    textXML += "</ext:UBLExtension>\n";
    textXML += "</ext:UBLExtensions>\n";
    textXML += "<cbc:UBLVersionID>2.0</cbc:UBLVersionID>\n";
    textXML += "<cbc:CustomizationID>1.0</cbc:CustomizationID>\n";
    textXML += "<cbc:ID>"+ui->lineEdit_serie->text()+"-"+ui->lineEdit_numero->text()+"</cbc:ID>\n";
    textXML += "<cbc:IssueDate>"+QDate::currentDate().toString("yyyy-MM-dd")+"</cbc:IssueDate>\n";
    textXML += "<cbc:IssueTime>"+QTime::currentTime().toString("hh:mm:ss")+"</cbc:IssueTime>\n";
    textXML += "<cbc:DocumentCurrencyCode>PEN</cbc:DocumentCurrencyCode>\n";
    textXML += "<cac:DiscrepancyResponse>\n";
    textXML += "<cbc:ReferenceID>"+ui->lineEdit_serie_doc->text()+"-"+ui->lineEdit_numero_doc->text()+"</cbc:ReferenceID>\n";
    textXML += "<cbc:ResponseCode>01</cbc:ResponseCode>\n";
    textXML += "<cbc:Description>"+ui->lineEdit_motivo->text()+"</cbc:Description>\n";
    textXML += "</cac:DiscrepancyResponse>\n";
    textXML += "<cac:BillingReference>\n";
    textXML += "<cac:InvoiceDocumentReference>\n";
    textXML += "<cbc:ID>"+ui->lineEdit_serie_doc->text()+"-"+ui->lineEdit_numero_doc->text()+"</cbc:ID>\n";
    if(str_tipo_doc.compare(BOLETA) == 0)
        textXML += "<cbc:DocumentTypeCode>03</cbc:DocumentTypeCode>\n";
    else
        textXML += "<cbc:DocumentTypeCode>01</cbc:DocumentTypeCode>\n";
    textXML += "</cac:InvoiceDocumentReference>\n";
    textXML += "</cac:BillingReference>\n";
    textXML += "\n";

    textXML += "<cac:Signature>\n";
    textXML += "<cbc:ID>IDSignST</cbc:ID>\n";
    textXML += "<cac:SignatoryParty>\n";
    textXML += "<cac:PartyIdentification>\n";
    textXML += "<cbc:ID>"+str_ruc+"</cbc:ID>\n";
    textXML += "</cac:PartyIdentification>\n";
    textXML += "<cac:PartyName>\n";
    textXML += "<cbc:Name>"+str_razon+"</cbc:Name>\n";
    textXML += "</cac:PartyName>\n";
    textXML += "</cac:SignatoryParty>\n";
    textXML += "<cac:DigitalSignatureAttachment>\n";
    textXML += "<cac:ExternalReference>\n";
    textXML += "<cbc:URI>#SignatureSP</cbc:URI>\n";
    textXML += "</cac:ExternalReference>\n";
    textXML += "</cac:DigitalSignatureAttachment>\n";
    textXML += "</cac:Signature>\n";
    textXML += "<cac:AccountingSupplierParty>\n";
    textXML += "<cbc:CustomerAssignedAccountID>"+str_ruc+"</cbc:CustomerAssignedAccountID>\n";
    textXML += "<cbc:AdditionalAccountID>6</cbc:AdditionalAccountID>\n";
    textXML += "<cac:Party>\n";
    textXML += "<cac:PostalAddress>\n";
    textXML += "<cbc:AddressTypeCode>0001</cbc:AddressTypeCode>\n";
    textXML += "</cac:PostalAddress>\n";
    textXML += "<cac:PartyLegalEntity>\n";
    textXML += "<cbc:RegistrationName>"+str_razon+"</cbc:RegistrationName>\n";
    textXML += "</cac:PartyLegalEntity>\n";
    textXML += "</cac:Party>\n";
    textXML += "</cac:AccountingSupplierParty>\n";
    textXML += "<cac:AccountingCustomerParty>\n";
    QString str_codigo, str_nombre, str_direccion;
    str_codigo = str_ruc_cus;
    str_nombre = str_razon_cus;
    str_direccion = str_direccion_cus;
    textXML += "<cbc:CustomerAssignedAccountID>"+str_codigo+"</cbc:CustomerAssignedAccountID>\n";
    if(str_codigo.length() == 11){
        textXML += "<cbc:AdditionalAccountID>6</cbc:AdditionalAccountID>\n";
    }else{
        return false;
    }
    //}else{
        //textXML += "<cbc:AdditionalAccountID>0</cbc:AdditionalAccountID>\n";
    //}
    textXML += "<cac:Party>\n";/*
    textXML += "<cac:PhysicalLocation>\n";
    textXML += "<cbc:Description>"+str_direccion+"</cbc:Description>\n";
    textXML += "</cac:PhysicalLocation>\n";
    */
    textXML += "<cac:PartyLegalEntity>\n";
    textXML += "<cbc:RegistrationName>"+str_nombre+"</cbc:RegistrationName>\n";
    textXML += "</cac:PartyLegalEntity>\n";
    textXML += "</cac:Party>\n";
    textXML += "</cac:AccountingCustomerParty>\n";
    textXML += "<cac:TaxTotal>\n";
    textXML += "<cbc:TaxAmount currencyID=\"PEN\">"+QString().setNum(total/1.18*0.18, ' ', 2)+"</cbc:TaxAmount>\n";
    textXML += "<cac:TaxSubtotal>\n";
    textXML += "<cbc:TaxAmount currencyID=\"PEN\">"+QString().setNum(total/1.18*0.18, ' ', 2)+"</cbc:TaxAmount>\n";
    textXML += "<cac:TaxCategory>\n";
    textXML += "<cac:TaxScheme>\n";
    textXML += "<cbc:ID>1000</cbc:ID>\n";
    textXML += "<cbc:Name>IGV</cbc:Name>\n";
    //textXML += "<cbc:TaxTypeCode>VAT</cbc:TaxTypeCode>\n";
    textXML += "</cac:TaxScheme>\n";
    textXML += "</cac:TaxCategory>\n";
    textXML += "</cac:TaxSubtotal>\n";
    textXML += "</cac:TaxTotal>\n";
    textXML += "<cac:LegalMonetaryTotal>\n";
    textXML += "<cbc:PayableAmount currencyID=\"PEN\">"+QString().setNum(total, ' ', 2)+"</cbc:PayableAmount>\n";
    textXML += "</cac:LegalMonetaryTotal>\n";

    /*
    for(int i=0; i < ui->tableWidget->rowCount(); i++){
        textXML += "<cac:InvoiceLine>\n";
        textXML += "<cbc:ID>"+QString().setNum(i+1)+"</cbc:ID>\n";
        textXML += "<cbc:InvoicedQuantity unitCode=\"NIU\">"+ui->tableWidget->item(i, CANTIDAD)->text()+"</cbc:InvoicedQuantity>\n";
        double p_precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
        textXML += "<cbc:LineExtensionAmount currencyID=\"PEN\">"+QString().setNum(p_precio/1.18, ' ', 2)+"</cbc:LineExtensionAmount>\n";
        textXML += "<cac:PricingReference>\n";
        textXML += "<cac:AlternativeConditionPrice>\n";
        double p_unit = ui->tableWidget->item(i, PRECIO)->text().toDouble()/ui->tableWidget->item(i, CANTIDAD)->text().toDouble();
        textXML += "<cbc:PriceAmount currencyID=\"PEN\">"+QString().setNum(p_unit, ' ', 2)+"</cbc:PriceAmount>\n";
        textXML += "<cbc:PriceTypeCode>01</cbc:PriceTypeCode>\n";
        textXML += "</cac:AlternativeConditionPrice>\n";
        textXML += "</cac:PricingReference>\n";
        textXML += "<cac:TaxTotal>\n";
        textXML += "<cbc:TaxAmount currencyID=\"PEN\">"+QString().setNum(p_precio/1.18*0.18, ' ', 2)+"</cbc:TaxAmount>\n";
        textXML += "<cac:TaxSubtotal>\n";
        textXML += "<cbc:TaxAmount currencyID=\"PEN\">"+QString().setNum(p_precio/1.18*0.18, ' ', 2)+"</cbc:TaxAmount>\n";
        textXML += "<cac:TaxCategory>\n";
        textXML += "<cbc:TaxExemptionReasonCode>10</cbc:TaxExemptionReasonCode>\n";
        textXML += "<cac:TaxScheme>\n";
        textXML += "<cbc:ID>1000</cbc:ID>\n";
        textXML += "<cbc:Name>IGV</cbc:Name>\n";
        textXML += "<cbc:TaxTypeCode>VAT</cbc:TaxTypeCode>\n";
        textXML += "</cac:TaxScheme>\n";
        textXML += "</cac:TaxCategory>\n";
        textXML += "</cac:TaxSubtotal>\n";
        textXML += "</cac:TaxTotal>\n";
        textXML += "<cac:Item>\n";
        textXML += "<cbc:Description>"+ui->tableWidget->item(i, NOMBRE)->text()+"</cbc:Description>\n";
        textXML += "<cac:SellersItemIdentification>\n";
        textXML += "<cbc:ID>"+ui->tableWidget->item(i, ID)->text()+"</cbc:ID>\n";
        textXML += "</cac:SellersItemIdentification>\n";
        textXML += "</cac:Item>\n";
        textXML += "<cac:Price>\n";
        textXML += "<cbc:PriceAmount currencyID=\"PEN\">"+QString().setNum(p_unit/1.18, ' ', 2)+"</cbc:PriceAmount>\n";
        textXML += "</cac:Price>\n";
        textXML += "</cac:InvoiceLine>\n";
    }*/
    textXML += "</CreditNote>\n";

    QXmlStreamWriter xmlWriter(&file);
    //xmlWriter.setAutoFormatting(true);
    xmlWriter.setCodec("ISO 8859-1");
    xmlWriter.writeStartDocument("1.0", false);


    xmlWriter.writeCharacters("");
    xmlWriter.device()->write(textXML.toStdString().c_str());
    xmlWriter.device()->close();
    //QTextStream out(&file);
    //out << textXML;
    //file.flush();
    file.close();
    return true;
}
