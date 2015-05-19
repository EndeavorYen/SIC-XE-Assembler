/*************************************************
             SIC/XE Assembler v.3
			  學號 : 7102056032
                姓名 : 顏至寬 
**************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "operations.h"

#define SYMTAB_SIZE 26
#define RETAB_SIZE  30

/* -------------------------------------------------    
	
	Coding 進度, 待處理問題: 
	1. 整理code 
		
 ------------------------------------------------- */

struct symbol{
	char label_Name[10];
	int  address;
};

struct relocation_addr{
	char ADDR[7];
};

typedef struct symbol Symbol;
typedef struct relocation_addr RE_ADDR;

/* ------------------- Common ---------------------- */

void init_Func(void);                // Initialization
int  hash_Func(char *token);         // OPTAB Hash Function 
int  search_OPTAB(SICXE_OP **current);// Searching Operation Table.
void get_OPformat(void);             // get OPCODE format

/* ------------------- PASS1 ----------------------- */

void pass1(void);                    // Pass 1.
void get_format(char *line);         // Get input line and transformate the format we want.
void insert_SYMTAB(char *str);       // insert Symbol Table
int  inst_Length(SICXE_OP *instPtr); // return length of instruction
int  byte_Length(void);              // retrun length of BYTE X or C
void insert_to_file(char *str);      // different printf type

/* ------------------- PASS2 ----------------------- */

void pass2(void);                    // Pass 2.
void get_format2(char *line);        // Get Intermediate file format (PASS2)
int  search_SYMTAB(char *str);       // search Symbol Table
void convert_Object(void);           // convert instruction to object code
int  reg_value(char *str);           // return register value
int  relative_mode(int *disp);       // calculate disp and return relative mode
void print_SYMTAB(void);             // for testing
void print_Status(void);             // for testing
void text_record(void);              // generate text record
void convert_ObToString(void);       // convert int Object code to string
void relocate(int mode);             // Relocation Function

/* ------------------------------------------------- */
/* --------------- Global Variable ----------------- */
/* ------------------------------------------------- */
/* ------------------- Common ---------------------- */

FILE *source_file, *intermediate_file, *object_file;
Symbol SYMTAB[SYMTAB_SIZE];                  // Symbol Table
int program_Length;                          // program Length
int OPCODE_format = 0;                       // OPCODE format
char LOC[10], SYMBOL[10], OPCODE[10], OPERAND1[10], OPERAND2[10];
int  LOC_ox = 0, SYMBOL_ox = 0, OPERAND_ox = 0;
int OPCODE_extended = 0;
SICXE_OP *instPtr;
int  BASE = 0;
int  reg_A = 0, reg_X = 0, reg_L = 0;
int  reg_S = 0, reg_R = 0, reg_F = 0;

/* ------------------- PASS1 ----------------------- */

int LOCCTR = 0, PREV_LOCCTR = 0, line_Counter = 1;
int print_type = 1;

/* ------------------- PASS2 ----------------------- */

int  SYMBOL_ADDR, OPERAND_ADDR, OBJECT_CODE;
int  OBJECT_ox = 1;
char OBJECT_CODEs[10];    // Object code string type
int S_L = 0;              // string length tag
RE_ADDR RETAB[RETAB_SIZE];        // relation address table
char text_line[70] = {};

/* ------------------------------------------------- */
/* ------------------------------------------------- */
/* ------------------------------------------------- */
/* Main Function */

int main(void){
	
	/* do PASS 1 */
	if(!(source_file = fopen("source_code.txt", "r"))){
		printf("SOURCE CODE OPENED ERROR.\n");
		exit(1);
	}
	
	if(!(intermediate_file = fopen("intermediate_file.txt", "w"))){
		printf("INTERMEDIATE FILE CREATED ERROR.\n");
		exit(1);
	}
	
	init_Func();     // Initialization
	
	pass1();
	fclose(source_file);
	fclose(intermediate_file);
	printf("\n\n======================= PASS 1 finished. =======================\n\n\n");
	/* PASS 1 END */
	
	/* do PASS 2 */
	if(!(intermediate_file = fopen("intermediate_file.txt", "r"))){
		printf("INTERMEDIATE FILE OPENED ERROR.\n");
		exit(1);
	}
	
	if(!(object_file = fopen("object_file.txt", "w"))){
		printf("OBJECT FILE CAN NOT BE CREATED.\n");
		exit(1);
	}
	
	pass2();
	
	fclose(intermediate_file);
	fclose(object_file);
	/* PASS 2 END */
	printf("\n\n======================= PASS 2 finished. =======================\n\n\n");
	
	system("PAUSE");
	return 0;
}

