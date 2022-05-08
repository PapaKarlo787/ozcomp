#define Rp 15
#define Gp 18
#define Bp 19
#define ip (*(uint32_t*)&screen_buffer[504])
#define sp (*(uint32_t*)&screen_buffer[508])
#define flags (*(uint32_t*)&screen_buffer[512])
#define screen_buffer lcd.screen_buffer
float Rf[16];
uint32_t R[16];
uint8_t r1, r2;
PS2 keyboard(3, 8);
PCD8544 lcd;
iarduino_RTC timeUnix;
#include "system_functions.h"
//realization of instructions
#include "float_ariphmetics.h"
#include "int_ariphmetics.h"
#include "moves.h"
#include "screen.h"
#include "inputs.h"
#include "logics.h"
#include "jumps_and_stack.h"
#include "geometry.h"
#include "bmp.h"
#include "fmoves.h"
#include "other_stuff.h"
#include "tones.h"
//end block
#include "instructions.h"
