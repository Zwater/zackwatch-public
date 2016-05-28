//Zack's
#include <pebble.h>
#define KEY_NAME1 0
#define KEY_NAME2 1
#define KEY_NAME3 2
#define KEY_NAME4 3
#define KEY_TEMP 4
#define KEY_HIGHTEMP 5
#define KEY_LOWTEMP 6
#define KEY_COND 7
#define KEY_BATT 8
#define KEY_VALUE1 10
#define KEY_VALUE2 11
#define KEY_VALUE3 12
#define KEY_VALUE4 13

/*json file looks like
{A":"1", "B":"1", "C":"0", D":"0"}
*/
static bool s_vibrate = true;
static int s_interval = 1;
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_people_layer;
static TextLayer *s_name1_layer;
static TextLayer *s_name2_layer;
static TextLayer *s_name3_layer;
static TextLayer *s_name4_layer;
static TextLayer *s_date_layer;
static TextLayer *s_temp_layer;
static TextLayer *s_cond_layer;
static TextLayer *s_hilo_layer;
static TextLayer *s_batt_layer;
static TextLayer *s_shit_layer;
static GFont s_time_font;
static GFont s_people_font;
static GFont s_weather_font;
static GFont s_dinsmall_font;
static GFont s_batt_font;
static char s_ppl[] = {'0','0','0','0'};
static bool s_changed = false;

static void set_all_text_layer(GColor color) {
            text_layer_set_text_color(s_time_layer, color);
            text_layer_set_text_color(s_date_layer, color);
            text_layer_set_text_color(s_temp_layer, color);
            text_layer_set_text_color(s_cond_layer, color);
            text_layer_set_text_color(s_hilo_layer, color);
            text_layer_set_text_color(s_batt_layer, color);
}

static void set_ppl(int ppl, char *name_buffer, Tuple *name_tuple,  Tuple *value_tuple,  TextLayer *s_name_layer) {

    static char value_buffer[2];
    if (name_tuple && value_tuple) {
      snprintf(name_buffer, sizeof(name_buffer), "%s", name_tuple->value->cstring);
      snprintf(value_buffer, sizeof(value_buffer), "%s", value_tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, name_buffer);
        APP_LOG(APP_LOG_LEVEL_INFO, value_buffer);
      text_layer_set_text(s_name_layer, name_buffer);
      
      switch(value_buffer[0]) {
            case '1':
            if (s_ppl[ppl] == '0') { 
              text_layer_set_background_color(s_name_layer, GColorWhite);
              text_layer_set_text_color(s_name_layer, GColorBlack);
              s_changed = true;
              s_ppl[ppl] = '1';
            }
            break;
            case '0':
            if (s_ppl[ppl] == '1') { 
              text_layer_set_background_color(s_name_layer, GColorBlack);
              text_layer_set_text_color(s_name_layer, GColorWhite);
              s_changed = true;
              s_ppl[ppl] = '2';
            }
            break;
        }
    }
  
}

static void bluetooth_callback(bool connected) {
  vibes_double_pulse();
}

