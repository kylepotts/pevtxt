#include <pebble.h>
#include "filter.h"
#include "pebbletodoTxt.h"
#define MAX_ITEMS 8
static  char* todoTexts[MAX_ITEMS];
static Tuple* tuples[MAX_ITEMS];

static Window *window;
static MenuLayer* menu_layer;
static TextLayer *text_layer;

static AppSync sync;
static uint8_t sync_buffer[256];

// These are all temp variables, used to copy into the taskItems struct
char tasks[8][18];
char projects[8][18];
char contexts[8][18];
int32_t size;

// Struct containing all string needed for displaying tasks and surrounding meta data.
taskItem taskItems[8];

enum TaskKey {
  task0 = 0x0,  
  task1 = 0x1, 
  task2 = 0x2,
  task3 = 0x3,
  task4 = 0x4,
  task5 = 0x5,
  task6 = 0x6,
  task7 = 0x7,
  proj0 = 0x8,
  proj1 = 0x9, 
  proj2 = 0xA,
  proj3 = 0xB,
  proj4 = 0xC,
  proj5 = 0xD,
  proj6 = 0xE,
  proj7 = 0xF,
  tsize  = 0X10

};


void getData(char* string){
 DictionaryIterator *iter;
 app_message_outbox_begin(&iter);
 Tuplet value = TupletCString(40, string);
 dict_write_tuplet(iter, &value);
  app_message_outbox_send();

}

static void app_message_init(){
  app_comm_set_sniff_interval(SNIFF_INTERVAL_REDUCED);
  // Init buffers
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  // Register message handlers

}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}



static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
	
  switch (key) {
    case task0:
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Task: %s", new_tuple->value->cstring);
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
    //persist_write_string(0,new_tuple->value->cstring);
	break;
    case task1:
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
   // persist_write_string(1,new_tuple->value->cstring);
	break;
    case task2:
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
    //persist_write_string(2,new_tuple->value->cstring); 
	break;
    case task3:
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
   // persist_write_string(3,new_tuple->value->cstring);
	break;
    case task4:
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
    //persist_write_string(4,new_tuple->value->cstring);
	break;
    case task5:
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
   // persist_write_string(5,new_tuple->value->cstring);
	break;
    case task6:
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
   // persist_write_string(6,new_tuple->value->cstring);
	break;
    case task7:
	strcpy(tasks[key],new_tuple->value->cstring);
	memcpy(taskItems[key].taskString,tasks[key],sizeof(tasks[key]));
    //persist_write_string(7,new_tuple->value->cstring);
	break;

  case proj0:
   APP_LOG(APP_LOG_LEVEL_DEBUG, "refresh!  %d", (int)key);
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
    //persist_write_string(proj0,new_tuple->value->cstring);
	break;
    case proj1:
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
   // persist_write_string(proj1,new_tuple->value->cstring);
	break;
    case proj2:
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
   // persist_write_string(proj2,new_tuple->value->cstring); 
	break;
    case proj3:
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
   // persist_write_string(proj3,new_tuple->value->cstring);
	break;
    case proj4:
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
   // persist_write_string(proj4,new_tuple->value->cstring);
	break;
    case proj5:
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
    //persist_write_string(proj5,new_tuple->value->cstring);
	break;
    case proj6:
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
    //persist_write_string(proj6,new_tuple->value->cstring);
	break;
    case proj7:
	strcpy(projects[key-8],new_tuple->value->cstring);
	memcpy(taskItems[key-8].taskProject,projects[key-8],sizeof(projects[key-8]));
   // persist_write_string(proj7,new_tuple->value->cstring);
	break;
    case tsize:
    size = new_tuple->value->int32;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Size update to: %d", (int)size);
    break;




                }
  menu_layer_reload_data(menu_layer);

}


static void draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
  
  menu_cell_basic_draw(ctx, cell_layer, tasks[index], NULL, NULL);
}

static uint16_t get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return (uint16_t)MAX_ITEMS;
}


static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
  filter_init();
}

static void select_long_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  return 44;
}







static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_frame = layer_get_frame(window_layer);
  menu_layer = menu_layer_create(window_frame);
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
    .get_cell_height = (MenuLayerGetCellHeightCallback) get_cell_height_callback,
    .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) get_num_rows_callback,
    .select_click = (MenuLayerSelectCallback) select_callback,
    .select_long_click = (MenuLayerSelectCallback) select_long_callback
  });



    int i;
    for(i=0; i<8; i++){
        persist_read_string(i+30,tasks[i],sizeof(tasks[i]));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Reading String %s to storage with id %d", tasks[i],i+30);
    
    }

	
	
	



 Tuplet initial_values[] = {
	TupletCString(task0,tasks[0]),
	TupletCString(task1,tasks[1]),
	TupletCString(task2,tasks[2]),
	TupletCString(task3,tasks[3]),
	TupletCString(task4,tasks[4]),
	TupletCString(task5,tasks[5]),
	TupletCString(task6,tasks[6]),
	TupletCString(task7,tasks[7]),
	TupletCString(proj0,projects[0]),
	TupletCString(proj1,projects[1]),
	TupletCString(proj2,projects[2]),
	TupletCString(proj3,projects[3]),
	TupletCString(proj4,projects[4]),
	TupletCString(proj5,projects[5]),
	TupletCString(proj6,projects[6]),
	TupletCString(proj7,projects[7]),
  TupletInteger(tsize,0),



  };

app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  menu_layer_set_click_config_onto_window(menu_layer, window);
  menu_layer_reload_data(menu_layer);
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
  menu_layer_reload_data(menu_layer);
  
  
}

static void window_unload(Window *window) {
  app_sync_deinit(&sync);
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  app_message_init();

  window_stack_push(window, animated);
}

static void deinit(void) {
 int i;
	for(i=0; i<8;i++){
		persist_write_string(i+30,tasks[i]);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Writing String %s to storage with id %d", tasks[i],i+30);


	}
  window_destroy(window);
  menu_layer_destroy(menu_layer);
}






int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  deinit();
}
