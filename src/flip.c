#include <pebble.h>
#include "flip_layer.h"
#include "autoconfig.h"

#define TEMPERATURE_KEY 0
#define WEATHERCODE_KEY 1
#define CITYNAME_KEY    2

static Window *window;

static AppTimer *weather_timer;

static Layer *top_layer;
static Layer *bottom_layer;

static BitmapLayer *battery_layer;
static GBitmap *battery_image;

static BitmapLayer *bluetooth_layer;
static GBitmap *bluetooth_image;

static BitmapLayer *weather_layer;
static GBitmap *weather_image;

static TextLayer *text_city_layer;
static char city_text[15] = "";

static TextLayer *text_date_layer;
static char date_text[] = "Xxxxxxxxx 00";

static TextLayer *text_temp_layer;
static char temp_text[6] = "";

#define LAYER_NUMBER 2
static FlipLayer *layer_number[LAYER_NUMBER];

#define NUMBER_IMAGE_COUNT 60

int NUMBER_IMAGE_RESOURCE_UP_IDS[NUMBER_IMAGE_COUNT] = {
  RESOURCE_ID_IMAGE_0_UP,
  RESOURCE_ID_IMAGE_1_UP,
  RESOURCE_ID_IMAGE_2_UP,
  RESOURCE_ID_IMAGE_3_UP,
  RESOURCE_ID_IMAGE_4_UP,
  RESOURCE_ID_IMAGE_5_UP,
  RESOURCE_ID_IMAGE_6_UP,
  RESOURCE_ID_IMAGE_7_UP,
  RESOURCE_ID_IMAGE_8_UP,
  RESOURCE_ID_IMAGE_9_UP,
  RESOURCE_ID_IMAGE_10_UP,
  RESOURCE_ID_IMAGE_11_UP,
  RESOURCE_ID_IMAGE_12_UP,
  RESOURCE_ID_IMAGE_13_UP,
  RESOURCE_ID_IMAGE_14_UP,
  RESOURCE_ID_IMAGE_15_UP,
  RESOURCE_ID_IMAGE_16_UP,
  RESOURCE_ID_IMAGE_17_UP,
  RESOURCE_ID_IMAGE_18_UP,
  RESOURCE_ID_IMAGE_19_UP,
  RESOURCE_ID_IMAGE_20_UP,
  RESOURCE_ID_IMAGE_21_UP,
  RESOURCE_ID_IMAGE_22_UP,
  RESOURCE_ID_IMAGE_23_UP,
  RESOURCE_ID_IMAGE_24_UP,
  RESOURCE_ID_IMAGE_25_UP,
  RESOURCE_ID_IMAGE_26_UP,
  RESOURCE_ID_IMAGE_27_UP,
  RESOURCE_ID_IMAGE_28_UP,
  RESOURCE_ID_IMAGE_29_UP,
  RESOURCE_ID_IMAGE_30_UP,
  RESOURCE_ID_IMAGE_31_UP,
  RESOURCE_ID_IMAGE_32_UP,
  RESOURCE_ID_IMAGE_33_UP,
  RESOURCE_ID_IMAGE_34_UP,
  RESOURCE_ID_IMAGE_35_UP,
  RESOURCE_ID_IMAGE_36_UP,
  RESOURCE_ID_IMAGE_37_UP,
  RESOURCE_ID_IMAGE_38_UP,
  RESOURCE_ID_IMAGE_39_UP,
  RESOURCE_ID_IMAGE_40_UP,
  RESOURCE_ID_IMAGE_41_UP,
  RESOURCE_ID_IMAGE_42_UP,
  RESOURCE_ID_IMAGE_43_UP,
  RESOURCE_ID_IMAGE_44_UP,
  RESOURCE_ID_IMAGE_45_UP,
  RESOURCE_ID_IMAGE_46_UP,
  RESOURCE_ID_IMAGE_47_UP,
  RESOURCE_ID_IMAGE_48_UP,
  RESOURCE_ID_IMAGE_49_UP,
  RESOURCE_ID_IMAGE_50_UP,
  RESOURCE_ID_IMAGE_51_UP,
  RESOURCE_ID_IMAGE_52_UP,
  RESOURCE_ID_IMAGE_53_UP,
  RESOURCE_ID_IMAGE_54_UP,
  RESOURCE_ID_IMAGE_55_UP,
  RESOURCE_ID_IMAGE_56_UP,
  RESOURCE_ID_IMAGE_57_UP,
  RESOURCE_ID_IMAGE_58_UP,
  RESOURCE_ID_IMAGE_59_UP,
};

