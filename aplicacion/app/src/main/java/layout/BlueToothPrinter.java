package layout;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.pdf.PdfDocument;
import android.os.CancellationSignal;
import android.os.ParcelFileDescriptor;
import android.print.PageRange;
import android.print.PrintAttributes;
import android.print.PrintDocumentAdapter;
import android.print.PrintDocumentInfo;
import android.print.PrintManager;
import android.print.pdf.PrintedPdfDocument;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.util.DisplayMetrics;

import com.example.app.aplicacion.R;

import org.w3c.dom.Text;

import java.io.FileOutputStream;
import java.io.IOException;

import util.App;

public class BlueToothPrinter extends AppCompatActivity {

    private DisplayMetrics dm;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_blue_tooth_printer);

        dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        Intent intent = getIntent();

        String fecha = intent.getStringExtra("FECHA") + "  ";
        String nroticket = intent.getStringExtra("NROTICKET");
        String galones = intent.getStringExtra("GALONES");
        String horometro = intent.getStringExtra("HOROMETRO");
        String kilometros = intent.getStringExtra("KILOMETROS");
        String codigointerno = intent.getStringExtra("CODIGOINTERNO");
        String centro = intent.getStringExtra("CENTRO");
        String placa = intent.getStringExtra("PLACA");

        TextView tw_fecha = (TextView) findViewById(R.id.textView_fecha);
        tw_fecha.setText(fecha);
        TextView tw_nroTicket = (TextView) findViewById(R.id.textView_nroTicket);
        tw_nroTicket.setText(nroticket);
        TextView tw_hora = (TextView) findViewById(R.id.textView_hora);
        tw_hora.setVisibility(View.GONE);

        TextView tw_galones = (TextView) findViewById(R.id.textView_galones);
        float px16 = App.convertDpToPixel(16.0f, getApplicationContext());
        int width = (dm.widthPixels - ((int) px16) * 2) / 2;
        tw_galones.setLayoutParams(new LinearLayout.LayoutParams(width, LinearLayout.LayoutParams.WRAP_CONTENT));
        TextView tw_horas = (TextView) findViewById(R.id.textView_horometro);
        tw_horas.setLayoutParams(new LinearLayout.LayoutParams(width, LinearLayout.LayoutParams.WRAP_CONTENT));
        TextView tw_kilometros = (TextView) findViewById(R.id.textView_kilometraje);
        tw_kilometros.setLayoutParams(new LinearLayout.LayoutParams(width, LinearLayout.LayoutParams.WRAP_CONTENT));

        TextView tw_nro_galones = (TextView) findViewById(R.id.textView_nro_galones);
        tw_nro_galones.setText(galones);
        TextView tw_nro_horas = (TextView) findViewById(R.id.textView_nro_horas);
        tw_nro_horas.setText(horometro);
        TextView tw_nro_kilometros = (TextView) findViewById(R.id.textView_nro_kilometros);
        tw_nro_kilometros.setText(kilometros);
        TextView tw_codigoInterno = (TextView) findViewById(R.id.item_CodigoInterno);
        tw_codigoInterno.setText(codigointerno);
        TextView tw_centro = (TextView) findViewById(R.id.item_Centro);
        tw_centro.setText(centro);
        TextView tw_placa = (TextView) findViewById(R.id.item_Placa);
        tw_placa.setText(placa);

        Button btn_imprimir = (Button) findViewById(R.id.button_imprimir);

        btn_imprimir.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                printDocument(view);
            }
        });
    }
    public void printDocument(View view)
    {
        PrintManager printManager = (PrintManager) this
                .getSystemService(Context.PRINT_SERVICE);

        String jobName = this.getString(R.string.app_name) +
                " Document";

        printManager.print(jobName, new MyPrintDocumentAdapter(this),
                null);
    }

    public class MyPrintDocumentAdapter extends PrintDocumentAdapter {
        Context context;
        private int pageHeight;
        private int pageWidth;
        public PdfDocument myPdfDocument;
        public int totalpages = 1;

        public MyPrintDocumentAdapter(Context context) {

            this.context = context;
        }
        @Override
        public void onLayout(PrintAttributes oldAttributes,
                             PrintAttributes newAttributes,
                             CancellationSignal cancellationSignal,
                             LayoutResultCallback callback,
                             Bundle metadata) {

            myPdfDocument = new PrintedPdfDocument(context, newAttributes);

            pageHeight =
                    newAttributes.getMediaSize().getHeightMils()/1000 * 72;
            pageWidth =
                    newAttributes.getMediaSize().getWidthMils()/1000 * 72;

            if (cancellationSignal.isCanceled() ) {
                callback.onLayoutCancelled();
                return;
            }

            if (totalpages > 0) {
                PrintDocumentInfo.Builder builder = new PrintDocumentInfo
                        .Builder("print_output.pdf")
                        .setContentType(PrintDocumentInfo.CONTENT_TYPE_DOCUMENT)
                        .setPageCount(totalpages);

                PrintDocumentInfo info = builder.build();
                callback.onLayoutFinished(info, true);
            } else {
                callback.onLayoutFailed("Page count is zero.");
            }
        }
        private boolean pageInRange(PageRange[] pageRanges, int page)
        {
            for (int i = 0; i<pageRanges.length; i++)
            {
                if ((page >= pageRanges[i].getStart()) &&
                        (page <= pageRanges[i].getEnd()))
                    return true;
            }
            return false;
        }
        @Override
        public void onWrite(final PageRange[] pageRanges,
                            final ParcelFileDescriptor destination,
                            final CancellationSignal cancellationSignal,
                            final WriteResultCallback callback) {

            for (int i = 0; i < totalpages; i++) {
                if (pageInRange(pageRanges, i))
                {
                    PdfDocument.PageInfo newPage = new PdfDocument.PageInfo.Builder(pageWidth,
                            pageHeight, i).create();

                    PdfDocument.Page page =
                            myPdfDocument.startPage(newPage);

                    if (cancellationSignal.isCanceled()) {
                        callback.onWriteCancelled();
                        myPdfDocument.close();
                        myPdfDocument = null;
                        return;
                    }
                    drawPage(page, i);
                    myPdfDocument.finishPage(page);
                }
            }

            try {
                myPdfDocument.writeTo(new FileOutputStream(
                        destination.getFileDescriptor()));
            } catch (IOException e) {
                callback.onWriteFailed(e.toString());
                return;
            } finally {
                myPdfDocument.close();
                myPdfDocument = null;
            }

            callback.onWriteFinished(pageRanges);
        }
        private void drawPage(PdfDocument.Page page,
                              int pagenumber) {
            Canvas canvas = page.getCanvas();

            pagenumber++; // Make sure page numbers start at 1

            TextView tw_titulo = (TextView) findViewById(R.id.textView_titulo);
            TextView tw_titulo2 = (TextView) findViewById(R.id.textView_titulo2);
            TextView tw_subtitulo = (TextView) findViewById(R.id.textView_subtitulo);
            TextView tw_nombreProducto = (TextView) findViewById(R.id.textView_nombreProducto);

            TextView tw_fecha = (TextView) findViewById(R.id.textView_fecha);
            TextView tw_nroTicket = (TextView) findViewById(R.id.textView_nroTicket);
            TextView tw_hora = (TextView) findViewById(R.id.textView_hora);

            TextView tw_galones = (TextView) findViewById(R.id.textView_galones);
            TextView tw_horas = (TextView) findViewById(R.id.textView_horometro);
            TextView tw_kilometros = (TextView) findViewById(R.id.textView_kilometraje);

            TextView tw_nro_galones = (TextView) findViewById(R.id.textView_nro_galones);
            TextView tw_nro_horas = (TextView) findViewById(R.id.textView_nro_horas);
            TextView tw_nro_kilometros = (TextView) findViewById(R.id.textView_nro_kilometros);
            TextView tw_codigoInterno = (TextView) findViewById(R.id.item_CodigoInterno);
            TextView tw_centro = (TextView) findViewById(R.id.item_Centro);
            TextView tw_placa = (TextView) findViewById(R.id.item_Placa);

            Paint paint = new Paint();

            String CurrentString = tw_titulo.getText().toString();
            String[] separated = CurrentString.split("\\n");

            float titleBaseLine = canvas.getHeight()*0.1f;

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_titulo.getTextSize());
            paint.setTypeface(tw_titulo.getTypeface());
            paint.setFontFeatureSettings(tw_titulo.getFontFeatureSettings());
            canvas.drawText(
                    separated[0],
                    canvas.getWidth()/3,
                    titleBaseLine + canvas.getHeight()/13,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_titulo.getTextSize());
            paint.setTypeface(tw_titulo.getTypeface());
            paint.setFontFeatureSettings(tw_titulo.getFontFeatureSettings());
            canvas.drawText(
                    separated[1],
                    canvas.getWidth()/3,
                    titleBaseLine + canvas.getHeight()/7.5f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_titulo2.getTextSize());
            paint.setTypeface(tw_titulo2.getTypeface());
            paint.setFontFeatureSettings(tw_titulo2.getFontFeatureSettings());
            canvas.drawText(
                    tw_titulo2.getText().toString(),
                    canvas.getWidth()/1.75f,
                    titleBaseLine + canvas.getHeight()/5.5f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_nroTicket.getTextSize());
            paint.setTypeface(tw_nroTicket.getTypeface());
            paint.setFontFeatureSettings(tw_nroTicket.getFontFeatureSettings());
            canvas.drawText(
                    tw_nroTicket.getText().toString(),
                    canvas.getWidth()/2.85f,
                    titleBaseLine + canvas.getHeight()/4.0f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_subtitulo.getTextSize());
            paint.setTypeface(tw_subtitulo.getTypeface());
            paint.setFontFeatureSettings(tw_subtitulo.getFontFeatureSettings());
            canvas.drawText(
                    tw_subtitulo.getText().toString(),
                    canvas.getWidth()/3.25f,
                    titleBaseLine + canvas.getHeight()/3.0f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_fecha.getTextSize());
            paint.setTypeface(tw_fecha.getTypeface());
            paint.setFontFeatureSettings(tw_fecha.getFontFeatureSettings());
            canvas.drawText(
                    tw_fecha.getText().toString(),
                    canvas.getWidth()*0.4f,
                    titleBaseLine + canvas.getHeight()/2.5f,
                    paint);

            /*
            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_hora.getTextSize());
            paint.setTypeface(tw_hora.getTypeface());
            paint.setFontFeatureSettings(tw_hora.getFontFeatureSettings());
            canvas.drawText(
                    tw_hora.getText().toString(),
                    canvas.getWidth()/1.75f,
                    titleBaseLine + canvas.getHeight()/2.5f,
                    paint);
                    */

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_nombreProducto.getTextSize());
            paint.setTypeface(tw_nombreProducto.getTypeface());
            paint.setFontFeatureSettings(tw_nombreProducto.getFontFeatureSettings());
            canvas.drawText(
                    tw_nombreProducto.getText().toString(),
                    canvas.getWidth()*0.10f,
                    titleBaseLine + canvas.getHeight()/2.0f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_codigoInterno.getTextSize());
            paint.setTypeface(tw_codigoInterno.getTypeface());
            paint.setFontFeatureSettings(tw_codigoInterno.getFontFeatureSettings());
            canvas.drawText(
                    tw_codigoInterno.getText().toString(),
                    canvas.getWidth()/3.0f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.425f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_centro.getTextSize());
            paint.setTypeface(tw_centro.getTypeface());
            paint.setFontFeatureSettings(tw_centro.getFontFeatureSettings());
            canvas.drawText(
                    tw_centro.getText().toString(),
                    canvas.getWidth()*0.10f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.380f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_placa.getTextSize());
            paint.setTypeface(tw_placa.getTypeface());
            paint.setFontFeatureSettings(tw_placa.getFontFeatureSettings());
            canvas.drawText(
                    tw_placa.getText().toString(),
                    canvas.getWidth()*0.10f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.345f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_galones.getTextSize());
            paint.setFontFeatureSettings(tw_galones.getFontFeatureSettings());
            canvas.drawText(
                    tw_galones.getText().toString(),
                    canvas.getWidth()*0.10f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.275f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_horas.getTextSize());
            paint.setFontFeatureSettings(tw_horas.getFontFeatureSettings());
            canvas.drawText(
                    tw_horas.getText().toString(),
                    canvas.getWidth()*0.10f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.225f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_kilometros.getTextSize());
            paint.setFontFeatureSettings(tw_kilometros.getFontFeatureSettings());
            canvas.drawText(
                    tw_kilometros.getText().toString(),
                    canvas.getWidth()*0.10f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.175f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_nro_galones.getTextSize());
            paint.setFontFeatureSettings(tw_nro_galones.getFontFeatureSettings());
            canvas.drawText(
                    tw_nro_galones.getText().toString(),
                    canvas.getWidth()*0.6f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.275f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_nro_horas.getTextSize());
            paint.setFontFeatureSettings(tw_nro_horas.getFontFeatureSettings());
            canvas.drawText(
                    tw_nro_horas.getText().toString(),
                    canvas.getWidth()*0.6f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.225f,
                    paint);

            paint.setColor(Color.BLACK);
            paint.setTextSize(tw_nro_kilometros.getTextSize());
            paint.setFontFeatureSettings(tw_nro_kilometros.getFontFeatureSettings());
            canvas.drawText(
                    tw_nro_kilometros.getText().toString(),
                    canvas.getWidth()*0.6f,
                    titleBaseLine + canvas.getHeight()-canvas.getHeight()*0.175f,
                    paint);

            /*
            canvas.drawCircle(pageInfo.getPageWidth()/2,
                    pageInfo.getPageHeight()/2,
                    150,
                    paint);
                    */
        }
    }

}
