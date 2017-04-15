package servicios;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapPrimitive;
import dominio.Equipo;
import util.ServiceFactory;
import util.VService;
/**
 * Created by ledys on 21/08/2016.
 */
public class wsequipos {
    public List<Equipo> GetEquipos()
    {
        String[] _params={};
        String[] _data={};
        SoapObject SOAPClientes= ServiceFactory.FactoryObject(_params, _data, "GetEquipos",VService.URL_SERVICIOWEB);

        List<Equipo> ListEquipos=new ArrayList<Equipo>();
        if(SOAPClientes == null){
            return ListEquipos;
        }
        for(int x=0;x<SOAPClientes.getPropertyCount();x++)
        {
            SoapObject so=(SoapObject) SOAPClientes.getProperty(x);
            Equipo contacto=new Equipo(Integer.parseInt(so.getProperty(0).toString()),
                    so.getProperty(1).toString(),
                    so.getProperty(2).toString(),
                    so.getProperty(3).toString(),
                    so.getProperty(4).toString(),
                    so.getProperty(5).toString());
            ListEquipos.add(contacto);
        }
        return ListEquipos;

    }

    public String InsertUpdateEquipo(String id,
                                  String centro,
                                  String planta,
                                  String cinterno,
                                  String placa,
                                  String clase)
{

        String[] _params={"id","centro","planta","cinterno","placa","clase"};
        String[] _data={id,centro,planta,cinterno,placa,clase};
        SoapPrimitive _SoapObject=ServiceFactory.FactoryPrimitive(_params, _data, "InsertUpdateEquipo",VService.URL_SERVICIOWEB);
        if(_SoapObject==null)
        {
            return null;
        }
        else{
            return _SoapObject.toString();
        }
    }
    public String DeleteEquipo(String id)
    {
        String[] _params={"id"};
        String[] _data={id};
        SoapPrimitive _SoapObject=ServiceFactory.FactoryPrimitive(_params, _data, "DeleteEquipo",VService.URL_SERVICIOWEB);
        if(_SoapObject==null)
        {
            return null;
        }
        else{
            return _SoapObject.toString();
        }
    }





}
