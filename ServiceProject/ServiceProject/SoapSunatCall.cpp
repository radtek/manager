#include "SoapSunatCall.h"

#include "BillServicePortBinding.nsmap"

//#include <unistd.h>		/* defines _POSIX_THREADS if pthreads are available */
#if defined(_POSIX_THREADS) || defined(_SC_THREADS)
#include <pthread.h>
#endif

#include <process.h>

#include <signal.h>		/* defines SIGPIPE */

int CRYPTO_thread_setup();
void CRYPTO_thread_cleanup();
void sigpipe_handle(int);

DWORD WINAPI process_request(void *soap);

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
	struct soap *soap = soap_new1(SOAP_XML_CANONICAL | SOAP_XML_INDENT);
	char *user, texto;

	soap_clr_imode(soap, SOAP_IO_KEEPALIVE);
	soap_clr_imode(soap, SOAP_IO_KEEPALIVE);

	soap_init(soap);

	//soap_register_plugin_arg(soap, soap_wsse, (void*)token_handler);

	soap_ssl_init();
	if (CRYPTO_thread_setup())
	{
		fprintf(stderr, "Cannot setup thread mutex for OpenSSL\n");
		return -1;
	}

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

	soap_wsse_add_Timestamp(soap, "Time", 10);
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

	soap->connect_timeout = 10;
	soap->send_timeout = 5;
	soap->recv_timeout = 5;
	//soap->accept_timeout = 3600; // server stops after 1 hour of inactivity 
	//soap->max_keep_alive = 100; // max keep-alive sequence 
	int ret = -1;
	//soap->accept_timeout = 5;
	//soap->transfer_timeout = 15;
	//soap_force_closesock(soap);
	cout << "call___sunat__sendBill" << endl;
			
	try {
		if (soap_call___sunat__sendBill(soap, str_server, NULL, &sb, sbr) == SOAP_OK) {

			size_t* size = new size_t;

			gzFile outfile = gzopen(str_zip_xml_out_filename, "wb");

			gzwrite(outfile, sbr.applicationResponse->__ptr, sbr.applicationResponse->__size);
			gzclose(outfile);

			str_error = "";

			ret = 0;
		}
		else {
			ret = -1;
		}
	}
	catch (std::runtime_error& e) {
		strcpy(str_error, e.what());
		
		return -1;
	}
	

	if (ret != -1) {
		//free(buffer);

		soap_wsse_delete_Security(soap);

		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);
		CRYPTO_thread_cleanup();
		return ret;
	}
	else {
		cout << "error" << endl;
		FILE* f = fopen("error.txt", "w");
		if (f != NULL) {
			soap_print_fault(soap, f);
			fclose(f);
		}

		//free(buffer);

		soap_wsse_delete_Security(soap);

		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);
		CRYPTO_thread_cleanup();
		return ret;
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

	//setlocale(LC_ALL, "en_US.UTF-8");
	FILE *f = fopen(str_xml_file, "w");
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

#ifdef WITH_OPENSSL

#if defined(WIN32)
# define MUTEX_TYPE		HANDLE
# define MUTEX_SETUP(x)		(x) = CreateMutex(NULL, FALSE, NULL)
# define MUTEX_CLEANUP(x)	CloseHandle(x)
# define MUTEX_LOCK(x)		WaitForSingleObject((x), INFINITE)
# define MUTEX_UNLOCK(x)	ReleaseMutex(x)
# define THREAD_ID		GetCurrentThreadId()
#elif defined(_POSIX_THREADS) || defined(_SC_THREADS)
# define MUTEX_TYPE		pthread_mutex_t
# define MUTEX_SETUP(x)		pthread_mutex_init(&(x), NULL)
# define MUTEX_CLEANUP(x)	pthread_mutex_destroy(&(x))
# define MUTEX_LOCK(x)		pthread_mutex_lock(&(x))
# define MUTEX_UNLOCK(x)	pthread_mutex_unlock(&(x))
# define THREAD_ID		pthread_self()
#else
# error "You must define mutex operations appropriate for your platform"
# error	"See OpenSSL /threads/th-lock.c on how to implement mutex on your platform"
#endif

struct CRYPTO_dynlock_value
{
	MUTEX_TYPE mutex;
};

static MUTEX_TYPE *mutex_buf;

static struct CRYPTO_dynlock_value *dyn_create_function(const char *file, int line)
{
	struct CRYPTO_dynlock_value *value;
	value = (struct CRYPTO_dynlock_value*)malloc(sizeof(struct CRYPTO_dynlock_value));
	if (value)
		MUTEX_SETUP(value->mutex);
	return value;
}

static void dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l, const char *file, int line)
{
	if (mode & CRYPTO_LOCK)
		MUTEX_LOCK(l->mutex);
	else
		MUTEX_UNLOCK(l->mutex);
}

static void dyn_destroy_function(struct CRYPTO_dynlock_value *l, const char *file, int line)
{
	MUTEX_CLEANUP(l->mutex);
	free(l);
}

void locking_function(int mode, int n, const char *file, int line)
{
	if (mode & CRYPTO_LOCK)
		MUTEX_LOCK(mutex_buf[n]);
	else
		MUTEX_UNLOCK(mutex_buf[n]);
}

unsigned long id_function()
{
	return (unsigned long)THREAD_ID;
}

int CRYPTO_thread_setup()
{
	int i;
	mutex_buf = (MUTEX_TYPE*)malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
	if (!mutex_buf)
		return SOAP_EOM;
	for (i = 0; i < CRYPTO_num_locks(); i++)
		MUTEX_SETUP(mutex_buf[i]);
	CRYPTO_set_id_callback(id_function);
	CRYPTO_set_locking_callback(locking_function);
	CRYPTO_set_dynlock_create_callback(dyn_create_function);
	CRYPTO_set_dynlock_lock_callback(dyn_lock_function);
	CRYPTO_set_dynlock_destroy_callback(dyn_destroy_function);
	return SOAP_OK;
}

void CRYPTO_thread_cleanup()
{
	int i;
	if (!mutex_buf)
		return;
	CRYPTO_set_id_callback(NULL);
	CRYPTO_set_locking_callback(NULL);
	CRYPTO_set_dynlock_create_callback(NULL);
	CRYPTO_set_dynlock_lock_callback(NULL);
	CRYPTO_set_dynlock_destroy_callback(NULL);
	for (i = 0; i < CRYPTO_num_locks(); i++)
		MUTEX_CLEANUP(mutex_buf[i]);
	free(mutex_buf);
	mutex_buf = NULL;
}

#else

/* OpenSSL not used, e.g. GNUTLS is used */

int CRYPTO_thread_setup()
{
	return SOAP_OK;
}

void CRYPTO_thread_cleanup()
{ }

#endif

/******************************************************************************\
*
*	SIGPIPE
*
\******************************************************************************/

void sigpipe_handle(int x) { }

DWORD WINAPI process_request(void *soap)
{	
	cout << "empezo el hilo" << endl;
	//_endthread();
	if (soap) {
		//soap_serve((struct soap*)soap);
		soap_wsse_delete_Security((struct soap*)soap);
		soap_destroy((struct soap*)soap); // dealloc C++ data 
		soap_end((struct soap*)soap); // dealloc data and clean up 
		soap_done((struct soap*)soap); // detach soap struct 
		CRYPTO_thread_cleanup();
		free(soap);
	}
	cout << "ya sali del hilo" << endl;
	return 0;
}