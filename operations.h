/******************************
    Operation Definitions
*******************************/

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------- */

struct SICXE_Operation{
	char *Mnemonic;
	int  Opcode;
	int  SIC_XE_Specific;
	int  Format;
	void *next;
};

typedef struct SICXE_Operation SICXE_OP;

/* ------------------------------------------------- */
/*            Mnemonic Opcode XE Format Next  */
SICXE_OP ADD  = {"ADD", 0x18, 0, 7, NULL};
SICXE_OP ADDF = {"ADDF", 0x58, 1, 7, &ADD};
SICXE_OP ADDR = {"ADDR", 0x90, 1, 2, &ADDF};
/* ---------------------------------------- */
SICXE_OP AND  = {"AND", 0x40, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP CLEAR = {"CLEAR", 0xB4, 1, 2, NULL};
/* ---------------------------------------- */
SICXE_OP COMP = {"COMP", 0x28, 0, 7, NULL};
SICXE_OP COMPF = {"COMPF", 0x88, 1, 7, &COMP};
SICXE_OP COMPR = {"COMPR", 0xA0, 1, 2, &COMPF};
/* ---------------------------------------- */
SICXE_OP DIV  = {"DIV", 0x24, 0, 7, NULL};
SICXE_OP DIVF = {"DIVF", 0x64, 1, 7, &DIV};
SICXE_OP DIVR = {"DIVR", 0x9C, 1, 2, &DIVF};
/* ---------------------------------------- */
SICXE_OP FIX  = {"FIX", 0xC4, 1, 1, NULL};
/* ---------------------------------------- */
SICXE_OP FLOAT = {"FLOAT", 0xC0, 1, 1, NULL};
/* ---------------------------------------- */
SICXE_OP HIO  = {"HIO", 0xF4, 1, 1, NULL};
/* ---------------------------------------- */
SICXE_OP J    = {"J",   0x3C, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP JEQ  = {"JEQ", 0x30, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP JGT  = {"JGT", 0x34, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP JLT  = {"JLT", 0x38, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP JSUB = {"JSUB", 0x48, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP LDA  = {"LDA", 0x00, 0, 7, NULL};
SICXE_OP LDB  = {"LDB", 0x68, 1, 7, &LDA};
SICXE_OP LDCH = {"LDCH", 0x50, 0, 7, &LDB};
SICXE_OP LDF  = {"LDF", 0x70, 1, 7, &LDCH};
SICXE_OP LDL  = {"LDL", 0x08, 0, 7, &LDF};
SICXE_OP LDS  = {"LDS", 0x6c, 1, 7, &LDL};
SICXE_OP LDT  = {"LDT", 0x74, 1, 7, &LDS};
SICXE_OP LDX  = {"LDX", 0x04, 0, 7, &LDT};
/* ---------------------------------------- */
SICXE_OP LPS  = {"LPS", 0xD0, 1, 7, NULL};
/* ---------------------------------------- */
SICXE_OP MUL  = {"MUL", 0x20, 0, 7, NULL};
SICXE_OP MULF = {"MULF", 0x60, 1, 7, &MUL};
SICXE_OP MULR = {"MULR", 0x98, 1, 2, &MULF}; 
/* ---------------------------------------- */
SICXE_OP NORM = {"NORM", 0xC8, 1, 1, NULL};
/* ---------------------------------------- */
SICXE_OP OR   = {"OR", 0x44, 0, 1, NULL};
/* ---------------------------------------- */
SICXE_OP RD   = {"RD", 0xD8, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP RMO  = {"RMO", 0xAC, 1, 2, NULL};
/* ---------------------------------------- */
SICXE_OP RSUB = {"RSUB", 0x4C, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP SHIFTL = {"SHIFTL", 0xA4, 1, 2, NULL};
SICXE_OP SHIFTR = {"SHIFTR", 0xA8, 1, 2, &SHIFTL};
/* ---------------------------------------- */
SICXE_OP SIO  = {"SIO", 0xF0, 1, 1, NULL};
/* ---------------------------------------- */
SICXE_OP SSK  = {"SSK", 0xEC, 1, 7, NULL};
/* ---------------------------------------- */
SICXE_OP STA  = {"STA", 0x0C, 0, 7, NULL};
SICXE_OP STB  = {"STB", 0x78, 1, 7, &STA};
SICXE_OP STCH = {"STCH", 0x54, 0, 7, &STB};
SICXE_OP STF  = {"STF", 0x80, 1, 7, &STCH};
SICXE_OP STI  = {"STI", 0xD4, 1, 7, &STF};
SICXE_OP STL  = {"STL", 0x14, 0, 7, &STI};
SICXE_OP STS  = {"STS", 0x7C, 1, 7, &STL};
SICXE_OP STSW = {"STSW", 0xE8, 0, 7, &STS};
SICXE_OP STT  = {"STT", 0x84, 1, 7, &STSW};
SICXE_OP STX  = {"STX", 0x10, 0, 7, &STT}; 
/* ---------------------------------------- */
SICXE_OP SUB  = {"SUB", 0x1C, 0, 7, NULL};
SICXE_OP SUBF = {"SUBF", 0x5C, 1, 7, &SUB};
SICXE_OP SUBR = {"SUBR", 0x94, 1, 2, &SUBF};
/* ---------------------------------------- */
SICXE_OP SVC  = {"SVC", 0xB0, 1, 2, NULL};
/* ---------------------------------------- */
SICXE_OP TD   = {"TD", 0xE0, 0, 7, NULL};
/* ---------------------------------------- */
SICXE_OP TIO  = {"TIO", 0xF8, 0, 1, NULL};
SICXE_OP TIX  = {"TIX", 0x2C, 0, 7, &TIO};
SICXE_OP TIXR = {"TIXR", 0xB8, 0, 2, &TIX};
/* ---------------------------------------- */
SICXE_OP WD   = {"WD", 0xDC, 0, 7, NULL};

/* ------------------------------------------------- */
/* OPTAB */

SICXE_OP *OPTAB[] = {&ADDR, &AND, &CLEAR, &COMPR,
					 &DIVR, &FIX, &FLOAT, &HIO  ,
					 &J   , &JEQ, &JGT  , &JLT  ,
					 &JSUB, &LDX, &LPS  , &MULR ,
					 &NORM, &OR,  &RD   , &RMO  , 
					 &RSUB, &SHIFTR, &SIO, &SSK ,
					 &STX,  &SUBR , &SVC,  &TD  , 
					 &TIXR, &WD };
					 
int size = sizeof(OPTAB) / sizeof(SICXE_OP *);