/* ------------------------------------------------- */
/* ------------------- Common ---------------------- */
/* ------------------------------------------------- */
/* Initialization */

void init_Func(void){
	
	int i;
	
	/* Initialize the SYMTAB */
	for(i = 0; i < SYMTAB_SIZE; i++){
		strcpy(SYMTAB[i].label_Name, "	");
	}
	
	return ;
}

/* ------------------------------------------------- */
/* hash Function */

int hash_Func(char *token){
	
	int i, x;
	
	if(!strncmp(token, "AD", 2))
		x = 0;
	else if(!strncmp(token, "AN", 2))
		x = 1;
	else if(!strncmp(token, "CL", 2))
		x = 2;
	else if(!strncmp(token, "CO", 2))
		x = 3;
	else if(!strncmp(token, "DI", 2))
		x = 4;
	else if(!strncmp(token, "FI", 2))
		x = 5;
	else if(!strncmp(token, "FL", 2))
		x = 6;
	else if(!strncmp(token, "HI", 2))
		x = 7;
	else if(!strncmp(token, "J", 2))
		x = 8;
	else if(!strncmp(token, "JE", 2))
		x = 9;
	else if(!strncmp(token, "JG", 2))
		x = 10;
	else if(!strncmp(token, "JL", 2))
		x = 11;
	else if(!strncmp(token, "JS", 2))
		x = 12;
	else if(!strncmp(token, "LD", 2))
		x = 13;
	else if(!strncmp(token, "LP", 2))
		x = 14;
	else if(!strncmp(token, "MU", 2))
		x = 15;
	else if(!strncmp(token, "NO", 2))
		x = 16;
	else if(!strncmp(token, "OR", 2))
		x = 17;
	else if(!strncmp(token, "RD", 2))
		x = 18;
	else if(!strncmp(token, "RM", 2))
		x = 19;
	else if(!strncmp(token, "RS", 2))
		x = 20;
	else if(!strncmp(token, "SH", 2))
		x = 21;
	else if(!strncmp(token, "SI", 2))
		x = 22;
	else if(!strncmp(token, "SS", 2))
		x = 23;
	else if(!strncmp(token, "ST", 2))
		x = 24;
	else if(!strncmp(token, "SU", 2))
		x = 25;
	else if(!strncmp(token, "SV", 2))
		x = 26;
	else if(!strncmp(token, "TD", 2))
		x = 27;
	else if(!strncmp(token, "TIXR", 2))
		x = 28;
	else if(!strncmp(token, "WD", 2))
		x = 29;
	else{  //ERROR 
		x = 1;
	}
		
	return x;
}

/* ------------------------------------------------- */
/* Search OPTAB */

int search_OPTAB(SICXE_OP **current){
		
	for(*current = OPTAB[hash_Func(OPCODE)];;){
	
		/* Get Operation Successfully */
		if(!strcmp((*current)->Mnemonic, OPCODE)){   // strcmp return 0 means same string.
			return 1;    // Found 
		}
		else if((*current)->next != NULL){
			*current = (SICXE_OP *)(*current)->next;
			continue;
		}
		else{
			return 0;   // not found
		}		
	}
	
	return 0;
}

/* ------------------------------------------------- */
/* Get OPCODE Format */

void get_OPformat(void){
	
	char *temp;	
	
	OPCODE_extended = 0;  // Initialization extended tag
	
	/* if OPCODE is extended format */
	if(OPCODE[0] == '+'){
		temp = &OPCODE[1];
		strcpy(OPCODE, temp);
		OPCODE_extended = 1;
	}
	
	/* Search OPTAB, if OPCODE is found, then */
	if(search_OPTAB(&instPtr)){
		OPCODE_format = inst_Length(instPtr);	
	}
	/* else if OPCODE is "BYTE" */
	else if(!strcmp(OPCODE, "BYTE")){
		OPCODE_format = 5;
	}
	/* else if OPCODE is "WORD" */
	else if(!strcmp(OPCODE, "WORD")){
		OPCODE_format = 6;
	}
	/* else if OPCODE is "RESB" */
	else if(!strcmp(OPCODE, "RESB")){
		OPCODE_format = 0;
	}
	/* else if OPCODE is "RESW" */
	else if(!strcmp(OPCODE, "RESW")){
		OPCODE_format = 0;
	}
	/* else if OPCODE is "BASE" */
	else if(!strcmp(OPCODE, "BASE")){
		OPCODE_format = 7;
	}
	/* else if OPCODE is "START" */
	else if(!strcmp(OPCODE, "START")){
		OPCODE_format = 0;
	}
	/* else if OPCODE is "END" */
	else if(!strcmp(OPCODE, "END")){
		OPCODE_format = -0;
	}
	/* else (undefined OPCODE) */
	else{
		printf("ERROR. %s is undefined OPCODE.\n", OPCODE);
		exit(0);
	}
	
	return ;
}

