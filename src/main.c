#include "global.h"
#include "accHandlers.h"
#include "animationHandlers.h"

// テキストレイヤを作成
void create_text_layer(){
  // 名言レイや
  text_layer = text_layer_create(GRect(0, 0, 146, 168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);

  // 時間レイヤ
  time_layer = text_layer_create(GRect(0, 53, 146, 84));
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);

  // tap 検知レイヤ
  acc_layer = text_layer_create(GRect(0, 66, 146, 168));
  text_layer_set_background_color(acc_layer, GColorClear);
  text_layer_set_text_color(acc_layer, GColorBlack);

  // acc raw　レイヤ
  acc_raw_layer = text_layer_create(GRect(0, 80, 146, 168));
  text_layer_set_background_color(acc_raw_layer, GColorClear);
  text_layer_set_text_color(acc_raw_layer, GColorBlack);
}

void set_text_layer(){
  text_layer_set_text(text_layer, "Divide each difficulty into as many parts as is feasible and necessary to resolve it.");
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  // update window
  strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);

  int seconds = tick_time->tm_sec;

  if(seconds == 59){
    //Slide offscreen to the right
    GRect start = GRect(0, 53, 144, 168);
    GRect finish = GRect(144, 53, 144, 168);
    animate_layer(text_layer_get_layer(time_layer), &start, &finish, 300, 500);
  }else if(seconds == 0){
    //Slide offscreen to the left
    GRect start = GRect(-144, 53, 144, 168);
    GRect finish = GRect(0, 53, 144, 168);
    animate_layer(text_layer_get_layer(time_layer), &start, &finish, 300, 500);
  }else{
    text_layer_set_text(time_layer, buffer);    
  }
  text_layer_set_text(acc_layer, "");
}

void init_time(){
  struct tm *t;
  time_t temp;
  temp = time(NULL);
  t = localtime(&temp);

  tick_handler(t, SECOND_UNIT);
}

// init window
void window_load(Window *window){
  create_text_layer();
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(acc_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(acc_raw_layer));
  
  set_text_layer();

  init_time();
}

// deinit window
void window_unload(Window *window){
  text_layer_destroy(text_layer);
  text_layer_destroy(time_layer);
  text_layer_destroy(acc_layer);
  text_layer_destroy(acc_raw_layer);
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

  accel_tap_service_subscribe(accel_tap_handler);
  accel_data_service_subscribe(1, accel_raw_handler);
}
 
void deinit()
{
  //De-initialize elements here to save memory!
  window_destroy(window);

  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
  accel_data_service_unsubscribe();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
