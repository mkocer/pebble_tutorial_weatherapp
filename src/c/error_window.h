#pragma once

void error_window_create();
void error_window_destroy(); 
void error_window_show();
void error_window_set_error(char *errorText);
Window *error_window_get_window();
