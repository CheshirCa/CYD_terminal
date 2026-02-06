/*
 * display.h - Display and touch screen management
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "config.h"

extern TFT_eSPI tft;

// Display functions
void displayInit();

// Touch functions
bool getTouch(uint16_t *x, uint16_t *y);

#endif
