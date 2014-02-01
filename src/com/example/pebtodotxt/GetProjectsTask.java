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
	

	  	  for(int i=8; i<8+MainActivity.listOfProjects.size(); i++){
	  		  Log.d("adding", "adding"+MainActivity.listOfProjects.get(i-8) + "with key " + ""+i);
	  		  dict.addString(i, MainActivity.listOfProjects.get(i-8));
	  		  
	  	  }
	  	Log.d("json-proj",dict.toJsonString().toString());
	  	PebbleKit.sendDataToPebble(context, MainActivity.PEBBLE_APP_UUID, dict);
	  	MainActivity.listOfProjects.clear();
		return null;
	}

}
