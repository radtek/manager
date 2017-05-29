package layout;

import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.app.aplicacion.R;
import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.utils.ColorTemplate;
import com.github.mikephil.charting.utils.LimitLine;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.List;

import dominio.ReporteRatio;
import servicios.wsreportes;
import sqlite.WSqlite;
import util.CallService;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link RatioDia.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link RatioDia#newInstance} factory method to
 * create an instance of this fragment.
 */
public class RatioDia extends Fragment {
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
        this.planta = planta;
        this.date_inicial = date_inicial;
        this.date_final = date_final;
        this.equipo_id = equipo_id;
    }

    public RatioDia() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment RatioDia.
     */
    // TODO: Rename and change types and number of parameters
    public static RatioDia newInstance(String param1, String param2) {
        RatioDia fragment = new RatioDia();
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
        rootView = inflater.inflate(R.layout.fragment_ratio_dia, container, false);
        context = container.getContext();

        TextView tw_planta = (TextView)rootView.findViewById(R.id.textView_planta);
        tw_planta.setText(planta);

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

        reporteRatio();
        return rootView;
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }
    public void reporteRatio()
    {
        WSqlite sqlite = new WSqlite();

        List<Pair<String, Double>> ratios = sqlite.GET_RATIOPLANTA_DIA(context, planta, date_inicial, date_final);
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
    public void RenderReporte(List<ReporteRatio> Ratios)
    {
        LimitLine line = new LimitLine(1.6f);

        BarChart chart = (BarChart) rootView.findViewById(R.id.chart);
        ArrayList<BarDataSet> dataSets = null;
        ArrayList<String> xAxis = new ArrayList<>();
        ArrayList<BarEntry> valueSet1 = new ArrayList<>();
        int x=0;
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
        chart.setDescription("Ratio x Dia");
        chart.animateXY(2000, 2000);
        chart.invalidate();
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
           // throw new RuntimeException(context.toString()
             //       + " must implement OnFragmentInteractionListener");
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
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }
}