//static GFont s_date_font;
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    text_layer_set_text(s_shit_layer, "");
    // Store incoming information
    static char temp_buffer[6];
    static char hightemp_buffer[6];
    static char lowtemp_buffer[6];
    static char cond_buffer[64];
    static char hilo_buffer[16];
    static char batt_buffer[8];
  
    // Read tuples for data
    Tuple *name1_tuple = dict_find(iterator, KEY_NAME1);
    Tuple *name2_tuple = dict_find(iterator, KEY_NAME2);
    Tuple *name3_tuple = dict_find(iterator, KEY_NAME3);
    Tuple *name4_tuple = dict_find(iterator, KEY_NAME4);
    Tuple *value1_tuple = dict_find(iterator, KEY_VALUE1);
    Tuple *value2_tuple = dict_find(iterator, KEY_VALUE2);
    Tuple *value3_tuple = dict_find(iterator, KEY_VALUE3);
    Tuple *value4_tuple = dict_find(iterator, KEY_VALUE4);
    Tuple *temp_tuple = dict_find(iterator, KEY_TEMP);
    Tuple *hightemp_tuple = dict_find(iterator, KEY_HIGHTEMP);
    Tuple *lowtemp_tuple = dict_find(iterator, KEY_LOWTEMP);
    Tuple *cond_tuple = dict_find(iterator, KEY_COND);
    Tuple *batt_tuple = dict_find(iterator, KEY_BATT);
    static char name1_buffer[2];
    static char name2_buffer[2];
    static char name3_buffer[2];
    static char name4_buffer[2];
  
    set_ppl(1,name1_buffer, name1_tuple, value1_tuple, s_name1_layer);
    set_ppl(2,name2_buffer, name2_tuple, value2_tuple, s_name2_layer);
    set_ppl(3,name3_buffer, name3_tuple, value3_tuple, s_name3_layer);
    set_ppl(4,name4_buffer, name4_tuple, value4_tuple, s_name4_layer);
    if(s_changed){
      s_changed = false;
      if(s_vibrate){
        vibes_double_pulse();
      }
    }
    
    if(batt_tuple){
      snprintf(batt_buffer, sizeof(batt_buffer), "%s", batt_tuple->value->cstring);
      text_layer_set_text(s_batt_layer, batt_buffer);
    }
  
    if (cond_tuple){
      snprintf(cond_buffer, sizeof(cond_buffer), "%s", cond_tuple->value->cstring);
      if (strcmp(cond_buffer, "clear-day")==0) {
            text_layer_set_text(s_cond_layer, "1");
            window_set_background_color(s_main_window, GColorBabyBlueEyes);
            set_all_text_layer(GColorWhite);
        }
        else if (strcmp(cond_buffer, "partly-cloudy-day")==0) {
            text_layer_set_text(s_cond_layer, "3");
            window_set_background_color(s_main_window, GColorBlue);
            set_all_text_layer(GColorLightGray);
        }
        else if (strcmp(cond_buffer, "cloudy")==0) {
            text_layer_set_text(s_cond_layer, "5");
            window_set_background_color(s_main_window, GColorDarkGray);
            set_all_text_layer(GColorWhite);
        }
        else if (strcmp(cond_buffer, "clear-night")==0) {
            text_layer_set_text(s_cond_layer, "2");
            window_set_background_color(s_main_window, GColorOxfordBlue);
            set_all_text_layer(GColorWhite);
        }
        else if (strcmp(cond_buffer, "rain")==0) {
            text_layer_set_text(s_cond_layer, "8");
            window_set_background_color(s_main_window, GColorOxfordBlue);
            set_all_text_layer(GColorPictonBlue);
        }
        else if (strcmp(cond_buffer, "snow")==0) {
            text_layer_set_text(s_cond_layer, "#");
            window_set_background_color(s_main_window, GColorWhite);
            set_all_text_layer(GColorPictonBlue);
        }
        else if (strcmp(cond_buffer, "sleet")==0) {
            text_layer_set_text(s_cond_layer, "$");
            window_set_background_color(s_main_window, GColorCeleste);
            set_all_text_layer(GColorBlue);
        }
        else if (strcmp(cond_buffer, "wind")==0) {
            text_layer_set_text(s_cond_layer, "F");
            window_set_background_color(s_main_window, GColorWhite);
            set_all_text_layer(GColorBlack);
        }
        else if (strcmp(cond_buffer, "fog")==0) {
            text_layer_set_text(s_cond_layer, "M");
            window_set_background_color(s_main_window, GColorLightGray);
            set_all_text_layer(GColorBlack);
        }
        else if (strcmp(cond_buffer, "partly-cloudy-night")==0) {
            text_layer_set_text(s_cond_layer, "4");
            window_set_background_color(s_main_window, GColorOxfordBlue);
            set_all_text_layer(GColorLightGray);
        }
        else {
            text_layer_set_text(s_cond_layer, ")");
        }
    }
    
    
    if(temp_tuple){
      snprintf(temp_buffer, sizeof(temp_buffer), "%d°", (int)temp_tuple->value->int32);
      APP_LOG(APP_LOG_LEVEL_INFO, temp_buffer);
      text_layer_set_text(s_temp_layer, temp_buffer);
    }
    // If all data is available, use it
    if(hightemp_tuple && lowtemp_tuple) {
        snprintf(hightemp_buffer, sizeof(hightemp_buffer), "%s", hightemp_tuple->value->cstring);
        snprintf(lowtemp_buffer, sizeof(lowtemp_buffer), "%s", lowtemp_tuple->value->cstring);
        snprintf(hilo_buffer, sizeof(hilo_buffer), "%d°\n%d°", (int)hightemp_tuple->value->int32, (int)lowtemp_tuple->value->int32);
        text_layer_set_text(s_hilo_layer, hilo_buffer);
    }
}
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
static void update_time() {
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    // Write the current hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
    "%H:%M" : "%I:%M", tick_time);
    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, s_buffer);
    // Copy date into buffer from tm structure
    static char date_buffer[16];
    strftime(date_buffer, sizeof(date_buffer), "%a, %b %d", tick_time);
    // Show the date
    text_layer_set_text(s_date_layer, date_buffer);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
    // Get weather update every x minutes
    if(tick_time->tm_min % s_interval == 0) {
        // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);
        // Send the message!
        app_message_outbox_send();
    }
}

