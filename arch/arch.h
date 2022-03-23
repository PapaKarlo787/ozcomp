#define Rp 15
#define Gp 18
#define Bp 19
uint32_t flags;
uint32_t ip = 0;
uint32_t sp = 0;
float Rf[16];
uint32_t R[16];
uint8_t r1, r2;
uint8_t color = 255;
PS2Keyboard keyboard;
PCD8544 lcd;
iarduino_RTC timeUnix;
#define screen_buffer lcd.screen_buffer
#include "system_functions.h"
//realization of instructions
#include "float_ariphmetics.h"
#include "int_ariphmetics.h"
#include "moves.h"
#include "screen_kbd.h"
#include "logics.h"
#include "jumps_and_stack.h"
#include "geometry.h"
#include "bmp.h"
#include "fmoves.h"
#include "other_stuff.h"
#include "tones.h"
//end block
#include "instructions.h"
