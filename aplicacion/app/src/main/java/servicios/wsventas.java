package servicios;

import org.ksoap2.serialization.SoapPrimitive;

import util.ServiceFactory;
import util.VService;

/**
 * Created by ledys on 11/09/2016.
 */
public class wsventas {
    public String InsertVenta(String FECHA,
                              String PRODUCTO,
                              String TICKET,
                              String NROGALONES,
                              String HOROMETRO,
                              String KM,
                              String EQUIPO)
    {

        String[] _params={"FECHA","PRODUCTO","TICKET","NROGALONES","HOROMETRO","KM","EQUIPO"};
        String[] _data={FECHA,PRODUCTO,TICKET,NROGALONES,HOROMETRO,KM,EQUIPO};
        SoapPrimitive _SoapObject= ServiceFactory.FactoryPrimitive(_params, _data, "InsertVenta", VService.URL_SERVICIOWEB);
        if(_SoapObject==null)
        {
            return null;
        }
        else{
            return _SoapObject.toString();
        }
    }
}
