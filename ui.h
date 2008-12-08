#ifndef UI_H

#include <curses.h>
#include "items.h"

void InitUI();
void UILoop();
void GetUserInput(WINDOW *, int, int, char *, int, char *);
void ShowMessage(char *);
void ShowStatus(float, int, int, char *, char *, char *);
void ShowItems(ItemSection *);
void EndUI();

#endif