static void create_ppl_layer(TextLayer *s_name_layer) {
    text_layer_set_background_color(s_name_layer, GColorBlack);
    text_layer_set_text_color(s_name_layer, GColorWhite);
    text_layer_set_text_alignment(s_name_layer, GTextAlignmentCenter);
    text_layer_set_text(s_name_layer, "");
    text_layer_set_font(s_name_layer, s_people_font);
}

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    s_time_layer = text_layer_create(
    GRect(0, -22, bounds.size.w, 80));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text(s_time_layer, "00:00");
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    // Create Fonts
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_80));
    s_people_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_BOLD_25));
    s_dinsmall_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_20));
    s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_METEOCONS_35));
    s_batt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_15));
    text_layer_set_font(s_time_layer, s_time_font);
    // Add it as a child layer to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    // Create date TextLayer
    s_date_layer = text_layer_create(GRect(0, 55, bounds.size.w, 30));
    text_layer_set_text_color(s_date_layer, GColorWhite);
    text_layer_set_background_color(s_date_layer, GColorClear);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    text_layer_set_font(s_date_layer, s_people_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
    //create People Layer
    s_people_layer = text_layer_create(
    GRect(0, 138, bounds.size.w, 30));
    text_layer_set_background_color(s_people_layer, GColorBlack);
    text_layer_set_text_color(s_people_layer, GColorWhite);
    text_layer_set_text_alignment(s_people_layer, GTextAlignmentCenter);
    text_layer_set_text(s_people_layer, "");
    text_layer_set_font(s_people_layer, s_people_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_people_layer));
    //create Zack's Layer
  
    s_name1_layer = text_layer_create(
    GRect(0, 0, 30, 30));
    create_ppl_layer(s_name1_layer);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_name1_layer));
    //create Sarah's Layer
    s_name2_layer = text_layer_create(
    GRect(114, 0, 30, 30));
    create_ppl_layer(s_name2_layer);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_name2_layer));
    //create Alec's Layer
    s_name3_layer = text_layer_create(
    GRect(35, 0, 30, 30));
    create_ppl_layer(s_name3_layer);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_name3_layer));
    //create Melissa's Layer
    s_name4_layer = text_layer_create(
    GRect(79, 0, 30, 30));
    create_ppl_layer(s_name4_layer);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_name4_layer));
    
    //create temperature Layer
    s_temp_layer = text_layer_create(
    GRect(10, 90, 50, 30));
    text_layer_set_background_color(s_temp_layer, GColorClear);
    text_layer_set_text_color(s_temp_layer, GColorBlue);
    text_layer_set_text_alignment(s_temp_layer, GTextAlignmentLeft);
    text_layer_set_text(s_temp_layer, "1");
    text_layer_set_font(s_temp_layer, s_people_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_temp_layer));
    //create conditions Layer
    s_cond_layer = text_layer_create(
    GRect(47, 88, 50, 50));
    text_layer_set_background_color(s_cond_layer, GColorClear);
    text_layer_set_text_color(s_cond_layer, GColorWhite);
    text_layer_set_text_alignment(s_cond_layer, GTextAlignmentCenter);
    text_layer_set_text(s_cond_layer, "");
    text_layer_set_font(s_cond_layer, s_weather_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_cond_layer));
    //create high/low temp Layer
    s_hilo_layer = text_layer_create(
    GRect(93, 85, 50, 60));
    text_layer_set_background_color(s_hilo_layer, GColorClear);
    text_layer_set_text_color(s_hilo_layer, GColorWhite);
    text_layer_set_text_alignment(s_hilo_layer, GTextAlignmentCenter);
    text_layer_set_text(s_hilo_layer, "1");
    text_layer_set_font(s_hilo_layer, s_dinsmall_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hilo_layer));
    //create battery Layer
    s_batt_layer = text_layer_create(
    GRect(66, 5, 40, 20));
    text_layer_set_background_color(s_batt_layer, GColorClear);
    text_layer_set_text_color(s_batt_layer, GColorWhite);
    text_layer_set_text_alignment(s_batt_layer, GTextAlignmentLeft);
    text_layer_set_text(s_batt_layer, "1");
    text_layer_set_font(s_batt_layer, s_batt_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_batt_layer));
    //create shit Layer
    s_shit_layer = text_layer_create(
    GRect(0, 90, bounds.size.w, 30));
    text_layer_set_background_color(s_shit_layer, GColorClear);
    text_layer_set_text_color(s_shit_layer, GColorWhite);
    text_layer_set_text_alignment(s_shit_layer, GTextAlignmentCenter);
    text_layer_set_text(s_shit_layer, "Loading...");
    text_layer_set_font(s_shit_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_shit_layer));
}
static void main_window_unload(Window *window) {
    // Unload Fonts
    fonts_unload_custom_font(s_time_font);
    fonts_unload_custom_font(s_people_font);
    fonts_unload_custom_font(s_weather_font);
    fonts_unload_custom_font(s_dinsmall_font);
    fonts_unload_custom_font(s_batt_font);
    // Destroy text layers
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_date_layer);
		text_layer_destroy(s_name1_layer);
    text_layer_destroy(s_name2_layer);
    text_layer_destroy(s_name3_layer);
    text_layer_destroy(s_name4_layer);
    text_layer_destroy(s_temp_layer);
    text_layer_destroy(s_cond_layer);
    text_layer_destroy(s_hilo_layer);
    text_layer_destroy(s_batt_layer);
    text_layer_destroy(s_shit_layer);
}
static void init() {
    // Create main Window element and assign to pointer
    s_main_window = window_create();
    GRect bounds = layer_get_bounds(window_get_root_layer(s_main_window));
    window_set_background_color(s_main_window, GColorBabyBlueEyes);
    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });
    window_stack_push(s_main_window, true);
    update_time();
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
    // Register for Bluetooth connection updates
    if (s_vibrate) {
      connection_service_subscribe((ConnectionHandlers) {
        .pebble_app_connection_handler = bluetooth_callback
      });
    }
}
static void deinit() {
    window_destroy(s_main_window);
}
int main(void) {
    init();
    app_event_loop();
    deinit();
}
