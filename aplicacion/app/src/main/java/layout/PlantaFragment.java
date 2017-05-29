package layout;

import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.app.aplicacion.R;

import java.util.ArrayList;
import java.util.List;

import Adapter.GenericAdapter;
import dominio.Equipo;
import dominio.Planta;
import servicios.wsequipos;
import sqlite.WSqlite;
import util.CallService;

import static android.content.Context.INPUT_METHOD_SERVICE;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link PlantaFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link PlantaFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class PlantaFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private List<Planta> plantaList;

    private boolean ratioBuscarFragment_active = false;

    private OnFragmentInteractionListener mListener;

    public void ratioBuscarFragment_active() {
        ratioBuscarFragment_active = true;
    }

    public PlantaFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment PlantaFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static PlantaFragment newInstance(String param1, String param2) {
        PlantaFragment fragment = new PlantaFragment();
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

        rootView = inflater.inflate(R.layout.fragment_planta, container, false);
        context = container.getContext();

        final ListView listPlantas = (ListView) rootView.findViewById(R.id.listPlantas);

        final EditText et_buscar  = (EditText) rootView.findViewById(R.id.editText_buscar);

        et_buscar.addTextChangedListener(new TextWatcher() {
            @Override
            public void onTextChanged(CharSequence cs, int arg1, int arg2, int arg3) {
                int textlength = cs.length();

                if(textlength==0)
                {
                    InputMethodManager imm = (InputMethodManager)getActivity().getSystemService(INPUT_METHOD_SERVICE);
                    imm.hideSoftInputFromWindow(et_buscar.getWindowToken(), 0);
                }

                List<Planta> tempArrayList = new ArrayList<Planta>();
                for(Planta e: plantaList){
                    if (textlength <= e.toString().length()) {
                        if (e.toString().toLowerCase().contains(cs.toString().toLowerCase())) {
                            tempArrayList.add(e);
                        }
                    }
                }

                listPlantas.setAdapter(new GenericAdapter(context, R.layout.activity_item_planta,
                        tempArrayList){
                    @Override
                    public void Create(Object entrada, View view) {
                        TextView planta = (TextView) view.findViewById(R.id.item_planta);
                        planta.setText(((Planta) entrada).getPlanta());
                    }
                });
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                // TODO Auto-generated method stub

            }

            @Override
            public void afterTextChanged(Editable s) {
                // TODO Auto-generated method stub

            }
        });

        cargarPlantas();

        return rootView;
    }

    public void cargarPlantas()
    {
        //boolean tieneAcceso = util.AccesoInternet.Verificar(getContext());
        //if (!tieneAcceso) {
            WSqlite sqlite = new WSqlite();
            List<Planta> result = sqlite.GET_LISTPLANTAS(context);

            final ListView listPlantas = (ListView) rootView.findViewById(R.id.listPlantas);
            plantaList = (List<Planta>) result;
            listPlantas.setAdapter(new GenericAdapter(context, R.layout.activity_item_planta,
                    plantaList) {
                @Override
                public void Create(Object entrada, View view) {
                    TextView planta = (TextView) view.findViewById(R.id.item_planta);
                    planta.setText(((Planta) entrada).getPlanta());
                }
            });

            listPlantas.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                    if (ratioBuscarFragment_active) {
                        String planta_id = String.valueOf(plantaList.get(position).getId());
                        String planta = ((TextView) view.findViewById(R.id.item_planta)).getText().toString();

                        RatioBuscarFragment ratioBuscarFragment = (RatioBuscarFragment) ((MainActivity) getActivity()).listFragments.get(((MainActivity) getActivity()).listFragments.size() - 2);
                        ratioBuscarFragment.setPlanta(planta_id, planta);

                        ((MainActivity) getActivity()).onBackPressed();
                        return;
                    }

                }
            });
        //}
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
