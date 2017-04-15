package dominio;
import java.io.Serializable;
/**
 * Created by ledys on 21/08/2016.
 */
public class Equipo implements Serializable {

    private int id;
    private String centro;
    private String planta;
    private String codigoInterno;
    private String placa;
    private String clase;

    public Equipo(int id,String centro,String planta,String codigoInterno, String placa,String clase ) {
        this.id=id;
        this.centro=centro;
        this.planta=planta;
        this.codigoInterno=codigoInterno;
        this.placa=placa;
        this.clase=clase;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public String getCentro() {
        return centro;
    }

    public String getClase() {
        return clase;
    }

    public String getCodigoInterno() {
        return codigoInterno;
    }

    public String getPlaca() {
        return placa;
    }

    public String getPlanta() {
        return planta;
    }

    public void setCentro(String centro) {
        this.centro = centro;
    }

    public void setClase(String clase) {
        this.clase = clase;
    }

    public void setCodigoInterno(String codigoInterno) {
        this.codigoInterno = codigoInterno;
    }

    public void setPlaca(String placa) {
        this.placa = placa;
    }

    public void setPlanta(String planta) {
        this.planta = planta;
    }

    @Override
    public String toString() {
        return codigoInterno;
    }
}