/* ------------------------------------------------- */
/* ------------------- PASS 1 ---------------------- */
/* ------------------------------------------------- */
/* PASS 1 */

void pass1(void){
	
	char line[100];
	char orig_line[100];
	//SICXE_OP *instPtr; 
	
	/* Read first input line */
	fgets(line, 100, source_file);
	strcpy(orig_line, line);
	get_format(line);
	
	/* if OPCODE = 'START' then */
	if(!strcmp(OPCODE, "START")){
		/* Save #[OPERAND] as starting address (Relocation -> start at 0)*/
		
		/* Initialize LOCCTR to starting address */
		LOCCTR = 0;    // Enable to be Relocation
		
		/* Write line to intermedite file */
		insert_to_file(orig_line);
		line_Counter++;
		
		/* Read next line */
		fgets(line, 100, source_file);
		
		strcpy(orig_line, line);
		get_format(line);	
	}
	else{
		/* Initialize LOCCTR to 0 */
		LOCCTR = 0;
	}
				
	/* while OPCODE != 'END' do */
	while(strcmp(OPCODE, "END")){
		
		print_type = 1;  // Normal
		PREV_LOCCTR = LOCCTR;
		
		/* if this is not a comment line then */
		if(1){	
			/* if there is a symbol in the LABEL field then */
			if(SYMBOL_ox == 1){
				insert_SYMTAB(SYMBOL);
			}
						
			/* search OPTAB for OPCODE, if found then */
			if(search_OPTAB(&instPtr)){
				/* add instruction length to LOCCTR */
				LOCCTR += (OPCODE_format);
			}
			/* else if OPCODE = 'WORD' */
			else if(!strcmp(OPCODE, "WORD")){
				/* add 3 to LOCCTR */
				LOCCTR += 3;
			}
			/* else if OPCODE = 'RESW' */
			else if(!strcmp(OPCODE, "RESW")){
				/* add 3 * #[OPERAND] to LOCCTR */
				LOCCTR += (3 * atoi(OPERAND1));			
			}
			/* else if OPCODE = 'RESB' */
			else if(!strcmp(OPCODE, "RESB")){
				/* add #[OPERAND] to LOCCTR */
				LOCCTR += atoi(OPERAND1);
			}
			/* else if OPCODE = 'BYTE' */
			else if(!strcmp(OPCODE, "BYTE")){
				/* find length of constant in bytes */
				/* add length to LOCCTR */		
				LOCCTR += (byte_Length());	
			}
			/* else if OPCODE = 'BASE' */
			else if(!strcmp(OPCODE, "BASE")){
				/* find length of constant in bytes */
				/* add length to LOCCTR */			
				print_type = 2;  // Directive
			}
			/* else */	
			else{
				/* Set EORROR flag (invalid operation code) */	
				printf("EORROR! %s is Invalid Operation Code.\n", OPCODE);
				exit(0); 
			}		
		}

		/* Write line to intermediate file */
		insert_to_file(orig_line);

		/* Read next line */
		fgets(line, 100, source_file);
		strcpy(orig_line, line);
		get_format(line);
		line_Counter++;
	}
	
	/* Write last line to intermediate file */
	print_type = 2;     // Directive
	insert_to_file(orig_line);
	
	/* Save (LOCCTR - starting address) as program length */
	program_Length = (LOCCTR - 0);
	
	return ;
}

/* ------------------------------------------------- */
/* Get input file line */

