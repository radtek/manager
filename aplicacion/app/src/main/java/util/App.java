package util;

/**
 * Created by lorda on 7/04/2017.
 */

public class App {

    private static volatile App APP_INSTANCE = new App();

    private static String tipo_usuario = "";

    private static String STR_ADMIN = "Administrador";
    private static String STR_CHOFER = "Chofer";
    private static String STR_JEFE = "Jefe";
    private App(){

    }
    public static App getSingleton(){
        return APP_INSTANCE;
    }
    public static void set_tipo_usuario(String tipo){
        tipo_usuario = tipo;
    }
    public static String get_usuario(){
        return tipo_usuario;
    }
}
