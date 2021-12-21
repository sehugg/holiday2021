
#include "cpctelera.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#link "snowbmp.s"

// background bitmap exported from Dithertron
// photo by Yeo Khee https://unsplash.com/photos/27cA1PpSd74
extern const char BITMAP[];

// assembly code exported from Dithertron
// draws the background bitmap
// and sets the palette colors
extern void LoadBitmap();

// data structure for a single snowflake
typedef struct Flake {
  u8 x,y; // x,y position
  u8 yinc;
  u8 pixbyte;
} Flake;

// how many snowflakes?
#define NUMFLAKES 150

// 2-pixel byte that draws a single snowflake
const u8 SNOWFLAKES[3] = { 0, 0xfc, 0xb9 };

// array of snowflakes
Flake flakes[NUMFLAKES];

// initialize a snowflake at a random position
void InitFlake(Flake* f) {
  f->x = rand();
  if (f->x >= 160) f->x -= 160; // force to 0..159
  f->y = rand();
  f->yinc = (rand() & 1) + 1;
  f->pixbyte = SNOWFLAKES[f->yinc];
}

// erase snowflake --
// copy byte from background bitmap to screen
void EraseFlake(Flake* f) {
  char* ptr;
  if (f->y < 200) {
    ptr = cpct_getScreenPtr ((void*)0x0, f->x, f->y);
    *(ptr + 0xc000) = *(ptr + (u16)BITMAP);
  }
}

// set snowflake byte on screen
void DrawFlake(Flake* f) {
  char* ptr;
  if (f->y < 200) {
    ptr = cpct_getScreenPtr ((void*)0xc000, f->x, f->y);
    *ptr = f->pixbyte;
  }
}

// make snowflake fall downward
void MoveFlake(Flake* f) {
  f->y += f->yinc;
}

// init snowflakes and make it snow forever
void MakeItSnow(void) {
  for (int i=0; i<NUMFLAKES; i++) {
    Flake* f = &flakes[i];
    InitFlake(f);
  }
  while (1) {
    for (int i=0; i<NUMFLAKES; i++) {
      Flake* f = &flakes[i];
      EraseFlake(f);
      MoveFlake(f);
      DrawFlake(f);
    }
  }
}

// main routine
void main(void) {
  
  cpct_setVideoMode(0);
  
  LoadBitmap();

  cpct_disableFirmware ();
  cpct_disableUpperROM ();

  MakeItSnow();
}
