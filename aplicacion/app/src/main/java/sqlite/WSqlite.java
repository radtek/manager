package sqlite;
import android.content.ContentValues;
import android.content.Context;
import android.database.SQLException;
import android.database.sqlite.*;
import java.util.HashMap;
import java.util.List;
import android.database.Cursor;
import java.util.ArrayList;
import com.example.app.aplicacion.R;
import android.util.Log;

import dominio.Equipo;
import dominio.Jefe;
import dominio.Venta;

public class WSqlite {
    public void INSERT_REPLACE_USUARIO(Context _context,
                                 int ID,
                                 String EMAIL,
                                 String USUARIO,
                                 String PASSWORD,
                                 String TIPO)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();

        String id = "";
        if(ID == -1){
            id = "null";
        }else{
            id = String.valueOf(ID);
        }
        String inserts = "("+id+", '"+EMAIL+"', '"+USUARIO+"', '"+PASSWORD+"', '"+TIPO+"')";
        String query = "INSERT OR REPLACE INTO TUSUARIOS(ID, EMAIL, USUARIO, PASSWORD, TIPO)VALUES"+inserts;

        Log.d("mysql", query);

        try {
            db.execSQL(query);
            db.close();
        } catch (SQLException e) {

        } finally {

        }
    }
    public String INSERT_USUARIO(Context _context,
                                 int ID,
                                 String EMAIL,
                                 String USUARIO,
                                 String PASSWORD,
                                 String TIPO)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        ContentValues valores = new ContentValues();
        try
        {
            valores.put("ID", ID);
            valores.put("NOMBRE", EMAIL);
            valores.put("USUARIO", USUARIO);
            valores.put("PASSWORD", PASSWORD);
            valores.put("TIPO", TIPO);

            db.insert("TUSUARIOS", "nullColumnHack", valores);

            db.close();
            return "Registro de Firmas OK";
        }catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }
    public String INSERT_REPLACE_EQUIPO(Context _context
                                       , String ID
                                       , String CENTRO
                                       , String PLANTA
                                       , String CODIGOINTERNO
                                       , String PLACA
                                       , String CLASE)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();

        String inserts = "("+ID+", '"+CENTRO+"', '"+PLANTA+"', '"+CODIGOINTERNO+"', '"+PLACA
                +"', '"+CLASE+"')";
        String query = "INSERT OR REPLACE INTO TEQUIPOS(ID, CENTRO, PLANTA, CODIGOINTERNO, PLACA, CLASE)VALUES"+inserts;

        Log.d("mysql", query);

        try {
            db.execSQL(query);
            db.close();
            return ID;
        } catch (SQLException e) {
            return "-1";
        } finally {

        }
    }
    public String INSERT_REPLACE_JEFE(Context _context,
                                       String ID,
                                       String NOMBRE,
                                       String USUARIO,
                                       String PASSWORD,
                                       String SUBGERENTE)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();


        String inserts = "("+ID+", '"+NOMBRE+"', '"+USUARIO+"', '"+PASSWORD+"', '"+SUBGERENTE+"')";
        String query = "INSERT OR REPLACE INTO TJEFES(ID, NOMBRE, USUARIO, PASSWORD, SUBGERENTE)VALUES"+inserts;

        Log.d("mysql", query);

        try {
            db.execSQL(query);

            db.close();
            Log.d("mysql", ID);
            return ID;
        } catch (SQLException e) {
            return "-1";
        } finally {

        }
    }
    public String INSERT_JEFES(Context _context,
                                int ID,
                                String NOMBRE,
                                String USUARIO,
                                String PASSWORD,
                                String SUBGERENTE)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        ContentValues valores = new ContentValues();
        try
        {
            valores.put("ID", ID);
            valores.put("NOMBRE", NOMBRE);
            valores.put("USUARIO", USUARIO);
            valores.put("PASSWORD", PASSWORD);
            valores.put("SUBGERENTE", SUBGERENTE);

            db.insert("TJEFES", "nullColumnHack", valores);

            db.close();
            return "Registro de Firmas OK";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }
    public String INSERT_EQUIPO(Context _context,
                                   int ID,
                                   String CENTRO,
                                   String PLANTA,
                                   String CODIGOINTERNO,
                                   String PLACA,
                                   String CLASE)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        ContentValues valores = new ContentValues();
        try
        {
                valores.put("ID", ID);
                valores.put("CENTRO", CENTRO);
                valores.put("PLANTA", PLANTA);
                valores.put("CODIGOINTERNO", CODIGOINTERNO);
                valores.put("PLACA", PLACA);
                valores.put("CLASE", CLASE);

            db.insert("TEQUIPOS", "nullColumnHack", valores);

            db.close();
            return "Registro de Firmas OK";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }
    public String INSERT_VENTA(Context _context,
                                String  FECHA,
                                String PRODUCTO,
                                String TICKET,
                                String GALONES,
                                String HOROMETRO,
                                String KILOMETROS,
                                String EQUIPO)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        ContentValues valores = new ContentValues();
        try
        {
            //valores.put("ID", ID);
            valores.put("FECHA", FECHA);
            valores.put("PRODUCTO", PRODUCTO);
            valores.put("TICKET", TICKET);
            valores.put("NROGALONES", GALONES);
            valores.put("HOROMETRO", HOROMETRO);
            valores.put("KM", KILOMETROS);
            valores.put("EQUIPO", EQUIPO);

            db.insert("TVENTAS", "nullColumnHack", valores);

            db.close();
            return "Registro de Despacho OK";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
        finally
        {
            INSERT_TICKET(_context);
        }
    }
    public String DELETE_JEFE(Context _context, String ID)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        try
        {
            //PRIMERO LIMPIAMOS LA TABLA
            String where = "id = "+ID;
            String[] whereArgs = new String[] {};
            db.delete("TJEFES", where, whereArgs);

            db.close();
            return "Jefe Eliminado";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }
    public String DELETE_EQUIPO(Context _context, String ID)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        try
        {
            //PRIMERO LIMPIAMOS LA TABLA
            String where = "id = "+ID;
            String[] whereArgs = new String[] {};
            db.delete("TEQUIPOS", where, whereArgs);

            db.close();
            return "Equipo Eliminado";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }
    public String DELETE_EQUIPOS(Context _context)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        try
        {
            //PRIMERO LIMPIAMOS LA TABLA
            String where = "";
            String[] whereArgs = new String[] {};
            db.delete("TEQUIPOS", where, whereArgs);

            db.close();
            return "Equipos Eliminados";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }
    public String DELETE_VENTA(Context _context,String ID)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        try
        {
            String where = "ID=?";
            String[] whereArgs = new String[] {ID};
            db.delete("TVENTAS", where, whereArgs);

            db.close();
            return "Venta Eliminada";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }
    public List<HashMap<String,String>> GET_LOGIN(Context _context, String usuario, String password)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        List<HashMap<String,String>> _usuario = new ArrayList<HashMap<String,String>>();
        if(db != null)
        {
            String selectQuery = "SELECT ID, EMAIL, USUARIO, PASSWORD, TIPO FROM TUSUARIOS"
                    + " WHERE (USUARIO = '"+usuario+"' OR EMAIL = '"+usuario+"') AND PASSWORD = '"+password+"'";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{
                    HashMap<String, String> hm = new HashMap<String,String>();
                    int ID = cursor.getInt(0);
                    String NOMBRE = cursor.getString(1);
                    String USUARIO = cursor.getString(2);
                    String PASSWORD = cursor.getString(3);
                    String SUBGERENTE = cursor.getString(4);

                    hm.put("ID", Integer.toString(ID));
                    hm.put("EMAIL", NOMBRE);
                    hm.put("USUARIO", USUARIO);
                    hm.put("PASSWORD", PASSWORD);
                    hm.put("TIPO", SUBGERENTE);

                    _usuario.add(hm);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return _usuario;
        }
        return _usuario;
    }
    public List<HashMap<String,String>> GET_LOGINJEFE(Context _context, String usuario, String password)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        List<HashMap<String,String>> _usuario = new ArrayList<HashMap<String,String>>();
        if(db != null)
        {
            String selectQuery = "SELECT ID, NOMBRE, USUARIO, PASSWORD, SUBGERENTE FROM TJEFES"
                    + " WHERE USUARIO = '"+usuario+"' AND PASSWORD = '"+password+"'";
            Cursor cursor = db.rawQuery(selectQuery, null);

            Log.d("mysql", selectQuery);
            if(cursor.moveToFirst()){
                do{
                    HashMap<String, String> hm = new HashMap<String,String>();
                    int ID = cursor.getInt(0);
                    String NOMBRE = cursor.getString(1);
                    String USUARIO = cursor.getString(2);
                    String PASSWORD = cursor.getString(3);
                    String SUBGERENTE = cursor.getString(4);

                    hm.put("ID", Integer.toString(ID));
                    hm.put("NOMBRE", NOMBRE);
                    hm.put("USUARIO", USUARIO);
                    hm.put("PASSWORD", PASSWORD);
                    hm.put("SUBGERENTE", SUBGERENTE);

                    _usuario.add(hm);

                    Log.d("mysql", PASSWORD);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return _usuario;
        }
        return _usuario;
    }
    public List<Jefe> GET_LISTJEFES(Context _context)
    {
        MySQLiteOpenHelper sqloh =new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        List<Jefe> listJefes = new ArrayList<Jefe>();
        if(db != null)
        {
            String selectQuery = "SELECT ID,NOMBRE,USUARIO,PASSWORD,SUBGERENTE FROM TJEFES";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{
                    int ID = cursor.getInt(0);
                    String NOMBRE = cursor.getString(1);
                    String USUARIO = cursor.getString(2);
                    String PASSWORD = cursor.getString(3);
                    String SUBGERENTE = cursor.getString(4);

                    Jefe jefe = new Jefe(ID, NOMBRE, USUARIO, PASSWORD, SUBGERENTE);

                    listJefes.add(jefe);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return listJefes;
        }
        return listJefes;
    }
    public List<HashMap<String,String>> GET_JEFES(Context _context)
    {
        MySQLiteOpenHelper sqloh =new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        List<HashMap<String,String>> listJefes = new ArrayList<HashMap<String,String>>();
        if(db != null)
        {
            String selectQuery = "SELECT ID,NOMBRE,USUARIO,PASSWORD,SUBGERENTE FROM TJEFES";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{
                    HashMap<String, String> hm = new HashMap<String,String>();
                    int ID = cursor.getInt(0);
                    String NOMBRE = cursor.getString(1);
                    String USUARIO = cursor.getString(2);
                    String PASSWORD = cursor.getString(3);
                    String SUBGERENTE = cursor.getString(4);

                    hm.put("ID", Integer.toString(ID));
                    hm.put("NOMBRE", NOMBRE);
                    hm.put("USUARIO", USUARIO);
                    hm.put("PASSWORD", PASSWORD);
                    hm.put("SUBGERENTE", SUBGERENTE);

                    listJefes.add(hm);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return listJefes;
        }
        return listJefes;
    }
    public List<Equipo> GET_LISTEQUIPOS(Context _context)
    {
        MySQLiteOpenHelper sqloh =new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        List<Equipo> listEquipos = new ArrayList<Equipo>();
        if(db != null)
        {
            String selectQuery = "SELECT ID,CENTRO,PLANTA,CODIGOINTERNO,PLACA,CLASE FROM TEQUIPOS ORDER BY CODIGOINTERNO";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{
                    int ID = cursor.getInt(0);
                    String CENTRO = cursor.getString(1);
                    String PLANTA = cursor.getString(2);
                    String CODIGOINTERNO = cursor.getString(3);
                    String PLACA = cursor.getString(4);
                    String CLASE = cursor.getString(5);

                    Equipo equipo = new Equipo(ID, CENTRO, PLANTA, CODIGOINTERNO, PLACA, CLASE);

                    listEquipos.add(equipo);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return listEquipos;
        }
        return listEquipos;
    }
    public List<HashMap<String,String>> GET_EQUIPOS(Context _context)
    {
        MySQLiteOpenHelper sqloh =new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        List<HashMap<String,String>> ListEquipos = new ArrayList<HashMap<String,String>>();
        if(db != null)
        {
            String selectQuery = "SELECT ID,CENTRO,PLANTA,CODIGOINTERNO,PLACA,CLASE FROM TEQUIPOS ORDER BY CODIGOINTERNO";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{

                    HashMap<String, String> hm = new HashMap<String,String>();
                    int ID = cursor.getInt(0);
                    String check = Integer.toString(R.drawable.ic_menu_send) ;
                    String CODIGOINTERNO = cursor.getString(3);
                    //Log.e("Codigo Interno",CODIGOINTERNO);
                    hm.put("ID", Integer.toString(ID));
                    hm.put("CODIGOINTERNO", CODIGOINTERNO);
                    hm.put("check", check);

                    ListEquipos.add(hm);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return ListEquipos;
        }
        return ListEquipos;
    }
    public List<Venta> GET_VENTAS(Context _context)
    {
        MySQLiteOpenHelper sqloh =new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        List<Venta> ListVentas = new ArrayList<Venta>();
        if(db != null)
        {
            String selectQuery = "SELECT ID,FECHA,PRODUCTO,TICKET,NROGALONES,HOROMETRO,KM,EQUIPO FROM TVENTAS";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{

                    int ID = cursor.getInt(0);
                    String FECHA=cursor.getString(1);
                    String PRODUCTO=cursor.getString(2);
                    String TICKET=cursor.getString(3);
                    String NROGALONES=cursor.getString(4);
                    String HOROMETRO=cursor.getString(5);
                    String KM=cursor.getString(6);
                    String EQUIPO=cursor.getString(7);

                    Venta v=new Venta(ID,FECHA,PRODUCTO,TICKET,NROGALONES,HOROMETRO,KM,EQUIPO);
                    ListVentas.add(v);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return ListVentas;
        }
        return ListVentas;
    }

    public int COUNT_VENTAS(Context _context)
    {
        MySQLiteOpenHelper sqloh =new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        int contador=0;
        if(db != null)
        {
            String selectQuery = "SELECT * FROM TVENTAS";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{

                    contador++;
                }
                while (cursor.moveToNext());
                db.close();
            }
            return contador;
        }
        return contador;
    }

    public int GET_IDTICKET(Context _context)
    {
        MySQLiteOpenHelper sqloh =new MySQLiteOpenHelper(_context, "BD", null, 1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        int ultimoID=0;
        if(db != null)
        {
            String selectQuery = "SELECT ID FROM TTICKETS";
            Cursor cursor = db.rawQuery(selectQuery, null);

            if(cursor.moveToFirst()){
                do{

                    ultimoID=cursor.getInt(0);
                }
                while (cursor.moveToNext());
                db.close();
            }
            return ultimoID+1;
        }
        return ultimoID;
    }
    public String INSERT_TICKET(Context _context)
    {
        MySQLiteOpenHelper sqloh = new MySQLiteOpenHelper(_context,"BD",null,1);
        SQLiteDatabase db = sqloh.getWritableDatabase();
        ContentValues valores = new ContentValues();
        try
        {
            valores.put("OBSERVACION", "NINGUNA");
            db.insert("TTICKETS", "nullColumnHack", valores);

            db.close();
            return "Registro de TICKET OK";
        }
        catch(Exception ex){
            return "Error en el registro de firmas";
        }
    }

}

