package dominio;
import java.io.Serializable;
/**
 * Created by lorda on 8/04/2017.
 */

public class Usuario implements Serializable{
    private int id;
    private String email;
    private String usuario;
    private String password;
    private String tipo;
    public Usuario(int id,String email,String usuario,String password,String tipo) {
        this.id=id;
        this.email=email;
        this.usuario=usuario;
        this.password=password;
        this.tipo=tipo;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setUsuario(String usuario) {
        this.usuario = usuario;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public int getId() {
        return id;
    }

    public String getEmail() {
        return email;
    }

    public String getUsuario() {
        return usuario;
    }

    public String getPassword() {
        return password;
    }

    public String getTipo() {
        return tipo;
    }
    @Override
    public String toString() { return email + " - " + usuario; }
}
