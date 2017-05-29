package layout;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.print.PrintManager;
import android.support.annotation.NonNull;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;

import com.example.app.aplicacion.R;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

import layout.EquiposFragment;
import layout.FragmentRatioPlanta;
import layout.RatioBuscarFragment;
import layout.JefesFragment;
import layout.RatioDia;
import layout.RatioEquipo;
import layout.SincronizarFragment;
import layout.VentaFragment;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private static final int REQUEST_PRINTERS = 2;

    int countFragments = 0;
    int countBacks = 0;

    Fragment fragment;
    public List<Fragment> listFragments;

    private String tipo = "";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent intent = getIntent();
        tipo = intent.getStringExtra("TIPO");
        Log.d("tipo", intent.getStringExtra("TIPO"));

        listFragments = new ArrayList<Fragment>();
        /*FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });*/

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        //Log.d("size", String.valueOf(navigationView.getMenu().size()));
        // Set action bar title
        //setTitle(item.getTitle());

        Fragment fragment = null;

        if(tipo.contentEquals("Administrador")){
            fragment = new EquiposFragment();
            MenuItem item = navigationView.getMenu().getItem(0).getSubMenu().getItem(0);
            item.setChecked(true);
        }
        if(tipo.contentEquals("Chofer")){
            fragment = new VentaFragment();
            MenuItem item = navigationView.getMenu().getItem(1).getSubMenu().getItem(0);
            item.setChecked(true);
        }
        if(tipo.contentEquals("Jefe")){
            fragment = new FragmentRatioPlanta();
            MenuItem item = navigationView.getMenu().getItem(2).getSubMenu().getItem(0);
            item.setChecked(true);
        }

        listFragments.add(fragment);

        FragmentManager fragmentManager = getSupportFragmentManager();
        fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                //.addToBackStack("Venta")
                .commit();
    }

    @Override
    public void onBackPressed() {
        //super.onBackPressed();
        Log.d("result", "back pressed");
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            //FragmentManager fragmentManager = getSupportFragmentManager();

            /*
            Log.d("fragments size: ", String.valueOf(getSupportFragmentManager().getFragments().size()));
            String f = getSupportFragmentManager().getFragments()
                    .get(getSupportFragmentManager().getFragments().size()-2)
                    .getClass().getSimpleName();
            Log.d("back pressed", f);
            */
            String f = listFragments
                    .get(listFragments.size()-1)
                    .getClass().getSimpleName();
            listFragments.remove(listFragments
                    .get(listFragments.size()-1));

            NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
            if(f.contentEquals("EquiposFragment")){
                MenuItem item = navigationView.getMenu().getItem(0).getSubMenu().getItem(0);
                item.setChecked(false);
            }
            if(f.contentEquals("JefesFragment")){
                MenuItem item = navigationView.getMenu().getItem(0).getSubMenu().getItem(1);
                item.setChecked(false);
            }
            if(f.contentEquals("VentaFragment")){
                MenuItem item = navigationView.getMenu().getItem(1).getSubMenu().getItem(0);
                item.setChecked(false);
            }
            if(f.contentEquals("SincronizarFragment")){
                MenuItem item = navigationView.getMenu().getItem(1).getSubMenu().getItem(1);
                item.setChecked(false);
            }
            if(f.contentEquals("FragmentRatioPlanta")){
                MenuItem item = navigationView.getMenu().getItem(2).getSubMenu().getItem(0);
                item.setChecked(false);
            }
            if(f.contentEquals("RatioDia")){
                MenuItem item = navigationView.getMenu().getItem(2).getSubMenu().getItem(1);
                item.setChecked(false);
            }
            if(f.contentEquals("RatioEquipo")){
                MenuItem item = navigationView.getMenu().getItem(2).getSubMenu().getItem(2);
                item.setChecked(false);
            }


            //Log.d("main ", String.valueOf(getSupportFragmentManager().getFragments().size()));
            //countFragments--;
            //countBacks++;

            //getSupportFragmentManager().getFragments().remove(getSupportFragmentManager().getFragments().size()-1);
            //getSupportFragmentManager().popBackStackImmediate();

            if(listFragments.size()<1){
            //if(countFragments<1){
                finish();
            }else{
                FragmentManager fragmentManager = getSupportFragmentManager();
                fragmentManager.popBackStackImmediate();

                Handler mainHandler = new Handler(getApplicationContext().getMainLooper());

                Runnable myRunnable = new Runnable() {
                    @Override
                    public void run() {
                        //MainActivity.super.onBackPressed();
                        FragmentManager fragmentManager = getSupportFragmentManager();
                        fragmentManager.beginTransaction().replace(R.id.flContent, listFragments
                                .get(listFragments.size()-1))
                                //.addToBackStack("Other")
                                .commit();
                    } // This is your code
                };
                mainHandler.post(myRunnable);

                f = listFragments
                    .get(listFragments.size()-1)
                    .getClass().getSimpleName();

                if(f.contentEquals("EquiposFragment")){
                    MenuItem item = navigationView.getMenu().getItem(0).getSubMenu().getItem(0);
                    item.setChecked(true);
                    setTitle(item.getTitle());
                }
                if(f.contentEquals("JefesFragment")){
                    MenuItem item = navigationView.getMenu().getItem(0).getSubMenu().getItem(1);
                    item.setChecked(true);
                    setTitle(item.getTitle());
                }
                if(f.contentEquals("VentaFragment")){
                    MenuItem item = navigationView.getMenu().getItem(1).getSubMenu().getItem(0);
                    item.setChecked(true);
                    setTitle(item.getTitle());
                }
                if(f.contentEquals("SincronizarFragment")){
                    MenuItem item = navigationView.getMenu().getItem(1).getSubMenu().getItem(1);
                    item.setChecked(true);
                    setTitle(item.getTitle());
                }
                if(f.contentEquals("FragmentRatioPlanta")){
                    MenuItem item = navigationView.getMenu().getItem(2).getSubMenu().getItem(0);
                    item.setChecked(true);
                    setTitle(item.getTitle());
                }
                if(f.contentEquals("RatioDia")){
                    MenuItem item = navigationView.getMenu().getItem(2).getSubMenu().getItem(1);
                    item.setChecked(true);
                    setTitle(item.getTitle());
                }
                if(f.contentEquals("RatioEquipo")){
                    MenuItem item = navigationView.getMenu().getItem(2).getSubMenu().getItem(2);
                    item.setChecked(true);
                    setTitle(item.getTitle());
                }
                //getSupportFragmentManager().getFragments().add(null);

            }
        }

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        switch(requestCode){
            case REQUEST_PRINTERS:{
                if(resultCode == RESULT_OK){

                }
            }break;

        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_printers) {
            PrintManager printManager =
                    (PrintManager) getApplicationContext().getSystemService(getApplicationContext().PRINT_SERVICE);
            return true;
        }
        if (id == R.id.action_settings) {
            return true;
        }
        if (id == R.id.action_exit) {
            Log.d("result", "exit");
            Intent intent = new Intent();
            intent.putExtra("hola", "hola");
            setResult(RESULT_OK,intent);

            finish();

            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        fragment=null;
        int id = item.getItemId();
        if (id == R.id.equipos && tipo.contentEquals("Administrador")) {
            fragment = new EquiposFragment();
            //Snackbar.make(item.getActionView(), "Administracion de Equipos", Snackbar.LENGTH_LONG)
            //       .setAction("Action", null).show();
        } else if (id == R.id.jefes && tipo.contentEquals("Administrador")) {
            fragment = new JefesFragment();
        } else if (id == R.id.venta && tipo.contentEquals("Chofer")) {
            fragment = new VentaFragment();
        } else if (id == R.id.sincronizar && tipo.contentEquals("Chofer")) {
            fragment = new SincronizarFragment();
        } else if (id == R.id.ratio_planta && tipo.contentEquals("Jefe")) {
            fragment = new FragmentRatioPlanta();

        } else if (id == R.id.ratio_dia && tipo.contentEquals("Jefe")) {
            fragment = new RatioDia();

        } else if (id == R.id.ratio_equipo && tipo.contentEquals("Jefe")) {
            fragment = new RatioEquipo();
        }

        if(fragment == null){
            return true;
        }
        listFragments.add(fragment);

        Handler mainHandler = new Handler(getApplicationContext().getMainLooper());

        Runnable myRunnable = new Runnable() {
            @Override
            public void run() {
                /*
                for(int i=0; i<countBacks;i++)
                    getSupportFragmentManager().popBackStackImmediate();
                countBacks = 0;
                */

                FragmentManager fragmentManager = getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                        //.addToBackStack("Other")
                        .commit();
            } // This is your code
        };
        mainHandler.post(myRunnable);




        item.setChecked(true);
        // Set action bar title
        setTitle(item.getTitle());

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);

        return true;
    }

    public boolean replaceVentaEquipo() {
        // Handle navigation view item clicks here.
        fragment = null;

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        MenuItem item = null;

        fragment = new EquiposFragment();
        item = navigationView.getMenu().getItem(0).getSubMenu().getItem(0);
        ((EquiposFragment)fragment).ventaFragmentActive();

        listFragments.add(fragment);

        Handler mainHandler = new Handler(getApplicationContext().getMainLooper());

        Runnable myRunnable = new Runnable() {
            @Override
            public void run() {
                /*
                for(int i=0; i<countBacks;i++)
                    getSupportFragmentManager().popBackStackImmediate();
                countBacks = 0;
                */

                FragmentManager fragmentManager = getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                        //.addToBackStack("Other")
                        .commit();
            } // This is your code
        };
        mainHandler.post(myRunnable);

        item.setChecked(true);
        // Set action bar title
        setTitle(item.getTitle());

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);

        return true;
    }
    public boolean replaceRatioBuscarEquipo() {
        // Handle navigation view item clicks here.
        fragment = null;

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        MenuItem item = null;

        fragment = new EquiposFragment();
        item = navigationView.getMenu().getItem(0).getSubMenu().getItem(0);
        ((EquiposFragment)fragment).ratioBuscarFragment_active();

        listFragments.add(fragment);

        Handler mainHandler = new Handler(getApplicationContext().getMainLooper());

        Runnable myRunnable = new Runnable() {
            @Override
            public void run() {
                /*
                for(int i=0; i<countBacks;i++)
                    getSupportFragmentManager().popBackStackImmediate();
                countBacks = 0;
                */

                FragmentManager fragmentManager = getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                        //.addToBackStack("Other")
                        .commit();
            } // This is your code
        };
        mainHandler.post(myRunnable);

        item.setChecked(true);
        // Set action bar title
        setTitle(item.getTitle());

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);

        return true;
    }
    public boolean replaceRatioBuscarPlanta() {
        // Handle navigation view item clicks here.
        fragment = null;

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        MenuItem item = null;

        fragment = new PlantaFragment();
        item = navigationView.getMenu().getItem(0).getSubMenu().getItem(0);
        ((PlantaFragment)fragment).ratioBuscarFragment_active();

        listFragments.add(fragment);

        Handler mainHandler = new Handler(getApplicationContext().getMainLooper());

        Runnable myRunnable = new Runnable() {
            @Override
            public void run() {
                /*
                for(int i=0; i<countBacks;i++)
                    getSupportFragmentManager().popBackStackImmediate();
                countBacks = 0;
                */

                FragmentManager fragmentManager = getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.flContent, fragment)
                        //.addToBackStack("Other")
                        .commit();
            } // This is your code
        };
        mainHandler.post(myRunnable);

        item.setChecked(true);
        // Set action bar title
        setTitle(item.getTitle());

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);

        return true;
    }
}
