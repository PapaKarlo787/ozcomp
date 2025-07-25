#define Rp 15
#define Gp 18
#define Bp 19
#define ip S[0]
#define sp S[1]
#define flags S[2]
float Rf[16];
uint32_t R[16];
uint32_t S[16];
uint8_t r1, r2;
PS2Keyboard kbd(10, 2, 8);
RealScreen lcd;
iarduino_RTC timeUnix;


union INT_FLOAT {
	float f;
	uint32_t i;
};

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
