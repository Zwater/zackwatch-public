//Zack's
#include <pebble.h>
#define KEY_ZACK 0
#define KEY_SARAH 1
#define KEY_ALEC 2
#define KEY_MELISSA 3
#define KEY_TEMP 4
#define KEY_HIGHTEMP 5
#define KEY_LOWTEMP 6
#define KEY_COND 7
#define KEY_BATT 8
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_people_layer;
static TextLayer *s_zack_layer;
static TextLayer *s_sarah_layer;
static TextLayer *s_alec_layer;
static TextLayer *s_melissa_layer;
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
//static GFont s_date_font;
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    // Store incoming information
    static char zack_buffer[2];
    static char sarah_buffer[2];
    static char alec_buffer[2];
    static char melissa_buffer[2];
    static char temp_buffer[6];
    static char hightemp_buffer[6];
    static char lowtemp_buffer[6];
    static char cond_buffer[64];
    static char hilo_buffer[16];
    static char batt_buffer[8];
    // Read tuples for data
    Tuple *zack_tuple = dict_find(iterator, KEY_ZACK);
    Tuple *sarah_tuple = dict_find(iterator, KEY_SARAH);
    Tuple *alec_tuple = dict_find(iterator, KEY_ALEC);
    Tuple *melissa_tuple = dict_find(iterator, KEY_MELISSA);
    Tuple *temp_tuple = dict_find(iterator, KEY_TEMP);
    Tuple *hightemp_tuple = dict_find(iterator, KEY_HIGHTEMP);
    Tuple *lowtemp_tuple = dict_find(iterator, KEY_LOWTEMP);
    Tuple *cond_tuple = dict_find(iterator, KEY_COND);
    Tuple *batt_tuple = dict_find(iterator, KEY_BATT);
    // If all data is available, use it
    if(zack_tuple && sarah_tuple && alec_tuple && melissa_tuple && batt_tuple && temp_tuple && hightemp_tuple && lowtemp_tuple && cond_tuple) {
        snprintf(zack_buffer, sizeof(zack_buffer), "%s", zack_tuple->value->cstring);
        snprintf(sarah_buffer, sizeof(sarah_buffer), "%s", sarah_tuple->value->cstring);
        snprintf(alec_buffer, sizeof(alec_buffer), "%s", alec_tuple->value->cstring);
        snprintf(melissa_buffer, sizeof(melissa_buffer), "%s", melissa_tuple->value->cstring);
        snprintf(temp_buffer, sizeof(temp_buffer), "%d°", (int)temp_tuple->value->int32);
        snprintf(hightemp_buffer, sizeof(hightemp_buffer), "%s", hightemp_tuple->value->cstring);
        snprintf(lowtemp_buffer, sizeof(lowtemp_buffer), "%s", lowtemp_tuple->value->cstring);
        snprintf(cond_buffer, sizeof(cond_buffer), "%s", cond_tuple->value->cstring);
        snprintf(hilo_buffer, sizeof(hilo_buffer), "%d°\n%d°", (int)hightemp_tuple->value->int32, (int)lowtemp_tuple->value->int32);
        snprintf(batt_buffer, sizeof(batt_buffer), "%s", batt_tuple->value->cstring);
        switch(zack_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_zack_layer, GColorWhite);
            text_layer_set_text_color(s_zack_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_zack_layer, GColorBlack);
            text_layer_set_text_color(s_zack_layer, GColorWhite);
            break;
        }
        switch(sarah_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_sarah_layer, GColorWhite);
            text_layer_set_text_color(s_sarah_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_sarah_layer, GColorBlack);
            text_layer_set_text_color(s_sarah_layer, GColorWhite);
            break;
        }
        switch(alec_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_alec_layer, GColorWhite);
            text_layer_set_text_color(s_alec_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_alec_layer, GColorBlack);
            text_layer_set_text_color(s_alec_layer, GColorWhite);
            break;
        }
        switch(melissa_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_melissa_layer, GColorWhite);
            text_layer_set_text_color(s_melissa_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_melissa_layer, GColorBlack);
            text_layer_set_text_color(s_melissa_layer, GColorWhite);
            break;
        }
        if (strcmp(cond_buffer, "clear-day")==0) {
            text_layer_set_text(s_cond_layer, "1");
            window_set_background_color(s_main_window, GColorBabyBlueEyes);
            text_layer_set_text_color(s_time_layer, GColorWhite);
            text_layer_set_text_color(s_date_layer, GColorWhite);
            text_layer_set_text_color(s_temp_layer, GColorWhite);
            text_layer_set_text_color(s_cond_layer, GColorWhite);
            text_layer_set_text_color(s_hilo_layer, GColorWhite);
            text_layer_set_text_color(s_batt_layer, GColorWhite);
        }
        else if (strcmp(cond_buffer, "partly-cloudy-day")==0) {
            text_layer_set_text(s_cond_layer, "3");
            window_set_background_color(s_main_window, GColorBlue);
            text_layer_set_text_color(s_time_layer, GColorLightGray);
            text_layer_set_text_color(s_date_layer, GColorLightGray);
            text_layer_set_text_color(s_temp_layer, GColorLightGray);
            text_layer_set_text_color(s_cond_layer, GColorLightGray);
            text_layer_set_text_color(s_hilo_layer, GColorLightGray);
            text_layer_set_text_color(s_batt_layer, GColorLightGray);
        }
        else if (strcmp(cond_buffer, "cloudy")==0) {
            text_layer_set_text(s_cond_layer, "5");
            window_set_background_color(s_main_window, GColorDarkGray);
            text_layer_set_text_color(s_time_layer, GColorWhite);
            text_layer_set_text_color(s_date_layer, GColorWhite);
            text_layer_set_text_color(s_temp_layer, GColorWhite);
            text_layer_set_text_color(s_cond_layer, GColorWhite);
            text_layer_set_text_color(s_hilo_layer, GColorWhite);
            text_layer_set_text_color(s_batt_layer, GColorWhite);
        }
        else if (strcmp(cond_buffer, "clear-night")==0) {
            text_layer_set_text(s_cond_layer, "2");
            window_set_background_color(s_main_window, GColorOxfordBlue);
            text_layer_set_text_color(s_time_layer, GColorWhite);
            text_layer_set_text_color(s_date_layer, GColorWhite);
            text_layer_set_text_color(s_temp_layer, GColorWhite);
            text_layer_set_text_color(s_cond_layer, GColorWhite);
            text_layer_set_text_color(s_hilo_layer, GColorWhite);
            text_layer_set_text_color(s_batt_layer, GColorWhite);
        }
        else if (strcmp(cond_buffer, "rain")==0) {
            text_layer_set_text(s_cond_layer, "8");
            window_set_background_color(s_main_window, GColorOxfordBlue);
            text_layer_set_text_color(s_time_layer, GColorPictonBlue);
            text_layer_set_text_color(s_date_layer, GColorPictonBlue);
            text_layer_set_text_color(s_temp_layer, GColorPictonBlue);
            text_layer_set_text_color(s_cond_layer, GColorPictonBlue);
            text_layer_set_text_color(s_hilo_layer, GColorPictonBlue);
            text_layer_set_text_color(s_batt_layer, GColorPictonBlue);
        }
        else if (strcmp(cond_buffer, "snow")==0) {
            text_layer_set_text(s_cond_layer, "#");
            window_set_background_color(s_main_window, GColorWhite);
            text_layer_set_text_color(s_time_layer, GColorPictonBlue);
            text_layer_set_text_color(s_date_layer, GColorPictonBlue);
            text_layer_set_text_color(s_temp_layer, GColorPictonBlue);
            text_layer_set_text_color(s_cond_layer, GColorPictonBlue);
            text_layer_set_text_color(s_hilo_layer, GColorPictonBlue);
            text_layer_set_text_color(s_batt_layer, GColorPictonBlue);
        }
        else if (strcmp(cond_buffer, "sleet")==0) {
            text_layer_set_text(s_cond_layer, "$");
            window_set_background_color(s_main_window, GColorCeleste);
            text_layer_set_text_color(s_time_layer, GColorBlue);
            text_layer_set_text_color(s_date_layer, GColorBlue);
            text_layer_set_text_color(s_temp_layer, GColorBlue);
            text_layer_set_text_color(s_cond_layer, GColorBlue);
            text_layer_set_text_color(s_hilo_layer, GColorBlue);
            text_layer_set_text_color(s_batt_layer, GColorBlue);
        }
        else if (strcmp(cond_buffer, "wind")==0) {
            text_layer_set_text(s_cond_layer, "F");
            window_set_background_color(s_main_window, GColorWhite);
            text_layer_set_text_color(s_time_layer, GColorBlack);
            text_layer_set_text_color(s_date_layer, GColorBlack);
            text_layer_set_text_color(s_temp_layer, GColorBlack);
            text_layer_set_text_color(s_cond_layer, GColorBlack);
            text_layer_set_text_color(s_hilo_layer, GColorBlack);
            text_layer_set_text_color(s_batt_layer, GColorBlack);
        }
        else if (strcmp(cond_buffer, "fog")==0) {
            text_layer_set_text(s_cond_layer, "M");
            window_set_background_color(s_main_window, GColorLightGray);
            text_layer_set_text_color(s_time_layer, GColorBlack);
            text_layer_set_text_color(s_date_layer, GColorBlack);
            text_layer_set_text_color(s_temp_layer, GColorBlack);
            text_layer_set_text_color(s_cond_layer, GColorBlack);
            text_layer_set_text_color(s_hilo_layer, GColorBlack);
            text_layer_set_text_color(s_batt_layer, GColorBlack);
        }
        else if (strcmp(cond_buffer, "partly-cloudy-night")==0) {
            text_layer_set_text(s_cond_layer, "4");
            window_set_background_color(s_main_window, GColorOxfordBlue);
            text_layer_set_text_color(s_time_layer, GColorLightGray);
            text_layer_set_text_color(s_date_layer, GColorLightGray);
            text_layer_set_text_color(s_temp_layer, GColorLightGray);
            text_layer_set_text_color(s_cond_layer, GColorLightGray);
            text_layer_set_text_color(s_hilo_layer, GColorLightGray);
            text_layer_set_text_color(s_batt_layer, GColorLightGray);
        }
        else {
            text_layer_set_text(s_cond_layer, ")");
        }
        text_layer_set_text(s_temp_layer, temp_buffer);
        text_layer_set_text(s_hilo_layer, hilo_buffer);
        text_layer_set_text(s_batt_layer, batt_buffer);
        text_layer_set_text(s_shit_layer, "");
        text_layer_set_text(s_zack_layer, "Z");
        text_layer_set_text(s_sarah_layer, "S");
        text_layer_set_text(s_alec_layer, "A");
        text_layer_set_text(s_melissa_layer, "M");
    }
    //if we only have people data
    else if(zack_tuple && sarah_tuple && alec_tuple && melissa_tuple && batt_tuple) {
        snprintf(zack_buffer, sizeof(zack_buffer), "%s", zack_tuple->value->cstring);
        snprintf(sarah_buffer, sizeof(sarah_buffer), "%s", sarah_tuple->value->cstring);
        snprintf(alec_buffer, sizeof(alec_buffer), "%s", alec_tuple->value->cstring);
        snprintf(melissa_buffer, sizeof(melissa_buffer), "%s", melissa_tuple->value->cstring);
        snprintf(batt_buffer, sizeof(batt_buffer), "%s", batt_tuple->value->cstring);
        switch(zack_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_zack_layer, GColorWhite);
            text_layer_set_text_color(s_zack_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_zack_layer, GColorBlack);
            text_layer_set_text_color(s_zack_layer, GColorWhite);
            break;
        }
        switch(sarah_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_sarah_layer, GColorWhite);
            text_layer_set_text_color(s_sarah_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_sarah_layer, GColorBlack);
            text_layer_set_text_color(s_sarah_layer, GColorWhite);
            break;
        }
        switch(alec_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_alec_layer, GColorWhite);
            text_layer_set_text_color(s_alec_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_alec_layer, GColorBlack);
            text_layer_set_text_color(s_alec_layer, GColorWhite);
            break;
        }
        switch(melissa_buffer[0]) {
            case '1':
            text_layer_set_background_color(s_melissa_layer, GColorWhite);
            text_layer_set_text_color(s_melissa_layer, GColorBlack);
            break;
            case '0':
            text_layer_set_background_color(s_melissa_layer, GColorBlack);
            text_layer_set_text_color(s_melissa_layer, GColorWhite);
            break;
        }
        window_set_background_color(s_main_window, GColorWindsorTan);
        text_layer_set_text(s_shit_layer, "\U0001F4A9");
        text_layer_set_text(s_batt_layer, batt_buffer);
        text_layer_set_text(s_zack_layer, "Z");
        text_layer_set_text(s_sarah_layer, "S");
        text_layer_set_text(s_alec_layer, "A");
        text_layer_set_text(s_melissa_layer, "M");
    }
    else {
        window_set_background_color(s_main_window, GColorWindsorTan);
        text_layer_set_text(s_shit_layer, "\U0001F4A9");
        text_layer_set_background_color(s_people_layer, GColorClear);
        text_layer_set_text(s_zack_layer, "");
        text_layer_set_text(s_sarah_layer, "");
        text_layer_set_text(s_alec_layer, "");
        text_layer_set_text(s_melissa_layer, "");
        text_layer_set_background_color(s_zack_layer, GColorClear);
        text_layer_set_background_color(s_sarah_layer, GColorClear);
        text_layer_set_background_color(s_alec_layer, GColorClear);
        text_layer_set_background_color(s_melissa_layer, GColorClear);
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
    // Get weather update every 5 minutes
    if(tick_time->tm_min % 10 == 0) {
        // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);
        // Send the message!
        app_message_outbox_send();
    }
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
    s_zack_layer = text_layer_create(
    GRect(0, 0, 30, 30));
    text_layer_set_background_color(s_zack_layer, GColorBlack);
    text_layer_set_text_color(s_zack_layer, GColorWhite);
    text_layer_set_text_alignment(s_zack_layer, GTextAlignmentCenter);
    text_layer_set_text(s_zack_layer, "Z");
    text_layer_set_font(s_zack_layer, s_people_font);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_zack_layer));
    //create Sarah's Layer
    s_sarah_layer = text_layer_create(
    GRect(35, 0, 30, 30));
    text_layer_set_background_color(s_sarah_layer, GColorBlack);
    text_layer_set_text_color(s_sarah_layer, GColorWhite);
    text_layer_set_text_alignment(s_sarah_layer, GTextAlignmentCenter);
    text_layer_set_text(s_sarah_layer, "S");
    text_layer_set_font(s_sarah_layer, s_people_font);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_sarah_layer));
    //create Alec's Layer
    s_alec_layer = text_layer_create(
    GRect(79, 0, 30, 30));
    text_layer_set_background_color(s_alec_layer, GColorBlack);
    text_layer_set_text_color(s_alec_layer, GColorWhite);
    text_layer_set_text_alignment(s_alec_layer, GTextAlignmentCenter);
    text_layer_set_text(s_alec_layer, "A");
    text_layer_set_font(s_alec_layer, s_people_font);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_alec_layer));
    //create Melissa's Layer
    s_melissa_layer = text_layer_create(
    GRect(114, 0, 30, 30));
    text_layer_set_background_color(s_melissa_layer, GColorBlack);
    text_layer_set_text_color(s_melissa_layer, GColorWhite);
    text_layer_set_text_alignment(s_melissa_layer, GTextAlignmentCenter);
    text_layer_set_text(s_melissa_layer, "M");
    text_layer_set_font(s_melissa_layer, s_people_font);
    layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(s_melissa_layer));
    //create temperature Layer
    s_temp_layer = text_layer_create(
    GRect(10, 90, 50, 30));
    text_layer_set_background_color(s_temp_layer, GColorClear);
    text_layer_set_text_color(s_temp_layer, GColorBlue);
    text_layer_set_text_alignment(s_temp_layer, GTextAlignmentLeft);
    text_layer_set_text(s_temp_layer, "");
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
    text_layer_set_text(s_hilo_layer, "");
    text_layer_set_font(s_hilo_layer, s_dinsmall_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hilo_layer));
    //create battery Layer
    s_batt_layer = text_layer_create(
    GRect(66, 5, 40, 20));
    text_layer_set_background_color(s_batt_layer, GColorClear);
    text_layer_set_text_color(s_batt_layer, GColorWhite);
    text_layer_set_text_alignment(s_batt_layer, GTextAlignmentLeft);
    text_layer_set_text(s_batt_layer, "");
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
		text_layer_destroy(s_zack_layer);
    text_layer_destroy(s_sarah_layer);
    text_layer_destroy(s_alec_layer);
    text_layer_destroy(s_melissa_layer);
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
}
static void deinit() {
    window_destroy(s_main_window);
}
int main(void) {
    init();
    app_event_loop();
    deinit();
}
