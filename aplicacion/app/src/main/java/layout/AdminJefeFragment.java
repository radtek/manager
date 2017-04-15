package layout;

import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
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
import android.widget.CheckBox;
import android.widget.TextView;
import android.widget.Toast;
import servicios.wsjefes;
import com.example.app.aplicacion.R;
import dominio.Jefe;
import sqlite.WSqlite;
import util.CallService;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link AdminJefeFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link AdminJefeFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class AdminJefeFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    AutoCompleteTextView nombreJefe = null;
    AutoCompleteTextView usuario = null;
    AutoCompleteTextView password = null;

    public AdminJefeFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment AdminJefeFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static AdminJefeFragment newInstance(String param1, String param2) {
        AdminJefeFragment fragment = new AdminJefeFragment();
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
    Jefe PARAM_JEFE=null;
    boolean checkSubgerente=false;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        System.out.println("view");
        rootView = inflater.inflate(R.layout.fragment_admin_jefe, container, false);

        final Button btnAdminJefe = (Button)rootView.findViewById(R.id.btnGuardarJefe);
        btnAdminJefe.setOnClickListener(ClickAgregarJefe);
        Button btnEliminarJefe = (Button)rootView.findViewById(R.id.btnEliminarJefe);
        CheckBox cbSubGerente = (CheckBox)rootView.findViewById(R.id.checkbox_subgerente);
        btnEliminarJefe.setOnClickListener(ClickEliminarJefe);
        cbSubGerente.setOnClickListener(ClickCheckBox);
        cbSubGerente.setChecked(checkSubgerente);
        context = container.getContext();

        nombreJefe = (AutoCompleteTextView) rootView.findViewById(R.id.input_nombrejefe);
        usuario = (AutoCompleteTextView) rootView.findViewById(R.id.input_usuario);
        password = (AutoCompleteTextView) rootView.findViewById(R.id.input_password);

        nombreJefe.requestFocus();
        InputMethodManager inputMethodManager = (InputMethodManager)getActivity(). getSystemService(getActivity().INPUT_METHOD_SERVICE);
        inputMethodManager.toggleSoftInput(InputMethodManager.SHOW_FORCED, InputMethodManager.HIDE_NOT_ALWAYS);
        nombreJefe.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                nombreJefe.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[a-zA-Z0-9 ]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    nombreJefe.setText(s);
                    nombreJefe.setSelection(start);
                }

                nombreJefe.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        nombreJefe.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {

                    return true;
                }
                return false;
            }
        });
        usuario.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                usuario.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[a-zA-Z0-9]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    usuario.setText(s);
                    usuario.setSelection(start);
                }

                usuario.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        usuario.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {

                    return true;
                }
                return false;
            }
        });
        password.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                password.removeTextChangedListener(this);
                String s = new String(cs.toString());
                boolean match = s.matches("^[a-zA-Z0-9]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    s = s.replace(s.subSequence(start, start+count), "");
                    password.setText(s);
                    password.setSelection(start);
                }

                password.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        password.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {
                    btnAdminJefe.callOnClick();
                    return true;
                }
                return false;
            }
        });
        //con esto verificamos si sera una edicion o creacion
        Jefe obj=(Jefe)getArguments().getSerializable("jefe");
        String id=String.valueOf(obj.getId());
        PARAM_JEFE=obj;
        if(!id.equals("0"))
        {
            PARAM_JEFE=(Jefe)getArguments().getSerializable("jefe");
            btnEliminarJefe.setVisibility(View.VISIBLE);
            CargarDatosJefe();
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
//            throw new RuntimeException(context.toString()
  //                  + " must implement OnFragmentInteractionListener");
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

    public void CargarDatosJefe()
    {
        AutoCompleteTextView input_nombres = (AutoCompleteTextView) rootView.findViewById(R.id.input_nombrejefe);
        AutoCompleteTextView input_usuario = (AutoCompleteTextView) rootView.findViewById(R.id.input_usuario);
        AutoCompleteTextView input_password = (AutoCompleteTextView) rootView.findViewById(R.id.input_password);
        CheckBox cbSubGerente = (CheckBox)rootView.findViewById(R.id.checkbox_subgerente);

        input_nombres.setText(PARAM_JEFE.getNombre());
        input_usuario.setText(PARAM_JEFE.getUsuario());
        input_password.setText(PARAM_JEFE.getPassword());
        if(PARAM_JEFE.getSubgerente().equals("SI"))
        {
            Log.e("MENSAJE","SI ENTRO");
            cbSubGerente.setChecked(true);
        }
        else
        {
            Log.e("MENSAJE","NO ENTRO");
            cbSubGerente.setChecked(false);
        }

    }
    private View.OnClickListener ClickCheckBox = new View.OnClickListener() {
        public void onClick(View v) {
            boolean checked = ((CheckBox) v).isChecked();
            if (checked) {
                checkSubgerente=true;
            }
            else
            {
                checkSubgerente=false;
            }
        }
        };

    private View.OnClickListener ClickAgregarJefe = new View.OnClickListener() {
        public void onClick(View v) {

            Log.e("Entro a registrar","MENSAJE");
            AutoCompleteTextView input_nombres = (AutoCompleteTextView) rootView.findViewById(R.id.input_nombrejefe);
            AutoCompleteTextView input_usuario = (AutoCompleteTextView) rootView.findViewById(R.id.input_usuario);
            AutoCompleteTextView input_password = (AutoCompleteTextView) rootView.findViewById(R.id.input_password);


            input_nombres.setError(null);
            input_usuario.setError(null);
            input_password.setError(null);

            // Store values at the time of the login attempt.
            String id=String.valueOf(PARAM_JEFE.getId());

            String nombres = input_nombres.getText().toString();
            String usuario = input_usuario.getText().toString();
            String password = input_password.getText().toString();
            String subgerente="NO";

            if(nombres.equals(""))
            {
                input_nombres.setError("Debes ingresar su NOMBRE");
                return;
            }
            if(usuario.equals(""))
            {
                input_usuario.setError("Debes ingresar su USUARIO");
                return;
            }
            if(password.equals(""))
            {
                input_password.setError("Debes ingresar su PASSWORD");
                return;
            }
            if(checkSubgerente)
                subgerente="SI";

            boolean tieneAcceso = util.AccesoInternet.Verificar(context);
            if (tieneAcceso) {
                final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Enviando Informacion...", true, false);
                pd.show();

                new CallService() {
                    @Override
                    protected void onPostExecute(Object result) {

                        pd.dismiss();
                        if (((String) result).equals("1")) {
                            Snackbar.make(rootView, "Los Datos del Jefe fueron creados correctamente", Snackbar.LENGTH_LONG)
                                    .setAction("Action", null).show();

                            Fragment fragment = new JefesFragment();
                            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                            fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();


                        } else if (((String) result).equals("2")) {
                            Snackbar.make(rootView, "Los Datos del Jefe fueron actualizados correctamente", Snackbar.LENGTH_LONG)
                                    .setAction("Action", null).show();
                            getFragmentManager().popBackStack();

                            Fragment fragment = new JefesFragment();
                            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                            fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                        } else {
                            Toast.makeText(context, "Data vacia", Toast.LENGTH_LONG).show();
                        }

                    }

                    @Override
                    protected Object doInBackground(String... args) {
                        wsjefes ws = new wsjefes();
                        return ws.InsertUpdateJefe(args[0], args[1], args[2], args[3], args[4]);
                    }
                }.execute(id, nombres, usuario, password, subgerente);
            }else{
                String ID = id;
                if(ID.contentEquals("0")){
                    ID = "null";
                }
                WSqlite wsqlite = new WSqlite();
                String result = wsqlite.INSERT_REPLACE_JEFE(context, ID, nombres, usuario, password, subgerente);

                if (((String) result).equals("null")) {
                    Snackbar.make(rootView, "Los Datos del Jefe fueron creados correctamente", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();

                    Fragment fragment = new JefesFragment();
                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();


                } else if (!((String) result).equals("-1")) {
                    Snackbar.make(rootView, "Los Datos del Jefe fueron actualizados correctamente", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();
                    getFragmentManager().popBackStack();

                    Fragment fragment = new JefesFragment();
                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                } else {
                    Toast.makeText(context, "Data vacia", Toast.LENGTH_LONG).show();
                }
            }
        }
    };
    private View.OnClickListener ClickEliminarJefe = new View.OnClickListener() {
        public void onClick(View v) {

            String id=String.valueOf(PARAM_JEFE.getId());

            boolean tieneAcceso = util.AccesoInternet.Verificar(context);
            if (tieneAcceso) {
                final ProgressDialog pd = ProgressDialog.show(context, "Por favor espere", "Enviando Informacion...", true, false);
                pd.show();

                new CallService() {
                    @Override
                    protected void onPostExecute(Object result) {

                        pd.dismiss();
                        if (((String) result).equals("1")) {
                            Snackbar.make(rootView, "Datos del Jefe fueron eliminados correctamente", Snackbar.LENGTH_LONG)
                                    .setAction("Action", null).show();

                            Fragment fragment = new JefesFragment();
                            FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                            fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                        } else {
                            Toast.makeText(context, "Ocurrio un Error", Toast.LENGTH_LONG).show();
                        }

                    }

                    @Override
                    protected Object doInBackground(String... args) {
                        wsjefes ws = new wsjefes();
                        return ws.DeleteJefe(args[0]);
                    }
                }.execute(id);
            }else{
                String ID = id;

                WSqlite wsqlite = new WSqlite();
                String result = wsqlite.DELETE_JEFE(context, ID);

                if (((String) result).equals("Jefe Eliminado")) {
                    Snackbar.make(rootView, "Datos del Jefe fueron eliminados correctamente", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();

                    Fragment fragment = new JefesFragment();
                    FragmentManager fragmentManager = getActivity().getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.flContent, fragment).commit();
                } else {
                    Toast.makeText(context, "Ocurrio un Error", Toast.LENGTH_LONG).show();
                }
            }
        }
    };
}
