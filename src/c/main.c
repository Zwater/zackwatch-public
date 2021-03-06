//Zack's
#include <pebble.h>
#define KEY_NAME1 0
#define KEY_NAME2 1
#define KEY_NAME3 2
#define KEY_NAME4 3
#define KEY_NAME5 4
#define KEY_NAME6 5
#define KEY_NAME7 6
#define KEY_NAME8 7
#define KEY_BATT 8
#define KEY_VALUE1 10
#define KEY_VALUE2 11
#define KEY_VALUE3 12
#define KEY_VALUE4 13
#define KEY_VALUE5 14
#define KEY_VALUE6 15
#define KEY_VALUE7 16
#define KEY_VALUE8 17
#define KEY_PCOND 28
#define KEY_HIGHTEMP 20
#define KEY_LOWTEMP 21
#define KEY_COND 22
#define KEY_PPL_TOTAL 23
#define KEY_PPL 24
#define KEY_REFRESH 25
#define KEY_TEMP 26
#define KEY_VIBRATE 27

/*json file looks like
  {A":"1", "B":"1", "C":"0", D":"0"}
  */
static bool s_vibrate;

static int s_interval;
static int block_height = 30;
//default
static int block_width = PBL_IF_ROUND_ELSE(20,30);
static int ppl_total;
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_people_layer;
static TextLayer *sa_name_layer[6];
static TextLayer *s_date_layer;
static TextLayer *s_temp_layer;
static TextLayer *s_cond_layer;
static TextLayer *s_hilo_layer;
static TextLayer *s_batt_layer;
static TextLayer *s_shit_layer;
static TextLayer *s_weather_layer;
static GFont s_time_font;
static GFont s_people_font;
static GFont s_weather_font;
static GFont s_dinsmall_font;
static GFont s_batt_font;
static char s_ppl[6][2];
static char s_ppl_name[6][8];
static bool s_changed = false;
static int s_sizew;
static GColor text;
static GColor bg;
static GColor bwbg;
static char *s_cond="1";
static char *s_prev_cond="BO";
static char cond_buffer[64];
static char *s_shit;
static char temp_buffer[6];
static char hilo_buffer[16];
static char hightemp_buffer[6];
static char lowtemp_buffer[6];

static void set_block_width(){
    //simple 1 pixel border
    block_width = (s_sizew-ppl_total-1)/ppl_total;
}

static void set_all_text_layer(GColor color) {
    text_layer_set_text_color(s_time_layer, COLOR_FALLBACK(color, GColorWhite));
    text_layer_set_text_color(s_date_layer, COLOR_FALLBACK(color, GColorWhite));
    text_layer_set_text_color(s_temp_layer, COLOR_FALLBACK(color, GColorWhite));
    text_layer_set_text_color(s_cond_layer, COLOR_FALLBACK(color, GColorWhite));
    text_layer_set_text_color(s_hilo_layer, COLOR_FALLBACK(color, GColorWhite));
    text_layer_set_text_color(s_batt_layer, COLOR_FALLBACK(color, GColorWhite));
}

static void setup_ppl_char(int ppl, char *name_buffer, char *value_buffer, TextLayer *s_name_layer, bool check) {
    text_layer_set_text(s_name_layer, name_buffer);
    bool doit = true;
    APP_LOG(APP_LOG_LEVEL_INFO, name_buffer);
    if (strcmp(value_buffer, "1")==0) {
        //prevent re-draw if not required etc

        if (check) { doit = (strcmp(s_ppl[ppl], "0")==0); }
        if (doit) {
            APP_LOG(APP_LOG_LEVEL_INFO, "home");
            text_layer_set_background_color(s_name_layer, text);
            text_layer_set_text_color(s_name_layer, bg);
            s_changed = true;
            snprintf(s_ppl[ppl], sizeof(s_ppl[ppl]), "%s", "1");
        }
    } else {
        if (check) { doit = (strcmp(s_ppl[ppl], "1")==0); }
        if (doit) {
            APP_LOG(APP_LOG_LEVEL_INFO, "away");
            text_layer_set_background_color(s_name_layer, bg);
            text_layer_set_text_color(s_name_layer, text);
            s_changed = true;
            snprintf(s_ppl[ppl], sizeof(s_ppl[ppl]), "%s", "0");
        }
    }
}


