#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "y86dis.h"
 
//global variables

int reg[8];
char name[4];   //to be used in regname method
int instructionaddress;
int end;

//condition codes, zero or 1, initialized to zero, set to one when hit
int OF = 0;    //overflow flag
int ZF = 0;	   //zero flag
int SF = 0;    //negative flag	

//memory array, will be size of size directive, contains all instructions and bytes etc
unsigned char * memory;

statuscode status;

int main(int argc, char ** argv)
{
	FILE * f;
	f = fopen(argv[1], "r");    //argv[1] holds the name of input file
	char str1[100];
	char str2[100];
	char str3[10000];   
	int put;
	int Doestextexist = 0;
	int address = (int)address;

	//eax, ecx, edx, ebx, esp, ebp, esi, edi

	while(fscanf(f, "%s", str1) > 0 )  //places first word into str1
	{	
			
		if(strcmp(str1, ".size") == 0)
		{
			fscanf(f, "%s", str2);
			long int sizenum = strtol(str2, NULL, 16);   //converts the string of hex number to decimal int base 10
			memory = (unsigned char *)malloc(sizeof(char) * sizenum);				
		}

		else if(strcmp(str1, ".string") == 0)
		{
			fscanf(f, "%s", str2);
			long int location = strtol(str2, NULL, 16); 
			int loc = (int)location;
			fscanf(f, " \"%[^\"]\"", str3);   //reads from first quote up to the next quote, adds quote to end, place in str3
			
			strncpy((char *)&memory[loc], &str3[0], strlen(str3));						
		}
		
		else if(strcmp(str1, ".long") == 0)
		{
			fscanf(f, "%s %s", str2, str3);
			long int thespot = strtol(str2, NULL, 16);
			int spot = (int)thespot;
			long int thevalue = strtol(str3, NULL, 10);
			int longvalue = (int)thevalue;
			memcpy(&memory[spot], &longvalue, 4);
		}
		
		else if(strcmp(str1, ".byte") == 0)
		{
			fscanf(f, " %s %s", str2, str3);
			address = strtol(str2, NULL, 16);
			long int value = strtol(str3, NULL, 16);
			int val = (int)value;
			memory[address] = val;
		}
		
		else if(strcmp(str1, ".text") == 0)
		{
			Doestextexist = 1;
			fscanf(f, " %s %s", str2, str3);
			long int theaddress = strtol(str2, NULL, 16);
			instructionaddress = (int)theaddress;
			int start = instructionaddress;
			int counter = 0;
			int thelength = strlen(str3);  //length of instructions

			char * mover = &(str3[0]);
			
			while(counter < thelength)  //this loop puts instructions into memory array at correct offset
			{
				sscanf(mover, "%2x", &put);
				memory[start] = (char)put;
				start = start+1;
				counter = counter + 2;
				mover = mover + 2;
			}

			end = instructionaddress + (thelength/2) - 1;
			printf("end is %d \n", end);
		}
		
		
		else if(strcmp(str1, ".bss") == 0)
		{		
			

		}	


	}	




	if(Doestextexist == 0)
	{
		fprintf(stderr, "Error, .text must be in the input file. \n");
		exit(0);
	}


	execute(instructionaddress);  //executes .text instructions starting at their location in memory


	fclose(f);

	return 0;

}



