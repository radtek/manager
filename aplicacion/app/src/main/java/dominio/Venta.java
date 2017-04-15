package dominio;

/**
 * Created by ledys on 10/09/2016.
 */
public class Venta {
    private int ID;
    private String fecha;
    private String producto;
    private String ticket;
    private String galones;
    private String horometro;
    private String km;
    private String equipo;

    public Venta(int ID,String fecha,String producto,String ticket, String galones,String horometro,String km,String equipo ) {
        this.ID=ID;
        this.fecha=fecha;
        this.producto=producto;
        this.ticket=ticket;
        this.galones=galones;
        this.horometro=horometro;
        this.km=km;
        this.equipo=equipo;
    }

    public int getID() {
        return ID;
    }

    public void setID(int ID) {
        this.ID = ID;
    }


    public String getFecha() {
        return fecha;
    }

    public String getEquipo() {
        return equipo;
    }

    public String getGalones() {
        return galones;
    }

    public String getHorometro() {
        return horometro;
    }

    public String getKm() {
        return km;
    }

    public String getProducto() {
        return producto;
    }

    public String getTicket() {
        return ticket;
    }

    public void setEquipo(String equipo) {
        this.equipo = equipo;
    }

    public void setFecha(String fecha) {
        this.fecha = fecha;
    }

    public void setGalones(String galones) {
        this.galones = galones;
    }

    public void setHorometro(String horometro) {
        this.horometro = horometro;
    }

    public void setKm(String km) {
        this.km = km;
    }

    public void setProducto(String producto) {
        this.producto = producto;
    }

    public void setTicket(String ticket) {
        this.ticket = ticket;
    }

}

