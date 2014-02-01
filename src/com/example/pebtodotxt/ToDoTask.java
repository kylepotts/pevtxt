package com.example.pebtodotxt;

import java.util.regex.*;

import android.util.Log;

public class ToDoTask {
	private String priorityPattern = "[(][A-Z][)]";
	private String datePattern ="\\d{4}\\-\\d{2}\\-\\d{2}";
	private String projectPattern = "\\s[+].*";
	private String contextPattern = "\\s[@].*";
	Pattern pattern;
	private String taskString;
	private String dateString;
	private String projectString;
	private String contextString;
	private String priorityString;
	
	
	public ToDoTask(String s){
		taskString = s;
		findPriority();
		findDate();
		findContext();
		findProject();
		if(contextString != null){
			Log.d("replacing", projectString);
			String[] split = projectString.split("\\s");
			projectString = split[1];
			
		
		}
	}
	
	public void findPriority(){
		if(taskString == null)
			return;
		pattern = Pattern.compile(priorityPattern);
		Matcher m = pattern.matcher(taskString);
		if(m.find()){
		priorityString = m.group();
		}
		
	}
	
	public void findDate(){
		pattern = Pattern.compile(datePattern);
		Matcher m = pattern.matcher(taskString);
		if(m.find()){
			dateString = m.group();
		}
	}
	
	public void findProject(){
		pattern = Pattern.compile(projectPattern);
		Matcher m = pattern.matcher(taskString);
		if(m.find()){
			projectString = m.group();
		}
	}
	
	public void findContext(){
		pattern = Pattern.compile(contextPattern);
		Matcher m = pattern.matcher(taskString);
		if(m.find()){
			Log.d("context!", m.group());
			contextString = m.group();
			
		}
	}
	
	public String getTaskString(){
		return taskString;
	}
	
	public String getDate(){
		return dateString;
	}
	
	public String getProject(){
		return projectString;
	}
	
	public String getContext(){
		return contextString;
	}
	
	public String getPriority(){
		return priorityString;
	}
	
	
	

}
