package layout;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.SQLException;
import android.os.StrictMode;
import android.support.annotation.NonNull;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.app.LoaderManager.LoaderCallbacks;

import android.content.CursorLoader;
import android.content.Loader;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;

import android.os.Build;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.app.aplicacion.R;

import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.lang.String;
import java.lang.*;
import java.sql.Connection;
import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

import dominio.Equipo;
import dominio.Jefe;
import dominio.Usuario;
import servicios.wsequipos;
import servicios.wsjefes;
import servicios.wsusuarios;
import sqlite.WSqlite;
import util.CallService;

/**
 * A login screen that offers login via email/password.
 */
public class LoginActivity extends AppCompatActivity implements LoaderCallbacks<Cursor> {

    /**
     * Id to identity READ_CONTACTS permission request.
     */
    private static final int REQUEST_READ_CONTACTS = 0;
    private static final int REQUEST_EXIT = 2;


    /**
     * A dummy authentication store containing known user names and passwords.
     * TODO: remove after connecting to a real authentication system.
     */
    private static final String[] DUMMY_CREDENTIALS = new String[]{
            "foo@example.com:hello", "bar@example.com:world"
    };
    public static final Pattern VALID_EMAIL_ADDRESS_REGEX =

        Pattern.compile("^([a-zA-Z0-9]+[.]{1,1}[a-zA-Z0-9]+)*" +
                "|([a-zA-Z0-9]+[_]{1,1}[a-zA-Z0-9]+)*$", Pattern.CASE_INSENSITIVE);

    public static final String STR_VALID_EMAIL_ADDRESS_REGEX = "^[a-zA-Z0-9]+([._]{1,1}[a-zA-Z0-9]+)*" +
            "@[a-zA-Z0-9]+[.]{1,1}[a-zA-Z0-9]+$";

    /**

     * Keep track of the login task to ensure we can cancel it if requested.
     */
    public static boolean validate(String emailStr) {
            Matcher matcher = VALID_EMAIL_ADDRESS_REGEX .matcher(emailStr);
            return matcher.find();
    }
    // UI references.
    private AutoCompleteTextView mEmailView;
    private EditText mPasswordView;
    private View mProgressView;
    private View mLoginFormView;

    private UserLoginTask mAuthTask = null;

    private boolean cargado = false;
    @Override
    protected void onStart() {
        super.onStart();

        if(!cargado) {
            cargado = true;
            boolean tieneAcceso = util.AccesoInternet.Verificar(this);
            if (tieneAcceso) {

                cargarUsuarios();
                cargarJefes();
                cargarEquipos();
                cargarVentas();
            }else{
                cargarUsuarios();
            }
        }
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        boolean tieneAcceso = util.AccesoInternet.Verificar(this);
        if(tieneAcceso) {

            /*
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("Sincronización");
            builder.setMessage("¿Deseas sincronizar con el servidor.");
            builder.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    //TODO
                    Log.d("yes", "ok");
                    dialog.dismiss();
                }
            });
            builder.setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    //TODO
                    Log.d("no", "ok");
                    dialog.dismiss();
                }
            });
            AlertDialog dialog = builder.create();
            dialog.show();
            */
        }else{
            Toast.makeText(this, "No tienes acceso a Internet.", Toast.LENGTH_LONG).show();
        }

        mEmailView = (AutoCompleteTextView) findViewById(R.id.email);
        mEmailView.addTextChangedListener(new TextWatcher() {
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                mEmailView.removeTextChangedListener(this);

                String sc = new String(s.toString());
                boolean match = sc.matches("^[a-zA-Z0-9._@]*$");
                if(match){
                    //Toast.makeText(getApplicationContext(),"OK!",Toast.LENGTH_SHORT).show();
                }else{
                    //Toast.makeText(getApplicationContext(),"invalid email",Toast.LENGTH_SHORT).show();
                    sc = sc.replace(s.subSequence(start, start+count), "");
                    mEmailView.setText(sc);
                    mEmailView.setSelection(start);
                }
                mEmailView.addTextChangedListener(this);
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
        mEmailView.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {
                    //attemptLogin();
                    return true;
                }
                return false;
            }
        });
        //populateAutoComplete();

