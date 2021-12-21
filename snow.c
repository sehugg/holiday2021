
#include "cpctelera.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#link "snowbmp.s"

// bitmap bytes (from Dithertron)
extern const char BITMAP[];

// ported from Dithertron, draws the background bitmap
// and sets the palette colors
extern void LoadBitmap();

// data structure for a single snowflake
typedef struct Flake {
  u8 x,y; // x,y position
} Flake;

// how many snowflakes?
#define NUMFLAKES 120

// 2-pixel byte that draws a single snowflake
#define SNOWFLAKE_BYTE 0xb9

// array of snowflakes
Flake flakes[NUMFLAKES];

// initialize a snowflake at a random position
void InitFlake(Flake* f) {
  f->x = rand();
  f->y = rand();
}

// erase snowflake --
// copy byte from background bitmap to screen
void EraseFlake(Flake* f) {
  char* ptr;
  if (f->y < 192) {
    ptr = cpct_getScreenPtr ((void*)0x0, f->x, f->y);
    *(ptr + 0xc000) = *(ptr + (u16)BITMAP);
  }
}

// set snowflake byte on screen
void DrawFlake(Flake* f) {
  char* ptr;
  if (f->y < 192) {
    ptr = cpct_getScreenPtr ((void*)0xc000, f->x, f->y);
    *ptr = SNOWFLAKE_BYTE;
  }
}

// make snowflake fall downward
void MoveFlake(Flake* f) {
  f->y++;
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
