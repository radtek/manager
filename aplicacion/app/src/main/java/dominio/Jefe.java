package dominio;
import java.io.Serializable;
/**
 * Created by ledys on 21/08/2016.
 */
public class    Jefe implements Serializable{
    private int id;
    private String nombre;
    private String usuario;
    private String password;
    private String subgerente;
    public Jefe(int id,String nombre,String usuario,String password,String subgerente) {
        this.id=id;
        this.nombre=nombre;
        this.usuario=usuario;
        this.password=password;
        this.subgerente=subgerente;
    }

    public String getSubgerente() {
        return subgerente;
    }

    public void setSubgerente(String subgerente) {
        this.subgerente = subgerente;
    }


    public void setId(int id) {
        this.id = id;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setUsuario(String usuario) {
        this.usuario = usuario;
    }

    public int getId() {
        return id;
    }

    public String getNombre() {
        return nombre;
    }

    public String getPassword() {
        return password;
    }

    public String getUsuario() {
        return usuario;
    }

    @Override
    public String toString() { return nombre; }
}
