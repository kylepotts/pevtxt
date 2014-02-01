package com.example.pebtodotxt;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;

public class GetTodoTasks extends AsyncTask<URL, Integer, Long> {
	private ProgressDialog dialog;
	private Context context;
	


	@Override
	protected Long doInBackground(URL... urls) {
		// TODO Auto-generated method stub
		URL fileURL = urls[0];
		try {
			MainActivity.fileInputStream = fileURL.openStream();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return null;
	}
	
	protected void onPostExecute(Long result) {
		MainActivity.fileDownloaded = true;

    }
	
	 protected void onPreExecute() {
	        //this.dialog.setMessage("Progress start");
	       // this.dialog.show();
	    }

}