int execute(int st)
{	
	cmd current;
	int pc = st;
	status = AOK;
	int put;

    while(pc <= end)
    { 


    	if(memory[pc] == 0)
    	{
    		current = nop;
    	}

		if(memory[pc] == 16) //10, halt
		{
			current = halt;
			status = HLT;
		}

		if(memory[pc] == 32)  //20, rrmovl
		{
			current = rrmovl;
		}

		else if(memory[pc] == 48)  //30, irmovl
		{
			current = irmovl;
		}

		else if(memory[pc] == 64)  //40,
		{
			current = rmmovl;
		}

		else if(memory[pc] == 80) //50
		{
			current = mrmovl;
		}
		
		else if(memory[pc] == 96)  //60
		{
			current = addl;
		}	

		else if(memory[pc] == 97)  //61
		{
			current = subl;
		}

		else if(memory[pc] == 98) //62
		{
			current = andl;
		}

		else if(memory[pc] == 99) //63
		{
			current = xorl;
		}

		else if(memory[pc] == 100) //64
		{
			current = mull;
		}

		else if(memory[pc] == 101) //65
		{
			current = cmpl;
		}

		else if(memory[pc] == 112)  //70
		{
			current = jmp;
		}

		else if(memory[pc] == 113)
		{
			current = jle;
		}

		else if(memory[pc] == 114)  //72
		{
			current = jl;
		}

		else if(memory[pc] == 115)
		{
			current = je;
		}

		else if(memory[pc] == 116) //74
		{
			current = jne;
		}

		else if(memory[pc] == 117)
		{
			current = jge;
		}

		else if(memory[pc] == 118)
		{
			current = jg;
		}

		else if(memory[pc] == 128)
		{
			current = call;
		}

		else if(memory[pc] == 144)
		{
			current = ret;
		}

		else if(memory[pc] == 160)
		{
			current = pushl;
		}

		else if(memory[pc] == 176)
		{
			current = popl;
		}

		else if(memory[pc] == 192)  //c0
		{
			current = readb;
		}

		else if(memory[pc] == 193)  //c1
		{
			current = readl;
		}

		else if(memory[pc] == 208) //d0
		{
			current = writeb;
		}

		else if(memory[pc] == 209)  //d1
		{
			current = writel;
		}

		else if(memory[pc] == 224)  //e0
		{
			current = movsbl;
		}

		// else   //if bytes dont correspond to any of the opcodes, raise an error
		// {
		// 	status = INS;
		// 	fprintf(stderr, "ERROR, invalid instruction");
		// 	exit(0);
		// }

		printf(".address: %d 	", pc);

		switch(current)
		{
			//pc is currently pointing at opcode
			//			printf("at mem address %d", pc);
			case halt:
				pc = pc + 1;
				printf("halt \n");
				break;

			case nop:
				pc = pc + 1;
				printf("nop \n");
				break;

			case rrmovl:
				printf("rrmovl %s", regname(pc, 1));
				printf(" %s \n", regname(pc, 2));
				pc = pc + 2;
				break;

			case irmovl:  
				memcpy(&put, &memory[pc + 2], 4);
				printf("irmovl $%d %s \n", put, regname(pc, 2));
				pc = pc + 6;
				break;

			case rmmovl:
				memcpy(&put, &memory[pc + 2], 4);
				printf("rmmovl %s %d", regname(pc, 1), put);
				printf("(%s) \n", regname(pc, 2));
				pc = pc + 6;
				break;

			case mrmovl:
				memcpy(&put, &memory[pc + 2], 4);
				printf("mrmovl %d(%s) %s \n", put, regname(pc, 1), regname(pc, 2));
				pc = pc + 6;
				break;

			case addl:
				printf("addl %s", regname(pc, 1));
				printf(" %s \n", regname(pc, 2));
				pc = pc + 2;
				break;

			case subl:
				printf("subl %s", regname(pc, 1));
				printf(" %s \n", regname(pc, 2));
				pc = pc + 2;
				break;

			case andl:
				printf("andl %s", regname(pc, 1));
				printf(" %s \n", regname(pc, 2));
				pc = pc + 2;
				break;

			case xorl:
				printf("xorl %s", regname(pc, 1));
				printf(" %s \n", regname(pc, 2));
				pc = pc + 2;
				break;

			case mull:
				printf("mull %s", regname(pc, 1));
				printf(" %s \n", regname(pc, 2));
				pc = pc + 2;
				break;

			case cmpl:
				printf("cmpl %s", regname(pc, 1));
				printf(" %s \n", regname(pc, 2));
				pc = pc + 2;
				break;

			case jmp:				
				put = jf(pc);
				printf("jmp %d \n", put);
				pc = pc + 5;
				break;

			case jl:
				put = jf(pc);
				printf("jl %d \n", put);
				pc = pc + 5;
				break;

			case jle:
				printf("jle %d \n", jf(pc));
				pc = pc + 5;
				break;

			case je:				
				put = jf(pc);
				printf("je %d \n", put);
				pc = pc + 5;   //plus five since you dont skip past register byte 
				break;

			case jne:				
				printf("jne %d \n", jf(pc));
				pc = pc + 5;				
				break;

			case jge:
				printf("jge %d \n", jf(pc));
				pc = pc + 5;				
				break;

			case jg:
				printf("jg %d \n", jf(pc));
				pc = pc + 5;				
				break;

			case call:
				memcpy(&put, &memory[pc + 1], 4);
				printf("call %d \n", put);
				pc = pc + 5;
				break;

			case ret:
				printf("ret \n");
				pc = pc + 1;
				break;

			case pushl:
				printf("pushl %s \n", regname(pc, 1));
				pc = pc + 2;
				break;

			case popl:
				printf("popl %s \n", regname(pc, 1));
				pc = pc + 2;
				break;

			case movsbl:
				memcpy(&put, &memory[pc + 2], 4);
				printf("movsbl %d(%s) ", put, regname(pc, 1));
				printf(" %s", regname(pc, 2));
				pc = pc + 6; 
				break;

			case writeb:
				memcpy(&put, &memory[pc + 2], 4);				
				printf("writeb %d(%s) \n", put, regname(pc, 1));
				pc = pc + 6;
				break;

			case writel:
				memcpy(&put, &memory[pc + 2], 4);				
				printf("writel %d(%s) \n", put, regname(pc, 1));
				pc = pc + 6;
				break;

			case readb:
				memcpy(&put, &memory[pc + 2], 4);				
				printf("readb %d(%s) \n", put, regname(pc, 1));
				pc = pc + 6;
				break;

			case readl:
				memcpy(&put, &memory[pc + 2], 4);				
				printf("readl %d(%s) \n", put, regname(pc, 1));
				pc = pc + 6;
				break;
							
		}


	}

	exit(0);

}

