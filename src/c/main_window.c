#include <pebble.h>
#include "main_window.h"
#include "error_window.h"


Window *mainWindow;
MenuLayer *mainMenuLayer;

uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 2;

}

uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index){
        case 0:
            return 1;
        break;
        case 1:
            return 1;
        break;
        default:
            return 0;
    }
    
}

int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    switch(section_index){
        case 0:
           menu_cell_basic_header_draw(ctx, cell_layer, "Cities");
        break;
        case 1:
           menu_cell_basic_header_draw(ctx, cell_layer, "Other");
        break;
    }
}

void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch(cell_index->section) {
       case 0:
           switch(cell_index->row){
               case 0:
                  menu_cell_basic_draw(ctx, cell_layer, "Gymji", "naše škola", NULL); // null je na miste ikony
               break;
           }
        break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, "Add City", NULL, NULL);
        break;
    }
    
}

void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    error_window_set_error("Vybráno menu!");	
    error_window_show();	
    
    // interaction with JS, getting user defined message key "testkey" defined in settings
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    if (iter == NULL) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "ITer is Null! Refusing to send");
        return;
    }
    dict_write_uint16(iter, MESSAGE_KEY_testkey, 713);
    app_message_outbox_send();
}

void setup_menu_layer(Window *window) {
	Layer *window_layer = window_get_root_layer(window);

    mainMenuLayer = menu_layer_create(GRect(0, 0, 144, 168));
    menu_layer_set_callbacks(mainMenuLayer, NULL, (MenuLayerCallbacks){
        .get_num_sections = menu_get_num_sections_callback,
        .get_num_rows = menu_get_num_rows_callback,
        .get_header_height = menu_get_header_height_callback,
        .draw_header = menu_draw_header_callback,
        .draw_row = menu_draw_row_callback,
        .select_click = menu_select_callback,
    });

    menu_layer_set_click_config_onto_window(mainMenuLayer, window);

    layer_add_child(window_layer, menu_layer_get_layer(mainMenuLayer));
}

// processing tupple from JS
void process_tuple(Tuple *t){
    int key = t->key;
    int value = t->value->int32;
    APP_LOG(APP_LOG_LEVEL_INFO, "Got key %d with value %d", key, value);
}

// function run when message from JS was received
void message_inbox(DictionaryIterator *iter, void *context){
    Tuple *t = dict_read_first(iter);
    if(t){
        process_tuple(t);
    }
    while(t != NULL){
        t = dict_read_next(iter);
        if(t){
            process_tuple(t);
        }
    }
}

// function called when messege from JS was dropped
void message_inbox_dropped(AppMessageResult reason, void *context){
    APP_LOG(APP_LOG_LEVEL_INFO, "Message dropped, reason %d.", reason);
}

void main_window_load(Window  *window){
    setup_menu_layer(window);
   
    // start interaction with JS side on Phone
    app_message_register_inbox_received(message_inbox);
    app_message_register_inbox_dropped(message_inbox_dropped);
    app_message_open(256, 256);
}

void main_window_unload(Window  *window){
    menu_layer_destroy(mainMenuLayer);
    
}

void main_window_create() {
    mainWindow = window_create();
    
    // adding handlers
    window_set_window_handlers(mainWindow, (WindowHandlers){
        .load = main_window_load,
        .unload = main_window_unload
    });
}

void main_window_destroy()  {
    window_destroy(mainWindow);
}

Window *main_window_get_window() {
    return mainWindow;
}
