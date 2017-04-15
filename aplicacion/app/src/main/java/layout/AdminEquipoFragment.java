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
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.support.design.widget.Snackbar;
import android.widget.TextView;
import android.widget.Toast;
import android.graphics.Color;
import com.example.app.aplicacion.R;

import java.util.List;

import Adapter.GenericAdapter;
import dominio.Equipo;
import servicios.wsequipos;
import sqlite.WSqlite;
import util.CallService;

import static java.lang.Integer.parseInt;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link AdminEquipoFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link AdminEquipoFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class AdminEquipoFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    private static String str_galones = "";
    private static String str_horometro = "";
    private static String str_kilometros = "";

    private static String equipo_id = "";
    private static String equipo_codigoInterno = "";
    private static String equipo_centro = "";
    private static String equipo_placa = "";

    private String str_bef = "";

    AutoCompleteTextView centro = null;
    AutoCompleteTextView planta = null;
    AutoCompleteTextView codigo_interno = null;
    AutoCompleteTextView placa = null;
    AutoCompleteTextView clase = null;

    public AdminEquipoFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment AdminEquipoFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static AdminEquipoFragment newInstance(String param1, String param2) {
        AdminEquipoFragment fragment = new AdminEquipoFragment();
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
    Equipo PARAM_EQUIPO=null;

    @Override
    public void onStart() {
        super.onStart();

    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        // Inflate the layout for this fragment
        rootView = inflater.inflate(R.layout.fragment_admin_equipo, container, false);
        final Button btnAdminEquipo = (Button)rootView.findViewById(R.id.btnGuardarEquipo);
        btnAdminEquipo.setOnClickListener(ClickAgregarEquipo);
        Button btnEliminarEquipo = (Button)rootView.findViewById(R.id.btnEliminarEquipo);
        btnEliminarEquipo.setOnClickListener(ClickEliminarEquipo);
        centro = (AutoCompleteTextView) rootView.findViewById(R.id.input_centro);
        planta = (AutoCompleteTextView) rootView.findViewById(R.id.input_planta);
        codigo_interno = (AutoCompleteTextView) rootView.findViewById(R.id.input_cinterno);
        placa = (AutoCompleteTextView) rootView.findViewById(R.id.input_placa);
        clase = (AutoCompleteTextView) rootView.findViewById(R.id.input_clase);

        context = container.getContext();

        /*
        InputMethodManager imm = (InputMethodManager) getActivity().getSystemService(getActivity().INPUT_METHOD_SERVICE);
        imm.toggleSoftInputFromWindow(
                centro.getApplicationWindowToken(),
                InputMethodManager.SHOW_FORCED, 0);
        */

        centro.requestFocus();
        InputMethodManager inputMethodManager = (InputMethodManager)getActivity(). getSystemService(getActivity().INPUT_METHOD_SERVICE);
        inputMethodManager.toggleSoftInput(InputMethodManager.SHOW_FORCED, InputMethodManager.HIDE_IMPLICIT_ONLY);
        centro.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                centro.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[0-9]*$ ");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    centro.setText(s);
                    centro.setSelection(start);
                }

                centro.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        centro.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {


                    //Log.d("e", str);
                    //galones.addTextChangedListener((TextWatcher) galones);

                    return true;
                }
                return false;
            }
        });
        planta.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                planta.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[A-Z0-9- ]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    planta.setText(s);
                    planta.setSelection(start);
                }

                planta.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        planta.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {


                    //Log.d("e", str);
                    //galones.addTextChangedListener((TextWatcher) galones);

                    return true;
                }
                return false;
            }
        });
        codigo_interno.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                codigo_interno.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[A-Z0-9- ]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    codigo_interno.setText(s);
                    codigo_interno.setSelection(start);
                }

                codigo_interno.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        codigo_interno.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {


                    //Log.d("e", str);
                    //galones.addTextChangedListener((TextWatcher) galones);

                    return true;
                }
                return false;
            }
        });
        placa.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                placa.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[A-Z0-9- ]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    placa.setText(s);
                    placa.setSelection(start);
                }

                placa.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        placa.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {


                    //Log.d("e", str);
                    //galones.addTextChangedListener((TextWatcher) galones);

                    return true;
                }
                return false;
            }
        });
        clase.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                clase.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[A-Z0-9- ]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    clase.setText(s);
                    clase.setSelection(start);
                }

                clase.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        clase.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {

                    btnAdminEquipo.callOnClick();
                    //Log.d("e", str);
                    //galones.addTextChangedListener((TextWatcher) galones);

                    return true;
                }
                return false;
            }
        });

        //con esto verificamos si sera una edicion o creacion
        Equipo obj = (Equipo)getArguments().getSerializable("equipo");
        String id = String.valueOf(obj.getId());
        PARAM_EQUIPO = obj;
        if(!id.equals("0"))
        {
            //Log.e("ENTRO EDICION",String.valueOf(obj.getId()));
            PARAM_EQUIPO=(Equipo)getArguments().getSerializable("equipo");
            btnEliminarEquipo.setVisibility(View.VISIBLE);
            CargarEquipoEditar();
        }



        /*
        ((InputMethodManager) (context).getSystemService(Context.INPUT_METHOD_SERVICE))
                .showSoftInput(centro, InputMethodManager.SHOW_FORCED);
                */
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
           // throw new RuntimeException(context.toString()
            //        + " must implement OnFragmentInteractionListener");
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
    public void CargarEquipoEditar()
    {
        AutoCompleteTextView input_centro = (AutoCompleteTextView) rootView.findViewById(R.id.input_centro);
        AutoCompleteTextView input_planta = (AutoCompleteTextView) rootView.findViewById(R.id.input_planta);
        AutoCompleteTextView input_cinterno = (AutoCompleteTextView) rootView.findViewById(R.id.input_cinterno);
        AutoCompleteTextView input_placa = (AutoCompleteTextView) rootView.findViewById(R.id.input_placa);
        AutoCompleteTextView input_clase = (AutoCompleteTextView) rootView.findViewById(R.id.input_clase);

        input_centro.setText(PARAM_EQUIPO.getCentro());
        input_planta.setText(PARAM_EQUIPO.getPlanta());
        input_cinterno.setText(PARAM_EQUIPO.getCodigoInterno());
        input_placa.setText(PARAM_EQUIPO.getPlaca());
        input_clase.setText(PARAM_EQUIPO.getClase());
    }
    private View.OnClickListener ClickAgregarEquipo = new View.OnClickListener() {
        public void onClick(View v) {

            AutoCompleteTextView input_centro = (AutoCompleteTextView) rootView.findViewById(R.id.input_centro);
            AutoCompleteTextView input_planta = (AutoCompleteTextView) rootView.findViewById(R.id.input_planta);
            AutoCompleteTextView input_cinterno = (AutoCompleteTextView) rootView.findViewById(R.id.input_cinterno);
            AutoCompleteTextView input_placa = (AutoCompleteTextView) rootView.findViewById(R.id.input_placa);
            AutoCompleteTextView input_clase = (AutoCompleteTextView) rootView.findViewById(R.id.input_clase);

            input_centro.setError(null);
            input_planta.setError(null);
            input_cinterno.setError(null);
            input_placa.setError(null);
            input_clase.setError(null);

            // Store values at the time of the login attempt.

            String id = String.valueOf(PARAM_EQUIPO.getId());

            String centro = input_centro.getText().toString();
            String planta = input_planta.getText().toString();
            String cinterno = input_cinterno.getText().toString();
            String placa = input_placa.getText().toString();
            String clase = input_clase.getText().toString();

            if (centro.equals("")) {
                input_centro.setError("Debes ingresar el CENTRO");
                return;
            }
            if (planta.equals("")) {
                input_planta.setError("Debes ingresar la PLANTA");
                return;
            }
            if (cinterno.equals("")) {
                input_cinterno.setError("Debes ingresar el CODIGO INTERNO");
                return;
            }
            if (placa.equals("")) {
                input_placa.setError("Debes ingresar la PLACA");
                return;
            }
            if (clase.equals("")) {
                input_clase.setError("Debes ingresar la CLASE");
                return;
            }
            /*Snackbar snackbar = Snackbar.make(rootView, "No internet connection!", Snackbar.LENGTH_LONG);
            View sbView = snackbar.getView();
            TextView textView = (TextView) sbView.findViewById(android.support.design.R.id.snackbar_text);
            textView.setTextColor(Color.RED);*/
            boolean tieneAcceso = util.AccesoInternet.Verificar(context);
            if (tieneAcceso) {
                final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Enviando Informacion...", true, false);
                pd.show();

                new CallService() {
                    @Override
                    protected void onPostExecute(Object result) {

                        pd.dismiss();
                        if (((String) result).equals("1")) {
                            Snackbar.make(rootView, "El equipo fue creado correctamente", Snackbar.LENGTH_LONG)
                                    .setAction("Action", null).show();

                            Fragment fragment = new EquiposFragment();
                            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                            fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();


                        } else if (((String) result).equals("2")) {
                            Snackbar.make(rootView, "El equipo fue actualizado correctamente", Snackbar.LENGTH_LONG)
                                    .setAction("Action", null).show();
                            getFragmentManager().popBackStack();

                            Fragment fragment = new EquiposFragment();
                            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                            fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                        } else {
                            Toast.makeText(context, "Data vacia", Toast.LENGTH_LONG).show();
                        }

                    }

                    @Override
                    protected Object doInBackground(String... args) {
                        wsequipos ws = new wsequipos();
                        return ws.InsertUpdateEquipo(args[0], args[1], args[2], args[3], args[4], args[5]);
                    }
                }.execute(id, centro, planta, cinterno, placa, clase);

            } else {
                String ID = id;
                if(ID.contentEquals("0")){
                    ID = "null";
                }
                WSqlite wsqlite = new WSqlite();
                String result = wsqlite.INSERT_REPLACE_EQUIPO(context, ID, centro, planta, cinterno, placa, clase);

                if (((String) result).equals("null")) {
                    Snackbar.make(rootView, "El equipo fue creado correctamente", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();

                    Fragment fragment = new EquiposFragment();
                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();


                } else if (!((String) result).equals("-1")) {
                    Snackbar.make(rootView, "El equipo fue actualizado correctamente", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();
                    getFragmentManager().popBackStack();

                    Fragment fragment = new EquiposFragment();
                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                } else {
                    Toast.makeText(context, "Data vacia", Toast.LENGTH_LONG).show();
                    return;
                }

                Fragment fragment = new EquiposFragment();
                FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
            }
        }
    };

    private View.OnClickListener ClickEliminarEquipo = new View.OnClickListener() {
        public void onClick(View v) {

            String id="0";
            if(!PARAM_EQUIPO.equals(null))
                id=String.valueOf(PARAM_EQUIPO.getId());

            boolean tieneAcceso = util.AccesoInternet.Verificar(context);
            if (tieneAcceso) {
                final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Enviando Informacion...", true, false);
                pd.show();

                new CallService() {
                    @Override
                    protected void onPostExecute(Object result) {

                        pd.dismiss();
                        if (((String) result).equals("1")) {
                            Snackbar.make(rootView, "El equipo fue eliminado correctamente", Snackbar.LENGTH_LONG)
                                    .setAction("Action", null).show();

                            Fragment fragment = new EquiposFragment();
                            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                            fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                        } else {
                            Toast.makeText(context, "Ocurrio un Error", Toast.LENGTH_LONG).show();
                        }

                    }

                    @Override
                    protected Object doInBackground(String... args) {
                        wsequipos ws = new wsequipos();
                        return ws.DeleteEquipo(args[0]);
                    }
                }.execute(id);
            }else{
                String ID = id;

                WSqlite wsqlite = new WSqlite();
                String result = wsqlite.DELETE_EQUIPO(context, ID);

                if (((String) result).equals("Equipo Eliminado")) {
                    Snackbar.make(rootView, "El equipo fue eliminado correctamente", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();

                    Fragment fragment = new EquiposFragment();
                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                } else {
                    Toast.makeText(context, "Ocurrio un Error", Toast.LENGTH_LONG).show();
                    return;
                }
                Fragment fragment = new EquiposFragment();
                FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
            }
        }
    };

}
