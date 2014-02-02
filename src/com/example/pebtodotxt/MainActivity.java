package com.example.pebtodotxt;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.UUID;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.dropbox.sync.android.DbxAccountManager;
import com.dropbox.sync.android.DbxException;
import com.dropbox.sync.android.DbxFile;
import com.dropbox.sync.android.DbxPath;
import com.dropbox.sync.android.DbxException.Unauthorized;
import com.dropbox.sync.android.DbxFileSystem;
import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Process;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;

public class MainActivity extends Activity {
	final static UUID PEBBLE_APP_UUID = UUID.fromString("0d77fef2-cf55-461f-9f4b-bdc70db0d0e7");
	private Handler handler;
	private PebbleKit.PebbleDataReceiver dataReceiver;
	private PebbleKit.PebbleAckReceiver ackReceiver;
	private PebbleKit.PebbleNackReceiver nackReceiver;
	static final int REQUEST_LINK_TO_DBX = 0;  // You can change this if needed
	private DbxAccountManager mDbxAcctMgr;
	static InputStream fileInputStream ;
	static boolean fileDownloaded = false;
	private GetTodoTasks tdTask;
	private String todoDownloadURL;
	static ArrayList<ToDoTask> todoTasks;
	static ArrayList<String> listOfProjects;
	static ArrayList<String> listOfContexts;
	private boolean acked = false;
	


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		boolean connected = PebbleKit.isWatchConnected(getApplicationContext());
		Log.i(getLocalClassName(), "Pebble is " + (connected ? "connected" : "not connected"));
		todoTasks = new ArrayList<ToDoTask>();
		listOfProjects = new ArrayList<String>();
		listOfContexts = new ArrayList<String>();
		dataReceiver = new PebbleKit.PebbleDataReceiver(PEBBLE_APP_UUID) {
		    @Override
			public void receiveData(Context context, int transactionId,
					PebbleDictionary data) {
				// TODO Auto-generated method
		    	String s = data.getString(40);
		    	if(s.equals("get_project_list")){
		    		Log.d("ask", "asking for project list");
		    		getListOfProjects();
		    		new GetProjectsTask(getApplicationContext()).execute();
		    		
		    	}
		    	
		    	if(s.equals("get_contexts_list")){
		    		Log.d("ask", "asking for contexts list");
		    		getListOfContexts();
		    		new GetContextsTask(getApplicationContext()).execute();
		    		
		    		
		    	}
		    	
		    	if(s.equals("reset")){
		    		
		    		;
		    	}
				PebbleKit.sendAckToPebble(getApplicationContext(), transactionId);
				
			}
		};
		
		ackReceiver = new PebbleKit.PebbleAckReceiver(PEBBLE_APP_UUID) {
			@Override
			public void receiveAck(Context context, int transactionId) {
				// TODO Auto-generated method stub
			    Log.i(getLocalClassName(), "Received ack for transaction " + transactionId);
			    acked = true;
				
			}
		};
		
		nackReceiver = new PebbleKit.PebbleNackReceiver(PEBBLE_APP_UUID) {
			@Override
			public void receiveNack(Context context, int transactionId) {
				// TODO Auto-generated method stub
				Log.i(getLocalClassName(), "Received nack for transaction " + transactionId);
				
			}
		};
		
		PebbleKit.registerReceivedDataHandler(getApplicationContext(), dataReceiver);
		PebbleKit.registerReceivedAckHandler(getApplicationContext(), ackReceiver);
		PebbleKit.registerReceivedNackHandler(getApplicationContext(), nackReceiver);
		
