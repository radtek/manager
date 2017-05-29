package layout;

import android.app.ProgressDialog;
import android.content.Context;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.util.Pools;
import android.support.v7.widget.LinearLayoutCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.HorizontalScrollView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.app.aplicacion.R;
import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.utils.ColorTemplate;
import com.github.mikephil.charting.utils.LimitLine;

import java.util.ArrayList;
import java.util.List;

import Adapter.GenericAdapter;
import dominio.Equipo;
import dominio.ReporteRatio;
import servicios.wsequipos;
import servicios.wsreportes;
import sqlite.WSqlite;
import util.CallService;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link FragmentRatioPlanta.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link FragmentRatioPlanta#newInstance} factory method to
 * create an instance of this fragment.
 */
public class FragmentRatioPlanta extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    private String planta = "";
    private String date_inicial = "";
    private String date_final = "";
    private String equipo_id = "";

    public void setData(String planta, String date_inicial, String date_final, String equipo_id){
        Log.d("ratio", planta);
        this.planta = planta;
        this.date_inicial = date_inicial;
        this.date_final = date_final;
        this.equipo_id = equipo_id;
    }

    private DisplayMetrics dm;

    public FragmentRatioPlanta() {
        // Required empty public constructor
    }


    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment FragmentRatioPlanta.
     */
    // TODO: Rename and change types and number of parameters
    public static FragmentRatioPlanta newInstance(String param1, String param2) {
        FragmentRatioPlanta fragment = new FragmentRatioPlanta();
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

        dm = new DisplayMetrics();
        getActivity().getWindowManager().getDefaultDisplay().getMetrics(dm);
        // Inflate the layout for this fragment
        rootView =inflater.inflate(R.layout.fragment_fragment_ratio_planta, container, false);
        context = container.getContext();

        Button btn_buscar = (Button)rootView.findViewById(R.id.button_buscar);
        btn_buscar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Fragment fragment = new RatioBuscarFragment();
                //Bundle bundle = new Bundle();
                /*
                Equipo equipo=new Equipo(0,null,null,null,null,null);
                bundle.putSerializable("equipo",equipo);
                fragment.setArguments(bundle);
                */
                /*
                EditText et_buscar = (EditText) rootView.findViewById(R.id.editText_buscar);
                InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(et_buscar.getWindowToken(), 0);
                */

                ((MainActivity)getActivity()).listFragments.add(fragment);

                FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                        .commit();
            }
        });

        //ReporteRatioPlanta();
        reporteRatio();

        return rootView;
    }

    public void reporteRatio()
    {
        WSqlite sqlite = new WSqlite();

        List<Pair<String, Double>> ratios = sqlite.GET_RATIOPLANTA(context, planta);
        List<ReporteRatio> resultado = new ArrayList<ReporteRatio>();
        //Log.d("ratio", "inicio");
        for(int i=0; i < ratios.size(); i++){
            String indicador = ratios.get(i).first;
            double ratio = ratios.get(i).second;

            //Log.d("ratio", indicador);
            //Log.d("ratio", String.valueOf(ratio));
            ReporteRatio reporteRatio = new ReporteRatio(indicador,
                    ratio);
            resultado.add(reporteRatio);
        }
        //Log.d("ratio", "fin");
        RenderReporte(resultado);
    }

    public void ReporteRatioPlanta()
    {
        final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Cargando Reporte....", true,false);
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
                    List<ReporteRatio> Resultado =  (List<ReporteRatio>)result;
                    RenderReporte(Resultado);
                }
                pd.dismiss();
            }
            @Override
            protected Object doInBackground(String... args) {
                wsreportes ws=new wsreportes();
                return ws.GetRatioPlanta();
            }
        }.execute();
    }

    public void RenderReporte(List<ReporteRatio> Ratios)
    {
        LimitLine line = new LimitLine(1.6f);

        BarChart chart = (BarChart) rootView.findViewById(R.id.chart);
        ArrayList<BarDataSet> dataSets = null;
        ArrayList<String> xAxis = new ArrayList<>();
        ArrayList<BarEntry> valueSet1 = new ArrayList<>();
        int x=0;
        int width = dm.widthPixels + dm.widthPixels*((Ratios.size()-1)/3);

        //ScrollView scroll = (ScrollView)rootView.findViewById(R.id.scrollView1);
        //scroll.setHorizontalScrollBarEnabled(true);
        //scroll.addView(chart);
        ViewGroup.LayoutParams layout = chart.getLayoutParams();
        layout.width = width;
        //chart.setLayoutParams(layout);
        //chart.setMinimumWidth(0);
        //chart.setLayoutParams(0, 1);

        Log.d("ratio", "width " + String.valueOf(chart.getLayoutParams().width));
        Log.d("ratio", "height " + String.valueOf(chart.getLayoutParams().height));

        for(ReporteRatio r:Ratios)
        {
            xAxis.add(r.getIndicador());
            String ratio=String.valueOf(r.getRatio());
            BarEntry v1e1 = new BarEntry(Float.parseFloat(ratio) , x); // Feb
            valueSet1.add(v1e1);
            x++;
        }
        BarDataSet barDataSet1 = new BarDataSet(valueSet1, "Brand 1");
        //barDataSet1.setColor(Color.rgb(0, 155, 0));
        barDataSet1.setColors(ColorTemplate.COLORFUL_COLORS);
        //BarDataSet barDataSet2 = new BarDataSet(valueSet2, "Brand 2");
        // barDataSet2.setColors(ColorTemplate.COLORFUL_COLORS);

        dataSets = new ArrayList<>();
        dataSets.add(barDataSet1);
        //dataSets.add(barDataSet2);

        BarData data = new BarData(xAxis, dataSets);
        data.addLimitLine(line);
        chart.setData(data);
        chart.setDescription("Ratio x Planta");
        chart.animateXY(2000, 2000);
        chart.invalidate();
    }
    private ArrayList<String> getXAxisValues(List<ReporteRatio> Ratios) {

        ArrayList<BarDataSet> dataSets = null;
        ArrayList<String> xAxis = new ArrayList<>();
        ArrayList<BarEntry> valueSet1 = new ArrayList<>();
        int x=0;
        for(ReporteRatio r:Ratios)
        {
            xAxis.add(r.getIndicador());

            BarEntry v1e1 = new BarEntry(40.000f, x); // Feb
            valueSet1.add(v1e1);
            x++;
        }
        BarDataSet barDataSet1 = new BarDataSet(valueSet1, "Brand 1");
        //barDataSet1.setColor(Color.rgb(0, 155, 0));
        barDataSet1.setColors(ColorTemplate.COLORFUL_COLORS);
        //BarDataSet barDataSet2 = new BarDataSet(valueSet2, "Brand 2");
        // barDataSet2.setColors(ColorTemplate.COLORFUL_COLORS);

        dataSets = new ArrayList<>();
        dataSets.add(barDataSet1);
        //dataSets.add(barDataSet2);



        return xAxis;
    }
    private ArrayList<BarDataSet> getDataSet(List<ReporteRatio> Ratios) {
        ArrayList<BarDataSet> dataSets = null;

        ArrayList<BarEntry> valueSet1 = new ArrayList<>();
        //BarEntry v1e1 = new BarEntry(110.000f, 0); // Jan
        for(ReporteRatio r:Ratios)
        {

            BarEntry v1e1 = new BarEntry(40.000f, 1); // Feb
            valueSet1.add(v1e1);
        }


       /*
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
*/
        BarDataSet barDataSet1 = new BarDataSet(valueSet1, "Brand 1");
        //barDataSet1.setColor(Color.rgb(0, 155, 0));
        barDataSet1.setColors(ColorTemplate.COLORFUL_COLORS);
        //BarDataSet barDataSet2 = new BarDataSet(valueSet2, "Brand 2");
       // barDataSet2.setColors(ColorTemplate.COLORFUL_COLORS);

        dataSets = new ArrayList<>();
        dataSets.add(barDataSet1);
        //dataSets.add(barDataSet2);
        return dataSets;
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
/*            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");*/
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p/>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }
}
