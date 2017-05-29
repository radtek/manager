package layout;

import android.annotation.SuppressLint;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.renderscript.ScriptGroup;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.text.InputType;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.example.app.aplicacion.R;

import java.text.SimpleDateFormat;
import java.util.Calendar;

import util.DatePickerFragment;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link RatioBuscarFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link RatioBuscarFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class RatioBuscarFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private static String planta_id = "";
    private static String planta = "";

    private static String equipo_id = "";
    private static String equipo_codigoInterno = "";
    private static String equipo_centro = "";
    private static String equipo_placa = "";

    private OnFragmentInteractionListener mListener;

    public RatioBuscarFragment() {
        // Required empty public constructor
    }

    public void setPlanta(String id, String planta) {
        planta_id = id;
        this.planta = planta;
    }

    public void setEquipo(String id, String codigoInterno, String centro, String placa) {
        equipo_id = id;
        equipo_codigoInterno = codigoInterno;
        equipo_centro = centro;
        equipo_placa = placa;
    }
    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment RatioBuscarFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static RatioBuscarFragment newInstance(String param1, String param2) {
        RatioBuscarFragment fragment = new RatioBuscarFragment();
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
        rootView = inflater.inflate(R.layout.fragment_ratio_buscar, container, false);
        context = container.getContext();

        TextView tw_planta = (TextView)rootView.findViewById(R.id.textView_planta);
        tw_planta.setText(planta);

        Button btn_planta = ((Button)rootView.findViewById(R.id.button_planta));
        btn_planta.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            ((MainActivity)getActivity()).replaceRatioBuscarPlanta();
            }
        });

        TextView tvPlanta = ((TextView)rootView.findViewById(R.id.textView_planta));
        tvPlanta.setText(planta);

        TextView tvCodigoInterno = ((TextView)rootView.findViewById(R.id.item_CodigoInterno));
        tvCodigoInterno.setText(equipo_codigoInterno);
        TextView tvCentro = ((TextView)rootView.findViewById(R.id.item_Centro));
        tvCentro.setText(equipo_centro);
        TextView tvPlaca = ((TextView)rootView.findViewById(R.id.item_Placa));
        tvPlaca.setText(equipo_placa);

        final TextView tv_equipo = (TextView)rootView.findViewById(R.id.textView_equipo);
        tv_equipo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).replaceRatioBuscarEquipo();
            }
        });

        final EditText date_ini = (EditText)rootView.findViewById(R.id.editText_ini_date);
        date_ini.setInputType(InputType.TYPE_NULL);

        date_ini.setOnClickListener(new View.OnClickListener() {

            public void onClick(View v) {
                DialogFragment picker = new DatePickerFragment(
                        date_ini);
                picker.show(getActivity()
                        .getSupportFragmentManager(), "datePicker");
            }
        });

        final EditText date_fin = (EditText)rootView.findViewById(R.id.editText_fin_date);
        date_fin.setInputType(InputType.TYPE_NULL);
        date_fin.setOnClickListener(new View.OnClickListener() {

            @SuppressLint("NewApi")
            @Override
            public void onClick(View v) {
                DialogFragment picker = new DatePickerFragment(
                        date_fin);
                picker.show(getActivity()
                        .getSupportFragmentManager(), "datePicker");
            }
        });

        Calendar c = Calendar.getInstance();

        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        c.set(c.get(Calendar.YEAR), c.get(Calendar.MONTH), c.get(Calendar.DAY_OF_MONTH)-4);
        String formattedDate = sdf.format(c.getTime());
        //Log.w("DatePickerFragment", formattedDate);
        date_ini.setText(formattedDate);

        sdf = new SimpleDateFormat("yyyy-MM-dd");
        c = Calendar.getInstance();
        formattedDate = sdf.format(c.getTime());
        date_fin.setText(formattedDate);

        Button btn_resultado = ((Button)rootView.findViewById(R.id.button_resultado));
        btn_resultado.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String planta = String.valueOf(RatioBuscarFragment.this.planta);
                String date_inicial = ((EditText) rootView.findViewById(R.id.editText_ini_date)).getText().toString();
                String date_final = ((EditText) rootView.findViewById(R.id.editText_fin_date)).getText().toString();
                String equipo_id = String.valueOf(RatioBuscarFragment.this.equipo_id);
                String codigoInterno = ((TextView) rootView.findViewById(R.id.item_CodigoInterno)).getText().toString();
                String centro = ((TextView) rootView.findViewById(R.id.item_Centro)).getText().toString();
                String placa = ((TextView) rootView.findViewById(R.id.item_Placa)).getText().toString();

                String className = ((MainActivity) getActivity()).listFragments
                        .get(((MainActivity) getActivity()).listFragments.size() - 2)
                        .getClass().getSimpleName();
                Log.d("ratio", "class name: "+className);
                if(className.contentEquals("FragmentRatioPlanta")){
                    Fragment fragment = ((MainActivity) getActivity()).listFragments
                            .get(((MainActivity) getActivity()).listFragments.size() - 2);
                    ((FragmentRatioPlanta)fragment).setData(planta
                            , date_inicial, date_final, equipo_id);
                }
                if(className.contentEquals("RatioDia")){
                    Fragment fragment = ((MainActivity) getActivity()).listFragments
                            .get(((MainActivity) getActivity()).listFragments.size() - 2);
                    ((RatioDia)fragment).setData(planta
                            , date_inicial, date_final, equipo_id);
                }
                if(className.contentEquals("RatioEquipo")){
                    Fragment fragment = ((MainActivity) getActivity()).listFragments
                            .get(((MainActivity) getActivity()).listFragments.size() - 2);
                    ((RatioEquipo)fragment).setData(planta
                            , date_inicial, date_final, equipo_id);
                }

                ((MainActivity) getActivity()).onBackPressed();
                return;
            }
        });

        final TextView tv_ini_date = (TextView)rootView.findViewById(R.id.textView_ini_date);
        final TextView tv_fin_date = (TextView)rootView.findViewById(R.id.textView_fin_date);
        String className = ((MainActivity) getActivity()).listFragments
                .get(((MainActivity) getActivity()).listFragments.size() - 2)
                .getClass().getSimpleName();
        Log.d("ratio", "class name: "+className);
        if(className.contentEquals("FragmentRatioPlanta")){
            tv_ini_date.setVisibility(View.GONE);
            tv_fin_date.setVisibility(View.GONE);
            date_ini.setVisibility(View.GONE);
            date_fin.setVisibility(View.GONE);

            tv_equipo.setVisibility(View.GONE);
            tvCodigoInterno.setVisibility(View.GONE);
            tvPlaca.setVisibility(View.GONE);
            tvCentro.setVisibility(View.GONE);
        }
        if(className.contentEquals("RatioDia")){
            tv_equipo.setVisibility(View.GONE);
            tvCodigoInterno.setVisibility(View.GONE);
            tvPlaca.setVisibility(View.GONE);
            tvCentro.setVisibility(View.GONE);
        }
        if(className.contentEquals("RatioEquipo")){

        }

        return rootView;
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
            /*
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
                    */
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
