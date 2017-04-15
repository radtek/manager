package servicios;

import org.ksoap2.serialization.SoapObject;

import java.util.ArrayList;
import java.util.List;

import dominio.Jefe;
import dominio.ReporteRatio;
import util.ServiceFactory;
import util.VService;

/**
 * Created by ledys on 24/10/2016.
 */
public class wsreportes {


    public List<ReporteRatio> GetRatioPlanta()
    {
        String[] _params={};
        String[] _data={};
        SoapObject SOAPClientes= ServiceFactory.FactoryObject(_params, _data, "GetRatioPlanta", VService.URL_SERVICIOWEB);

        List<ReporteRatio> ListRatios=new ArrayList<ReporteRatio>();
        if(SOAPClientes == null){
            return ListRatios;
        }
        for(int x=0;x<SOAPClientes.getPropertyCount();x++)
        {
            SoapObject so=(SoapObject) SOAPClientes.getProperty(x);
            ReporteRatio ratio=new ReporteRatio(so.getProperty(0).toString(),
                    Double.parseDouble(so.getProperty(1).toString()));
            ListRatios.add(ratio);
        }
        return ListRatios;

    }
    public List<ReporteRatio> GetRatioDia()
    {
        String[] _params={};
        String[] _data={};
        SoapObject SOAPClientes= ServiceFactory.FactoryObject(_params, _data, "GetRatioDia", VService.URL_SERVICIOWEB);

        List<ReporteRatio> ListRatios=new ArrayList<ReporteRatio>();
        if(SOAPClientes == null){
            return ListRatios;
        }
        for(int x=0;x<SOAPClientes.getPropertyCount();x++)
        {
            SoapObject so=(SoapObject) SOAPClientes.getProperty(x);
            ReporteRatio ratio=new ReporteRatio(so.getProperty(0).toString(),
                    Double.parseDouble(so.getProperty(1).toString()));
            ListRatios.add(ratio);
        }
        return ListRatios;

    }
    public List<ReporteRatio> GetRatioEquipo()
    {
        String[] _params={};
        String[] _data={};
        SoapObject SOAPClientes= ServiceFactory.FactoryObject(_params, _data, "GetRatioEquipo", VService.URL_SERVICIOWEB);

        List<ReporteRatio>ListRatios=new ArrayList<ReporteRatio>();
        if(SOAPClientes == null){
            return ListRatios;
        }
        for(int x=0;x<SOAPClientes.getPropertyCount();x++)
        {
            SoapObject so=(SoapObject) SOAPClientes.getProperty(x);
            ReporteRatio ratio=new ReporteRatio(so.getProperty(0).toString(),
                    Double.parseDouble(so.getProperty(1).toString()));
            ListRatios.add(ratio);
        }
        return ListRatios;

    }
}
