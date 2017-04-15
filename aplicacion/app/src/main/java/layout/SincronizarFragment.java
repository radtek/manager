package layout;

import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.support.design.widget.Snackbar;
import com.example.app.aplicacion.R;
import java.util.Iterator;
import java.util.List;
import com.github.mikephil.charting.data.*;
import Adapter.GenericAdapter;
import com.github.mikephil.charting.charts.BarChart;
import dominio.Equipo;
import dominio.Jefe;
import dominio.Venta;
import servicios.wsequipos;
import servicios.wsventas;
import sqlite.WSqlite;
import util.CallService;
import java.util.ArrayList;
import com.github.mikephil.charting.utils.ColorTemplate;
import android.graphics.Color;
/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link SincronizarFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link SincronizarFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class SincronizarFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    public SincronizarFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment SincronizarFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static SincronizarFragment newInstance(String param1, String param2) {
        SincronizarFragment fragment = new SincronizarFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }
    Context context;
    View rootView;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        rootView =inflater.inflate(R.layout.fragment_sincronizar, container, false);
        context = container.getContext();

        Button btnSincronizar = (Button)rootView.findViewById(R.id.btnSincronizar);
        btnSincronizar.setOnClickListener(ClickSincronizar);

        ActualizarVentasSincronizar();

        boolean tieneacceso=util.AccesoInternet.Verificar(context);
        if(!tieneacceso)
            Toast.makeText(context, "No tienes acceso a Internet la aplicacion podria fallar.", Toast.LENGTH_LONG).show();




       /* ArrayList<BarEntry> entries = new ArrayList<>();
        entries.add(new BarEntry(4f, 0));
        entries.add(new BarEntry(8f, 1));
        entries.add(new BarEntry(6f, 2));
        entries.add(new BarEntry(12f, 3));
        entries.add(new BarEntry(18f, 4));
        entries.add(new BarEntry(9f, 5));
        BarDataSet dataset = new BarDataSet(entries, "# of Calls");
        ArrayList<String> labels = new ArrayList<String>();
        labels.add("January");
        labels.add("February");
        labels.add("March");
        labels.add("April");
        labels.add("May");
        labels.add("June");
        BarChart chart = new BarChart(context);
        //setContentView(chart);
        BarData data = new BarData(labels, dataset);
        chart.setData(data);
        chart.setDescription("# of times Alice called Bob");
*/
        return rootView;
    }


    private ArrayList<BarDataSet> getDataSet() {
        ArrayList<BarDataSet> dataSets = null;

        ArrayList<BarEntry> valueSet1 = new ArrayList<>();
        BarEntry v1e1 = new BarEntry(110.000f, 0); // Jan
        valueSet1.add(v1e1);
        BarEntry v1e2 = new BarEntry(40.000f, 1); // Feb
        valueSet1.add(v1e2);
        BarEntry v1e3 = new BarEntry(60.000f, 2); // Mar
        valueSet1.add(v1e3);
        BarEntry v1e4 = new BarEntry(30.000f, 3); // Apr
        valueSet1.add(v1e4);
        BarEntry v1e5 = new BarEntry(90.000f, 4); // May
        valueSet1.add(v1e5);
        BarEntry v1e6 = new BarEntry(100.000f, 5); // Jun
        valueSet1.add(v1e6);

        ArrayList<BarEntry> valueSet2 = new ArrayList<>();
        BarEntry v2e1 = new BarEntry(150.000f, 0); // Jan
        valueSet2.add(v2e1);
        BarEntry v2e2 = new BarEntry(90.000f, 1); // Feb
        valueSet2.add(v2e2);
        BarEntry v2e3 = new BarEntry(120.000f, 2); // Mar
        valueSet2.add(v2e3);
        BarEntry v2e4 = new BarEntry(60.000f, 3); // Apr
        valueSet2.add(v2e4);
        BarEntry v2e5 = new BarEntry(20.000f, 4); // May
        valueSet2.add(v2e5);
        BarEntry v2e6 = new BarEntry(80.000f, 5); // Jun
        valueSet2.add(v2e6);

        BarDataSet barDataSet1 = new BarDataSet(valueSet1, "Brand 1");
        barDataSet1.setColor(Color.rgb(0, 155, 0));
        BarDataSet barDataSet2 = new BarDataSet(valueSet2, "Brand 2");
        barDataSet2.setColors(ColorTemplate.COLORFUL_COLORS);

        dataSets = new ArrayList<>();
        dataSets.add(barDataSet1);
        dataSets.add(barDataSet2);
        return dataSets;
    }

    private ArrayList<String> getXAxisValues() {
        ArrayList<String> xAxis = new ArrayList<>();
        xAxis.add("JAN");
        xAxis.add("FEB");
        xAxis.add("MAR");
        xAxis.add("APR");
        xAxis.add("MAY");
        xAxis.add("JUN");
        return xAxis;
    }
    public void ActualizarVentasSincronizar()
    {
        WSqlite ws=new WSqlite();
        int NroVentas= ws.COUNT_VENTAS(context);
        TextView TextNroVentas=(TextView)rootView.findViewById(R.id.NroVentas);
        TextNroVentas.setText("Se encontraron "+ String.valueOf(NroVentas)+ " Despachos para Sincronizar");

    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
           // throw new RuntimeException(context.toString()
            //        + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }
    private View.OnClickListener ClickSincronizar = new View.OnClickListener() {
        public void onClick(View v) {
            boolean tieneacceso=util.AccesoInternet.Verificar(context);
            if(!tieneacceso)
                return;
            CargarEquipos();
        }
    };

    public void EnviarVentas()
    {

        final WSqlite sqlite=new WSqlite();
        List<Venta> ListaVentas= sqlite.GET_VENTAS(context);
        if(ListaVentas.size()==0) {
            Toast.makeText(context, "No Hay Despachos Para Sincronizar", Toast.LENGTH_LONG).show();
        }
        else {
            final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Sincronizando Informacion de Despachos....", true,false);
            pd.show();
            for (Iterator<Venta> v = ListaVentas.iterator(); v.hasNext(); ) {
                final Venta item = v.next();
                new CallService() {
                    @Override
                    protected void onPostExecute(Object result) {

                        pd.dismiss();
                        if (result == null) {
                            Toast.makeText(context, "Se produjo un Error", Toast.LENGTH_LONG).show();
                        } else {
                            String IDLOCAL = result.toString().split("-")[1];
                            sqlite.DELETE_VENTA(context, IDLOCAL);//Eliminamos la venta sincronizada
                            ActualizarVentasSincronizar();
                            Snackbar.make(rootView, "Despacho Sincronizado",
                                    Snackbar.LENGTH_LONG).setAction("Action", null).show();
                        }
                    }

                    @Override
                    protected Object doInBackground(String... args) {
                        wsventas ws = new wsventas();
                        String estado = ws.InsertVenta(item.getFecha(), item.getProducto(), item.getTicket(), item.getGalones(),
                                item.getHorometro(), item.getKm(), item.getEquipo());
                        estado = estado + "-" + String.valueOf(item.getID());
                        return estado;
                    }
                }.execute();
            }
        }
    }

    public void CargarEquipos()
    {
        final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Sincronizando Informacion de Equipos....", true,false);
        pd.show();


        new CallService() {
            @Override
            protected void onPostExecute(Object result) {

                pd.dismiss();
                if(result==null)
                {
                    Toast.makeText(context,"Data vacia", Toast.LENGTH_LONG).show();
                }
                else
                {
                    WSqlite sqlite=new WSqlite();

                    List<Equipo> ListaEquipos=(List<Equipo>)result;
                    sqlite.DELETE_EQUIPOS(context);
                    for(Iterator<Equipo> i = ListaEquipos.iterator(); i.hasNext();)
                    {
                        Equipo item = i.next();
                        //Log.e("SincronizarFragment-Equipo", item.getCodigoInterno());
                        sqlite.INSERT_EQUIPO(context,
                                            item.getId(),
                                            item.getCentro(),
                                            item.getPlanta(),
                                            item.getCodigoInterno(),
                                            item.getPlaca(),
                                            item.getClase());

                    }
                    EnviarVentas();
                    Snackbar.make(rootView, "Equipos Sincronizados", Snackbar.LENGTH_SHORT)
                            .setAction("Action", null).show();
                }
            }

            @Override
            protected Object doInBackground(String... args) {
                wsequipos ws=new wsequipos();
                return ws.GetEquipos();
            }
        }.execute();
    }

}
