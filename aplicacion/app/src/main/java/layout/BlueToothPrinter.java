package layout;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.example.app.aplicacion.R;

public class BlueToothPrinter extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_blue_tooth_printer);

        Intent intent = getIntent();

        String fecha = intent.getStringExtra("FECHA");
        String nroticket = intent.getStringExtra("NROTICKET");
        String galones = intent.getStringExtra("GALONES");
        String horometro = intent.getStringExtra("HOROMETRO");
        String kilometros = intent.getStringExtra("KILOMETROS");
        String codigointerno = intent.getStringExtra("CODIGOINTERNO");
        String centro = intent.getStringExtra("CENTRO");
        String placa = intent.getStringExtra("PLACA");

        TextView tw_fecha = (TextView)findViewById(R.id.textView_fecha);
        tw_fecha.setText(fecha);
        TextView tw_hora = (TextView)findViewById(R.id.textView_hora);
        tw_hora.setVisibility(View.GONE);
        TextView tw_galones = (TextView)findViewById(R.id.textView_nro_galones);
        tw_galones.setText(galones);
        TextView tw_nro_horas = (TextView)findViewById(R.id.textView_nro_horas);
        tw_nro_horas.setText(horometro);
        TextView tw_nro_kilometros = (TextView)findViewById(R.id.textView_nro_kilometros);
        tw_nro_kilometros.setText(kilometros);
        TextView tw_codigoInterno = (TextView)findViewById(R.id.item_CodigoInterno);
        tw_codigoInterno.setText(codigointerno);
        TextView tw_centro = (TextView)findViewById(R.id.item_Centro);
        tw_centro.setText(centro);
        TextView tw_placa = (TextView)findViewById(R.id.item_Placa);
        tw_placa.setText(placa);
    }
}