void get_format(char *line){
	
	char *temp, str[100];
	
	/* Copy line */
	strcpy(str, line);
	
	/* check there are SYMBOL? if yes */
	if(str[0] != '\t'){
		/* Get SYMBOL */
		temp = strtok(str, "	");
		strcpy(SYMBOL, temp);
		SYMBOL_ox = 1;
		
		/* Get next partition */
		line += strlen(SYMBOL) + 1;
		strcpy(str, line);
	}	
	/* else */
	else{
		/* Get next partition */
		line++;
		strcpy(str, line);
		SYMBOL_ox = 0;
	}
			
	/* Get OPCODE and format */
	temp = strtok(str, "	");
	strcpy(OPCODE, temp);
	get_OPformat();
	
	/* Get next partition */
	
	if(OPCODE_format == 4){
		line += strlen(OPCODE) + 2;      // exception
	}
	else{
		line += strlen(OPCODE) + 1;
	}
	
	strcpy(str, line);
	
	/* check there are OPERAND? if yes */
	if((str[0] != '\t') || (str[0] != '\n')){
		/* Get OPERAND */
		temp = strtok(str, ",");
		strcpy(OPERAND1, temp);
		temp = strtok(NULL, ",");
		
		/* Have OPERNAD2 ? if yes */
		if(temp != NULL){
			OPERAND_ox = 2;
			strcpy(OPERAND2, temp);
			OPERAND2[strlen(OPERAND2)] = '\0';  // delete new line
		}
		else{
			OPERAND_ox = 1;	
			OPERAND1[strlen(OPERAND1)] = '\0'; // delete new line
		}	
	}
	/* else (No OERAND) */
	else{
		OPERAND_ox = 0;
	}
		
	/* END */
	
	return ;
}

/* ------------------------------------------------- */
/* insert the Symbol Table */

void insert_SYMTAB(char *str){
	
	int x = (int)str[0] % SYMTAB_SIZE;
	
	/* search SYMTAB for LABEL */
	for(;;){
		/* empty SYMTAB slot */
		if(!strcmp(SYMTAB[x].label_Name, "	")){
			strcpy(SYMTAB[x].label_Name, str);
			SYMTAB[x].address = LOCCTR;
			return ;	
		}
		/* else if found then set ERROR flag (duplicate symbol) */
		else if(!strcmp(SYMTAB[x].label_Name, str)){
			printf("str = %s\n", str);
			printf("SYMTAB[%d] = %s\n", x, SYMTAB[x].label_Name);
			printf("ERROR! %s has already declared.\n", SYMBOL);
			exit(0);
		}
		/* not found , insert (LABEL, LOCCTR) into SYMTAB */
		else{
			x = (x + 1) % SYMTAB_SIZE;
		}	
	}
}

/* ------------------------------------------------- */
/* Return Length of instruction */

int  inst_Length(SICXE_OP *instPtr){
	
	int x = instPtr->Format;
	
	switch(x){
		case 1 : 
			return 1;
		case 2 : 
			return 2;
		default :       // x = 7
			if(OPCODE_extended == 1)
				return 4;
			else
				return 3;	
	}
	
	return 0;
}

/* ------------------------------------------------- */
/* Return Length of BYTE Type */

int  byte_Length(void){
	
	char *str1, *str2;
	
	str1 = strtok(OPERAND1, "'");
	str2 = strtok(NULL, "'");
	
	/* X, Hex Interger */
	if(str1[0] == 'X'){
		return 1;
	}
	/* C, string */
	else{
		return strlen(str2);
	}
	
	return 0;
}

/* ------------------------------------------------- */
/* different printf type */

void insert_to_file(char *str){
	
	switch(print_type){
		/* Normal Type */
		case 1 : 
			fprintf(intermediate_file, "%4d	%04X	%s", line_Counter, PREV_LOCCTR, str);
			break;
		/* Directive Type */
		case 2 :
			fprintf(intermediate_file, "%4d		%s", line_Counter, str);
			break;
		default : 
			printf("print type ERROR!");
			exit(1);
			break;
	}
	
	return ;
}

/* ------------------------------------------------- */
/* ------------------- PASS 2 ---------------------- */
/* ------------------------------------------------- */
/* PASS 2 */

void pass2(void){
	
	char line[100];
	
	/* Read first input line form intermediate file */
	fgets(line, 100, intermediate_file);
	get_format2(line);
	
	/* if OPCODE != 'START' then */
	if(strncmp(OPCODE, "START", 5)){	
		printf("ERROR. There are no START entry.\n");
		exit(0);
	}
				
	/* Init text record */
	fprintf(object_file, "H%-6s00%s%05X\n", SYMBOL, LOC, program_Length);
	text_record();
	
	/* Read next input line */
	fgets(line, 100, intermediate_file);
	get_format2(line);
		
	
	/* While OPCODE != 'END' do */
	while(strncmp(OPCODE, "END", 3)){
		/* if this is not a comment line then */
		if(1){  // if there are no comment	
			/* Convert to Object Code */		
			convert_Object();      // convert instruction to object code
			//print_Status();      // for testing
			if(OBJECT_ox == 1)
				text_record();     // generate text record
		}
		
		/* Read Next input line */
		fgets(line, 100, intermediate_file);
		get_format2(line);
	}
		
	/* END While */
	
	/* Write last line into file */
	text_record();
	
	/* Relocation */
	relocate(1);         // write relocatable information to object file
	
	/* Write End record to object program */
	fprintf(object_file, "E000000");
	
	/* END PASS2 */
							
	return ;
}

