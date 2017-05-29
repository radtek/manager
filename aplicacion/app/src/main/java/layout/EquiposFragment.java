package layout;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
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

import com.example.app.aplicacion.R;
import android.app.ProgressDialog;
import Adapter.GenericAdapter;
import dominio.Equipo;
import java.util.ArrayList;
import android.widget.Toast;

import sqlite.WSqlite;
import util.CallService;
import servicios.wsequipos;

import java.util.List;

import android.widget.AdapterView.OnItemClickListener;

import static android.content.Context.INPUT_METHOD_SERVICE;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link EquiposFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link EquiposFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class EquiposFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private List<Equipo> equipoList;

    private boolean ventaFragment_active = false;

    private boolean ratioBuscarFragment_active = false;

    private OnFragmentInteractionListener mListener;

    public void ventaFragmentActive() {
        ventaFragment_active = true;
    }
    public void ratioBuscarFragment_active() {
        ratioBuscarFragment_active = true;
    }

    public EquiposFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment EquiposFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static EquiposFragment newInstance(String param1, String param2) {
        EquiposFragment fragment = new EquiposFragment();
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
        rootView = inflater.inflate(R.layout.fragment_equipos, container, false);
        Button btnAdminEquipo = (Button)rootView.findViewById(R.id.OnClickAdminEquipo);
        btnAdminEquipo.setOnClickListener(AdminEquipo);
        context = container.getContext();

        final ListView _ListContactos = (ListView) rootView.findViewById(R.id.listEquipos);

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

                List<Equipo> tempArrayList = new ArrayList<Equipo>();
                for(Equipo e: equipoList){
                    if (textlength <= e.toString().length()) {
                        if (e.toString().toLowerCase().contains(cs.toString().toLowerCase())) {
                            tempArrayList.add(e);
                        }
                    }
                }

                _ListContactos.setAdapter(new GenericAdapter(context, R.layout.activity_item_equipo,
                        tempArrayList){
                    @Override
                    public void Create(Object entrada, View view) {
                        TextView _txtNombre = (TextView) view.findViewById(R.id.item_CodigoInterno);
                        _txtNombre.setText(((Equipo) entrada).getCodigoInterno());

                        TextView _txtTelefono = (TextView) view.findViewById(R.id.item_Centro);
                        _txtTelefono.setText(((Equipo) entrada).getCentro());

                        TextView _txtEmail = (TextView) view.findViewById(R.id.item_Placa);
                        _txtEmail.setText(((Equipo) entrada).getPlaca());
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

        CargarEquipos();

        return rootView;
    }

    public void CargarEquipos()
    {
        /*
        boolean tieneAcceso = util.AccesoInternet.Verificar(getContext());
        if (tieneAcceso) {
            Log.d("cargarEquipos", "call service");
            final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Cargando Equipos....", true, false);
            pd.show();
            final ListView _ListContactos = (ListView) rootView.findViewById(R.id.listEquipos);

            new CallService() {
                @Override
                protected void onPostExecute(Object result) {

                    pd.dismiss();
                    if (result == null) {
                        Toast.makeText(context, "Data vacia", Toast.LENGTH_LONG).show();
                    } else {
                        equipoList = (List<Equipo>) result;
                        _ListContactos.setAdapter(new GenericAdapter(context, R.layout.activity_item_equipo,
                                equipoList) {
                            @Override
                            public void Create(Object entrada, View view) {
                                TextView _txtNombre = (TextView) view.findViewById(R.id.item_CodigoInterno);
                                _txtNombre.setText(((Equipo) entrada).getCodigoInterno());

                                TextView _txtTelefono = (TextView) view.findViewById(R.id.item_Centro);
                                _txtTelefono.setText(((Equipo) entrada).getCentro());

                                TextView _txtEmail = (TextView) view.findViewById(R.id.item_Placa);
                                _txtEmail.setText(((Equipo) entrada).getPlaca());
                            }
                        });

                        _ListContactos.setOnItemClickListener(new OnItemClickListener() {
                            @Override
                            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                                if (ventaFragment_active) {
                                    String equipo_id = String.valueOf(equipoList.get(position).getId());
                                    String codigoInterno = ((TextView) view.findViewById(R.id.item_CodigoInterno)).getText().toString();
                                    String centro = ((TextView) view.findViewById(R.id.item_Centro)).getText().toString();
                                    String placa = ((TextView) view.findViewById(R.id.item_Placa)).getText().toString();

                                    VentaFragment ventaFragment = (VentaFragment) ((MainActivity) getActivity()).listFragments.get(((MainActivity) getActivity()).listFragments.size() - 2);
                                    ventaFragment.setEquipo(equipo_id, codigoInterno, centro, placa);

                                    ((MainActivity) getActivity()).onBackPressed();

                                    return;
                                }
                                if (ratioBuscarFragment_active) {
                                    String equipo_id = String.valueOf(equipoList.get(position).getId());
                                    String codigoInterno = ((TextView) view.findViewById(R.id.item_CodigoInterno)).getText().toString();
                                    String centro = ((TextView) view.findViewById(R.id.item_Centro)).getText().toString();
                                    String placa = ((TextView) view.findViewById(R.id.item_Placa)).getText().toString();

                                    RatioBuscarFragment ratioBuscarFragment = (RatioBuscarFragment) ((MainActivity) getActivity()).listFragments.get(((MainActivity) getActivity()).listFragments.size() - 2);
                                    ratioBuscarFragment.setEquipo(equipo_id, codigoInterno, centro, placa);

                                    ((MainActivity) getActivity()).onBackPressed();
                                    return;
                                }
                                Fragment fragment = new AdminEquipoFragment();

                                int itemPosition = position;
                                Equipo _equipo = (Equipo) _ListContactos.getItemAtPosition(position);

                                Bundle bundle = new Bundle();
                                bundle.putSerializable("equipo", _equipo);
                                fragment.setArguments(bundle);


                                FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                                fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();

                            }
                        });

                   /* String mensaje=String.format("Bienvenido %s %s", ((Coordinador)result).nombre,((Coordinador)result).apellido);
                    ((Coordinador)getApplication()).usuario=((Coordinador)result).usuario;
                    ((Coordinador)getApplication()).nombre=((Coordinador)result).nombre;
                    ((Coordinador)getApplication()).nombre=((Coordinador)result).apellido;
                    ((Coordinador)getApplication()).email=((Coordinador)result).email;
                    ((Coordinador)getApplication()).dni=((Coordinador)result).dni;
                    Toast.makeText(_context,mensaje, Toast.LENGTH_LONG).show();

                    //Intent obj = new Intent(_context,MenuActivity.class);
                    Intent obj = new Intent(_context,MenuChecklistActivity.class);
                    startActivity(obj);*/
        /*
                    }
                    // pd.dismiss();
                }

                @Override
                protected Object doInBackground(String... args) {
                    wsequipos ws = new wsequipos();
                    return ws.GetEquipos();
                }
            }.execute();
        }else {
        */
            WSqlite sqlite = new WSqlite();
            List<Equipo> result = sqlite.GET_LISTEQUIPOS(context);

            final ListView listEquipos = (ListView) rootView.findViewById(R.id.listEquipos);
            equipoList = (List<Equipo>) result;
            listEquipos.setAdapter(new GenericAdapter(context, R.layout.activity_item_equipo,
                    equipoList) {
                @Override
                public void Create(Object entrada, View view) {
                    TextView _txtNombre = (TextView) view.findViewById(R.id.item_CodigoInterno);
                    _txtNombre.setText(((Equipo) entrada).getCodigoInterno());

                    TextView _txtTelefono = (TextView) view.findViewById(R.id.item_Centro);
                    _txtTelefono.setText(((Equipo) entrada).getCentro());

                    TextView _txtEmail = (TextView) view.findViewById(R.id.item_Placa);
                    _txtEmail.setText(((Equipo) entrada).getPlaca());
                }
            });

            listEquipos.setOnItemClickListener(new OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    if (ventaFragment_active) {
                        String equipo_id = String.valueOf(equipoList.get(position).getId());
                        String codigoInterno = ((TextView) view.findViewById(R.id.item_CodigoInterno)).getText().toString();
                        String centro = ((TextView) view.findViewById(R.id.item_Centro)).getText().toString();
                        String placa = ((TextView) view.findViewById(R.id.item_Placa)).getText().toString();

                        VentaFragment ventaFragment = (VentaFragment) ((MainActivity) getActivity()).listFragments.get(((MainActivity) getActivity()).listFragments.size() - 2);
                        ventaFragment.setEquipo(equipo_id, codigoInterno, centro, placa);

                        ((MainActivity) getActivity()).onBackPressed();
                        return;
                    }
                    if (ratioBuscarFragment_active) {
                        String equipo_id = String.valueOf(equipoList.get(position).getId());
                        String codigoInterno = ((TextView) view.findViewById(R.id.item_CodigoInterno)).getText().toString();
                        String centro = ((TextView) view.findViewById(R.id.item_Centro)).getText().toString();
                        String placa = ((TextView) view.findViewById(R.id.item_Placa)).getText().toString();

                        RatioBuscarFragment ratioBuscarFragment = (RatioBuscarFragment) ((MainActivity) getActivity()).listFragments.get(((MainActivity) getActivity()).listFragments.size() - 2);
                        ratioBuscarFragment.setEquipo(equipo_id, codigoInterno, centro, placa);

                        ((MainActivity) getActivity()).onBackPressed();
                        return;
                    }

                    Fragment fragment = new AdminEquipoFragment();

                    int itemPosition = position;
                    Equipo _equipo = (Equipo) listEquipos.getItemAtPosition(position);

                    Bundle bundle = new Bundle();
                    bundle.putSerializable("equipo", _equipo);
                    fragment.setArguments(bundle);

                    EditText et_buscar = (EditText) rootView.findViewById(R.id.editText_buscar);
                    InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);
                    imm.hideSoftInputFromWindow(et_buscar.getWindowToken(), 0);

                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
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
            //throw new RuntimeException(context.toString()
              //      + " must implement OnFragmentInteractionListener");
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



    private View.OnClickListener AdminEquipo = new View.OnClickListener() {
        public void onClick(View v) {

            Fragment fragment = new AdminEquipoFragment();
            Bundle bundle = new Bundle();
            Equipo equipo=new Equipo(0,null,null,null,null,null);
            bundle.putSerializable("equipo",equipo);
            fragment.setArguments(bundle);

            EditText et_buscar = (EditText) rootView.findViewById(R.id.editText_buscar);
            InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(et_buscar.getWindowToken(), 0);

            ((MainActivity)getActivity()).listFragments.add(fragment);

            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
            fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                    .commit();
        }
    };


}
