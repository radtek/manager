
package Adapter;
import java.util.*;
import android.content.Context;
import android.view.*;
import android.widget.*;
/**
 * Created by ledys on 20/08/2016.
 */
public abstract class GenericAdapter extends BaseAdapter{

    private List<?> entradas;
    private int R_layout_IdView;
    private Context contexto;
    public GenericAdapter(Context contexto, int R_layout_IdView, List<?> entradas) {
        super();
        this.contexto = contexto;
        this.entradas = entradas;
        this.R_layout_IdView = R_layout_IdView;
    }
    @Override
    public View getView(int posicion, View view, ViewGroup pariente) {
        if (view == null) {
            LayoutInflater vi = (LayoutInflater) contexto.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = vi.inflate(R_layout_IdView, null);
        }
        Create (entradas.get(posicion), view);
        return view;
    }
    @Override
    public int getCount() {
        return entradas.size();
    }
    @Override
    public Object getItem(int posicion) {
        return entradas.get(posicion);
    }
    @Override
    public long getItemId(int posicion) {
        return posicion;
    }
    public abstract void Create (Object entrada, View view);
}