/* ---------------------------------------------- */
/* Get intermediate file format */

void get_format2(char *line){
	
	char str[100], *temp;
		
	/* Copy original string */
	strcpy(str, line);
	
	/* Get Line # */
	temp = strtok(str, "	");
	
	/* Get Next Partition */
	line += strlen(temp) + 1;
	strcpy(str, line);
	
	/* have Loc ? if yes */
	if(str[0] != '\t'){
		/* Get LOC */
		temp = strtok(str, "	");
		strcpy(LOC, temp);
		LOC_ox = 1;
		
		/* Get next partition */
		line += strlen(LOC) + 1;
		strcpy(str, line);
		
		/* Have SYMBOL ? if Yes */
		if(str[0] != '\t'){
			/* Get SYMBOL */
			temp = strtok(str, "	");
			strcpy(SYMBOL, temp);
			SYMBOL_ox = 1;
			
			/* Get next partion */
			line += strlen(SYMBOL) + 1;
			strcpy(str, line);
		}					
		/* else (have no SYMBOL) */
		else{
			/* Get next partition */
			line++;
			strcpy(str, line);
			SYMBOL_ox = 0;        // no Symbol
		}
	}
	/* Have no LOC and SYMBOL */
	else{
		/* Get next partition */
		line += 2;
		strcpy(str, line);
		LOC_ox = 0;
		SYMBOL_ox = 0;
	}
	
	/* Get OPCODE */
	temp = strtok(str, "	");
	strcpy(OPCODE, temp);
	
	/* Get next partition */
	line += strlen(OPCODE) + 1;
	strcpy(str, line);
	
	/* Have OPERAND ? if yes */
	if(str[0] != '\t'){
		/* Get OPERAND */
		temp = strtok(str, ",");
		strcpy(OPERAND1, temp);
		temp = strtok(NULL, ",");
				
		/* Have OPERNAD2 ? if yes */
		if(temp != NULL){
			OPERAND_ox = 2;
			strcpy(OPERAND2, temp);
			OPERAND2[strlen(OPERAND2) - 1] = '\0';  // delete new line
		}
		else{
			OPERAND_ox = 1;	
			OPERAND1[strlen(OPERAND1) - 1] = '\0'; // delete new line
		}	
	}
	/* else (No OERAND) */
	else{
		OPERAND_ox = 0;
	}
				
	return ;
}

/* ------------------------------------------------- */
/* search the Symbol Table */

int search_SYMTAB(char *str){
	
	int x = (int)str[0] % SYMTAB_SIZE;
	int orig_x = x;
	
	/* search SYMTAB for LABEL */
	for(;;){
		/* if found */
		if(!strcmp(SYMTAB[x].label_Name, str)){
			SYMBOL_ADDR = SYMTAB[x].address;
			return 1;
		}
		/* else if empty slot (not found)*/
		else if(!strcmp(SYMTAB[x].label_Name, "	")){
			return 0;
		}
		/* not found */
		else{
			x++;
			x %= SYMTAB_SIZE;
		}	
	}
}

/* ------------------------------------------------- */
/* search the Symbol Table */

