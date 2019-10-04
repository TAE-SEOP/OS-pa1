/**********************************************************************
 * Copyright (c) 2019
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

/* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */
#define MAX_NR_TOKENS	32	/* Maximum length of tokens in a command */
#define MAX_TOKEN_LEN	64	/* Maximum length of single token */
#define MAX_ASSEMBLY	256 /* Maximum length of assembly string */

typedef unsigned char bool;
#define true	1
#define false	0
/*          ****** DO NOT MODIFY ANYTHING UP TO THIS LINE ******      */
/*====================================================================*/


/***********************************************************************
 * translate
 *
 * DESCRIPTION
 *   Translate assembly represented in @tokens[] into a MIPS instruction.
 * This translate should support following 13 assembly commands
 *
 *	- add
 *	- addi
 *	- sub
 *	- and
 *	- andi
 *	- or
 *	- ori
 *	- nor
 *	- lw
 *	- sw
 *	- sll
 *	- srl
 *	- sra
 *
 * RETURN VALUE
 *   Return a 32-bit MIPS instruction
 *
 */
static unsigned int translate(int nr_tokens, char *tokens[])
{
	int category =0;
	int bin[32] = {0, };
	long int deci[3] = {0, };
	char *nothing = (char *)malloc(sizeof(char));
	char str[100];

	for (int i = 0; i < strlen(tokens[0]); i++) {  // R instruction 인지 i instruction인지 구분하는 반복문 i가 있으면 i 아니면 R
		if ((tokens[0][i] == 'i')  || (tokens[0][1] =='w')) category = 1;
	}
	if(tokens[1][0] == 't' && tokens[1][1] >= '8') deci[0] = tokens[1][1] - 32;  
	else if (tokens[1][0] == 't' && tokens[1][1] < '8') deci[0] = tokens[1][1] - 40;
	else if (tokens[1][0] == 'z') deci[0] = 0;
	else if (tokens[1][0] == 'a' && tokens[1][1] == 't') deci[0] = 1;
	else if (tokens[1][0] == 'v') deci[0] = tokens[1][1] - 46;
	else if (tokens[1][0] == 'a') deci[0] = tokens[1][1] - 44;
	else if (tokens[1][0] == 's' && tokens[1][1] != 'p') deci[0] = tokens[1][1] - 32;
	else if (tokens[1][0] == 'k') deci[0] = tokens[1][1] - 23;
	else if (tokens[1][0] == 'g') deci[0] = 28;
	else if (tokens[1][0] == 's' && tokens[1][1] == 'p') deci[0] = 29;
	else if (tokens[1][0] == 'f') deci[0] = 30;
	else if (tokens[1][0] == 'r') deci[0] = 31;

	if (tokens[2][0] == 't' && tokens[2][1] >= '8') deci[1] = tokens[2][1] - 32;
	else if (tokens[2][0] == 't' && tokens[2][1] < '8') deci[1] = tokens[2][1] - 40;
	else if (tokens[2][0] == 'z') deci[1] = 0;
	else if (tokens[2][0] == 'a' && tokens[2][1] == 't') deci[1] = 1;
	else if (tokens[2][0] == 'v') deci[1] = tokens[2][1] - 46;
	else if (tokens[2][0] == 'a') deci[1] = tokens[2][1] - 44;
	else if (tokens[2][0] == 's' && tokens[2][1] != 'p') deci[1] = tokens[2][1] - 32;
	else if (tokens[2][0] == 'k') deci[1] = tokens[2][1] - 23;
	else if (tokens[2][0] == 'g') deci[1] = 28;
	else if (tokens[2][0] == 's' && tokens[2][1] == 'p') deci[1] = 29;
	else if (tokens[2][0] == 'f') deci[1] = 30;
	else if (tokens[2][0] == 'r') deci[1] = 31;

	if (tokens[3][0] == 't' && tokens[3][1] >= '8') deci[2] = tokens[3][1] - 32;
	else if (tokens[3][0] == 't' && tokens[3][1] < '8') deci[2] = tokens[3][1] - 40;
	else if (tokens[3][0] == 'z') deci[2] = 0;
	else if (tokens[3][0] == 'a' && tokens[3][1] == 't') deci[3] = 1;
	else if (tokens[3][0] == 'v') deci[2] = tokens[3][1] - 46;
	else if (tokens[3][0] == 'a') deci[2] = tokens[3][1] - 44;
	else if (tokens[3][0] == 's' && tokens[3][1] != 'p') deci[2] = tokens[3][1] - 32;
	else if (tokens[3][0] == 'k') deci[2] = tokens[3][1] - 23;
	else if (tokens[3][0] == 'g') deci[2] = 28;
	else if (tokens[3][0] == 's' && tokens[3][1] == 'p') deci[2] = 29;
	else if (tokens[3][0] == 'f') deci[2] = 30;
	else if (tokens[3][0] == 'r') deci[2] = 31;

		
	if(category == 0){   //R instruction
		if (tokens[0][0] == 's' && tokens[0][1] != 'u') {  //sll , srl ,sra
			for (int i = 0; i < strlen(tokens[3]); i++) {
				if (i == strlen(tokens[3]) - 1) str[i + 1] = '\0';
				str[i] = tokens[3][i];
			}

			if (str[1] == 'x' || str[2] == 'x') deci[2] = strtoimax(str, &nothing, 16);
			else deci[2] = strtol(str, &nothing, 10);

				for (int i = 15; i > 10; i--) {
					bin[i] = deci[1] % 2;
					deci[1] = deci[1] / 2;
				}
				for (int i = 20; i > 15; i--) {
					bin[i] = deci[0] % 2;
					deci[0] = deci[0] / 2;
				}
				if (deci[2] > 0) {
					for (int i = 25; i > 20; i--) {
						bin[i] = deci[2] % 2;
						deci[2] = deci[2] / 2;
					}
				}
				else if(deci[2] < 0){
					for (int i = 25; i > 20; i--) {
						if (deci[2] % 2 == 1) bin[i] = 0;
						else if (deci[2] % 2 == 0) bin[i] = 1;
						deci[2] = deci[2] / 2;
					}
					bin[25]++;
					if (bin[15] == 2) {
						for (int i = 25; i > 20; i--) {
							if (bin[i] == 2) {
								bin[i] = 0;
								bin[i - 1] += 1;
							}
							else break;
						}
					}
				}
				if (tokens[0][1] == 'r' && tokens[0][2] != 'a') bin[30] = 1;  //srl
				else if (tokens[0][2] == 'a') bin[30] = 1, bin[31] = 1;   //sra
		}
	    else {
		for (int i = 10; i > 5; i--) {
			bin[i] = deci[1] % 2;
			deci[1] = deci[1] / 2;
		}
		for (int i = 15; i > 10; i--) {
			bin[i] = deci[2] % 2;
			deci[2] = deci[2] / 2;
		}
		for (int i = 20; i > 15; i--) {
			bin[i] = deci[0] % 2;
			deci[0] = deci[0] / 2;
		}
		if (tokens[0][0] == 'a' && tokens[0][1] == 'd') bin[26] = 1;  //add
		else if (tokens[0][0] == 'a' && tokens[0][1] == 'n') bin[26] = 1, bin[29] = 1;  //and
		else if (tokens[0][0] == 'o') bin[26] = 1, bin[29] = 1, bin[31] = 1;   //or
		else if (tokens[0][0] == 'n') bin[26] = 1, bin[29] = 1, bin[30] = 1, bin[31] = 1;  //nor
		else if (tokens[0][0] == 's') bin[26] = 1, bin[30] = 1; //sub
		}
	}

	else if(category==1){  // i instruction
		for (int i = 0; i < strlen(tokens[3]); i++) {
			if (i == strlen(tokens[3]) - 1) str[i + 1] = '\0';
			str[i] = tokens[3][i];
		}
		if (str[1] == 'x' || str[2] == 'x') deci[2] = strtoimax(str, &nothing, 16);
		else deci[2] = strtol(str, &nothing, 10);
		

		if (deci[2] < 0) {
			for (int i = 10; i > 5; i--) {
				bin[i] = deci[1] % 2;
				deci[1] = deci[1] / 2;
			}
			for (int i = 15; i > 10; i--) {
				bin[i] = deci[0] % 2;
				deci[0] = deci[0] / 2;
			}
			for (int i = 31; i > 15; i--) {
				if( deci[2] % 2 == 1) bin[i] = 0;
				else if (deci[2] % 2 == 0) bin[i] = 1;
				deci[2] = deci[2] / 2;
			}
			bin[31]++;
			if (bin[31] == 2) {
				for (int i = 31; i > 15; i--) {
					if (bin[i] == 2) {
						bin[i] = 0;
						bin[i - 1] += 1;
					}
					else break;
				}
			}
		}
		else {
			for (int i = 10; i > 5; i--) {
				bin[i] = deci[1] % 2;
				deci[1] = deci[1] / 2;
			}
			for (int i = 15; i > 10; i--) {
				bin[i] = deci[0] % 2;
				deci[0] = deci[0] / 2;
			}
			for (int i = 31; i > 15; i--) {
				bin[i] = deci[2] % 2;
				deci[2] = deci[2] / 2;
			}
		}
		if (tokens[0][0] == 'a' && tokens[0][1] == 'd') bin[2] = 1;  //addi
		else if (tokens[0][0] == 'a' && tokens[0][1] == 'n') bin[2] = 1, bin[3] = 1;  //andi
		else if (tokens[0][0] == 'o') bin[2] = 1, bin[3] = 1, bin[5] = 1;    //ori
		else if (tokens[0][0] == 'l') bin[0] = 1, bin[4] = 1, bin[5] = 1;  //lw
		else if (tokens[0][0] == 's') bin[0] = 1, bin[2] = 1, bin[4] = 1, bin[5] = 1;   //sw
	}

	for(int i = 0; i < 32; i++) {
		if (i % 4 == 0) printf(" ");
		printf("%d", bin[i]);
	}
	printf("\n");
	int result = 0;
	long long int bi = 1;
	for (int i = 31; i >= 0; i--) {
		result += bin[i] * bi;
		bi = bi * 2;
	}
	return result;
}