int NUMBER_IMAGE_RESOURCE_DOWN_IDS[NUMBER_IMAGE_COUNT] = {
  RESOURCE_ID_IMAGE_0_DOWN,
  RESOURCE_ID_IMAGE_1_DOWN,
  RESOURCE_ID_IMAGE_2_DOWN,
  RESOURCE_ID_IMAGE_3_DOWN,
  RESOURCE_ID_IMAGE_4_DOWN,
  RESOURCE_ID_IMAGE_5_DOWN,
  RESOURCE_ID_IMAGE_6_DOWN,
  RESOURCE_ID_IMAGE_7_DOWN,
  RESOURCE_ID_IMAGE_8_DOWN,
  RESOURCE_ID_IMAGE_9_DOWN,
  RESOURCE_ID_IMAGE_10_DOWN,
  RESOURCE_ID_IMAGE_11_DOWN,
  RESOURCE_ID_IMAGE_12_DOWN,
  RESOURCE_ID_IMAGE_13_DOWN,
  RESOURCE_ID_IMAGE_14_DOWN,
  RESOURCE_ID_IMAGE_15_DOWN,
  RESOURCE_ID_IMAGE_16_DOWN,
  RESOURCE_ID_IMAGE_17_DOWN,
  RESOURCE_ID_IMAGE_18_DOWN,
  RESOURCE_ID_IMAGE_19_DOWN,
  RESOURCE_ID_IMAGE_20_DOWN,
  RESOURCE_ID_IMAGE_21_DOWN,
  RESOURCE_ID_IMAGE_22_DOWN,
  RESOURCE_ID_IMAGE_23_DOWN,
  RESOURCE_ID_IMAGE_24_DOWN,
  RESOURCE_ID_IMAGE_25_DOWN,
  RESOURCE_ID_IMAGE_26_DOWN,
  RESOURCE_ID_IMAGE_27_DOWN,
  RESOURCE_ID_IMAGE_28_DOWN,
  RESOURCE_ID_IMAGE_29_DOWN,
  RESOURCE_ID_IMAGE_30_DOWN,
  RESOURCE_ID_IMAGE_31_DOWN,
  RESOURCE_ID_IMAGE_32_DOWN,
  RESOURCE_ID_IMAGE_33_DOWN,
  RESOURCE_ID_IMAGE_34_DOWN,
  RESOURCE_ID_IMAGE_35_DOWN,
  RESOURCE_ID_IMAGE_36_DOWN,
  RESOURCE_ID_IMAGE_37_DOWN,
  RESOURCE_ID_IMAGE_38_DOWN,
  RESOURCE_ID_IMAGE_39_DOWN,
  RESOURCE_ID_IMAGE_40_DOWN,
  RESOURCE_ID_IMAGE_41_DOWN,
  RESOURCE_ID_IMAGE_42_DOWN,
  RESOURCE_ID_IMAGE_43_DOWN,
  RESOURCE_ID_IMAGE_44_DOWN,
  RESOURCE_ID_IMAGE_45_DOWN,
  RESOURCE_ID_IMAGE_46_DOWN,
  RESOURCE_ID_IMAGE_47_DOWN,
  RESOURCE_ID_IMAGE_48_DOWN,
  RESOURCE_ID_IMAGE_49_DOWN,
  RESOURCE_ID_IMAGE_50_DOWN,
  RESOURCE_ID_IMAGE_51_DOWN,
  RESOURCE_ID_IMAGE_52_DOWN,
  RESOURCE_ID_IMAGE_53_DOWN,
  RESOURCE_ID_IMAGE_54_DOWN,
  RESOURCE_ID_IMAGE_55_DOWN,
  RESOURCE_ID_IMAGE_56_DOWN,
  RESOURCE_ID_IMAGE_57_DOWN,
  RESOURCE_ID_IMAGE_58_DOWN,
  RESOURCE_ID_IMAGE_59_DOWN,
};

