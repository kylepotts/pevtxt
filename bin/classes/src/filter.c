#include <pebble.h>
#include "filter.h"
#include "filter_project.h"
#include "pebbletodoTxt.h"
#define MAX_ITEMS 3
static Window *window;
static MenuLayer* menu_layer;


static void draw_row_callback(GContext* ctx, Layer *cell_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
  if(index == 0){
    menu_cell_basic_draw(ctx,cell_layer, "Filter By:", NULL, NULL);  

  }

  if(index == 1){
  
     menu_cell_basic_draw(ctx, cell_layer, "Projects", NULL, NULL);
	
  
  }

  if(index == 2){
  menu_cell_basic_draw(ctx, cell_layer, "Context", NULL, NULL);
  
  }
  //menu_cell_basic_draw(ctx, cell_layer, task, NULL, NULL);
}

static uint16_t get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return (uint16_t)MAX_ITEMS;
}


static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  const int index = cell_index->row;
  if(index == 0){
    return;
  }

  if(index == 1){
    getData("get_project_list");
    filter_project_init(index-1);
    
	
  
  }

  if(index == 2){
    getData("get_contexts_list");
    filter_project_init(index-1);
    
    //context_project_init();
  
  }
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
  
}

 void filter_init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
  menu_layer_destroy(menu_layer);
}







