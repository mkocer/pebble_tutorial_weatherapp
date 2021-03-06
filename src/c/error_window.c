#include <pebble.h>
#include "error_window.h"

Window *errorWindow;
Layer *errorGraphicLayer;
static char currentErrorText[1][20];

void error_window_set_error(char *errorText) {
    strncpy(currentErrorText[0], errorText, sizeof(currentErrorText[0]));
}

void error_window_show() {
    if (errorGraphicLayer) {
        layer_mark_dirty(errorGraphicLayer);
    }
    window_stack_push(errorWindow, true);
}

void error_graphic_proc(Layer *layer, GContext *ctx){
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_draw_text(ctx, currentErrorText[0], 
                      fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
                      GRect(0,0,144,50),
                      GTextOverflowModeWordWrap,
                      GTextAlignmentCenter,
                      NULL
                     );
    graphics_context_set_stroke_color(ctx, GColorRed);
    graphics_context_set_stroke_width(ctx, 3);
    // draw two lines like X
    graphics_draw_line(ctx, GPoint(10,30), GPoint(124, 138));
    graphics_draw_line(ctx, GPoint(124,30), GPoint(10, 138));
    // draw filled circle
    graphics_context_set_fill_color(ctx, GColorBlue);
    graphics_fill_circle(ctx, GPoint(67,84), 10);
}

void error_window_load(Window  *window){
    Layer *window_layer = window_get_root_layer(window);
    
    errorGraphicLayer = layer_create(GRect(0,0,144,168));
    layer_set_update_proc(errorGraphicLayer, error_graphic_proc);
    layer_add_child(window_layer, errorGraphicLayer);
}

void error_window_unload(Window  *window){
    
}

void error_window_create() {
    errorWindow = window_create();
    
    // adding handlers
    window_set_window_handlers(errorWindow, (WindowHandlers){
        .load = error_window_load,
        .unload = error_window_unload
    });
}

void error_window_destroy()  {
    window_destroy(errorWindow);
    errorGraphicLayer = NULL;
}

Window *error_window_get_window() {
    return errorWindow;
}