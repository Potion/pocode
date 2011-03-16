#pragma once
#include "poEnums.h"

class poWindow;

void makeNSWindow(poWindow *poWin, void **win, void **glview, int x, int y, int w, int h, poWindowType type);
void destroyNSWindow(void *win, void *glview);
void moveNSWindow(void *win, void *glview, int x, int y, int w, int h);
void setNSWindowFullscreen(void *win, void *glview, bool fullscreen);