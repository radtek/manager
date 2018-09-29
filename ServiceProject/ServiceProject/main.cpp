#include "SoapSunatCall.h"

#include "soapH.h"
//#include "BillServicePortBinding.nsmap"

int main(int argc, char *argv[]) {
	SoapSunatCall obj;
	obj.set_str_keyfile((char *)"mycertificate.pem");
	obj.set_str_password((char *)"4t48gWXqdd358Msx");
	obj.set_str_cafile((char *)"myca.pem");
	obj.set_str_certfile((char *)"mycert.pem");
	obj.set_str_sec_username((char *)"20498590587ELBIBAM6");
	obj.set_str_sec_password((char *)"nArputo-");
	obj.set_str_zip_xml_filename((char *)"20498590587-03-B004-1430.zip");
	obj.set_str_zip_xml_out_filename((char *)"res.zip");
	obj.set_str_xml_file((char *)"20498590587-03-B004-1430.xml");

	obj.sendXML();
	/*
	struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT | SOAP_ZLIB_GZIP);
	char *user, texto;		

	soap_init(soap);

	//soap_register_plugin_arg(soap, soap_wsse, (void*)token_handler);
	
	soap_ssl_init();

	if (soap_ssl_client_context(soap,
		SOAP_SSLv3_TLSv1,
		"mycertificate.pem",
		"4t48gWXqdd358Msx",
		//NULL,
		//NULL,
		"myca.pem",
		NULL,
		NULL
	))
	{
		soap_print_fault(soap, stderr);

		texto = cin.get();
		cout << "El primer caracter introducido es: " << texto << endl;
		exit(1);
	}
	
	user = getenv("USER");
	if (!user)
		user = "lorda";

	FILE *fd = NULL;	
	if ((fd = fopen("mycertificate.pem", "r")))
	{				
		PEM_read_PrivateKey(fd, &rsa_privk, NULL, (void*)"4t48gWXqdd358Msx");
		
		fclose(fd);
		if (!rsa_privk)
		{
			fprintf(stderr, "Could not read private RSA key from server.pem\n");
			exit(1);
		}

	}
	else
		fprintf(stderr, "Could not read server.pem\n");
	
	if ((fd = fopen("mycertificate.pem", "r")))
	{
		cert = PEM_read_X509(fd, NULL, NULL, NULL);
		fclose(fd);
		if (!cert)
		{
			fprintf(stderr, "Could not read certificate from servercert.pem\n");
			exit(1);
		}
	}
	else
		fprintf(stderr, "Could not read server.pem\n");

	//cin.get();
	rsa_pubk = X509_get_pubkey(cert);
	
	if (!rsa_pubk)
	{
		fprintf(stderr, "Could not get public key from certificate\n");
		exit(1);
	}
	
	soap->cafile = "myca.pem";
	
	soap_wsse_add_Security(soap);

	soap_wsse_add_Timestamp(soap, "Time", 10);
	soap_wsse_add_UsernameTokenDigest(soap, "Id", "20498590587ELBIBAM6", "nArputo-");

	
	/*
	soap_wsse_add_KeyInfo_X509Certificate(soap, cert);
	soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert);
	soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token");
	/
	soap_wsse_add_Signature(soap);
	soap_wsse_SignedInfo(soap);
	//soap_wsse_sign(soap, SOAP_SMD_SIGN_RSA_SHA256, rsa_privk, 0);
	soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_privk, 0);
	soap_wsse_add_KeyInfo(soap);
	soap_wsse_add_SignatureValue(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_privk, 0);
	*/
	//unsigned char digest[64];
	//soap_wsse_verify_digest(soap, SOAP_SMD_SIGN_RSA_SHA1, SOAP_XML_CANONICAL, "Id", digest);
	//cout << "digest: " << digest << endl;
	//soap_wsse_add_UsernameTokenDigest(soap, "Id", "20498590587MODDATO1", "lealcito20587");
	/*
	_wsse__Security *security = soap_wsse_Security(soap);
	
	if (security)
	{
		
		security->UsernameToken->Username = "20498590587ELBIBAM6";
		security->UsernameToken->Password->__item = "nArputo-";
		ds__SignatureType* ds = new ds__SignatureType();
		//ds->Id = "signatureKG";
		
		ds__SignedInfoType *ds_si = new ds__SignedInfoType();		
		ds__CanonicalizationMethodType *ds_cm = new ds__CanonicalizationMethodType();
		ds_si->CanonicalizationMethod = ds_cm;
		ds_cm->Algorithm = "http://www.w3.org/TR/2001/REC-xml-c14n20010315";
		ds__SignatureMethodType *ds_sm = new ds__SignatureMethodType();
		ds_si->SignatureMethod = ds_sm;
		ds_sm->Algorithm = "http://www.w3.org/2000/09/xmldsig#rsa-sha1";
		ds__ReferenceType *ds_rt = new ds__ReferenceType();
		ds_si->Reference = &ds_rt;
		ds_rt->URI = "";
		
		ds__TransformsType * ds_tst = new ds__TransformsType();
		ds_rt->Transforms = ds_tst;
		ds__TransformType * ds_tt = new ds__TransformType();
		ds_tst->Transform = ds_tt;
		
		ds_tt->Algorithm = "http://www.w3.org/2000/09/xmldsig#envelopedsignature";		
		ds__DigestMethodType * ds_dmt = new ds__DigestMethodType();
		ds_rt->DigestMethod = ds_dmt;
		ds_dmt->Algorithm = "http://www.w3.org/2000/09/xmldsig#sha1";		
		//(*ds->SignedInfo->Reference)->DigestValue
		security->ds__Signature = ds;
		//security->ds__Signature->Id;
		//cout << "add signature" << security->ds__Signature->Id << endl;
		
	}
	
	ns1__sendBill sb;	
	ns1__sendBillResponse sbr;
	string *str_fileName = new string("20498590587-03-B004-1430.zip");
	xsd__base64Binary* xsd = new xsd__base64Binary();

	unsigned char * buffer;
	int fileLen;
	FILE *file;

	//OPEN FILE
	file = fopen(str_fileName->c_str(), "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", str_fileName->c_str());
		//exit(1);
	}

	//GET FILE LENGTH
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	//ALLOCATE MEMORY
	buffer = (unsigned char *)malloc(fileLen + 1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error! :( ");
		fclose(file);
		//exit(1);
	}

	//READ FILE CONTENTS INTO BUFFER AND CLOSE FILE
	fread(buffer, fileLen, 1, file);
	fclose(file);

	//USE THE BUFFER ....
	

	//FREE YOUR ALLOCATED BUFFER
	
	//string* str_fName = new string("20498590587-01-F001-1.zip");
	sb.fileName = str_fileName;
	xsd->__ptr = buffer;
	xsd->__size = fileLen;
	sb.contentFile = xsd;
		
	soap->connect_timeout = 30;	
	soap->send_timeout = soap->recv_timeout = 60;
	cout << "call___sunat__sendBill" << endl;
	
	//cout << "digest hash: " << soap_wsse_digest().hash << endl;
	//sbr.soap = soap;
	//soap_wsse_add_BinarySecurityTokenPEM(soap, NULL, "mycertificate.pem");
	//soap_wsse_decrypt_auto(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_privk, 0);

	if (soap_call___sunat__sendBill(soap, server, NULL, &sb, sbr) == SOAP_OK) {
		
		size_t* size = new size_t;
		FILE *f = fopen("file.txt", "wb");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		
		char *str_res = sbr.soap->buf;

		gzFile outfile = gzopen("res.zip", "wb");
		/*
		char inbuffer[128];
		int num_read = 0;
		unsigned long total_read = 0, total_wrote = 0;
		while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), f)) > 0) {
			total_read += num_read;
			gzwrite(outfile, inbuffer, num_read);			
		}*/
/*
		gzwrite(outfile, sbr.applicationResponse->__ptr, sbr.applicationResponse->__size);
		gzclose(outfile);

		cout << "response: " << sbr.soap->buf << endl;
		fprintf(f, "Some text : %s\n", str_res);

		fclose(f);
	}
	else {
		cout << "error" << endl;
		soap_print_fault(soap, stderr);
		
	}
	free(buffer);
	soap_wsse_delete_Security(soap);
	
	soap_destroy(soap); 
	soap_end(soap);
	soap_done(soap);
	
	texto = cin.get();
	cout << "El primer caracter introducido es: " << texto << endl;
	*/
	return 0;
}
// ////////////////////////////////////////////////////////////////////////////////////////
