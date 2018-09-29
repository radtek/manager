#include "SoapSunatCall.h"

#include "BillServicePortBinding.nsmap"

SoapSunatCall::SoapSunatCall()
{
	cert = NULL;
	rsa_privk = NULL;
	rsa_pubk = NULL;

	str_keyfile = NULL;
	str_password = NULL;
	str_cafile = NULL;
	str_capath = NULL;
	str_randfile = NULL;

	str_certfile = NULL;

	str_sec_username = NULL;
	str_sec_password = NULL;

	str_zip_xml_filename = NULL;
	str_zip_xml_out_filename = NULL;

	str_xml_file = NULL;
	str_error = NULL;

	str_server = "https://e-factura.sunat.gob.pe:443/ol-ti-itcpfegem/billService";
}


SoapSunatCall::~SoapSunatCall()
{
	delete cert;
	delete rsa_privk;
	delete rsa_pubk;
}

int SoapSunatCall::sendXML()
{
	struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT | SOAP_ZLIB_GZIP);
	char *user, texto;

	soap_init(soap);

	//soap_register_plugin_arg(soap, soap_wsse, (void*)token_handler);

	soap_ssl_init();

	if (soap_ssl_client_context(soap,
		SOAP_SSLv3_TLSv1,
		str_certificatekeyfile,
		str_password,
		str_cafile,
		str_capath,
		str_randfile
	))
	{
		soap_print_fault(soap, stderr);

		texto = cin.get();
		cout << "El primer caracter introducido es: " << texto << endl;
		return -1;
	}

	user = getenv("USER");
	if (!user)
		user = "lorda";
	
	FILE *fd = NULL;
	if ((fd = fopen(str_certificatekeyfile, "r")))
	{
		PEM_read_PrivateKey(fd, &rsa_privk, NULL, (void*)str_password);

		fclose(fd);
		if (!rsa_privk)
		{
			fprintf(stderr, "Could not read private RSA key from server.pem\n");
			return -1;
		}

	}
	else
		fprintf(stderr, "Could not read server.pem\n");

	if ((fd = fopen(str_certificatekeyfile, "r")))
	{
		cert = PEM_read_X509(fd, NULL, NULL, NULL);
		fclose(fd);
		if (!cert)
		{
			fprintf(stderr, "Could not read certificate from servercert.pem\n");
			return -1;
		}
	}
	else
		fprintf(stderr, "Could not read server.pem\n");

	//cin.get();
	rsa_pubk = X509_get_pubkey(cert);

	if (!rsa_pubk)
	{
		fprintf(stderr, "Could not get public key from certificate\n");
		//return -1;
	}

	soap->cafile = str_cafile;

	soap_wsse_add_Security(soap);

	//soap_wsse_add_Timestamp(soap, "Time", 10);
	soap_wsse_add_UsernameTokenDigest(soap, "Id", str_sec_username, str_sec_password);


	
	soap_wsse_add_KeyInfo_X509Certificate(soap, cert);
	soap_wsse_add_BinarySecurityTokenX509(soap, "X509Token", cert);
	soap_wsse_add_KeyInfo_SecurityTokenReferenceX509(soap, "#X509Token");

	soap_wsse_add_Signature(soap);
	soap_wsse_SignedInfo(soap);
	//soap_wsse_sign(soap, SOAP_SMD_SIGN_RSA_SHA256, rsa_privk, 0);
	soap_wsse_sign_body(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_privk, 0);
	soap_wsse_add_KeyInfo(soap);
	soap_wsse_add_SignatureValue(soap, SOAP_SMD_SIGN_RSA_SHA1, rsa_privk, 0);
	
	//unsigned char digest[64];
	//soap_wsse_verify_digest(soap, SOAP_SMD_SIGN_RSA_SHA1, SOAP_XML_CANONICAL, "Id", digest);
	//cout << "digest: " << digest << endl;
	//soap_wsse_add_UsernameTokenDigest(soap, "Id", "20498590587MODDATO1", "lealcito20587");


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
	/*
	{
	unsigned char * buffer;
	int fileLen;
	FILE *file;

	size_t* size = new size_t;
	FILE *f = fopen(str_xml_file, "rb");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return -1;
	}

	fseek(f, 0, SEEK_END);
	fileLen = ftell(f);
	fseek(f, 0, SEEK_SET);

	//ALLOCATE MEMORY
	buffer = (unsigned char *)malloc(fileLen + 1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error! :( ");
		fclose(f);
		return -1;
	}

	//READ FILE CONTENTS INTO BUFFER AND CLOSE FILE
	fread(buffer, fileLen, 1, f);
	//string str_mod_xml_file_zip(str_xml_file);
	//str_mod_xml_file_zip += ".zip";
	gzFile outfile = gzopen(str_zip_xml_filename, "wb");
	
	gzwrite(outfile, buffer, fileLen);
	gzclose(outfile);
	fclose(f);
}*/

	//cout << "Termino de crear el zip" << endl;
	ns1__sendBill sb;
	ns1__sendBillResponse sbr;
	string *str_fileName = new string(str_zip_xml_filename);
	xsd__base64Binary* xsd = new xsd__base64Binary();	

	unsigned char * buffer;
	int fileLen;
	FILE *file;
	//OPEN FILE
	file = fopen(str_fileName->c_str(), "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", str_fileName->c_str());
		return -1;
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
		return -1;
	}

	//READ FILE CONTENTS INTO BUFFER AND CLOSE FILE
	fread(buffer, fileLen, 1, file);
	fclose(file);

	//USE THE BUFFER ....


	//FREE YOUR ALLOCATED BUFFER
	
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

	if (soap_call___sunat__sendBill(soap, str_server, NULL, &sb, sbr) == SOAP_OK) {

		size_t* size = new size_t;
		//FILE *f = fopen(str_zip_xml_out_filename, "wb");
		//if (f == NULL)
		//{
			//printf("Error opening file!\n");
			//return -1;
		//}
		
		//char *str_res = sbr.soap->buf;
		//char *str_res = (char* )sbr.applicationResponse->__ptr;
		
		gzFile outfile = gzopen(str_zip_xml_out_filename, "wb");
		
		gzwrite(outfile, sbr.applicationResponse->__ptr, sbr.applicationResponse->__size);
		gzclose(outfile);

		str_error = "";
		free(buffer);
		soap_wsse_delete_Security(soap);

		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);

		return 0;
		//fprintf(f, "%s", str_res);

		//cout << "response: " << sbr.soap->buf << endl;
		//fclose(f);
		//string str_deco;
		//str_deco += " x -o+";
		//str_deco += str_zip_xml_out_filename;
		//system();
		//ShellExecute(0, "open", "c:\\Program Files\\WinRAR\\WinRAR.exe", str_deco.c_str(), 0, SW_HIDE);
	}
	else {
		cout << "error" << endl;
		//soap_print_fault(soap, stderr);
		/*
		if (soap_check_state(soap))
		{
			fprintf(stderr, "Error: soap struct state not initialized with soap_init\n");
		}
		else if (soap->error)
		{
			const char **c, *v = NULL, *s, *d;
			c = soap_faultcode(soap);
			if (!*c)
			{
				soap_set_fault(soap);
				c = soap_faultcode(soap);
			}
			if (soap->version == 2)
				v = soap_check_faultsubcode(soap);
			s = *soap_faultstring(soap);
			d = soap_check_faultdetail(soap);			

			sprintf(str_error, "%s%d fault %s [%s]\n\"%s\"\nDetail: %s\n", soap->version ? "SOAP 1." : "Error ", soap->version ? (int)soap->version : soap->error, *c, v ? v : "no subcode", s ? s : "[no reason]", d ? d : "[no detail]");
		}

		*/		
		str_error = soap->fault->faultstring;
		//sprintf(str_error, )
		free(buffer);
		soap_wsse_delete_Security(soap);

		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);
		return -1;
	}	
}