static void set_ppl(int ppl, char *name_buffer, Tuple *name_tuple,  Tuple *value_tuple,  TextLayer *s_name_layer) {

    static char value_buffer[2];
    if (name_tuple && value_tuple) {

        snprintf(name_buffer, sizeof(name_buffer), "%s", name_tuple->value->cstring);
        snprintf(value_buffer, sizeof(value_buffer), "%s", value_tuple->value->cstring);
        snprintf(s_ppl_name[ppl], sizeof(s_ppl_name[ppl]), "%s", name_buffer);

        setup_ppl_char(ppl,name_buffer,value_buffer,s_name_layer,true);
    }

}

static void bluetooth_callback(bool connected) {
    vibes_double_pulse();
}

static void  deinit_ppl() {
    for(int i = 1; i <= ppl_total; i++){
        text_layer_set_text(sa_name_layer[i], "");
        text_layer_set_background_color(sa_name_layer[i], GColorClear);
        text_layer_destroy(sa_name_layer[i]);
    }
}

static void create_ppl_layer(TextLayer *s_name_layer, int num) {
    text_layer_set_background_color(s_name_layer, GColorBlack);
    text_layer_set_text_color(s_name_layer, GColorWhite);
/*#if defined(PBL_ROUND)
        printf("We're running on a round display");
        if(num == 1) {
          printf("first");
          text_layer_set_text_alignment(s_name_layer, GTextAlignmentRight);
        }
        else if(num == ppl_total) {
          printf("last");
          text_layer_set_text_alignment(s_name_layer, GTextAlignmentLeft);
        }
        else {
          text_layer_set_text_alignment(s_name_layer, GTextAlignmentCenter);
        };
#elif defined(PBL_RECT)*/
    text_layer_set_text_alignment(s_name_layer, GTextAlignmentCenter);
//#endif
    text_layer_set_text(s_name_layer, "");
    text_layer_set_font(s_name_layer, s_people_font);
}

static int get_ppl_pos(double pos) {
    int unit_size = s_sizew / ppl_total;
    int gap_size = (unit_size - block_width)/2;
    int rpos = s_sizew/ppl_total * pos - block_width - gap_size;
    return rpos;
}

