#include <pebble.h>
#include "filter_project.h"
#include "pebbletodoTxt.h"
#include "filter_project_display.h"
#define MAX_ITEMS 7
static Window *window;
static MenuLayer* menu_layer;
AppTimer* appTimer;
static int type;
char strings[8][18];



void timerCallBack(void* data){
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerCall");
	menu_layer_reload_data(menu_layer);



}

void getList(){
		int i;
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "getList type =  %d", type);
    /*
        Before this android we called getData(), so android phone should have set data from AppSync.
        Android App will use indexes 8-15 for all types (Projects,Contexts,Priorites)
        we now need to copy the values of the strings in taskItems into our local variable strings to display them
    */
		for(i=0; i<8; i++){
			if(type == 0){
			strcpy(strings[i], taskItems[i].taskProject);
		}
			if(type == 1){
			strcpy(strings[i],taskItems[i].taskProject);
			}

         }
     		


}







static void draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
  getList();
  menu_cell_basic_draw(ctx, cell_layer, strings[index], NULL, NULL);


 
}

static uint16_t get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return (uint16_t)MAX_ITEMS;
}


static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
  filter_project_display_init(index);

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
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
   getList();
   menu_layer_reload_data(menu_layer);
  char* data = "";
   appTimer = app_timer_register(1000,&timerCallBack,data);



  
  
}

static void window_unload(Window *window) {
}

void filter_project_init(int typeList) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  type = typeList;
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
  menu_layer_destroy(menu_layer);
}