int SoapSunatCall::xmlDSig()
{
#ifndef XMLSEC_NO_XSLT
	xsltSecurityPrefsPtr xsltSecPrefs = NULL;
#endif /* XMLSEC_NO_XSLT */

	xmlSecInit();
	/* Init libxml and libxslt libraries */
	xmlInitParser();
	LIBXML_TEST_VERSION
		xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
	xmlSubstituteEntitiesDefault(1);
#ifndef XMLSEC_NO_XSLT
	xmlIndentTreeOutput = 1;
#endif /* XMLSEC_NO_XSLT */

	/* Init libxslt */
#ifndef XMLSEC_NO_XSLT
	/* disable everything */
	xsltSecPrefs = xsltNewSecurityPrefs();
	xsltSetSecurityPrefs(xsltSecPrefs, XSLT_SECPREF_READ_FILE, xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs, XSLT_SECPREF_WRITE_FILE, xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs, XSLT_SECPREF_CREATE_DIRECTORY, xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs, XSLT_SECPREF_READ_NETWORK, xsltSecurityForbid);
	xsltSetSecurityPrefs(xsltSecPrefs, XSLT_SECPREF_WRITE_NETWORK, xsltSecurityForbid);
	xsltSetDefaultSecurityPrefs(xsltSecPrefs);
#endif /* XMLSEC_NO_XSLT */

	/* Init xmlsec library */
	if (xmlSecInit() < 0) {
		fprintf(stderr, "Error: xmlsec initialization failed.\n");
		return -1;
	}

	/* Check loaded library version */
	if (xmlSecCheckVersion() != 1) {
		fprintf(stderr, "Error: loaded xmlsec library version is not compatible.\n");
		return -1;
	}

	/* Load default crypto engine if we are supporting dynamic
	* loading for xmlsec-crypto libraries. Use the crypto library
	* name ("openssl", "nss", etc.) to load corresponding
	* xmlsec-crypto library.
	*/
#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
	if (xmlSecCryptoDLLoadLibrary(NULL) < 0) {
		fprintf(stderr, "Error: unable to load default xmlsec-crypto library. Make sure\n"
			"that you have it installed and check shared libraries path\n"
			"(LD_LIBRARY_PATH and/or LTDL_LIBRARY_PATH) envornment variables.\n");
		return -1;
	}
#endif /* XMLSEC_CRYPTO_DYNAMIC_LOADING */

	/* Init crypto library */
	if (xmlSecCryptoAppInit(NULL) < 0) {
		fprintf(stderr, "Error: crypto initialization failed.\n");
		return -1;
	}

	/* Init xmlsec-crypto library */
	if (xmlSecCryptoInit() < 0) {
		fprintf(stderr, "Error: xmlsec-crypto initialization failed.\n");
		return -1;
	}

	if (sign_file(str_xml_file, str_keyfile, str_certfile) < 0) {
		return -1;
	}

	// success
	return 0;

	/* Shutdown xmlsec-crypto library */
	xmlSecCryptoShutdown();

	/* Shutdown crypto library */
	xmlSecCryptoAppShutdown();

	/* Shutdown xmlsec library */
	xmlSecShutdown();

	/* Shutdown libxslt/libxml */
#ifndef XMLSEC_NO_XSLT
	xsltFreeSecurityPrefs(xsltSecPrefs);
	xsltCleanupGlobals();
#endif /* XMLSEC_NO_XSLT */
	xmlCleanupParser();
}

