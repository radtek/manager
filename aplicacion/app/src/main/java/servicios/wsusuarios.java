package servicios;


import android.util.Log;

import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapPrimitive;

import java.util.ArrayList;
import java.util.List;

import dominio.Equipo;
import dominio.Usuario;
import util.ServiceFactory;
import util.VService;
/**
 * Created by lorda on 8/04/2017.
 */

public class wsusuarios {
    public List<Usuario> GetUsuarios()
    {
        String[] _params={};
        String[] _data={};
        SoapObject SOAPClientes= ServiceFactory.FactoryObject(_params, _data, "GetUsuarios",VService.URL_SERVICIOWEB);

        List<Usuario> ListUsuarios = new ArrayList<Usuario>();
        if(SOAPClientes == null){
            return ListUsuarios;
        }
        for(int x=0;x<SOAPClientes.getPropertyCount();x++)
        {
            SoapObject so=(SoapObject) SOAPClientes.getProperty(x);
            Usuario jefe=new Usuario(Integer.parseInt(so.getProperty(0).toString()),
                    so.getProperty(1).toString(),
                    so.getProperty(2).toString(),
                    so.getProperty(3).toString(),
                    so.getProperty(4).toString());
            //Log.e("ES JEFE",so.getProperty(4).toString());
            ListUsuarios.add(jefe);
        }
        return ListUsuarios;
    }
    public String InsertUpdateJefe(String id,
                                   String email,
                                   String usuario,
                                   String password,
                                   String tipo)
    {

        String[] _params={"id","email","usuario","password","tipo"};
        String[] _data={id,email,usuario,password,tipo};
        SoapPrimitive _SoapObject=ServiceFactory.FactoryPrimitive(_params, _data, "InsertUpdateUsuario",VService.URL_SERVICIOWEB);
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
        SoapPrimitive _SoapObject=ServiceFactory.FactoryPrimitive(_params, _data, "DeleteUsuario",VService.URL_SERVICIOWEB);
        if(_SoapObject==null)
        {
            return null;
        }
        else{
            return _SoapObject.toString();
        }
    }
}
