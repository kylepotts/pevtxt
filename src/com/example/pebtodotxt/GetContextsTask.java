package com.example.pebtodotxt;

import java.net.URL;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

public class GetContextsTask extends AsyncTask<URL, Integer, Long> {
	private Context context;
	
	public GetContextsTask(Context context){
		this.context = context;
	}

	@Override
	protected Long doInBackground(URL... params) {
		// TODO Auto-generated method stub
		PebbleDictionary dict = new PebbleDictionary();
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	  	  for(int i=8; i<8+MainActivity.listOfContexts.size(); i++){
	  		  Log.d("adding-context", "adding"+MainActivity.listOfContexts.get(i-8) + "with key " + ""+i);
	  		  dict.addString(i, MainActivity.listOfContexts.get(i-8));
	  		  
	  	  }
	  	Log.d("json-contexts",dict.toJsonString().toString());
	  	PebbleKit.sendDataToPebble(context, MainActivity.PEBBLE_APP_UUID, dict);
	  	MainActivity.listOfContexts.clear();
		return null;
	}

}