/***********************************************************************
 * parse_command
 *
 * DESCRIPTION
 *  Parse @assembly, and put each assembly token into @tokens[] and the number of
 *  tokes into @nr_tokens. You may use this implemention or your own from PA0.
 *
 * A assembly token is defined as a string without any whitespace (i.e., *space*
 * and *tab* in this programming assignment). For exmaple,
 *   command = "  add t1   t2 s0 "
 *
 * then, nr_tokens = 4, and tokens is
 *   tokens[0] = "add"
 *   tokens[1] = "t0"
 *   tokens[2] = "t1"
 *   tokens[3] = "s0"
 *
 * You can assume that the input string is all lowercase for testing.
 *
 * RETURN VALUE
 *  Return 0 after filling in @nr_tokens and @tokens[] properly
 *
 */
static bool __is_separator(char *c)
{
	char *separators = " \t\r\n,.";

	for (int i = 0; i < strlen(separators); i++) {
		if (*c == separators[i]) return true;	
	}

	return false;
}
static int parse_command(char *assembly, int *nr_tokens, char *tokens[])
{
	char *curr = assembly;
	int token_started = false;
	*nr_tokens = 0;

	while (*curr != '\0') {  
		if (__is_separator(curr)) {  
			*curr = '\0';
			token_started = false;
		} else {
			if (!token_started) {
				tokens[*nr_tokens] = curr;
				*nr_tokens += 1;
				token_started = true;
			}
		}
		curr++;
	}

	return 0;
}