		 mDbxAcctMgr = DbxAccountManager.getInstance(getApplicationContext(),"dhl4f4vxcwutqhu", "8j6057hwvkm79k7");
		 Log.d("debug", mDbxAcctMgr.toString());
		 PebbleKit.startAppOnPebble(getApplicationContext(), PEBBLE_APP_UUID);
		 View v = null;
		// opendbClick(v);
		 //sendData(v);
		

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public void sendData(View v){
		PebbleDictionary data = new PebbleDictionary();
		Log.d("size", ""+todoTasks.size());
		//data.addUint32(0, todoTasks.size());
		for(int i=0; i<todoTasks.size(); i++){
			ToDoTask td = todoTasks.get(i);
			String task = td.getTaskString();
			if(td.getPriority() != null){
				task = task.replaceAll("[(][A-Z][)]", "");
			}
			if(td.getDate() != null){
				task = task.replaceAll("\\d{4}\\-\\d{2}\\-\\d{2}", "");
			}
			
			if(td.getContext() != null){
				task = task.replaceAll("\\s[@].*", "");
			}
			
			if(td.getProject() != null){
				task = task.replaceAll("\\s[+].*", "");
			}
			if(task.length() >= 13){
				task = task.substring(0,17);
			}
			Log.d("length of string", task+" "+task.length());
			data.addString(i, task);
		}
		

		Log.d("data", data.toJsonString().toString());
		PebbleKit.sendDataToPebble(getApplicationContext(), PEBBLE_APP_UUID, data);
		
		
	}
	
	public void loadData(View v){
		if(mDbxAcctMgr.hasLinkedAccount()){
			try {
				handleDBXFiles();
			} catch (DbxException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	
	public void opendbClick(View v){
         if(!mDbxAcctMgr.hasLinkedAccount()){
		mDbxAcctMgr.startLink((Activity)this, REQUEST_LINK_TO_DBX);
         }
		/*
		try {
			handleDBXFiles();
		} catch (DbxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		*/
		
		
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
	    if (requestCode == REQUEST_LINK_TO_DBX) {
	        if (resultCode == Activity.RESULT_OK) {
					try {
						handleDBXFiles();
					} catch (DbxException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}


	        } else {
	        	
	            // Failed or was cancelled by the user.
	        }
	    } else {
	        super.onActivityResult(requestCode, resultCode, data);
	    }
	}
	
	public void readToDo() throws Exception{
			ProgressDialog dialog = new ProgressDialog(MainActivity.this);
			dialog.setMessage("Reading ToDo");
			dialog.show();
			Thread.sleep(4000);
			dialog.dismiss();
			Log.d("read", "fileDownload = " + fileDownloaded);
			Scanner in = new Scanner(fileInputStream);
			while(in.hasNextLine()){
			String s1 = in.nextLine();
			Log.d("task", s1);
			todoTasks.add(new ToDoTask(s1));
			}
	}
	
	public void handleDBXFiles() throws DbxException{
		Log.d("debug", DbxFileSystem.forAccount(mDbxAcctMgr.getLinkedAccount()).toString());
		DbxFileSystem dbxFs = DbxFileSystem.forAccount(mDbxAcctMgr.getLinkedAccount());
		DbxPath path = new DbxPath("/todo/todo.txt");
		DbxFile todoFile = dbxFs.open(path);
		FileInputStream fis = null;
		try {
			 fis = todoFile.getReadStream();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Scanner in = new Scanner(fis);
		while(in.hasNextLine()){
			String task = in.nextLine();
			todoTasks.add(new ToDoTask(task));
		}
		todoFile.close();
		
	}
	
	
	public void getListOfProjects(){
		for(int i=0; i<8; i++){
			if(!(i>=todoTasks.size())){
			String task = todoTasks.get(i).getProject();
			Log.d("listProj", task);
				if(!todoTasks.contains(task)){
				listOfProjects.add(task);
				}
			}
				else if(i >= todoTasks.size()){
					listOfProjects.add(" ");
				}
			
		}
		
	}
	
	public void getListOfContexts(){
		for(int i=0; i<8; i++){
			if(!(i>=todoTasks.size())){
			if(todoTasks.get(i).getContext() != null){
			String context = todoTasks.get(i).getContext();
			if(!(listOfContexts.contains(context)))
			listOfContexts.add(context);
			}
		}
			else if (i>= todoTasks.size()){
				listOfContexts.add(" ");
			}
		}
	}
	

	

	
	

}
