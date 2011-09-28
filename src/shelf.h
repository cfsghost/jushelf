#ifndef JSH_SHELF_H
#define JSH_SHELF_H

JshShelf *jsh_shelf_new();
void      jsh_shelf_init(JshShelf *shelf);
void      jsh_shelf_update_place(JshShelf *shelf);
void      jsh_shelf_reset_place(JshShelf *shelf, gfloat offset);
void      jsh_shelf_window_init(JshShelf *shelf);

#endif