char * regname(int opcodeaddress, int which)
{	
	int regaddress = opcodeaddress + 1;
	int whichreg;

	if(which == 1)
	{
		whichreg = 1;
	}

	else if(which == 2)
	{
		whichreg = 2;
	}

	int regnum = getregister(regaddress, whichreg);

	if(regnum == 0)
	{
		strcpy(name, "eax");
	}

	else if(regnum == 1)
	{
		strcpy(name, "ecx");
	}

	else if(regnum == 2)
	{
		strcpy(name, "edx");
	}

	else if(regnum == 3)
	{
		strcpy(name, "ebx");
	}

	else if(regnum == 4)
	{
		strcpy(name, "esp");
	}

	else if(regnum == 5)
	{
		strcpy(name, "ebp");
	}

	else if(regnum == 6)
	{
		strcpy(name, "esi");
	}

	else if(regnum == 7)
	{
		strcpy(name, "edi");
	}

	return name;

}


int getregister(int address, int which)
{
	char buff[3];
	if(sprintf(buff, "%02x", memory[address])< 0)
	{
		status = ADR;
		fprintf(stderr,"ERROR invalid address");
		exit(0);
	}

	buff[2] = '\0';
	
	if(which == 1)
	{
		int a = buff[0] - '0';
		return a;	
	}

	else if(which == 2)
	{
		int b = buff[1] - '0';
		return b;
	}

	else
	{
		printf("Error, you need to choose 1st register or 2nd !! ");
		return 0;
	}

}

int jf(int mover)
{
	//mover is at opcode, right before 32 bit value
	int put;
	memcpy(&put, &memory[mover + 1], 4);	
	return put;
}