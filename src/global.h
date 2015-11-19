#include <pebble.h>

#ifndef GLOBAL_H
#define GLOBAL_H

// ウィンドウ構造体
Window *window;

// テキストレイヤ
TextLayer *text_layer;
TextLayer *time_layer;
TextLayer *acc_layer;
TextLayer *acc_raw_layer;

// 時間を保持しておくbuffer
char buffer[] = "00:00:00";

#endif
