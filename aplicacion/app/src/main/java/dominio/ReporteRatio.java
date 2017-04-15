package dominio;

/**
 * Created by ledys on 24/10/2016.
 */
public class ReporteRatio {

    private String indicador;
    private double ratio;

    public ReporteRatio(String indicador,double ratio){
        this.indicador=indicador;
        this.ratio=ratio;

    }

    public void setIndicador(String planta) {
        this.indicador = planta;
    }

    public void setRatio(double ratio) {
        this.ratio = ratio;
    }

    public double getRatio() {
        return ratio;
    }

    public String getIndicador() {
        return indicador;
    }

}
