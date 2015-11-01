#include <pebble.h>

// ウィンドウ構造体
Window *window;

// テキストレイヤ
TextLayer *text_layer;
TextLayer *time_layer;

// 時間を保持しておくbuffer
char buffer[] = "00:00:00";

void create_text_layer(){
  text_layer = text_layer_create(GRect(0, 0, 146, 168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);

  time_layer = text_layer_create(GRect(0, 84, 146, 84));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
}

void set_text_layer(){
  text_layer_set_text(text_layer, "Divide each difficulty into as many parts as is feasible and necessary to resolve it.");
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  // update window
  strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);

  text_layer_set_text(time_layer, buffer);
}

// init window
void window_load(Window *window){
  create_text_layer();
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  
  set_text_layer();
}

// deinit window
void window_unload(Window *window){
  text_layer_destroy(text_layer);
}

void init()
{
  //Initialize the app elements here!
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  window_stack_push(window, true);

  tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
}
 
void deinit()
{
  //De-initialize elements here to save memory!
  window_destroy(window);

  tick_timer_service_unsubscribe();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