/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */

/***********************************************************************
 * The main function of this program.
 */
int main(int argc, char * const argv[])
{
	char assembly[MAX_ASSEMBLY] = { '\0' };
	FILE *input = stdin;

	if (argc > 1) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (input == stdin) {
		printf("*********************************************************\n");
		printf("*          >> SCE212 MIPS translator  v0.01 <<          *\n");
		printf("*                                                       *\n");
		printf("*                                       .---.           *\n");
		printf("*                           .--------.  |___|           *\n");
		printf("*                           |.------.|  |=. |           *\n");
		printf("*                           || >>_  ||  |-- |           *\n");
		printf("*                           |'------'|  |   |           *\n");
		printf("*                           ')______('~~|___|           *\n");
		printf("*                                                       *\n");
		printf("*                                   Fall 2019           *\n");
		printf("*********************************************************\n\n");
		printf(">> ");
	}

	while (fgets(assembly, sizeof(assembly), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;
		unsigned int machine_code;

		for (size_t i = 0; i < strlen(assembly); i++) {
			assembly[i] = tolower(assembly[i]);
		}

		if (parse_command(assembly, &nr_tokens, tokens) < 0)
			continue;

		machine_code = translate(nr_tokens, tokens);

		fprintf(stderr, "0x%08x\n", machine_code);

		if (input == stdin) printf(">> ");
	}

	if (input != stdin) fclose(input);

	return EXIT_SUCCESS;
}