static void create_ppl() {
    set_block_width();
    for(int i = 1; i <= ppl_total; i++){
        //s_ppl[i] = 0;
        sa_name_layer[i] = text_layer_create(
        GRect(get_ppl_pos(i), 0, block_width, block_height));
        create_ppl_layer(sa_name_layer[i], i);
        layer_add_child(text_layer_get_layer(s_people_layer), text_layer_get_layer(sa_name_layer[i]));
        setup_ppl_char(i, s_ppl_name[i], s_ppl[i], sa_name_layer[i], false);
    }
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

static void set_colors() {
    text_layer_set_text(s_cond_layer, s_cond);
    window_set_background_color(s_main_window, COLOR_FALLBACK(bg, bwbg));
    set_all_text_layer(text);
    for(int i = 1; i <= ppl_total; i++){
        if (strcmp(s_ppl[i], "0")==0){
            text_layer_set_background_color(sa_name_layer[i], COLOR_FALLBACK(bg, GColorWhite));
            text_layer_set_text_color(sa_name_layer[i], text);
        } else {
            text_layer_set_background_color(sa_name_layer[i], COLOR_FALLBACK(text, bwbg));
            text_layer_set_text_color(sa_name_layer[i], bg);
        }
    }
}

static void set_cond_color(char *cond_buffer) {
    s_cond="1";
    if (strcmp(cond_buffer, "clear-day")==0) {
        s_cond="1";
        bg = GColorBabyBlueEyes;
        text = GColorDarkGray;
    }
    else if (strcmp(cond_buffer, "partly-cloudy-day")==0) {
        s_cond="3";
        bg = GColorBlue;
        text = GColorLightGray;
    }
    else if (strcmp(cond_buffer, "cloudy")==0) {
        s_cond="5";
        bg = GColorDarkGray;
        text = GColorWhite;
    }
    else if (strcmp(cond_buffer, "clear-night")==0) {
        s_cond="2";
        bg = GColorOxfordBlue;
        text = GColorWhite;
    }
    else if (strcmp(cond_buffer, "rain")==0) {
        s_cond="8";
        bg = GColorOxfordBlue;
        text = GColorPictonBlue;
    }
    else if (strcmp(cond_buffer, "snow")==0) {
        s_cond="#";
        bg = GColorBlack;
        bwbg = GColorWhite;
        text = GColorPictonBlue;
    }
    else if (strcmp(cond_buffer, "sleet")==0) {
        s_cond="$";
        bg = GColorCeleste;
        text = GColorBlue;
    }
    else if (strcmp(cond_buffer, "wind")==0) {
        s_cond="F";
        bg = GColorBlack;
        bwbg = GColorWhite;
        text = GColorBlack;
    }
    else if (strcmp(cond_buffer, "fog")==0) {
        s_cond="M";
        bg = GColorLightGray;
        text = GColorBlack;
    }
    else if (strcmp(cond_buffer, "partly-cloudy-night")==0) {
        s_cond="4";
        bg = GColorOxfordBlue;
        text = GColorLightGray;
    }
    else {
        s_cond=")";
    }
}

//static GFont s_date_font;
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    text_layer_set_text(s_shit_layer, "");
    // Store incoming information
    static char batt_buffer[8];
    static char refresh_buffer[8];
    static char vibration_buffer[8];
    static char ppl_buffer[8];
    static char ppl_total_buffer[8];
    static char buffer[6][8];
    Tuple *name_tuple[6];
    Tuple *value_tuple[6];

    Tuple *temp_tuple = dict_find(iterator, KEY_TEMP);
    Tuple *hightemp_tuple = dict_find(iterator, KEY_HIGHTEMP);
    Tuple *lowtemp_tuple = dict_find(iterator, KEY_LOWTEMP);
    Tuple *cond_tuple = dict_find(iterator, KEY_COND);
    Tuple *batt_tuple = dict_find(iterator, KEY_BATT);
    Tuple *refresh_tuple = dict_find(iterator, KEY_REFRESH);
    Tuple *vibration_tuple = dict_find(iterator, KEY_VIBRATE);
    Tuple *ppl_tuple = dict_find(iterator, KEY_PPL);
    Tuple *ppl_total_tuple = dict_find(iterator, KEY_PPL_TOTAL);

    if(ppl_total_tuple){
        snprintf(ppl_total_buffer, sizeof(ppl_total_buffer), "%s", ppl_total_tuple->value->cstring);
        if (ppl_total != 0) {
            if (atoi(ppl_total_buffer) != ppl_total){
                //deinit before changing ppl_total
                deinit_ppl();
                ppl_total = atoi(ppl_total_buffer);
                create_ppl();
                //if number of people change, then have to wait till next refresh
            }
        }

    }

    for(int i = 1; i <= ppl_total; i++){
        name_tuple[i] = dict_find(iterator, i-1);
        value_tuple[i] = dict_find(iterator, i+9);
        set_ppl(i,buffer[i], name_tuple[i], value_tuple[i], sa_name_layer[i]);
    }

    if(s_changed){
        APP_LOG(APP_LOG_LEVEL_INFO, "changed");
        s_changed = false;
        if(s_vibrate){
            vibes_double_pulse();
        }
    }

    if(vibration_tuple){
        snprintf(vibration_buffer, sizeof(vibration_buffer), "%s", vibration_tuple->value->cstring);
        if (strcmp(vibration_buffer, "true")==0){
            s_vibrate = true;
        } else {
            s_vibrate = false;
        }
    }

    if(refresh_tuple){
        snprintf(refresh_buffer, sizeof(refresh_buffer), "%s", refresh_tuple->value->cstring);
        if (atoi(refresh_buffer) != s_interval){
            s_interval = atoi(refresh_buffer);
            update_time();
            tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
        }
    }

    if(ppl_tuple){
        snprintf(ppl_buffer, sizeof(ppl_buffer), "%s", ppl_tuple->value->cstring);
        if (strcmp(ppl_buffer, "true")==0){
            deinit_ppl();
            ppl_total=0;
        } else {
            ppl_total = persist_exists(KEY_PPL_TOTAL) ? persist_read_int(KEY_PPL_TOTAL) : 4;
            create_ppl();
        }
    }
    if(batt_tuple){
        snprintf(batt_buffer, sizeof(batt_buffer), "%s", batt_tuple->value->cstring);
        text_layer_set_text(s_batt_layer, batt_buffer);
    }

    if (cond_tuple){
        snprintf(cond_buffer, sizeof(cond_buffer), "%s", cond_tuple->value->cstring);
        bwbg = GColorBlack;
        snprintf(s_prev_cond, sizeof(s_prev_cond), "%s", s_cond);

        set_cond_color(cond_buffer);

        if (strcmp(s_cond, s_prev_cond)!=0) {
            APP_LOG(APP_LOG_LEVEL_INFO, "set colors");
            set_colors();
        }
    }


    if(temp_tuple){
        snprintf(temp_buffer, sizeof(temp_buffer), "%d°", (int)temp_tuple->value->int32);
        text_layer_set_text(s_temp_layer, temp_buffer);
    }
    // If all data is available, use it
    if(hightemp_tuple && lowtemp_tuple) {
        snprintf(hightemp_buffer, sizeof(hightemp_buffer), "%d", (int)hightemp_tuple->value->int32);
        snprintf(lowtemp_buffer, sizeof(lowtemp_buffer), "%d", (int)lowtemp_tuple->value->int32);
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


static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    s_sizew = PBL_IF_ROUND_ELSE(130, bounds.size.w);
    s_time_layer = text_layer_create(
            GRect(0, 0, bounds.size.w, PBL_IF_ROUND_ELSE(75, 80)));
    text_layer_set_background_color(s_time_layer, GColorClear);
    //text_layer_set_text_color(s_time_layer, GColorWhite);
    text_layer_set_text(s_time_layer, "00:00");
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    // Create Fonts
    s_time_font = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_DIN_40, RESOURCE_ID_FONT_DIN_40)));
    s_people_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_BOLD_25));
    s_dinsmall_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_20));
    s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_METEOCONS_35));
    s_batt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIN_15));

    text_layer_set_font(s_time_layer, s_time_font);
    // Add it as a child layer to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    // Create date TextLayer
    s_date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(68, 55), bounds.size.w, 30));
    //text_layer_set_text_color(s_date_layer, GColorWhite);
    text_layer_set_background_color(s_date_layer, GColorClear);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    text_layer_set_font(s_date_layer, s_people_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
    //create People Layer
    s_people_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(25, 0), 138, bounds.size.w, block_height));
    text_layer_set_background_color(s_people_layer, GColorClear);
    text_layer_set_text_alignment(s_people_layer, GTextAlignmentCenter);
    text_layer_set_text_color(s_people_layer, text);
    text_layer_set_text(s_people_layer, "");
    text_layer_set_font(s_people_layer, s_people_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_people_layer));
    if (ppl_total != 0) {
        create_ppl();
    }
    //Create Weather Layer. Makes stuff easier.
    s_weather_layer = text_layer_create(GRect(0,PBL_IF_ROUND_ELSE(80, 70), bounds.size.w, 65));
    text_layer_set_background_color(s_weather_layer, GColorClear);
    text_layer_set_text_color(s_weather_layer, text);
    text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
    text_layer_set_text(s_weather_layer, "");
    text_layer_set_font(s_weather_layer, s_people_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
    //create temperature Layer
    s_temp_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(25, 10), 18, 50, 30));
    text_layer_set_background_color(s_temp_layer, GColorClear);
    //text_layer_set_text_color(s_temp_layer, GColorWhite);
    text_layer_set_text_alignment(s_temp_layer, GTextAlignmentLeft);
    text_layer_set_text(s_temp_layer, temp_buffer);
    text_layer_set_font(s_temp_layer, s_people_font);
    layer_add_child(text_layer_get_layer(s_weather_layer), text_layer_get_layer(s_temp_layer));
    //create conditions Layer
    s_cond_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(65, 47), 16, 50, 50));
    text_layer_set_background_color(s_cond_layer, GColorClear);
    //text_layer_set_text_color(s_cond_layer, GColorWhite);
    text_layer_set_text_alignment(s_cond_layer, GTextAlignmentCenter);
    text_layer_set_text(s_cond_layer, s_cond);
    text_layer_set_font(s_cond_layer, s_weather_font);
    layer_add_child(text_layer_get_layer(s_weather_layer), text_layer_get_layer(s_cond_layer));
    //create high/low temp Layer
    s_hilo_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(115, 93), 12, 50, 60));
    text_layer_set_background_color(s_hilo_layer, GColorClear);
    //text_layer_set_text_color(s_hilo_layer, GColorWhite);
    text_layer_set_text_alignment(s_hilo_layer, GTextAlignmentCenter);
    text_layer_set_text(s_hilo_layer, hilo_buffer);
    text_layer_set_font(s_hilo_layer, s_dinsmall_font);
    layer_add_child(text_layer_get_layer(s_weather_layer), text_layer_get_layer(s_hilo_layer));
    //create battery Layer
    s_batt_layer = text_layer_create(GRect(53, 5, 40, 20));
    text_layer_set_background_color(s_batt_layer, GColorClear);
    //text_layer_set_text_color(s_batt_layer, GColorWhite);
    text_layer_set_text_alignment(s_batt_layer, GTextAlignmentCenter);
    text_layer_set_text(s_batt_layer, "");
    text_layer_set_font(s_batt_layer, s_batt_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_batt_layer));
    //create shit Layer
    s_shit_layer = text_layer_create(
            GRect(0, 90, bounds.size.w, 30));
    text_layer_set_background_color(s_shit_layer, GColorClear);
    //text_layer_set_text_color(s_shit_layer, GColorWhite);
    text_layer_set_text_alignment(s_shit_layer, GTextAlignmentCenter);
    text_layer_set_text(s_shit_layer, s_shit);
    text_layer_set_font(s_shit_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_shit_layer));

    set_colors();
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

    deinit_ppl();
    text_layer_destroy(s_people_layer);
    text_layer_destroy(s_temp_layer);
    text_layer_destroy(s_cond_layer);
    text_layer_destroy(s_hilo_layer);
    text_layer_destroy(s_batt_layer);
    text_layer_destroy(s_shit_layer);
    text_layer_destroy(s_weather_layer);
}

