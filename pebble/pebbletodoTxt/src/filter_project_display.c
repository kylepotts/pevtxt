#include <pebble.h>
#include "filter_project.h"
#include "pebbletodoTxt.h"
#define MAX_ITEMS 7
static Window *window;
static MenuLayer* menu_layer;

char strings [8][18];

int current = 0;



enum TaskKey {
  proj0 = 0x8,
  proj1 = 0x9,  // TUPLE_CSTRING
  proj2 = 0xA,
  proj3 = 0xB,
  proj4 = 0xC,
  proj5 = 0xD,
  proj6 = 0xE,
  proj7 = 0xF,

};

void getListOfTasks(char* task){
	int i;
	for(i=0; i<8; i++){
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Comparing %s and %s", task, taskItems[i].taskProject);
		if(strcmp(task,taskItems[i].taskProject) == 0){
			strcpy(strings[current],taskItems[i].taskString);
			current++;
			

		}
	}
	
}










static void draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
  menu_cell_basic_draw(ctx, cell_layer, strings[index], NULL, NULL);


 
}

static uint16_t get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return (uint16_t)MAX_ITEMS;
}


static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
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

  menu_layer_set_click_config_onto_window(menu_layer, window);
  menu_layer_reload_data(menu_layer);
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));



  
  
}

static void window_unload(Window *window) {
	int i;
	for(i=0; i<8; i++){
		strcpy(strings[i]," ");
		current = 0;
	}
  
}

void filter_project_display_init(int index) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  getListOfTasks(taskItems[index].taskProject);
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
  menu_layer_destroy(menu_layer);
}

	

	

