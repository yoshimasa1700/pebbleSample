#include "global.h"

#ifndef ANIMATIONHANDLERS_H
#define ANIMATIONHANDLERS_H

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

#endif
