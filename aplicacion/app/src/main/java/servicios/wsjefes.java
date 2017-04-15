package servicios;

import android.util.Log;

import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapPrimitive;

import java.util.ArrayList;
import java.util.List;

import dominio.Equipo;
import dominio.Jefe;
import util.ServiceFactory;
import util.VService;

/**
 * Created by ledys on 21/08/2016.
 */
public class wsjefes {
    public List<Jefe> GetJefes()
    {
        String[] _params={};
        String[] _data={};
        SoapObject SOAPClientes= ServiceFactory.FactoryObject(_params, _data, "GetJefes",VService.URL_SERVICIOWEB);

        List<Jefe> ListJefes=new ArrayList<Jefe>();
        if(SOAPClientes == null){
            return ListJefes;
        }
        for(int x=0;x<SOAPClientes.getPropertyCount();x++)
        {
            SoapObject so=(SoapObject) SOAPClientes.getProperty(x);
            Jefe jefe=new Jefe(Integer.parseInt(so.getProperty(0).toString()),
                    so.getProperty(1).toString(),
                    so.getProperty(2).toString(),
                    so.getProperty(3).toString(),
                    so.getProperty(4).toString());
            //Log.e("ES JEFE",so.getProperty(4).toString());
            ListJefes.add(jefe);
        }
        return ListJefes;

    }
    public String InsertUpdateJefe(String id,
                                     String nombres,
                                     String usuario,
                                     String password,
                                     String subgerente)
    {

        String[] _params={"id","nombres","usuario","password","subgerente"};
        String[] _data={id,nombres,usuario,password,subgerente};
        SoapPrimitive _SoapObject=ServiceFactory.FactoryPrimitive(_params, _data, "InsertUpdateJefe",VService.URL_SERVICIOWEB);
        if(_SoapObject==null)
        {
            return null;
        }
        else{
            return _SoapObject.toString();
        }
    }
    public String DeleteJefe(String id)
    {
        String[] _params={"id"};
        String[] _data={id};
        SoapPrimitive _SoapObject=ServiceFactory.FactoryPrimitive(_params, _data, "DeleteJefe",VService.URL_SERVICIOWEB);
        if(_SoapObject==null)
        {
            return null;
        }
        else{
            return _SoapObject.toString();
        }
    }
}