#define WEATHER_IMAGE_COUNT 48
int WEATHER_IMAGE_RESOURCE[NUMBER_IMAGE_COUNT] = {
  RESOURCE_ID_IMAGE_BW_0,
  RESOURCE_ID_IMAGE_BW_0,
  RESOURCE_ID_IMAGE_BW_0,
  RESOURCE_ID_IMAGE_BW_3,
  RESOURCE_ID_IMAGE_BW_3,
  RESOURCE_ID_IMAGE_BW_5,
  RESOURCE_ID_IMAGE_BW_6,
  RESOURCE_ID_IMAGE_BW_6,
  RESOURCE_ID_IMAGE_BW_8,
  RESOURCE_ID_IMAGE_BW_9,

  RESOURCE_ID_IMAGE_BW_10,
  RESOURCE_ID_IMAGE_BW_8,
  RESOURCE_ID_IMAGE_BW_8,
  RESOURCE_ID_IMAGE_BW_13,
  RESOURCE_ID_IMAGE_BW_14,
  RESOURCE_ID_IMAGE_BW_13,
  RESOURCE_ID_IMAGE_BW_13,
  RESOURCE_ID_IMAGE_BW_5,
  RESOURCE_ID_IMAGE_BW_6,
  RESOURCE_ID_IMAGE_BW_19,

  RESOURCE_ID_IMAGE_BW_20,
  RESOURCE_ID_IMAGE_BW_20,
  RESOURCE_ID_IMAGE_BW_20,
  RESOURCE_ID_IMAGE_BW_23,
  RESOURCE_ID_IMAGE_BW_23,
  RESOURCE_ID_IMAGE_BW_25,
  RESOURCE_ID_IMAGE_BW_25,
  RESOURCE_ID_IMAGE_BW_27,
  RESOURCE_ID_IMAGE_BW_28,
  RESOURCE_ID_IMAGE_BW_27,

  RESOURCE_ID_IMAGE_BW_28,
  RESOURCE_ID_IMAGE_BW_27,
  RESOURCE_ID_IMAGE_BW_32,
  RESOURCE_ID_IMAGE_BW_27,
  RESOURCE_ID_IMAGE_BW_28,
  RESOURCE_ID_IMAGE_BW_6,
  RESOURCE_ID_IMAGE_BW_32,
  RESOURCE_ID_IMAGE_BW_3,
  RESOURCE_ID_IMAGE_BW_3,
  RESOURCE_ID_IMAGE_BW_3,

  RESOURCE_ID_IMAGE_BW_14,
  RESOURCE_ID_IMAGE_BW_13,
  RESOURCE_ID_IMAGE_BW_14,
  RESOURCE_ID_IMAGE_BW_13,
  RESOURCE_ID_IMAGE_BW_25,
  RESOURCE_ID_IMAGE_BW_3,
  RESOURCE_ID_IMAGE_BW_13,
  RESOURCE_ID_IMAGE_BW_3,
};

static char* MONTHS[][12] = {
  {"January",   "February", "March",  "April",  "May",  "June",   "July",     "August",   "September",  "October",  "November",   "December"},
  {"Janvier",   "Février",  "Mars",   "Avril",  "Mai",  "Juin",   "Juillet",  "Août",     "Septembre",  "Octobre",  "Novembre",   "Décembre"},
  {"Enero",     "Febrero",  "Marzo",  "Abril",  "Mayo", "Junio",  "Julio",    "Agosto",   "Septiembre", "Octubre",  "Noviembre",  "Diciembre"},
};

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  if(clock_is_24h_style()){
    flip_layer_animate_to(layer_number[0], tick_time->tm_hour);
  }
  else {
    flip_layer_animate_to(layer_number[0], tick_time->tm_hour % 12 == 0 ? 12 : tick_time->tm_hour % 12);
  }
  
  flip_layer_animate_to(layer_number[1], tick_time->tm_min);
  snprintf(date_text, sizeof(date_text), "%s %d", MONTHS[getLanguage()][tick_time->tm_mon],tick_time->tm_mday);

  layer_mark_dirty(text_layer_get_layer(text_date_layer));

  if (units_changed & HOUR_UNIT && getHourly_vibration()) vibes_double_pulse();
}

