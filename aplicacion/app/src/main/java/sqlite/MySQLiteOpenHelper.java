package sqlite;
import android.content.Context;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class MySQLiteOpenHelper extends  SQLiteOpenHelper {


    private static final int Version = 6;

    private static final String TUSUARIOS= "create table " + "TUSUARIOS" + " (ID INTEGER PRIMARY KEY NOT NULL"
            + ", EMAIL VARCHAR, USUARIO VARCHAR, PASSWORD VARCHAR, TIPO VARCHAR"
            + ", CONSTRAINT usuario_unique UNIQUE (USUARIO));";

    private static final String TJEFES= "create table " + "TJEFES" + " (ID INTEGER PRIMARY KEY NOT NULL"
            + ", NOMBRE VARCHAR, USUARIO VARCHAR, PASSWORD VARCHAR, SUBGERENTE VARCHAR"
            + ", CONSTRAINT usuario_unique UNIQUE (USUARIO));";

    private static final String TEQUIPOS= "create table " + "TEQUIPOS" + " (ID INTEGER PRIMARY KEY NOT NULL ,"
                + "CENTRO VARCHAR, PLANTA VARCHAR, CODIGOINTERNO VARCHAR,PLACA VARCHAR, CLASE VARCHAR);";

    private static final String TVENTAS= "create table " + "TVENTAS" + " (ID INTEGER PRIMARY KEY autoincrement NOT NULL,"
            + "FECHA VARCHAR, PRODUCTO VARCHAR, TICKET VARCHAR,NROGALONES VARCHAR, HOROMETRO VARCHAR,KM VARCHAR,EQUIPO VARCHAR);";


    private static final String TTICKETS= "create table " + "TTICKETS" + " (ID INTEGER PRIMARY KEY autoincrement NOT NULL,OBSERVACION VARCHAR);";

    public MySQLiteOpenHelper(Context contexto, String nombre,
                              CursorFactory factory, int version) {
        super(contexto, nombre, null, Version);
        // TODO Auto-generated constructor stub
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        // TODO Auto-generated method stub
        try {
            db.execSQL(TUSUARIOS);
            db.execSQL(TJEFES);
            db.execSQL(TEQUIPOS);
            db.execSQL(TVENTAS);
            db.execSQL(TTICKETS);
        }catch(SQLException e){

        }
      //  db.execSQL(tClienteProveedor);
       // db.execSQL(tConductores);

    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int AntiguaVersion, int NuevaVersion) {
        // TODO Auto-generated method stub
        Log.w(MySQLiteOpenHelper.class.getName(), "Actualizando la BD : " + AntiguaVersion + " a " + NuevaVersion  );
        try {
            db.execSQL("DROP TABLE IF EXISTS " + "TUSUARIOS");
            db.execSQL("DROP TABLE IF EXISTS " + "TJEFES");
            db.execSQL("DROP TABLE IF EXISTS " + "TEQUIPOS");
            db.execSQL("DROP TABLE IF EXISTS " + "TVENTAS");
            db.execSQL("DROP TABLE IF EXISTS " + "TTIKECTS");
        }catch(SQLException e){

        }
       // db.execSQL("DROP TABLE IF EXISTS " + "tClienteProveedor");
        onCreate(db);
    }

}