/**
* sign_file:
* @xml_file:           the XML file name.
* @key_file:           the PEM private key file name.
* @cert_file:          the x509 certificate PEM file.
*
* Signs the @xml_file using private key from @key_file and dynamicaly
* created enveloped signature template. The certificate from @cert_file
* is placed in the <dsig:X509Data/> node.
*
* Returns 0 on success or a negative value if an error occurs.
*/
int SoapSunatCall::sign_file(const char* xml_file, const char* key_file, const char* cert_file) {
	xmlDocPtr doc = NULL;
	xmlNodePtr signNode = NULL;
	xmlNodePtr refNode = NULL;
	xmlNodePtr keyInfoNode = NULL;
	xmlNodePtr x509DataNode = NULL;
	xmlSecDSigCtxPtr dsigCtx = NULL;
	int res = -1;

	assert(xml_file);
	assert(key_file);
	assert(cert_file);

	/* load doc file */
	doc = xmlParseFile(xml_file);
	if ((doc == NULL) || (xmlDocGetRootElement(doc) == NULL)) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", xml_file);
		return -1;
	}
	
	
	/* create signature template for RSA-SHA1 enveloped signature */
	signNode = xmlSecTmplSignatureCreate(doc
		, xmlSecTransformExclC14NId,
		xmlSecTransformRsaSha1Id, (const xmlChar*)str_id_signature);
	if (signNode == NULL) {
		fprintf(stderr, "Error: failed to create signature template\n");
		return -1;
	}	

	/* add <dsig:Signature/> node to the doc */
	cout << (char*)xmlDocGetRootElement(doc)->children->next->children->next->next->next->children->next->name << endl;
	//cout << (char*)xmlSecFindChild(xmlDocGetRootElement(doc)->children->next, (const xmlChar*)"ext:UBLExtension", (const xmlChar*)"")->name<<endl;
	xmlAddChild(xmlDocGetRootElement(doc)->children->next->children->next->next->next->children->next, signNode);
	//xmlAddChild(xmlSecFindChild(doc->next, (const xmlChar*)"ext:UBLExtensions", NULL), signNode);

	/* add reference */
	refNode = xmlSecTmplSignatureAddReference(signNode, xmlSecTransformSha1Id,
		NULL, (const xmlChar*)"", NULL);
	if (refNode == NULL) {
		fprintf(stderr, "Error: failed to add reference to signature template\n");
		return -1;
	}

	/* add enveloped transform */
	if (xmlSecTmplReferenceAddTransform(refNode, xmlSecTransformEnvelopedId) == NULL) {
		fprintf(stderr, "Error: failed to add enveloped transform to reference\n");
		return -1;
	}

	/* add <dsig:KeyInfo/> and <dsig:X509Data/> */
	keyInfoNode = xmlSecTmplSignatureEnsureKeyInfo(signNode, NULL);
	if (keyInfoNode == NULL) {
		fprintf(stderr, "Error: failed to add key info\n");
		return -1;
	}

	x509DataNode = xmlSecTmplKeyInfoAddX509Data(keyInfoNode);
	if (x509DataNode == NULL) {
		fprintf(stderr, "Error: failed to add X509Data node\n");
		return -1;
	}

	if (xmlSecTmplX509DataAddSubjectName(x509DataNode) == NULL) {
		fprintf(stderr, "Error: failed to add X509SubjectName node\n");
		return -1;
	}

	if (xmlSecTmplX509DataAddCertificate(x509DataNode) == NULL) {
		fprintf(stderr, "Error: failed to add X509Certificate node\n");
		return -1;
	}

	/* create signature context, we don't need keys manager in this example */
	dsigCtx = xmlSecDSigCtxCreate(NULL);
	if (dsigCtx == NULL) {
		fprintf(stderr, "Error: failed to create signature context\n");
		return -1;
	}

	/* load private key, assuming that there is not password */
	dsigCtx->signKey = xmlSecCryptoAppKeyLoad(key_file, xmlSecKeyDataFormatPem, NULL, NULL, NULL);
	if (dsigCtx->signKey == NULL) {
		fprintf(stderr, "Error: failed to load private pem key from \"%s\"\n", key_file);
		return -1;
	}

	/* load certificate and add to the key */
	if (xmlSecCryptoAppKeyCertLoad(dsigCtx->signKey, cert_file, xmlSecKeyDataFormatPem) < 0) {
		fprintf(stderr, "Error: failed to load pem certificate \"%s\"\n", cert_file);
		return -1;
	}

	/* set key name to the file name, this is just an example! */
	//xmlChar* xml_key_file = new xmlChar();
	
	if (xmlSecKeySetName(dsigCtx->signKey, (const xmlChar*)key_file) < 0) {
		fprintf(stderr, "Error: failed to set key name for key from \"%s\"\n", key_file);
		return -1;
	}

	/* sign the template */
	if (xmlSecDSigCtxSign(dsigCtx, signNode) < 0) {
		fprintf(stderr, "Error: signature failed\n");
		return -1;
	}

	FILE *f = fopen(str_xml_file, "wb");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return -1;
	}
	/* print signed document to stdout */
	xmlDocDump(f, doc);				

	fclose(f);
	/* success */
	res = 0;

done:
	/* cleanup */
	if (dsigCtx != NULL) {
		xmlSecDSigCtxDestroy(dsigCtx);
	}

	if (doc != NULL) {
		xmlFreeDoc(doc);
	}
	return(res);
}