void convert_Object(void){
	
	char *temp;
	int n, i, x, b, p, e;
	int disp = 0;
	int j;
	char str[3];
	
	get_OPformat();
		
	/* if is format 1 */
	if(OPCODE_format == 1){
		OBJECT_CODE = instPtr->Opcode; 	
		OBJECT_ox = 1;          // need to generate OBJECT CODE
		convert_ObToString();   // convert int object code to string.
	}
	/* else if is format 2 */
	else if(OPCODE_format == 2){
		OBJECT_CODE = (instPtr->Opcode) << 8;  // shit left 1 Byte
		
		if(OPERAND_ox == 2){
			OBJECT_CODE += ((reg_value(OPERAND1) << 4) | reg_value(OPERAND2));    // 2 registers
		}
		else{
			OBJECT_CODE += (reg_value(OPERAND1) << 4);                            // 1 register
		}
		
		OBJECT_ox = 1;          // need to generate OBJECT CODE
		convert_ObToString();   // convert int object code to string.
	}	
	/* else (format 3) */
	else if(OPCODE_format == 3){
		
		/* if is Immediate addressing */
		if(OPERAND1[0] == '#'){
			n = 0;
			i = 1;
		} 
		/* else if is Indirect addressing */
		else if(OPERAND1[0] == '@'){
			n = 1;
			i = 0;
		}
		/* else (simple) */
		else{
			i = 1;
			n = 1;
		}
		
		/* Calculate displacement, 
		if OPERAND1 is immediate address constant */
		if((OPERAND1[0] == '#') && (OPERAND1[1] >= '0') && (OPERAND1[1] <= '9')){
			disp = atoi(&OPERAND1[1]);  // we don't need #
			b = 0;
			p = 0;
		}
		/* else if PC relative */
		else if(relative_mode(&disp)){
			b = 0;
			p = 1;
		} 
		/* else (BASE relative) */
		else{
			b = 1;
			p = 0;
		}
		
		/* if is index address */
		if((OPERAND_ox == 2) && (OPERAND2[0] == 'X')){
			x = 1;
		}
		/* else (not index address) */
		else{
			x = 0;
		}
		
		e = 0;    // format 3
		
		/* exception RSUB <-- L */
		if(!strcmp(OPCODE, "RSUB")){
			n = 1;
			i = 1;
			x = 0;
			b = 0;
			p = 0;
			e = 0;
			disp = reg_L;
		}
		
		/* generate object code */
		OBJECT_CODE = (instPtr->Opcode) << 16;  // shift left 16 bits
		OBJECT_CODE += ((n << 17) + (i << 16) + (x << 15) + (b << 14) + (p << 13) + (e << 12));
		OBJECT_CODE |= (disp & 0x00000FFF) ;
		OBJECT_ox = 1;          // need to generate OBJECT CODE
		convert_ObToString();   // convert int object code to string.
	}
	/* else if is format 4 */
	else if(OPCODE_format == 4){
		
		/* if is Immediate addressing */
		if(OPERAND1[0] == '#'){
			n = 0;
			i = 1;
		} 
		/* else if is Indirect addressing */
		else if(OPERAND1[0] == '@'){
			n = 1;
			i = 0;
		}
		/* else (simple) */
		else{
			i = 1;
			n = 1;
		}	
		
		/* Calculate address, 
		if OPERAND1 is immediate address constant */
		if((OPERAND1[0] == '#') && (OPERAND1[1] >= '0') && (OPERAND1[1] <= '9')){
			disp = atoi(&OPERAND1[1]);        // we don't need #
			b = 0;
			p = 0;
		}
		/* else (direct addressing) */
		else{
			/* Clean '@' or '#' */
			if((OPERAND1[0] == '@') || (OPERAND1[0] == '#')){
				temp = &OPERAND1[1];
				strcpy(OPERAND1, temp);
			}
			/* search SYMTAB */
			if(search_SYMTAB(OPERAND1) == 0){
				printf("ERROR! %s is not found. It's an invalid SYMBOL\n", OPERAND1);
				exit(0);
			}
			b = 0;
			p = 0;
			disp = SYMBOL_ADDR;
			
			/* Relocation */
			relocate(0);        // store address
		}
			
		/* if is index address */
		if((OPERAND_ox == 2) && (OPERAND2[0] == 'X')){
			x = 1;
		}
		/* else (not index address) */
		else{
			x = 0;
		}
		
		e = 1;    // format 4
		
		/* generate object code */
		OBJECT_CODE = (instPtr->Opcode) << 24;  // shift left 16 bits
		OBJECT_CODE += ((n << 25) + (i << 24) + (x << 23) + (b << 22) + (p << 21) + (e << 20));
		OBJECT_CODE += disp;
		OBJECT_ox = 1;          // need to generate OBJECT CODE
		convert_ObToString();   // convert int object code to string.
		
	}
	/* else if OPCODE = 'BYTE', then */
	else if(!strcmp(OPCODE, "BYTE")){
		/* if string */
		if(OPERAND1[0] == 'C'){
			OBJECT_CODE = (int)OPERAND1[2];
			for(j = 3, S_L = 1; OPERAND1[j] != '\''; j++, S_L++){
				OBJECT_CODE = (int)OPERAND1[j] + (OBJECT_CODE << 8);
			}
		}
		/* else (char) */
		else{
			str[0] = OPERAND1[2];
			str[1] = OPERAND1[3];
			str[2] = '\0';
			OBJECT_CODE = (int)strtol(str, NULL, 16);
			S_L = 1;
		}
		OBJECT_ox = 1;
		convert_ObToString();
	}
	/* else if OPCODE = 'WORD', then */
	else if(!strcmp(OPCODE, "BYTE")){
		OBJECT_CODE = atoi(OPERAND1);
		OBJECT_ox = 1;
		S_L = 1;
		convert_ObToString();
	}	
	/* else if OPCODE = 'BASE', then */
	else if(OPCODE_format == 7){
		/* search SYMTAB */
		if(search_SYMTAB(OPERAND1) == 0){
			printf("ERROR! %s is not found. It's an invalid SYMBOL\n", OPERAND1);
			exit(0);
		}
		
		BASE = SYMBOL_ADDR;
		OBJECT_ox = 0;
	}
	else if(OPCODE_format == 0){
		OBJECT_ox = 0;
	}		
	else{
		/* Set ERROR Flag */
		printf("ERROR.\n");
		exit(0);
	}
				
	// printf("x = %d, b = %d, p = %d, e = %d, disp = %X\n", x, b, p, e, disp);  // for testing
	return ;
}

