package dominio;

import java.io.Serializable;

/**
 * Created by lorda on 25/04/2017.
 */

public class Planta implements Serializable {
    private int id;
    private String planta;

    public Planta(int id, String planta) {
        this.id=id;
        this.planta=planta;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public String getPlanta() {
        return planta;
    }

    public void setPlanta(String planta) {
        this.planta = planta;
    }

    @Override
    public String toString() {
        return planta;
    }
}
