#ifndef JSH_WINDOW_H
#define JSH_WINDOW_H

void jsh_window_set_type(Display *disp, Window w, JshWindowType wtype);
void jsh_window_set_decorator(Display *disp, Window w, gboolean enabled);

#endif
