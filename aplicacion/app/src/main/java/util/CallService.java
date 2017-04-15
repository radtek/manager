package util;

import android.os.AsyncTask;
/**
 * Created by ledys on 21/08/2016.
 */
public abstract class CallService extends AsyncTask<String, Void, Object> {

    @Override
    protected abstract Object doInBackground(String... args) ;
    @Override
    protected abstract void onPostExecute(Object result);
}
