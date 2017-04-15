package util;

import android.util.Log;

import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapPrimitive;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.ksoap2.transport.HttpTransportSE;

public class ServiceFactory {

    public static SoapObject FactoryObject(String[] _params, Object[] _data,String _webMethod,String _URL)
    {
        String METHOD_NAME = _webMethod;
        String NAMESPACE = VService.NAMESPACE;
        String SOAP_ACTION = NAMESPACE + METHOD_NAME;
        String URL = _URL;
        try{

            SoapSerializationEnvelope envelope;
            SoapObject soapobject = new SoapObject(NAMESPACE, METHOD_NAME);
            envelope = new SoapSerializationEnvelope(SoapEnvelope.VER11);
            //110
            envelope.bodyOut = soapobject;
           // envelope.setOutputSoapObject(soapobject);
            envelope.dotNet = true;

            HttpTransportSE httptransportse = new HttpTransportSE(URL);

            for(int x=0;x<_params.length;x++)
            {
                soapobject.addProperty(_params[x], _data[x]);
            }

            SoapObject SOAP;
           httptransportse.debug = true;
            httptransportse.call(SOAP_ACTION, envelope);
            SOAP = (SoapObject)envelope.getResponse();

            return  SOAP;
        }
        catch(Exception ex)
        {
            Log.e("Error Inesperado",ex.getMessage());
            return null;

        }
    }

    public static SoapPrimitive FactoryPrimitive(String[] _params, Object[] _data,String _webMethod,String _URL)
    {
        String METHOD_NAME = _webMethod;
        String NAMESPACE = VService.NAMESPACE;
        String SOAP_ACTION = NAMESPACE + METHOD_NAME;
        String URL = _URL;
        try{

            SoapSerializationEnvelope soapserializationenvelope;
            SoapObject soapobject = new SoapObject(NAMESPACE, METHOD_NAME);
            soapserializationenvelope = new SoapSerializationEnvelope(110);
            soapserializationenvelope.bodyOut = soapobject;
            soapserializationenvelope.dotNet = true;
            HttpTransportSE httptransportse = new HttpTransportSE(URL);
            for(int x=0;x<_params.length;x++)
            {
                soapobject.addProperty(_params[x], _data[x]);
            }

            SoapPrimitive SOAP;
            httptransportse.debug = false;
            httptransportse.call(SOAP_ACTION, soapserializationenvelope);
            SOAP = (SoapPrimitive)soapserializationenvelope.getResponse();

            return  SOAP;
        }
        catch(Exception ex)
        {
            Log.e("Error", ex.getMessage());
            return null;

        }
    }



    public  SoapPrimitive FactoryPrimitive1(String[] _params, Object[] _data,String _webMethod,String _URL)
    {
        String METHOD_NAME = _webMethod;
        String NAMESPACE = VService.NAMESPACE;
        String SOAP_ACTION = NAMESPACE + METHOD_NAME;
        String URL = _URL;
        try{

            SoapSerializationEnvelope soapserializationenvelope;
            SoapObject soapobject = new SoapObject(NAMESPACE, METHOD_NAME);
            soapserializationenvelope = new SoapSerializationEnvelope(110);
            soapserializationenvelope.bodyOut = soapobject;
            soapserializationenvelope.dotNet = true;
            HttpTransportSE httptransportse = new HttpTransportSE(URL);
            for(int x=0;x<_params.length;x++)
            {
                soapobject.addProperty(_params[x], _data[x]);
            }

            SoapPrimitive SOAP;
            httptransportse.debug = false;
            httptransportse.call(SOAP_ACTION, soapserializationenvelope);
            SOAP = (SoapPrimitive)soapserializationenvelope.getResponse();

            return  SOAP;
        }
        catch(Exception ex)
        {
            return null;

        }
    }


    public  SoapPrimitive FactoryPrimitive2(String[] _params, Object[] _data,String _webMethod,String _URL)
    {
        String METHOD_NAME = _webMethod;
        String NAMESPACE = VService.NAMESPACE;
        String SOAP_ACTION = NAMESPACE + METHOD_NAME;
        String URL = _URL;
        try{

            SoapSerializationEnvelope soapserializationenvelope;
            SoapObject soapobject = new SoapObject(NAMESPACE, METHOD_NAME);
            soapserializationenvelope = new SoapSerializationEnvelope(110);
            soapserializationenvelope.bodyOut = soapobject;
            soapserializationenvelope.dotNet = true;
            HttpTransportSE httptransportse = new HttpTransportSE(URL);
            for(int x=0;x<_params.length;x++)
            {
                soapobject.addProperty(_params[x], _data[x]);
            }

            SoapPrimitive SOAP;
            httptransportse.debug = false;
            httptransportse.call(SOAP_ACTION, soapserializationenvelope);
            SOAP = (SoapPrimitive)soapserializationenvelope.getResponse();

            return  SOAP;
        }
        catch(Exception ex)
        {
            return null;

        }
    }



}
