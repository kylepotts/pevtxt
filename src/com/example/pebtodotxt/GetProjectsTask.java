package com.example.pebtodotxt;

import java.net.URL;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

public class GetProjectsTask extends AsyncTask<URL, Integer, Long> {
	private Context context;
	
	public GetProjectsTask(Context context){
		this.context = context;
	}

	@Override
	protected Long doInBackground(URL... params) {
		// TODO Auto-generated method stub
		PebbleDictionary dict = new PebbleDictionary();
		Log.d("size of proj", ""+MainActivity.listOfProjects.size());
		dict.addInt32(16, MainActivity.listOfProjects.size());
		Log.d("add", dict.toJsonString().toString());
	  	  for(int i=8; i<12; i++){
	  		  Log.d("adding", "adding"+MainActivity.listOfProjects.get(i-8) + "with key " + ""+i);
	  		  dict.addString(i, MainActivity.listOfProjects.get(i-8));
	  		  
	  	  }
	  	 Log.d("json_proj", dict.toJsonString().toString());
	  	PebbleKit.sendDataToPebble(context, MainActivity.PEBBLE_APP_UUID, dict);
	  	try {
			Thread.sleep(4000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	  	PebbleDictionary dict2 = new PebbleDictionary();
	  	
	  	for(int j=12; j<16; j++){
	  		dict2.addString(j, MainActivity.listOfProjects.get(j-8));
	  	}
	  	 Log.d("json_proj", dict2.toJsonString().toString());
	  	PebbleKit.sendDataToPebble(context, MainActivity.PEBBLE_APP_UUID, dict2);
	  	
	  	MainActivity.listOfProjects.clear();
		return null;
	}

}
