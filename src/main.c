#include <pebble.h>

// ウィンドウ構造体
Window *window;

// テキストレイヤ
TextLayer *text_layer;
TextLayer *time_layer;
TextLayer *acc_layer;

// 時間を保持しておくbuffer
char buffer[] = "00:00:00";
char* axis_direction;
int32_t x_direction = 0;
int32_t y_direction = 0;
int32_t z_direction = 0;

// テキストレイヤを作成
void create_text_layer(){
  // 名言レイや
  text_layer = text_layer_create(GRect(0, 0, 146, 168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);

  // 時間レイや
  time_layer = text_layer_create(GRect(0, 53, 146, 84));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);

  // 時間レイや
  acc_layer = text_layer_create(GRect(0, 100, 146, 168));
  text_layer_set_background_color(acc_layer, GColorClear);
  text_layer_set_text_color(acc_layer, GColorBlack);
}

void set_text_layer(){
  text_layer_set_text(text_layer, "Divide each difficulty into as many parts as is feasible and necessary to resolve it.");

  text_layer_set_text(acc_layer, "hoge");
}

// アニメーションが停止した際のハンドラ
void on_animation_stopped(Animation *anim, bool finished, void *context){
  // メモリ解放
  property_animation_destroy((PropertyAnimation*) anim);
}

// アニメーションレイヤ
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay){
  // アニメーション構造体の宣言
  PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);

  // アニメの特徴を設定
  animation_set_duration((Animation*)anim, duration);
  animation_set_delay((Animation*)anim, delay);

  // アニメが停止した際にメモリを解放するためのハンドラを設定
  AnimationHandlers handlers = {
    .stopped = (AnimationStoppedHandler) on_animation_stopped
  };
  animation_set_handlers((Animation*)anim, handlers, NULL);

  // アニメーション開始
  animation_schedule((Animation*)anim);
}


char *itoa(int32_t num)
{
  static char buff[20] = {};
  int i = 0, temp_num = num, digits = 0;

  if (num < 0){
    num *= -1;
  }

  char *string = buff;

  while(temp_num){
    temp_num /= 10;
    digits++;
  }

  for (i = 0; i < digits; i++){
    buff[digits - 1 - i] = '0' + (num % 10);
    num /= 10;
  }
  buff[i] = '\0';

  return string;
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction){
  switch(axis){
  case ACCEL_AXIS_X:
    x_direction = direction;
    break;
  case ACCEL_AXIS_Y:
    y_direction = direction;
    break;
  case ACCEL_AXIS_Z:
    z_direction = direction;
    break;
  }

  char* x = itoa(x_direction);
  char* y = itoa(y_direction);
  char* z = itoa(z_direction);

  axis_direction = "";

  strcat(axis_direction, x);
  strcat(axis_direction, y);
  strcat(axis_direction, z);

  text_layer_set_text(acc_layer, "tapped");
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
  
  set_text_layer();

  init_time();
}

// deinit window
void window_unload(Window *window){
  text_layer_destroy(text_layer);
  text_layer_destroy(time_layer);
  text_layer_destroy(acc_layer);
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
}
 
void deinit()
{
  //De-initialize elements here to save memory!
  window_destroy(window);

  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
