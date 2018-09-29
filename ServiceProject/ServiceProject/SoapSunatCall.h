#ifndef SOAPSUNATCALL_H
#define SOAPSUNATCALL_H

#if defined(SOAPSUNATCALL_LIBRARY)
#  define SOAPSUNATCALL_EXPORT __declspec(dllexport)
#else
#  define SOAPSUNATCALL_EXPORT __declspec(dllimport)
#endif

#include <cstdint>
#include <cmath>
#include <time.h>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include <libxml/xmlstring.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifndef XMLSEC_NO_XSLT
#include <libxslt/xslt.h>
#include <libxslt/security.h>
#endif /* XMLSEC_NO_XSLT */

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/templates.h>
#include <xmlsec/crypto.h>

#include "wsseapi.h"
#include "soapH.h"
#include "openssl\evp.h"
#include <openssl/pem.h>



#include <iostream>
using namespace std;

class SOAPSUNATCALL_EXPORT SoapSunatCall
{
private:
	X509 *cert;
	EVP_PKEY *rsa_privk;
	EVP_PKEY *rsa_pubk;
	
	char* str_certificatekeyfile;
	char* str_keyfile;
	char* str_password;
	char* str_cafile;
	char* str_capath;
	char* str_randfile;

	char* str_certfile;

	char* str_sec_username;
	char* str_sec_password;

	char* str_server;
	char* str_zip_xml_filename;
	char* str_zip_xml_out_filename;

	char* str_xml_file;

	char* str_id_signature;
	
	char* str_error;
public:
	SoapSunatCall();
	virtual ~SoapSunatCall();

	void set_str_certificatekeyfile(char* str_certificatekeyfile) { this->str_certificatekeyfile = str_certificatekeyfile; };
	void set_str_keyfile(char* str_keyfile) { this->str_keyfile = str_keyfile;  };
	void set_str_password(char* str_password) { this->str_password = str_password; };
	void set_str_cafile(char* str_cafile) { this->str_cafile = str_cafile; };
	void set_str_capath(char* str_capath) { this->str_capath = str_capath; };
	void set_str_randfile(char* str_randfile) { this->str_randfile = str_randfile; };

	void set_str_certfile(char* str_certfile) { this->str_certfile = str_certfile; };

	void set_str_sec_username(char* str_sec_username) { this->str_sec_username = str_sec_username; };
	void set_str_sec_password(char* str_sec_password) { this->str_sec_password = str_sec_password; };

	void set_str_server(char* str_server) { this->str_server = str_server; };
	void set_str_zip_xml_filename(char* str_zip_xml_filename) { this->str_zip_xml_filename = str_zip_xml_filename; };
	void set_str_zip_xml_out_filename(char* str_zip_xml_out_filename) { this->str_zip_xml_out_filename = str_zip_xml_out_filename; };
	void set_str_xml_file(char* str_xml_file) { this->str_xml_file = str_xml_file; };
	void set_str_id_signature(char* str_id_signature) { this->str_id_signature = str_id_signature; };

	char *get_str_error() {
		return str_error;
	}
	int sendXML();
		
	int xmlDSig();

private:
	int sign_file(const char* xml_file, const char* key_file, const char* cert_file);

};

#endif // SOAPSUNATCALL_H