        mPasswordView = (EditText) findViewById(R.id.password);
        mPasswordView.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_ACTION_DONE) {
                    attemptLogin();
                    return true;
                }
                return false;
            }
        });

        Button mEmailSignInButton = (Button) findViewById(R.id.email_sign_in_button);
        mEmailSignInButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                attemptLogin();
            }
        });

        mLoginFormView = findViewById(R.id.login_form);
        mProgressView = findViewById(R.id.login_progress);
    }

   /* private void populateAutoComplete() {
        if (!mayRequestContacts()) {
            return;
        }

        getLoaderManager().initLoader(0, null, this);
    }*/

    /*private boolean mayRequestContacts() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            return true;
        }
        if (checkSelfPermission(READ_CONTACTS) == PackageManager.PERMISSION_GRANTED) {
            return true;
        }
        if (shouldShowRequestPermissionRationale(READ_CONTACTS)) {
            Snackbar.make(mEmailView, R.string.permission_rationale, Snackbar.LENGTH_INDEFINITE)
                    .setAction(android.R.string.ok, new View.OnClickListener() {
                        @Override
                        @TargetApi(Build.VERSION_CODES.M)
                        public void onClick(View v) {
                            requestPermissions(new String[]{READ_CONTACTS}, REQUEST_READ_CONTACTS);
                        }
                    });
        } else {
            requestPermissions(new String[]{READ_CONTACTS}, REQUEST_READ_CONTACTS);
        }
        return false;
    }*/

    /**
     * Callback received when a permissions request has been completed.
     */
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == REQUEST_READ_CONTACTS) {
            if (grantResults.length == 1 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
               // populateAutoComplete();
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        switch(requestCode){
            case REQUEST_EXIT:{
                if(resultCode == RESULT_OK){
                    finish();
                }
            }break;
        }
    }
    /**
     * Attempts to sign in or register the account specified by the login form.
     * If there are form errors (invalid email, missing fields, etc.), the
     * errors are presented and no actual login attempt is made.
     */
    private void attemptLogin() {
        if (mAuthTask != null) {
            return;
        }

        // Reset errors.
        mEmailView.setError(null);
        mPasswordView.setError(null);

        // Store values at the time of the login attempt.
        String email = mEmailView.getText().toString();
        String password = mPasswordView.getText().toString();

        if(email.equals(""))
        {
            mEmailView.setError("Usuario incorrecto");
        }
        if(password.equals(""))
        {
            mPasswordView.setError("Password incorrecto");
        }

        boolean tieneacceso=util.AccesoInternet.Verificar(getApplicationContext());
        if(tieneacceso) {
            WSqlite wsqlite = new WSqlite();

            List<HashMap<String, String>> user = wsqlite.GET_LOGIN(this, email, password);
            if (user.size() == 1) {
                InputMethodManager imm = (InputMethodManager) getSystemService(this.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(mEmailView.getWindowToken(), 0);

                //App.getSingleton();
                //App.set_tipo_usuario(user.get(0).get("TIPO"));

                Intent obj = new Intent(this, MainActivity.class);
                obj.putExtra("TIPO", user.get(0).get("TIPO"));
                startActivityForResult(obj, REQUEST_EXIT);
            } else {
                List<HashMap<String, String>> jefe = wsqlite.GET_LOGINJEFE(this, email, password);
                if (jefe.size() == 1) {
                    Intent obj = new Intent(this, MainActivity.class);
                    int subgerente = Integer.parseInt(jefe.get(0).get("SUBGERENTE"));
                    if (subgerente == 1) {
                        obj.putExtra("TIPO", "Jefe");
                    } else {
                        obj.putExtra("TIPO", "Administrador");
                    }
                    startActivityForResult(obj, REQUEST_EXIT);
                } else {
                    Toast.makeText(this, "Usuario o contraseña incorrecta.", Toast.LENGTH_LONG).show();
                }
            }
        }else{
            WSqlite wsqlite = new WSqlite();

            List<HashMap<String, String>> user = wsqlite.GET_LOGIN(this, email, password);
            if (user.size() == 1) {
                InputMethodManager imm = (InputMethodManager) getSystemService(this.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(mEmailView.getWindowToken(), 0);

                //App.getSingleton();
                //App.set_tipo_usuario(user.get(0).get("TIPO"));
                if(user.get(0).get("TIPO").contentEquals("Administrador"))
                    return;

                Intent obj = new Intent(this, MainActivity.class);
                obj.putExtra("TIPO", user.get(0).get("TIPO"));
                startActivityForResult(obj, REQUEST_EXIT);
            }
        }
    }

    private boolean isEmailValid(String email) {
        //TODO: Replace this with your own logic
        return email.contains("@");
    }

    private boolean isPasswordValid(String password) {
        //TODO: Replace this with your own logic
        return password.length() > 4;
    }

    /**
     * Shows the progress UI and hides the login form.
     */
    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private void showProgress(final boolean show) {
        // On Honeycomb MR2 we have the ViewPropertyAnimator APIs, which allow
        // for very easy animations. If available, use these APIs to fade-in
        // the progress spinner.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
            int shortAnimTime = getResources().getInteger(android.R.integer.config_shortAnimTime);

            mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
            mLoginFormView.animate().setDuration(shortAnimTime).alpha(
                    show ? 0 : 1).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
                }
            });

            mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
            mProgressView.animate().setDuration(shortAnimTime).alpha(
                    show ? 1 : 0).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
                }
            });
        } else {
            // The ViewPropertyAnimator APIs are not available, so simply show
            // and hide the relevant UI components.
            mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
            mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
        }
    }

    @Override
    public Loader<Cursor> onCreateLoader(int i, Bundle bundle) {
        return new CursorLoader(this,
                // Retrieve data rows for the device user's 'profile' contact.
                Uri.withAppendedPath(ContactsContract.Profile.CONTENT_URI,
                        ContactsContract.Contacts.Data.CONTENT_DIRECTORY), ProfileQuery.PROJECTION,

                // Select only email addresses.
                ContactsContract.Contacts.Data.MIMETYPE +
                        " = ?", new String[]{ContactsContract.CommonDataKinds.Email
                .CONTENT_ITEM_TYPE},

                // Show primary email addresses first. Note that there won't be
                // a primary email address if the user hasn't specified one.
                ContactsContract.Contacts.Data.IS_PRIMARY + " DESC");
    }

    @Override
    public void onLoadFinished(Loader<Cursor> cursorLoader, Cursor cursor) {
        List<String> emails = new ArrayList<>();
        cursor.moveToFirst();
        while (!cursor.isAfterLast()) {
            emails.add(cursor.getString(ProfileQuery.ADDRESS));
            cursor.moveToNext();
        }

        addEmailsToAutoComplete(emails);
    }

    @Override
    public void onLoaderReset(Loader<Cursor> cursorLoader) {

    }

    private void addEmailsToAutoComplete(List<String> emailAddressCollection) {
        //Create adapter to tell the AutoCompleteTextView what to show in its dropdown list.
        ArrayAdapter<String> adapter =
                new ArrayAdapter<>(LoginActivity.this,
                        android.R.layout.simple_dropdown_item_1line, emailAddressCollection);

        mEmailView.setAdapter(adapter);
    }

    private interface ProfileQuery {
        String[] PROJECTION = {
                ContactsContract.CommonDataKinds.Email.ADDRESS,
                ContactsContract.CommonDataKinds.Email.IS_PRIMARY,
        };

        int ADDRESS = 0;
        int IS_PRIMARY = 1;
    }
    public void cargarUsuarios()
    {
        List<Usuario> usuarioList = new ArrayList<Usuario>();
        usuarioList.add(new Usuario(1, "admin@supermix.com", "mgutierrez", "12345", "Administrador"));
        usuarioList.add(new Usuario(2, "chofer@supermix.com", "kcruz", "12345", "Chofer"));
        WSqlite sqlite = new WSqlite();

        Log.d("mysql", "cargar usuario inicio");
        for(int i=0; i<usuarioList.size(); i++) {
            int ID = usuarioList.get(i).getId();
            String EMAIL = usuarioList.get(i).getEmail();
            String USUARIO = usuarioList.get(i).getUsuario();
            String PASSWORD = usuarioList.get(i).getPassword();
            String TIPO = usuarioList.get(i).getTipo();

            sqlite.INSERT_REPLACE_USUARIO(LoginActivity.this
                    , ID, EMAIL, USUARIO, PASSWORD
                    , TIPO);
        }


        final ProgressDialog pd = ProgressDialog.show(LoginActivity.this, "Por favor espere", "Cargando Usuarios....", true,false);
        pd.show();

        new CallService() {
            @Override
            protected void onPostExecute(Object result) {

                pd.dismiss();
                if(result==null)
                {
                    Toast.makeText(LoginActivity.this,"Data vacia", Toast.LENGTH_LONG).show();
                }
                else
                {
                    List<Usuario> usuarioList = (List<Usuario>)result;
                    WSqlite sqlite = new WSqlite();
                    for(int i=0; i<usuarioList.size(); i++) {
                        int ID = usuarioList.get(i).getId();
                        String EMAIL = usuarioList.get(i).getEmail();
                        String USUARIO = usuarioList.get(i).getUsuario();
                        String PASSWORD = usuarioList.get(i).getPassword();
                        String TIPO = usuarioList.get(i).getTipo();

                        sqlite.INSERT_REPLACE_USUARIO(LoginActivity.this
                                , ID, EMAIL, USUARIO, PASSWORD
                                , TIPO);
                    }
                    Log.d("mysql", "cargar usuario fin");
                }
            }
            @Override
            protected Object doInBackground(String... args) {
                wsusuarios ws=new wsusuarios();
                return ws.GetUsuarios();
            }
        }.execute();
    }
    public void cargarJefes()
    {
        List<Jefe> jefeList = new ArrayList<Jefe>();
        jefeList.add(new Jefe(1, "Jefaso SAMA", "jefe", "1234", "SUBGERENTE"));
        WSqlite sqlite = new WSqlite();

        for(int i=0; i<jefeList.size(); i++) {
            String ID = String.valueOf(jefeList.get(i).getId());
            String NOMBRE = jefeList.get(i).getNombre();
            String USUARIO = jefeList.get(i).getUsuario();
            String PASSWORD = jefeList.get(i).getPassword();
            String SUBGERENTE = jefeList.get(i).getSubgerente();

            sqlite.INSERT_REPLACE_JEFE(LoginActivity.this
                    , ID, NOMBRE, USUARIO, PASSWORD
                    , SUBGERENTE);
        }

        Log.d("mysql", "cargar jefes inicio");
        final ProgressDialog pd = ProgressDialog.show(LoginActivity.this, "Por favor espere", "Cargando Jefes....", true,false);
        pd.show();

        new CallService() {
            @Override
            protected void onPostExecute(Object result) {

                pd.dismiss();
                if(result==null)
                {
                    Toast.makeText(LoginActivity.this,"Data vacia", Toast.LENGTH_LONG).show();
                }
                else
                {
                    List<Jefe> jefeList= (List<Jefe>)result;
                    WSqlite sqlite = new WSqlite();
                    for(int i=0; i<jefeList.size(); i++) {
                        String ID = String.valueOf(jefeList.get(i).getId());
                        String NOMBRE = jefeList.get(i).getNombre();
                        String USUARIO = jefeList.get(i).getUsuario();
                        String PASSWORD = jefeList.get(i).getPassword();
                        String SUBGERENTE = jefeList.get(i).getSubgerente();

                        sqlite.INSERT_REPLACE_JEFE(LoginActivity.this
                                , ID, NOMBRE, USUARIO, PASSWORD
                                , SUBGERENTE);
                    }
                    Log.d("mysql", "cargar jefes fin");
                }
            }
            @Override
            protected Object doInBackground(String... args) {
                wsjefes ws=new wsjefes();
                return ws.GetJefes();
            }
        }.execute();
    }
    public void cargarEquipos()
    {
        final ProgressDialog pd = ProgressDialog.show(LoginActivity.this, "Por favor espere", "Sincronizando Informacion de Equipos....", true,false);
        pd.show();


        new CallService() {
            @Override
            protected void onPostExecute(Object result) {

                pd.dismiss();
                if(result==null)
                {
                    Toast.makeText(LoginActivity.this,"Data vacia", Toast.LENGTH_LONG).show();
                }
                else
                {
                    WSqlite sqlite=new WSqlite();

                    List<Equipo> ListaEquipos=(List<Equipo>)result;
                    sqlite.DELETE_EQUIPOS(LoginActivity.this);
                    for(Iterator<Equipo> i = ListaEquipos.iterator(); i.hasNext();)
                    {
                        Equipo item = i.next();
                        //Log.e("SincronizarFragment-Equipo", item.getCodigoInterno());
                        sqlite.INSERT_EQUIPO(LoginActivity.this,
                                item.getId(),
                                item.getCentro(),
                                item.getPlanta(),
                                item.getCodigoInterno(),
                                item.getPlaca(),
                                item.getClase());

                    }
                    final View view = findViewById(android.R.id.content).getRootView();
                    Snackbar.make(view, "Equipos Sincronizados", Snackbar.LENGTH_SHORT)
                            .setAction("Action", null).show();
                }
            }

            @Override
            protected Object doInBackground(String... args) {
                wsequipos ws=new wsequipos();
                return ws.GetEquipos();
            }
        }.execute();
    }
    /**
     * Represents an asynchronous login/registration task used to authenticate
     * the user.
     */
    public class UserLoginTask extends AsyncTask<Void, Void, Boolean> {

        private final String mEmail;
        private final String mPassword;

        UserLoginTask(String email, String password) {
            mEmail = email;
            mPassword = password;
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            // TODO: attempt authentication against a network service.

            try {
                // Simulate network access.
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                return false;
            }

            for (String credential : DUMMY_CREDENTIALS) {
                String[] pieces = credential.split(":");
                if (pieces[0].equals(mEmail)) {
                    // Account exists, return true if the password matches.
                    return pieces[1].equals(mPassword);
                }
            }

            // TODO: register the new account here.
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            mAuthTask = null;
            showProgress(false);

            if (success) {
                finish();
            } else {
                mPasswordView.setError(getString(R.string.error_incorrect_password));
                mPasswordView.requestFocus();
            }
        }

        @Override
        protected void onCancelled() {
            mAuthTask = null;
            showProgress(false);
        }
    }
    public void cargarVentas(){
        StrictMode.ThreadPolicy policy = new
                StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);


        Log.d("mssql", "ventas");
        try{
            Class.forName("net.sourceforge.jtds.jdbc.Driver");
            String cn = "jdbc:jtds:sqlserver://209.217.248.94;databaseName=solucionsmart;user=temporal;password=Zzb3a_12;";
            Connection connection = DriverManager.getConnection(cn);
            Statement statement = connection.createStatement();
            ResultSet resultSet = statement.executeQuery("select * from ggamarra.TVENTASAPP");
            while(resultSet.next()) {
                String id = resultSet.getString("ID");
                String fecha = resultSet.getString("FECHA");
                String producto = resultSet.getString("PRODUCTO");
                String ticket = resultSet.getString("TICKET");
                String nrogalones = resultSet.getString("NROGALONES");
                String horometro = resultSet.getString("HOROMETRO");
                String km = resultSet.getString("KM");
                String equipo = resultSet.getString("EQUIPO");
                Log.d("mssql", id);
                WSqlite sqlite = new WSqlite();
                sqlite.INSERT_REPLACE_VENTA(LoginActivity.this, id, fecha, producto
                , ticket, nrogalones, horometro, km, equipo);
            }
            //Log.d("mssql", dato);
            connection.close();
        }catch(ClassNotFoundException e){
            e.printStackTrace();
        }catch(SQLException e){
            e.printStackTrace();
        } catch (java.sql.SQLException e) {
            e.printStackTrace();
        }
    }
}