/* ------------------------------------------------- */
/* return register value */

int  reg_value(char *str){
	
	/* A = 0 */
	if(!strcmp(str, "A")){
		return 0;
	}
	/* X = 1 */
	else if(!strcmp(str, "X")){
		return 1;
	}
	/* L = 2 */
	else if(!strcmp(str, "L")){
		return 2;
	}
	/* B = 3 */
	else if(!strcmp(str, "B")){
		return 3;
	}
	/* S = 4 */
	else if(!strcmp(str, "S")){
		return 4;
	}
	/* T = 5 */
	else if(!strcmp(str, "T")){
		return 5;
	}
	/* F = 6 */
	else if(!strcmp(str, "F")){
		return 6;
	}
	/* PC = 8 */
	else if(!strcmp(str, "PC")){
		return 8;
	}
	/* SW = 9 */
	else if(!strcmp(str, "SW")){
		return 9;
	}
	/* else (undefined register) */
	else{
		printf("ERROR. %s is not a defined register.\n", str);
		exit(0);
	}
	
	return 0;
}

/* ------------------------------------------------- */
/* calculate disp and return relative mode */

int  relative_mode(int *disp){
	
	int PC;
	char *temp;
	
	/* Clean '@' or '#' */
	if((OPERAND1[0] == '@') || (OPERAND1[0] == '#')){
		temp = &OPERAND1[1];
		strcpy(OPERAND1, temp);
	}
	
	if(!search_SYMTAB(OPERAND1)){
		printf("ERROR. %s is not found. It's an undeclared SYMBOL\n", OPERAND1);
		exit(0);
	} 
	
	PC = (int)strtol(LOC, NULL, 16) + OPCODE_format;  // PC point to next instruction.
	
	/* try PC relative first */
	*disp = SYMBOL_ADDR - PC;
	
	if((*disp >= -2048) && (*disp < 2048)){
		return 1;
	}
	
	/* try Base relative */
	*disp = SYMBOL_ADDR - BASE;
	
	if(*disp < 4096){
		return 0;
	}
	/* else (invalid addressing mode) */
	else{
		printf("ERROR! Invalid Addressing Mode.\n");
		exit(1);
	}
}

/* ------------------------------------------------- */
/* Generate text record */

