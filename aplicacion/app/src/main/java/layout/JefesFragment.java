package layout;

import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.text.Editable;
import android.text.TextWatcher;
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
import servicios.wsjefes;
import dominio.Jefe;
import servicios.wsequipos;
import sqlite.WSqlite;
import util.CallService;

import static android.content.Context.INPUT_METHOD_SERVICE;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link JefesFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link JefesFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class JefesFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private List<Jefe> jefeList;

    private OnFragmentInteractionListener mListener;

    public JefesFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment JefesFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static JefesFragment newInstance(String param1, String param2) {
        JefesFragment fragment = new JefesFragment();
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
        rootView =inflater.inflate(R.layout.fragment_jefes, container, false);
        context = container.getContext();
        CargarJefes();
        Button btnAdminEquipo = (Button)rootView.findViewById(R.id.OnClickNuevoJefe);
        btnAdminEquipo.setOnClickListener(AdminJefe);

        final ListView _ListContactos = (ListView) rootView.findViewById(R.id.listJefes);

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

                List<Jefe> tempArrayList = new ArrayList<>();
                for(Jefe j: jefeList){
                    if (textlength <= j.toString().length()) {
                        if (j.toString().toLowerCase().contains(cs.toString().toLowerCase())) {
                            tempArrayList.add(j);
                        }
                    }
                }

                _ListContactos.setAdapter(new GenericAdapter(context, R.layout.activity_item_jefe,
                        tempArrayList){
                    @Override
                    public void Create(Object entrada, View view) {
                        TextView nombreJefe = (TextView) view.findViewById(R.id.item_NombreJefe);
                        nombreJefe.setText(((Jefe) entrada).getNombre());

                        TextView usuarioJefe = (TextView) view.findViewById(R.id.item_UsuarioJefe);
                        usuarioJefe.setText(((Jefe) entrada).getUsuario());
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

        return rootView;
    }

    public void CargarJefes()
    {
        boolean tieneAcceso = util.AccesoInternet.Verificar(getContext());
        if (tieneAcceso) {
            final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Cargando Jefes....", true, false);
            pd.show();
            final ListView _ListJefes = (ListView) rootView.findViewById(R.id.listJefes);

            new CallService() {
                @Override
                protected void onPostExecute(Object result) {

                    pd.dismiss();
                    if (result == null) {
                        Toast.makeText(context, "Data vacia", Toast.LENGTH_LONG).show();
                    } else {
                        jefeList = (List<Jefe>) result;
                        _ListJefes.setAdapter(new GenericAdapter(context, R.layout.activity_item_jefe,
                                (List<Jefe>) result) {
                            @Override
                            public void Create(Object entrada, View view) {
                                TextView _txtNombre = (TextView) view.findViewById(R.id.item_NombreJefe);
                                _txtNombre.setText(((Jefe) entrada).getNombre());

                                TextView _txtUsuario = (TextView) view.findViewById(R.id.item_UsuarioJefe);
                                _txtUsuario.setText(((Jefe) entrada).getUsuario());


                            }
                        });
                        _ListJefes.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                            @Override
                            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                                Fragment fragment = new AdminJefeFragment();

                                int itemPosition = position;
                                Jefe _jefe = (Jefe) _ListJefes.getItemAtPosition(position);

                                Bundle bundle = new Bundle();
                                bundle.putSerializable("jefe", _jefe);
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
                    }
                    // pd.dismiss();
                }

                @Override
                protected Object doInBackground(String... args) {
                    wsjefes ws = new wsjefes();
                    return ws.GetJefes();
                }
            }.execute();
        }else{
            WSqlite sqlite = new WSqlite();
            List<Jefe> result = sqlite.GET_LISTJEFES(context);

            jefeList = (List<Jefe>) result;

            final ListView _ListJefes = (ListView) rootView.findViewById(R.id.listJefes);
            _ListJefes.setAdapter(new GenericAdapter(context, R.layout.activity_item_jefe,
                    (List<Jefe>) result) {
                @Override
                public void Create(Object entrada, View view) {
                    TextView _txtNombre = (TextView) view.findViewById(R.id.item_NombreJefe);
                    _txtNombre.setText(((Jefe) entrada).getNombre());

                    TextView _txtUsuario = (TextView) view.findViewById(R.id.item_UsuarioJefe);
                    _txtUsuario.setText(((Jefe) entrada).getUsuario());


                }
            });
            _ListJefes.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                    Fragment fragment = new AdminJefeFragment();

                    int itemPosition = position;
                    Jefe _jefe = (Jefe) _ListJefes.getItemAtPosition(position);

                    Bundle bundle = new Bundle();
                    bundle.putSerializable("jefe", _jefe);
                    fragment.setArguments(bundle);


                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();


                }
            });
        }
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


    private View.OnClickListener AdminJefe = new View.OnClickListener() {
        public void onClick(View v) {

            Fragment fragment = new AdminJefeFragment();

            Bundle bundle = new Bundle();
            Jefe jefe = new Jefe(0,null,null,null,null);
            bundle.putSerializable("jefe",jefe);
            fragment.setArguments(bundle);


            ((MainActivity)getActivity()).listFragments.add(fragment);

            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
            fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                    .commit();

        }
    };
}