static void get_values() {
    s_vibrate = persist_exists(KEY_VIBRATE) ? persist_read_int(KEY_VIBRATE) : true;
    s_interval = persist_exists(KEY_REFRESH) ? persist_read_int(KEY_REFRESH) : 5;
    ppl_total = persist_exists(KEY_PPL_TOTAL) ? persist_read_int(KEY_PPL_TOTAL) : 4;

    for(int i = 1; i <= ppl_total; i++){
        if (persist_exists(i+9)) {
            persist_read_data(i+9, s_ppl[i], sizeof(s_ppl[i]));
        } else {
            snprintf(s_ppl[i], sizeof(s_ppl[i]), "%s", "0");
        }
        if (persist_exists(i-1)) {
            persist_read_data(i-1, &s_ppl_name[i], sizeof(&s_ppl_name[i]));
        } else {
            snprintf(s_ppl_name[i], sizeof(s_ppl_name[i]), "%s", "");
        }
    }
    if (persist_exists(KEY_TEMP)) {
        persist_read_data(KEY_TEMP, &temp_buffer, sizeof(temp_buffer));
    }

    if (persist_exists(KEY_LOWTEMP)) {
        persist_read_data(KEY_LOWTEMP, &lowtemp_buffer, sizeof(lowtemp_buffer));
        if (persist_exists(KEY_HIGHTEMP)) {
            persist_read_data(KEY_HIGHTEMP, &hightemp_buffer, sizeof(hightemp_buffer));
        }
        snprintf(hilo_buffer, sizeof(hilo_buffer), "%s°\n%s°", hightemp_buffer, lowtemp_buffer);
    }


    //char cond[64];
    if (persist_exists(KEY_COND)) {
        persist_read_data(KEY_COND, &cond_buffer, sizeof(cond_buffer));
        set_cond_color(cond_buffer);
        //set_colors();
    } else {
        s_cond=")";
        text = GColorDarkGray;
        bg = GColorBabyBlueEyes;
        bwbg = GColorBlack;
        s_shit="Loading...";
    }

}

