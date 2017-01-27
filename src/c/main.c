#include <pebble.h>
#include "splash_window.h"
#include "main_window.h"
#include "error_window.h"

void launch_main_window() {
    window_stack_push(main_window_get_window(), true);
}

int main(){
    splash_window_create();
    main_window_create();
    error_window_create();
    
    window_stack_push(splash_window_get_window(), true);
    AppTimer *splash_window_timer = app_timer_register(2000, launch_main_window, NULL);
    
    app_event_loop();
    
    error_window_destroy();
    splash_window_destroy();
    main_window_destroy();
}