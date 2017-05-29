package layout;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.AutoCompleteTextView;
import android.widget.Button;

import com.example.app.aplicacion.R;

import sqlite.WSqlite;

import android.widget.TextView;
import android.widget.Toast;
import java.text.SimpleDateFormat;
import java.text.DateFormat;
import java.util.Date;
import android.view.inputmethod.InputMethodManager;

import static java.lang.Integer.parseInt;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link VentaFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link VentaFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class VentaFragment extends Fragment {
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

    AutoCompleteTextView galones = null;
    AutoCompleteTextView horometro = null;
    AutoCompleteTextView kilometros = null;

    public void setEquipo(String id, String codigoInterno, String centro, String placa) {
        equipo_id = id;
        equipo_codigoInterno = codigoInterno;
        equipo_centro = centro;
        equipo_placa = placa;
    }
    public VentaFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment VentaFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static VentaFragment newInstance(String param1, String param2) {
        VentaFragment fragment = new VentaFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(savedInstanceState != null) {
            str_galones = savedInstanceState.getString("galones", "");
            str_horometro = savedInstanceState.getString("horometro", "");
            str_kilometros = savedInstanceState.getString("kilometros", "");
        }
    }
    @Override
    public void onSaveInstanceState(final Bundle outState) {
        super.onSaveInstanceState(outState);
        final AutoCompleteTextView galones = ((AutoCompleteTextView)rootView.findViewById(R.id.input_galones));
        outState.putString("galones", galones.getText().toString());
        final AutoCompleteTextView horometro = ((AutoCompleteTextView)rootView.findViewById(R.id.input_horometro));
        outState.putString("horometro", horometro.getText().toString());
        final AutoCompleteTextView kilometros = ((AutoCompleteTextView)rootView.findViewById(R.id.input_kilometros));
        outState.putString("kilometros", kilometros.getText().toString());
    }
    @Override
    public void onDestroyView() {
        super.onDestroyView();
    }
    Context context;
    View rootView;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        rootView = inflater.inflate(R.layout.fragment_venta, container, false);
        context = container.getContext();

        AutoCompleteTextView t_galones = ((AutoCompleteTextView)rootView.findViewById(R.id.input_galones));
        t_galones.setText(str_galones);
        t_galones.setSelection(str_galones.length());
        AutoCompleteTextView t_horometro = ((AutoCompleteTextView)rootView.findViewById(R.id.input_horometro));
        t_horometro.setText(str_horometro);
        t_horometro.setSelection(str_horometro.length());
        AutoCompleteTextView t_kilometros = ((AutoCompleteTextView)rootView.findViewById(R.id.input_kilometros));
        t_kilometros.setText(str_kilometros);
        t_kilometros.setSelection(str_kilometros.length());

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
                ((MainActivity)getActivity()).replaceVentaEquipo();
            }
        });

        DateFormat df = new SimpleDateFormat("dd-MM-yyyy");
        String now = df.format(new Date());

        TextView txtFecha=(TextView)rootView.findViewById(R.id.txtFecha);
        txtFecha.setText(now);

        Button btnGuardarVenta = (Button)rootView.findViewById(R.id.btnGuardarVenta);
        btnGuardarVenta.setOnClickListener(ClickGuardarVenta);

        Button btnImprimir = (Button)rootView.findViewById(R.id.btnImprimir);
        btnImprimir.setOnClickListener(ClickImprimir);

        WSqlite sqlite = new WSqlite();

        int CodigoTIcket = sqlite.GET_IDTICKET(context);

        final TextView ticket= ((TextView)rootView.findViewById(R.id.textView_nroTicket));
        if (CodigoTIcket == 0) {
            ticket.setText("Sin código");
        }else{
            String codigoTicket = String.valueOf(CodigoTIcket);
            String r = "";
            for(int i = 0; i < 4 - codigoTicket.length(); i++) {
                r += "0";
            }
            r += codigoTicket;
            ticket.setText("AQP - "+r);
        }

        galones = ((AutoCompleteTextView)rootView.findViewById(R.id.input_galones));

        galones.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                galones.removeTextChangedListener(this);
                str_bef = s.toString();
                galones.addTextChangedListener(this);
            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                galones.removeTextChangedListener(this);
                double d = 0.0;
                try {
                    d = Double.parseDouble(cs.toString());
                }catch(NumberFormatException e){

                }finally{

                }
                if(d > 300.0){
                    galones.setText(str_bef);
                    galones.setSelection(galones.getText().length());
                    galones.addTextChangedListener(this);
                    return;
                }
                int posDot = -1;
                String currentText = cs.toString();
                for(int i=0; i<currentText.length(); i++){
                    if(currentText.substring(i, i+1).contentEquals(".")) {
                        posDot = i;
                        break;
                    }
                }
                if(posDot == -1) {
                    if(currentText.length() > 3) {
                        galones.setText(str_bef);
                        galones.setSelection(galones.getText().length());
                    }
                }else{
                    String parte_entera = currentText.substring(0, posDot);
                    String parte_decimal = currentText.substring(posDot+1, currentText.length());
                    if(parte_entera.length() > 3) {
                        galones.setText(str_bef);
                        galones.setSelection(galones.getText().length());
                    }
                    if(parte_decimal.length() > 3) {
                        galones.setText(str_bef);
                        galones.setSelection(galones.getText().length());
                    }
                }

                galones.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        galones.setOnEditorActionListener(new TextView.OnEditorActionListener() {
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

        //Log.d("qee", str_galones);
        //galones.setText(str_galones);

        horometro = ((AutoCompleteTextView)rootView.findViewById(R.id.input_horometro));
        horometro.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                horometro.removeTextChangedListener(this);

                str_bef = s.toString();

                horometro.addTextChangedListener(this);
            }

            @Override
            public void onTextChanged(CharSequence cs, int start, int before, int count) {
                horometro.removeTextChangedListener(this);
                int posDot = -1;
                String currentText = cs.toString();
                for(int i=0; i<currentText.length(); i++){
                    if(currentText.substring(i, i+1).contentEquals(".")) {
                        posDot = i;
                        break;
                    }
                }
                if(posDot == -1) {
                    if(currentText.length() > 4) {
                        horometro.setText(str_bef);
                        horometro.setSelection(horometro.getText().length());
                    }
                }else{
                    String parte_entera = currentText.substring(0, posDot);
                    String parte_decimal = currentText.substring(posDot+1, currentText.length());
                    if(parte_entera.length() > 4) {
                        horometro.setText(str_bef);
                        horometro.setSelection(horometro.getText().length());
                    }
                    if(parte_decimal.length() > 2) {
                        horometro.setText(str_bef);
                        horometro.setSelection(horometro.getText().length());
                    }
                }
                horometro.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        horometro.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {

                    return true;
                }
                return false;
            }
        });

        kilometros = ((AutoCompleteTextView)rootView.findViewById(R.id.input_kilometros));
        kilometros.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                kilometros.removeTextChangedListener(this);
                str_bef = s.toString();
                kilometros.addTextChangedListener(this);
            }

            @Override
            public void onTextChanged(CharSequence cs, int arg1, int arg2, int arg3) {
                kilometros.removeTextChangedListener(this);
                int posDot = -1;
                String currentText = cs.toString();
                for(int i=0; i<currentText.length(); i++){
                    if(currentText.substring(i, i+1).contentEquals(".")) {
                        posDot = i;
                        break;
                    }
                }
                if(posDot == -1) {
                    if(currentText.length() > 4) {
                        kilometros.setText(str_bef);
                        kilometros.setSelection(kilometros.getText().length());
                    }
                }else{
                    String parte_entera = currentText.substring(0, posDot);
                    String parte_decimal = currentText.substring(posDot+1, currentText.length());
                    if(parte_entera.length() > 4) {
                        kilometros.setText(str_bef);
                        kilometros.setSelection(kilometros.getText().length());
                    }
                    if(parte_decimal.length() > 2) {
                        kilometros.setText(str_bef);
                        kilometros.setSelection(kilometros.getText().length());
                    }
                }
                kilometros.addTextChangedListener(this);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        kilometros.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {
                    InputMethodManager imm = (InputMethodManager) getContext().getSystemService(getActivity().INPUT_METHOD_SERVICE);
                    imm.hideSoftInputFromWindow(kilometros.getWindowToken(), 0);
                    //tv_equipo.callOnClick();
                    return true;
                }
                return false;
            }
        });

        return rootView;
    }
    private View.OnClickListener ClickGuardarVenta = new View.OnClickListener() {
        public void onClick(View v) {
            GuardarVenta();
        }
    };
    private View.OnClickListener ClickImprimir = new View.OnClickListener() {
        public void onClick(View v) {
            imprimir();
        }
    };

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
           //         + " must implement OnFragmentInteractionListener");
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

    public void CargarEquipo()
    {

    }
    String IDEQUIPO = "";
    public void GuardarVenta()
    {
        IDEQUIPO = equipo_id;
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd");
        String now = df.format(new Date());
        String Fecha=now;
        String Producto="MAX/D B5(BA)";

        String str_ticket= ((TextView)rootView.findViewById(R.id.textView_nroTicket)).getText().toString();
        String str_galones= ((AutoCompleteTextView)rootView.findViewById(R.id.input_galones)).getText().toString();
        String str_horometro= ((AutoCompleteTextView)rootView.findViewById(R.id.input_horometro)).getText().toString();
        String str_kilometros= ((AutoCompleteTextView)rootView.findViewById(R.id.input_kilometros)).getText().toString();
        String str_CodigoEquipo=IDEQUIPO;

        System.out.println(str_ticket);
        System.out.println(str_CodigoEquipo);
        String error = "LLene y/o modifique los campos o reinicie.";
        if(str_ticket.contentEquals("Sin código")) {
            Toast.makeText(context, error, Toast.LENGTH_LONG).show();
            return;
        }

        if(str_galones.contentEquals("")) {
            Toast.makeText(context, error, Toast.LENGTH_LONG).show();
            return;
        }else{
            /*
            int num;
            try {
                num = parseInt(str_galones.toString());
            }catch(NumberFormatException e) {
                num = 0;
            }
            error = "El campo no puede ser cero.";
            if(num == 0) {
                Toast.makeText(context, error, Toast.LENGTH_LONG).show();
                return;
            }
            */
            int posDot = -1;
            for(int i=0; i<str_galones.length(); i++){
                if(str_galones.substring(i, i+1).contentEquals(".")) {
                    posDot = i;
                    break;
                }
            }
            if(posDot == -1) {
                /*
                int num_zeros = 3-str_galones.length();
                for(int i=0; i<num_zeros; i++){
                    str_galones = "0" + str_galones;
                }*/
                str_galones = str_galones + ".000";

            }else{
                String parte_entera = str_galones.substring(0, posDot);
                String parte_decimal = str_galones.substring(posDot+1, str_galones.length());
                int num_zeros;
                /*
                int num_zeros = 3-parte_entera.length();
                for(int i=0; i<num_zeros; i++){
                    parte_entera = "0" + parte_entera;
                }*/
                num_zeros = 3-parte_decimal.length();
                for(int i=0; i<num_zeros; i++){
                    parte_decimal = parte_decimal + "0";
                }
                str_galones = parte_entera + "." + parte_decimal;
            }
            galones.setText(str_galones);
        }
        if(str_horometro.contentEquals("")) {
            Toast.makeText(context, error, Toast.LENGTH_LONG).show();
            return;
        }else{
            /*
            double dec;
            try {
                dec = Double.parseDouble(str_horometro.toString());
            }catch(NumberFormatException e) {
                dec = 0.0;
            }
            error = "El campo no puede ser cero.";
            if(dec == 0) {
                Toast.makeText(context, error, Toast.LENGTH_LONG).show();
                return;
            }*/
            int posDot = -1;
            for(int i=0; i<str_horometro.length(); i++){
                if(str_horometro.substring(i, i+1).contentEquals(".")) {
                    posDot = i;
                    break;
                }
            }
            if(posDot == -1) {
                /*
                int num_zeros = 4-str_horometro.length();
                for(int i=0; i<num_zeros; i++){
                    str_horometro = "0" + str_horometro;
                }*/
                str_horometro = str_horometro + ".00";
            }else{
                String parte_entera = str_horometro.substring(0, posDot);
                String parte_decimal = str_horometro.substring(posDot+1, str_horometro.length());
                int num_zeros;
                /*
                int num_zeros = 4-parte_entera.length();
                for(int i=0; i<num_zeros; i++){
                    parte_entera = "0" + parte_entera;
                }*/
                num_zeros = 2-parte_decimal.length();
                for(int i=0; i<num_zeros; i++){
                    parte_decimal = parte_decimal + "0";
                }
                str_horometro = parte_entera + "." + parte_decimal;
            }
            horometro.setText(str_horometro);
        }
        if(str_kilometros.contentEquals("")) {
            Toast.makeText(context, error, Toast.LENGTH_LONG).show();
            return;
        }else{
            /*
            double dec;
            try {
                dec = Double.parseDouble(str_horometro.toString());
            }catch(NumberFormatException e) {
                dec = 0.0;
            }
            error = "El campo no puede ser cero.";
            if(dec == 0) {
                Toast.makeText(context, error, Toast.LENGTH_LONG).show();
                return;
            }*/
            int posDot = -1;
            for(int i=0; i<str_kilometros.length(); i++){
                if(str_kilometros.substring(i, i+1).contentEquals(".")) {
                    posDot = i;
                    break;
                }
            }
            if(posDot == -1) {
                /*
                int num_zeros = 4-str_kilometros.length();
                for(int i=0; i<num_zeros; i++){
                    str_kilometros = "0" + str_kilometros;
                }*/
                //str_kilometros = str_kilometros;
            }else{
                String parte_entera = str_kilometros.substring(0, posDot);
                String parte_decimal = str_kilometros.substring(posDot+1, str_kilometros.length());
                int num_zeros = 4-parte_entera.length();
                for(int i=0; i<num_zeros; i++){
                    parte_entera = "0" + parte_entera;
                }
                num_zeros = 2-parte_decimal.length();
                for(int i=0; i<num_zeros; i++){
                    parte_decimal = parte_decimal + "0";
                }
                str_kilometros = parte_entera + "." + parte_decimal;
            }
            kilometros.setText(str_kilometros);
        }

        if(str_CodigoEquipo.contentEquals("")) {
            Toast.makeText(context, error, Toast.LENGTH_LONG).show();
            return;
        }

        WSqlite sqlite = new WSqlite();

        String Result = sqlite.INSERT_VENTA(context,Fecha,Producto,str_ticket
                ,str_galones,str_horometro,str_kilometros,str_CodigoEquipo);

        Snackbar.make(rootView,Result, Snackbar.LENGTH_LONG).setAction("Action", null).show();

        TextView Autoticket=((TextView)rootView.findViewById(R.id.textView_nroTicket));
        //Autoticket.setEnabled(false);
        ((AutoCompleteTextView)rootView.findViewById(R.id.input_galones)).setEnabled(false);
        ((AutoCompleteTextView)rootView.findViewById(R.id.input_horometro)).setEnabled(false);
        ((AutoCompleteTextView)rootView.findViewById(R.id.input_kilometros)).setEnabled(false);
        ((TextView)rootView.findViewById(R.id.textView_equipo)).setEnabled(false);

        ((Button)rootView.findViewById(R.id.btnImprimir)).setVisibility(View.VISIBLE);
        ((Button)rootView.findViewById(R.id.btnGuardarVenta)).setVisibility(View.GONE);

        InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(Autoticket.getWindowToken(), 0);

    }
    public void imprimir() {

        String fecha = ((TextView)rootView.findViewById(R.id.txtFecha)).getText().toString();
        String nroTicket = ((TextView)rootView.findViewById(R.id.textView_nroTicket)).getText().toString();

        String galones = ((AutoCompleteTextView)rootView.findViewById(R.id.input_galones)).getText().toString();
        String horometro = ((AutoCompleteTextView)rootView.findViewById(R.id.input_horometro)).getText().toString();
        String kilometros = ((AutoCompleteTextView)rootView.findViewById(R.id.input_kilometros)).getText().toString();
        ((AutoCompleteTextView)rootView.findViewById(R.id.input_galones)).setText("");
        ((AutoCompleteTextView)rootView.findViewById(R.id.input_horometro)).setText("");
        ((AutoCompleteTextView)rootView.findViewById(R.id.input_kilometros)).setText("");

        TextView tvCodigoInterno = ((TextView)rootView.findViewById(R.id.item_CodigoInterno));
        String codigoInterno = tvCodigoInterno.getText().toString();
        TextView tvCentro = ((TextView)rootView.findViewById(R.id.item_Centro));
        String centro = tvCentro.getText().toString();
        TextView tvPlaca = ((TextView)rootView.findViewById(R.id.item_Placa));
        String placa = tvPlaca.getText().toString();
        //tvCodigoInterno.setText("");
        //tvCentro.setText("");
        //tvPlaca.setText("");

        Fragment currentFragment = this;
        FragmentTransaction fragTransaction = getFragmentManager().beginTransaction();
        fragTransaction.detach(currentFragment);
        fragTransaction.attach(currentFragment);
        fragTransaction.commit();

        Intent obj = new Intent(context, BlueToothPrinter.class);
        obj.putExtra("FECHA", fecha);
        obj.putExtra("NROTICKET", nroTicket);
        obj.putExtra("GALONES", galones);
        obj.putExtra("HOROMETRO", horometro);
        obj.putExtra("KILOMETROS", kilometros);
        obj.putExtra("CODIGOINTERNO", codigoInterno);
        obj.putExtra("CENTRO", centro);
        obj.putExtra("PLACA", placa);
        startActivity(obj);
    }

       /* Toast.makeText(context,"TICKET:" + ticket,Toast.LENGTH_SHORT).show();
        Toast.makeText(context,"galones:" + galones,Toast.LENGTH_SHORT).show();
        Toast.makeText(context,"horometro:" + horometro,Toast.LENGTH_SHORT).show();
        Toast.makeText(context,"kilometros:" + kilometros,Toast.LENGTH_SHORT).show();
        Toast.makeText(context,"CodigoEquipo:" + CodigoEquipo,Toast.LENGTH_SHORT).show();*/


}