static void init() {
    get_values();

    // Create main Window element and assign to pointer
    s_main_window = window_create();
    //window_set_background_color(s_main_window, PBL_IF_BW_ELSE(GColorBlack, GColorBabyBlueEyes));
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
    //set_colors();
}
static void deinit() {
    persist_write_int(KEY_REFRESH, s_interval);
    persist_write_int(KEY_PPL_TOTAL, ppl_total);
    persist_write_int(KEY_VIBRATE, s_vibrate);
    if (persist_exists(KEY_COND)) { persist_delete(KEY_COND); }
    persist_write_string(KEY_COND, cond_buffer);
    if (persist_exists(KEY_HIGHTEMP)) { persist_delete(KEY_HIGHTEMP); }
    persist_write_string(KEY_HIGHTEMP, hightemp_buffer);
    if (persist_exists(KEY_LOWTEMP)) { persist_delete(KEY_LOWTEMP); }
    persist_write_string(KEY_LOWTEMP, lowtemp_buffer);
    if (persist_exists(KEY_TEMP)) { persist_delete(KEY_TEMP); }
    persist_write_string(KEY_TEMP, temp_buffer);

    for(int i = 1; i <= ppl_total; i++){
        if (persist_exists(i-1)) { persist_delete(i-1); }
        persist_write_string(i-1, s_ppl_name[i]);
        if (persist_exists(i+9)) { persist_delete(i+9); }
        persist_write_string(i+9, s_ppl[i]);
    }
    window_destroy(s_main_window);
}
int main(void) {
    init();
    app_event_loop();
    deinit();
}