static void battery_state_handler(BatteryChargeState charge) {
  if(battery_image){
    gbitmap_destroy(battery_image);
  }
  if(charge.is_charging){
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_CHARGING);
  }
  else if(charge.charge_percent == 100 ){
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_100);
  }
  else if(charge.charge_percent > 90 ){
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_90);
  }
  else if(charge.charge_percent > 80 ){
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_80);
  }
  else if(charge.charge_percent > 60 ){
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_60);
  }
  else if(charge.charge_percent > 40 ){
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_40);
  }
  else if(charge.charge_percent > 20 ){
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_20);
  }
  else {
    battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_0);
  }

  bitmap_layer_set_bitmap(battery_layer, battery_image);
}

static void bluetooth_connection_handler(bool connected){
  if(connected){
    bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_image);
  }
  else{
    bitmap_layer_set_bitmap(bluetooth_layer, NULL);
  }
}

static void update_top_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx,GRect(0,0,144,18),0,0);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx,GRect(0,16,144,1),0,0);
}

static void update_bottom_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx,GRect(1,146,142,21),2,GCornersAll);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  top_layer = layer_create(bounds);
  layer_set_update_proc(top_layer, update_top_callback);
  layer_add_child(window_layer, top_layer);

  weather_image = gbitmap_create_with_resource(WEATHER_IMAGE_RESOURCE[0]);
  weather_layer = bitmap_layer_create(GRect(0, 93, 72, 50));
  bitmap_layer_set_bitmap(weather_layer, battery_image);
  bitmap_layer_set_alignment(weather_layer, GAlignCenter);
  bitmap_layer_set_compositing_mode(weather_layer,GCompOpAssignInverted);
  layer_add_child(window_layer, bitmap_layer_get_layer(weather_layer));

  text_temp_layer = text_layer_create(GRect(65, 91, 79, 50));
  text_layer_set_text_color(text_temp_layer, GColorWhite);
  text_layer_set_text_alignment(text_temp_layer,GTextAlignmentCenter);
  text_layer_set_background_color(text_temp_layer, GColorClear);
  text_layer_set_font(text_temp_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text(text_temp_layer, temp_text);
  layer_add_child(window_layer, text_layer_get_layer(text_temp_layer));

  bottom_layer = layer_create(bounds);
  layer_set_update_proc(bottom_layer, update_bottom_callback);
  layer_add_child(window_layer, bottom_layer);

  for(int i=0; i<LAYER_NUMBER; i++){
    layer_number[i] = flip_layer_create(GRect(3 + i * 69, 21, 69, 36 * 2));
    flip_layer_set_images(layer_number[i], NUMBER_IMAGE_RESOURCE_UP_IDS, NUMBER_IMAGE_RESOURCE_DOWN_IDS, NUMBER_IMAGE_COUNT);
    layer_add_child(window_layer, flip_layer_get_layer(layer_number[i]));
  }

  text_city_layer = text_layer_create(GRect(2, -1, 90, 16));
  text_layer_set_text_color(text_city_layer, GColorBlack);
  text_layer_set_text_alignment(text_city_layer,GTextAlignmentLeft);
  text_layer_set_background_color(text_city_layer, GColorClear);
  text_layer_set_font(text_city_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text(text_city_layer, city_text);
  layer_add_child(window_layer, text_layer_get_layer(text_city_layer));

  // text_temp_layer = text_layer_create(GRect(4, 144, 50, 20));
  // text_layer_set_text_color(text_temp_layer, GColorBlack);
  // text_layer_set_text_alignment(text_temp_layer,GTextAlignmentLeft);
  // text_layer_set_background_color(text_temp_layer, GColorClear);
  // text_layer_set_font(text_temp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  // text_layer_set_text(text_temp_layer, temp_text);
  // layer_add_child(window_layer, text_layer_get_layer(text_temp_layer));

  text_date_layer = text_layer_create(GRect(4, 144, 138, 20));
  text_layer_set_text_color(text_date_layer, GColorBlack);
  text_layer_set_text_alignment(text_date_layer,GTextAlignmentRight);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(text_date_layer, date_text);
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  battery_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_100);
  battery_layer = bitmap_layer_create(GRect(144-18, 0, 16, 16));
  bitmap_layer_set_bitmap(battery_layer, battery_image);
  bitmap_layer_set_alignment(battery_layer, GAlignCenter);
  bitmap_layer_set_compositing_mode(battery_layer,GCompOpClear);
  layer_add_child(window_layer, bitmap_layer_get_layer(battery_layer));

  bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_ON);
  bluetooth_layer = bitmap_layer_create(GRect(112, 2, 11, 12));
  bitmap_layer_set_bitmap(bluetooth_layer, battery_image);
  bitmap_layer_set_alignment(bluetooth_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));

  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  handle_minute_tick(tick_time, MINUTE_UNIT);
  battery_state_handler(battery_state_service_peek());
  bluetooth_connection_handler(bluetooth_connection_service_peek());
}

