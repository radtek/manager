package layout;

import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
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

import dominio.ReporteRatio;
import servicios.wsreportes;
import util.CallService;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link RatioEquipo.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link RatioEquipo#newInstance} factory method to
 * create an instance of this fragment.
 */
public class RatioEquipo extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    public RatioEquipo() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment RatioEquipo.
     */
    // TODO: Rename and change types and number of parameters
    public static RatioEquipo newInstance(String param1, String param2) {
        RatioEquipo fragment = new RatioEquipo();
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

        rootView = inflater.inflate(R.layout.fragment_ratio_equipo, container, false);
        context = container.getContext();

        ReporteRatioEquipo();
        return rootView;


    }

    public void ReporteRatioEquipo()
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
                    List<ReporteRatio> Resultado=  (List<ReporteRatio>)result;
                    RenderReporte(Resultado);
                }
                pd.dismiss();
            }

            @Override
            protected Object doInBackground(String... args) {
                wsreportes ws=new wsreportes();
                return ws.GetRatioEquipo();
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
        chart.setDescription("Ratio x Equipo");
        chart.animateXY(2000, 2000);
        chart.invalidate();
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
          //  throw new RuntimeException(context.toString()
           //         + " must implement OnFragmentInteractionListener");
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
