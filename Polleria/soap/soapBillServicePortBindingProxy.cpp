/* soapBillServicePortBindingProxy.cpp
   Generated by gSOAP 2.8.69 for C:\gsoap-2.8\gsoap\bin\win32\service.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapBillServicePortBindingProxy.h"

BillServicePortBindingProxy::BillServicePortBindingProxy()
{	this->soap = soap_new();
	this->soap_own = true;
	BillServicePortBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

BillServicePortBindingProxy::BillServicePortBindingProxy(const BillServicePortBindingProxy& rhs)
{	this->soap = rhs.soap;
	this->soap_own = false;
	this->soap_endpoint = rhs.soap_endpoint;
}

BillServicePortBindingProxy::BillServicePortBindingProxy(struct soap *_soap)
{	this->soap = _soap;
	this->soap_own = false;
	BillServicePortBindingProxy_init(_soap->imode, _soap->omode);
}

BillServicePortBindingProxy::BillServicePortBindingProxy(const char *endpoint)
{	this->soap = soap_new();
	this->soap_own = true;
	BillServicePortBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = endpoint;
}

BillServicePortBindingProxy::BillServicePortBindingProxy(soap_mode iomode)
{	this->soap = soap_new();
	this->soap_own = true;
	BillServicePortBindingProxy_init(iomode, iomode);
}

BillServicePortBindingProxy::BillServicePortBindingProxy(const char *endpoint, soap_mode iomode)
{	this->soap = soap_new();
	this->soap_own = true;
	BillServicePortBindingProxy_init(iomode, iomode);
	soap_endpoint = endpoint;
}

BillServicePortBindingProxy::BillServicePortBindingProxy(soap_mode imode, soap_mode omode)
{	this->soap = soap_new();
	this->soap_own = true;
	BillServicePortBindingProxy_init(imode, omode);
}

BillServicePortBindingProxy::~BillServicePortBindingProxy()
{	if (this->soap_own)
		soap_free(this->soap);
}

void BillServicePortBindingProxy::BillServicePortBindingProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this->soap, imode);
	soap_omode(this->soap, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] = {
        {"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
        {"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
        {"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
        {"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
        {"sunat", "http://service.gem.factura.comppago.registro.servicio.sunat.gob.pe/", NULL, NULL},
        {"ns1", "http://service.sunat.gob.pe", NULL, NULL},
        {NULL, NULL, NULL, NULL}
    };
	soap_set_namespaces(this->soap, namespaces);
}

BillServicePortBindingProxy *BillServicePortBindingProxy::copy()
{	BillServicePortBindingProxy *dup = SOAP_NEW_UNMANAGED(BillServicePortBindingProxy);
	if (dup)
	{	soap_done(dup->soap);
		soap_copy_context(dup->soap, this->soap);
	}
	return dup;
}

BillServicePortBindingProxy& BillServicePortBindingProxy::operator=(const BillServicePortBindingProxy& rhs)
{	if (this->soap != rhs.soap)
	{	if (this->soap_own)
			soap_free(this->soap);
		this->soap = rhs.soap;
		this->soap_own = false;
		this->soap_endpoint = rhs.soap_endpoint;
	}
	return *this;
}

void BillServicePortBindingProxy::destroy()
{	soap_destroy(this->soap);
	soap_end(this->soap);
}

void BillServicePortBindingProxy::reset()
{	this->destroy();
	soap_done(this->soap);
	soap_initialize(this->soap);
	BillServicePortBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void BillServicePortBindingProxy::soap_noheader()
{	this->soap->header = NULL;
}

::SOAP_ENV__Header *BillServicePortBindingProxy::soap_header()
{	return this->soap->header;
}

::SOAP_ENV__Fault *BillServicePortBindingProxy::soap_fault()
{	return this->soap->fault;
}

const char *BillServicePortBindingProxy::soap_fault_string()
{	return *soap_faultstring(this->soap);
}

const char *BillServicePortBindingProxy::soap_fault_detail()
{	return *soap_faultdetail(this->soap);
}

int BillServicePortBindingProxy::soap_close_socket()
{	return soap_closesock(this->soap);
}

int BillServicePortBindingProxy::soap_force_close_socket()
{	return soap_force_closesock(this->soap);
}

void BillServicePortBindingProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this->soap, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void BillServicePortBindingProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this->soap, os);
}
#endif

char *BillServicePortBindingProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this->soap, buf, len);
}
#endif

int BillServicePortBindingProxy::getStatus(const char *endpoint, const char *soap_action, ns1__getStatus *ns1__getStatus_, ns1__getStatusResponse &ns1__getStatusResponse_)
{
	struct __sunat__getStatus soap_tmp___sunat__getStatus;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "https://e-factura.sunat.gob.pe:443/ol-ti-itcpfegem/billService";
	if (soap_action == NULL)
		soap_action = "urn:getStatus";
	soap_tmp___sunat__getStatus.ns1__getStatus_ = ns1__getStatus_;
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize___sunat__getStatus(soap, &soap_tmp___sunat__getStatus);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___sunat__getStatus(soap, &soap_tmp___sunat__getStatus, "-sunat:getStatus", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___sunat__getStatus(soap, &soap_tmp___sunat__getStatus, "-sunat:getStatus", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!static_cast<ns1__getStatusResponse*>(&ns1__getStatusResponse_)) // NULL ref?
		return soap_closesock(soap);
	ns1__getStatusResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__getStatusResponse_.soap_get(soap, "ns1:getStatusResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int BillServicePortBindingProxy::sendBill(const char *endpoint, const char *soap_action, ns1__sendBill *ns1__sendBill_, ns1__sendBillResponse &ns1__sendBillResponse_)
{
	struct __sunat__sendBill soap_tmp___sunat__sendBill;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "https://e-factura.sunat.gob.pe:443/ol-ti-itcpfegem/billService";
	if (soap_action == NULL)
		soap_action = "urn:sendBill";
	soap_tmp___sunat__sendBill.ns1__sendBill_ = ns1__sendBill_;
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize___sunat__sendBill(soap, &soap_tmp___sunat__sendBill);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___sunat__sendBill(soap, &soap_tmp___sunat__sendBill, "-sunat:sendBill", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___sunat__sendBill(soap, &soap_tmp___sunat__sendBill, "-sunat:sendBill", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!static_cast<ns1__sendBillResponse*>(&ns1__sendBillResponse_)) // NULL ref?
		return soap_closesock(soap);
	ns1__sendBillResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__sendBillResponse_.soap_get(soap, "ns1:sendBillResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int BillServicePortBindingProxy::sendPack(const char *endpoint, const char *soap_action, ns1__sendPack *ns1__sendPack_, ns1__sendPackResponse &ns1__sendPackResponse_)
{
	struct __sunat__sendPack soap_tmp___sunat__sendPack;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "https://e-factura.sunat.gob.pe:443/ol-ti-itcpfegem/billService";
	if (soap_action == NULL)
		soap_action = "urn:sendPack";
	soap_tmp___sunat__sendPack.ns1__sendPack_ = ns1__sendPack_;
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize___sunat__sendPack(soap, &soap_tmp___sunat__sendPack);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___sunat__sendPack(soap, &soap_tmp___sunat__sendPack, "-sunat:sendPack", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___sunat__sendPack(soap, &soap_tmp___sunat__sendPack, "-sunat:sendPack", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!static_cast<ns1__sendPackResponse*>(&ns1__sendPackResponse_)) // NULL ref?
		return soap_closesock(soap);
	ns1__sendPackResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__sendPackResponse_.soap_get(soap, "ns1:sendPackResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int BillServicePortBindingProxy::sendSummary(const char *endpoint, const char *soap_action, ns1__sendSummary *ns1__sendSummary_, ns1__sendSummaryResponse &ns1__sendSummaryResponse_)
{
	struct __sunat__sendSummary soap_tmp___sunat__sendSummary;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "https://e-factura.sunat.gob.pe:443/ol-ti-itcpfegem/billService";
	if (soap_action == NULL)
		soap_action = "urn:sendSummary";
	soap_tmp___sunat__sendSummary.ns1__sendSummary_ = ns1__sendSummary_;
	soap_begin(soap);
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize___sunat__sendSummary(soap, &soap_tmp___sunat__sendSummary);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___sunat__sendSummary(soap, &soap_tmp___sunat__sendSummary, "-sunat:sendSummary", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___sunat__sendSummary(soap, &soap_tmp___sunat__sendSummary, "-sunat:sendSummary", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!static_cast<ns1__sendSummaryResponse*>(&ns1__sendSummaryResponse_)) // NULL ref?
		return soap_closesock(soap);
	ns1__sendSummaryResponse_.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	ns1__sendSummaryResponse_.soap_get(soap, "ns1:sendSummaryResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
/* End of client proxy code */