static void window_unload(Window *window) {
  for(int i=0; i<LAYER_NUMBER; i++){
    flip_layer_destroy(layer_number[i]);
  }
  text_layer_destroy(text_city_layer);
  text_layer_destroy(text_temp_layer);
  text_layer_destroy(text_date_layer);

  layer_destroy(top_layer);
  layer_destroy(bottom_layer);

  gbitmap_destroy(weather_image);
  bitmap_layer_destroy(weather_layer);

  gbitmap_destroy(battery_image);
  bitmap_layer_destroy(battery_layer);

  gbitmap_destroy(bluetooth_image);
  bitmap_layer_destroy(bluetooth_layer);
}

static void timer_callback(void *data) {
  DictionaryIterator *iter;
  uint8_t value = 1;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, TEMPERATURE_KEY, &value, 1, true);
  dict_write_end(iter);
  app_message_outbox_send();
  
  weather_timer = app_timer_register(getInterval() * 60 * 1000 /* milliseconds */, timer_callback, NULL);
}

static void cb_in_received_handler(DictionaryIterator *iter, void *context) {
  autoconfig_in_received_handler(iter, context);

  Tuple *code_tuple = dict_find(iter, WEATHERCODE_KEY);
  if(code_tuple){
    int32_t weather_code = code_tuple->value->int32;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "code:%ld", weather_code);
    
    if(weather_code != 3200){
        Tuple *temp_tuple = dict_find(iter, TEMPERATURE_KEY);
        if (temp_tuple) {
            int32_t temperature = temp_tuple->value->int32;
            // temperature = -99;
            snprintf(temp_text, sizeof(temp_text), "%ld°", temperature);
            layer_mark_dirty(text_layer_get_layer(text_temp_layer));
        }
        Tuple *city_tuple = dict_find(iter, CITYNAME_KEY);
        if (city_tuple) {
            snprintf(city_text, sizeof(city_text), "%s", city_tuple->value->cstring);
            layer_mark_dirty(text_layer_get_layer(text_city_layer));
        }
        // weather_code = 28;
        if(weather_code > 47){
          weather_code = 0;
        }
    }
    else {
      weather_code = 0;
    }
    if(weather_image){
      gbitmap_destroy(weather_image);
    }
    weather_image = gbitmap_create_with_resource(WEATHER_IMAGE_RESOURCE[weather_code]);
    bitmap_layer_set_bitmap(weather_layer,  weather_image);
  }

  if(dict_find(iter, INTERVAL_PKEY)){
    app_timer_cancel(weather_timer);
    weather_timer = app_timer_register(getInterval() * 1000 /* milliseconds */, timer_callback, NULL);
  }

  if(dict_find(iter, LANGUAGE_PKEY)){
    time_t now = time(NULL);
    struct tm *tick_time = localtime(&now);
    handle_minute_tick(tick_time, MINUTE_UNIT);
  }
}

static void init(void) {
  autoconfig_init();
  
  window = window_create();
  window_set_background_color(window,GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  // Register our custom receive handler which in turn will call Pebble Autoconfigs receive handler
  app_message_register_inbox_received(cb_in_received_handler);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  battery_state_service_subscribe(battery_state_handler);
  bluetooth_connection_service_subscribe(bluetooth_connection_handler);

  weather_timer = app_timer_register(getInterval() * 1000 /* milliseconds */, timer_callback, NULL);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  window_destroy(window);

  app_timer_cancel(weather_timer);

  autoconfig_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