void text_record(void){
	
	static init = 1;           // initialize text record tag
	char str[5];
	char temp1[10], temp2[10];
	
	/* check buffer full, if yes, then */
	if((69 - strlen(text_line)) < (2 * OPCODE_format)){
		/* calculate string length */
		sprintf(str, "%02X", (strlen(text_line) - 9) / 2);
		text_line[7] = str[0];
		text_line[8] = str[1];
		
		/* Write into file */
		fprintf(object_file, "%s\n", text_line);
		
		/* Expand LOC to 6 bits string  */
		strcpy(temp1, LOC);
		sprintf(temp2, "%06s", temp1);
			
		/* new line, set new text record */
		strcpy(text_line, "T");
		strcat(text_line, temp2);
		strcat(text_line, "XX");
			
		/* insert object into line */
		strcat(text_line, OBJECT_CODEs);

		return ;
	}
	
	/* if init = 1, then */
	if(init == 1){
		/* Expand LOC to 6 bits string  */
		strcpy(temp1, LOC);
		sprintf(temp2, "%06s", temp1);
		
		/* set new text record */
		strcpy(text_line, "T");
		strcat(text_line, temp2);
		strcat(text_line, "XX");

		init = 0;
		
		/* insert object into line */
			strcat(text_line, OBJECT_CODEs);
	}
	/* next EOF line */
	else if(init == 2){	
		/* calculate string length */
		sprintf(str, "%02X", (strlen(text_line) - 9) / 2);
		text_line[7] = str[0];
		text_line[8] = str[1];
		
		/* Write into file */
		fprintf(object_file, "%s\n", text_line);
		
		/* Expand LOC to 6 bits string  */
		strcpy(temp1, LOC);
		sprintf(temp2, "%06s", temp1);
			
		/* new line, set new text record */
		strcpy(text_line, "T");
		strcat(text_line, temp2);
		strcat(text_line, "XX");
			
		/* insert object into line */
		strcat(text_line, OBJECT_CODEs);
	
		init = 0;
	}	
	/* else if OPCODE = 'END' */
	else if(!strcmp(OPCODE, "END")){
		/* calculate string length */
		sprintf(str, "%02X", (strlen(text_line) - 9) / 2);
		text_line[7] = str[0];
		text_line[8] = str[1];
					
		/* Write into file */
		fprintf(object_file, "%s\n", text_line);
	}
	/* else */
	else{
		/* insert line */
		strcat(text_line, OBJECT_CODEs);
		
		/* set EOF tag */
		if(!strcmp(SYMBOL, "EOF")){
			init = 2;
		}
	}	
		
	return ;
}

/* ------------------------------------------------- */
/* Convert int Object Code to String */

void convert_ObToString(void){
	
	switch(OPCODE_format){
		case 1 :
			sprintf(OBJECT_CODEs, "%02X", OBJECT_CODE);
			break;
		case 2 :
			sprintf(OBJECT_CODEs, "%04X", OBJECT_CODE);
			break;
		case 3 : 
			sprintf(OBJECT_CODEs, "%06X", OBJECT_CODE);
			break;
		case 4 :
			sprintf(OBJECT_CODEs, "%08X", OBJECT_CODE);
			break;
		case 5 : 
		case 6 :
			if(S_L == 1){
				sprintf(OBJECT_CODEs, "%02X", OBJECT_CODE);
			}
			else if(S_L == 2){
				sprintf(OBJECT_CODEs, "%04X", OBJECT_CODE);
			}
			else{
				sprintf(OBJECT_CODEs, "%06X", OBJECT_CODE);
			}
			break; 
		default :
			break;
	}
	return ;
}

/* ------------------------------------------------- */
/* Relocation Function */

void relocate(int mode){
	
	static top = 0;
	int temp;
	int k;
	
	/* Mode 1 = record mode */
	if(mode == 0){
		temp = (int)strtol(LOC, NULL, 16) + 1;    // load Location number;
		sprintf(RETAB[top].ADDR, "%06X", temp);
		top++;
		/* over store */
		if(top > RETAB_SIZE){
			printf("ERROR. RETAB explode!\n");
			exit(0);
		} 
	}
	/* Mode 2 = write to object file */
	else if(mode == 1){
		for(k = 0; k < top; k++){
			fprintf(object_file, "M%s05\n", RETAB[k].ADDR);
		}
	}
	/* else (ERROR) */
	else{
		printf("ERROR! Undefined Relocation Mode.\n");
		exit(0);
	}
	
	return ;
}

/* ------------------------------------------------- */
/* TESTING */

void print_SYMTAB(void){
	
	int i ;
	
	for(i = 0; i < SYMTAB_SIZE; i++){
		printf("SYMTAB[%d] = %s - %04X.\n", i, SYMTAB[i].label_Name, SYMTAB[i].address);
	}
}

/* ------------------------------------------------- */
/* TESTING */

void print_Status(void){
	
	printf("Loc = %s, ", LOC);
	
	if(SYMBOL_ox == 1){
		printf("SYMBOL = %s, ", SYMBOL);
	}
	
	printf("OPCODE = %s, format = %d\n", OPCODE, OPCODE_format);
	
	if(OPERAND_ox == 1){
		printf("OPERAND1 = %s, OB = %X\n\n", OPERAND1, OBJECT_CODE);
	}
	else if(OPERAND_ox = 2){
		printf("OPERAND1 = %s, OPERAND2 = %s, OB = %X\n\n", OPERAND1, OPERAND2, OBJECT_CODE);
	}
	else{
		printf("OB = %X\n\n", OBJECT_CODE);
	}
	
}

