
#include "cpctelera.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#link "snowbmp.s"

extern const char BITMAP[];
extern void LoadBitmap();

typedef struct Flake {
  u8 x,y;
} Flake;

#define NUMFLAKES 120

Flake flakes[NUMFLAKES];

void InitFlake(Flake* f) {
  f->x = rand();
  f->y = rand();
}

// copy byte from source bitmap to screen
void EraseFlake(Flake* f) {
  char* ptr;
  if (f->y < 192) {
    ptr = cpct_getScreenPtr ((void*)0x0, f->x, f->y);
    *(ptr + 0xc000) = *(ptr + (u16)BITMAP);
  }
}

// set byte on screen
void DrawFlake(Flake* f) {
  char* ptr;
  if (f->y < 192) {
    ptr = cpct_getScreenPtr ((void*)0xc000, f->x, f->y);
    *ptr = 0x31;
  }
}

void MoveFlake(Flake* f) {
  f->y++;
}

void MakeItSnow(void) {
  for (int i=0; i<NUMFLAKES; i++) {
    Flake* f = &flakes[i];
    InitFlake(f);
    //XorFlake(f);
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

void main(void) {
  
  cpct_setVideoMode(0);
  
  LoadBitmap();

  cpct_disableFirmware ();
  cpct_disableUpperROM ();

  MakeItSnow();
}
