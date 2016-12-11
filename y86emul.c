#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "y86emul.h"
 
//global variables

int reg[8];

int instructionaddress;


//condition codes, zero or 1, initialized to zero, set to one when hit
int OF = 0;    //overflow flag
int ZF = 0;	   //zero flag
int SF = 0;    //negative flag	

//memory array, will be size of size directive, contains all instructions and bytes etc
unsigned char * memory;



int main(int argc, char ** argv)
{
	FILE * f;
	f = fopen(argv[1], "r");    //argv[1] holds the name of input file
	char str1[100];
	char str2[100];
	char str3[10000];   
	int put;
	int Doestextexist = 0;	  
	int start;

	

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
			
			strncpy((char*)&memory[loc], &str3[0], strlen(str3));						
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
			int address = strtol(str2, NULL, 16);
			long int value = strtol(str3, NULL, 16);
			int val = (	int)value;
			memory[address] = val;
		}

		else if(strcmp(str1, ".text") == 0)
		{
			Doestextexist = 1;
			fscanf(f, " %s %s", str2, str3);
			long int theaddress = strtol(str2, NULL, 16);
			instructionaddress = (int)theaddress;
			start = instructionaddress;
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



void execute(int st)
{	

	cmd current;
	int pc = st;
	status = AOK;
    while(status != HLT && status != ADR && status != INS)
    { 

    	if(memory[pc] == 0)
    	{
    		current = nop;
    	}

		if(memory[pc] == 16) //10, halt
		{
			status = HLT;
			exit(0);
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

		else   //if bytes dont correspond to any of the opcodes, raise an error
		{
			status = INS;
			fprintf(stderr, "ERROR, invalid instruction");
			exit(0);
		}

		switch(current)
		{
			//pc is currently pointing at opcode
//			printf("at mem address %d", pc);

			case nop:
				pc = pc + 1;
//				printf("i did nop \n");
				break;

			case rrmovl:
				rrmovlf(pc);
//				printf("i did rrmovl, pc is %d, im gonna increase it by 2 now \n", pc);
				pc = pc + 2;
				break;

			case irmovl:  
				irmovlf(pc);
//				printf("i did irmovl, pc is %d \n", pc);
				pc = pc + 6;
				break;

			case rmmovl:
				rmmovlf(pc);
				pc = pc + 6;
//				printf("i did rmmovl \n");
				break;

			case mrmovl:
				mrmovlf(pc);
				pc = pc + 6;
//				printf("i did mrmovl \n");
				break;

			case addl:
				addlf(pc);
				pc = pc + 2;
//				printf("i did addl \n");
				break;

			case subl:
				sublf(pc);
				pc = pc + 2;
//				printf("i did subl \n");
				break;

			case andl:
				andlf(pc);
				pc = pc + 2;
//				printf("i did andl \n");
				break;

			case xorl:
				xorlf(pc);
				pc = pc + 2;
//				printf("i did xorl \n");
				break;

			case mull:
				mullf(pc);
				pc = pc + 2;
//				printf("i did mull \n");
				break;

			case cmpl:
				cmplf(pc);
				pc = pc + 2;
//				printf("i did cmpl \n");
				break;

			case jmp:				
				pc = jf(pc);
//				printf("i did jmp \n");
				break;

			case jle:
				if(ZF == 1 || SF == 1)  //if zero flag or neg flag is set, move to 32 bit address
				{
					pc = jf(pc);
//					printf("i actually jumped jle\n");
				}

				else
				{
					pc = pc + 5;
//					printf("i didnt jump but did jle\n");
				}

				break;

			case jl:
				if(SF == 1)
				{
					pc = jf(pc);
				}

				else
				{
					pc = pc + 5;
				}

			case je:
				if(ZF == 1)   //if zero flag is set move to 32 bit address
				{
					pc = jf(pc);
//					printf("i actually jumped je \n");
				}

				else  //if flag not set, just move to next instruction
				{
					pc = pc + 5;   //plus five since you dont skip past register byte 
//					printf("i didnt jump but did je \n");
				}

//				printf("i dide je \n");
				break;

			case jne:
				if(ZF == 0)
				{
					pc = jf(pc);
//					printf("i actually jumped jne \n");
				}

				else
				{
					pc = pc + 5;
//					printf("i didnt jump but did jne \n");
				}

//				printf("i did jne \n");
				break;

			case jge:
				if(ZF == 1 || (SF == 0 && ZF == 0))  //if its zero or positive
				{
					pc = jf(pc);
//					printf("i actually jumped jge \n");
				}

				else
				{
					pc = pc + 5;
//					printf("i didnt jump but did jge \n");
				}

//				printf("i did jge \n");
				break;

			case jg:
				if(ZF == 0 && SF == 0)  //if its positive	
				{
					pc = jf(pc);
//					printf("i actually jumped jg \n");
				}

				else
				{
					pc = pc + 5;
//					printf("i didnt jump but did jg \n");
				}
//				printf("i did jg \n");
				break;

			case call:
				callf(pc);  //pc currently at op code, after opcode is 32 bit value
				pc = hextodecimal(pc);
//				printf("i did call \n");
				break;

			case ret:
				memcpy(&pc, &memory[reg[4]], 4);   //pop value at esp to pc, the value is address of next instruction in original function
				reg[4] = reg[4] + 4;
//				printf("i did ret \n");
				break;

			case pushl:
				pushlf(pc);
				pc = pc + 2;
//				printf("i did pushl \n");
				break;

			case popl:
				poplf(pc);
				pc = pc + 2;
//				printf("i did pop \n");
				break;

			case movsbl:
				movsblf(pc);
				pc = pc + 6; 
//				printf("i did movsbl \n");
				break;

			case writeb:
				writebf(pc);
//				printf("eax, which should be 1, is %d \n", reg[0]);
//				printf("ebx, which is memory to be printed is %d \n", reg[3]);
				pc = pc + 6;
//				printf(" \n i did writeb \n");
				break;

			case writel:

				writelf(pc);
				pc = pc + 6;
//				printf(" \n i did writel \n");
				break;

			case readb:
				readbf(pc);
				pc = pc + 6;
//				printf("i did readb \n");
				break;

			case readl:
				readlf(pc);
				pc = pc + 6;
//				printf("i did readl \n");
				break;

		}

	}

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

void callf(int mover)
{
	reg[4] = reg[4] - 4;
	int nextinstruction = mover + 5; 
	int * numberptr = &nextinstruction;
	//memory[reg[4]] = nextinstruction
	memcpy(&memory[reg[4]], numberptr, 4);

}

void poplf(int mover)
{
	mover = mover + 1; //mover now at register byte
	int * result = (int * )malloc(sizeof(int));
	result = (int *)memcpy(result, &memory[reg[4]], 4);

	reg[getregister(mover, 1)] = *result;   //put value at memory address of value of esp into register specified by pop
	reg[4] = reg[4] + 4;
}

void pushlf(int mover)
{
	mover = mover + 1; //mover now at register byte
	reg[4] = reg[4] - 4;   //decrement esp by 4																
//	int * numberptr = (int *)(&memory[reg[4]]);
//	*numberptr = reg[getregister(mover, 1)];	//put value of register into address in memory of esps value 
	int put = reg[getregister(mover, 1)];
	memcpy(&memory[reg[4]], &put, 4);  //copies value of pushed register into new spot in memory that the value of esp is in

}

void readlf(int mover)
{
	//	printf("im in readlf \n");
	mover = mover + 2; //mover now at beginning of 32 bit displacement
	int put;
	memcpy(&put, &memory[mover], 4);  //copys int from memory to int put
	mover = mover - 1;  //mover now at register byte
	int address =  reg[getregister(mover, 1)] + put;
	int * ptr = (int *)&memory[address];

	if(scanf("%d", ptr) == 0)
	{
		ZF = 1;
	}

	else
	{
		ZF = 0;
	}

//	printf("lets see if the numebr you put in is in memroy : %d \n", memory[address]);

}

void readbf(int mover)
{	
	mover = mover + 1;  //mover is now at register byte
	int address = reg[getregister(mover, 1)] + hextodecimal(mover);
	scanf("%c", &memory[address]);
}

void writelf(int mover)
{
//	printf("im in here");

	mover = mover + 2;  //mover is now at first byte in 4 byte value
	int put;
	memcpy(&put, &memory[mover], 4); //copies 4 byte value into int put
	mover = mover - 1;  //mover is now at register byte
	int address = reg[getregister(mover, 1)] + put;
	int display;
	memcpy(&display, &memory[address], 4);  //copy 4 bytes from main memory at specified address, copy into int display
	printf("%d \n", display);
}

void writebf(int mover)
{	
	mover = mover + 1;  //mover  is now at register byte
	int put;
	memcpy(&put, &memory[mover +1], 4);  //copies 4 bytes starting at first byte of 4 byte little endian number
	int address = reg[getregister(mover, 1)] + put;
	printf("%c", memory[address]);
	
}

void movsblf(int mover)  //take least significant byte, move into 
{
	mover = mover + 1; //mover at register byte, before 32 bit value
	int address = reg[getregister(mover, 2)] + hextodecimal(mover);
	int value = memory[address];
	//	printf("movsbl address is %x and value is %x", address, value);

	int check = value & 0x000000ff;

	if(check >= 0)
	{
		reg[getregister(mover, 1)] = check;
	}

	else 
	{
		int final = check ^ 0xffffff00;
		reg[getregister(mover, 1)] = final;
	}


}


int jf(int mover)
{
	//mover is at opcode, right before 32 bit value
	int put;
	memcpy(&put, &memory[mover + 1], 4);	
	return put;
}

void irmovlf(int mover)
{
	mover = mover + 1;  //mover is now at register byte
	int put;
	memcpy(&put, &memory[mover + 1], 4);   //moves 4 bytes starting at first byte of 32 bit value into int put
	reg[getregister(mover, 2)] = put;  //move value into register
}

void rrmovlf(int mover)
{
	mover = mover + 1;  //mover is now at register byte
	reg[getregister(mover, 2)] = reg[getregister(mover, 1)]; 
}

void rmmovlf(int mover)
{
	mover = mover + 1;  //mover is now at register byte
	int put;
	memcpy(&put, (int *)&memory[mover + 1], 4);  //move 4 bytes starting at first byte of 32 bit value to int put
	int memvalue = reg[getregister(mover,2)] + put;	
	//	memory[memvalue] = reg[getregister(mover,1)];
	int value = reg[getregister(mover, 1)];
	memcpy(&memory[memvalue], &value, 4);
}

void mrmovlf(int mover)
{
	mover = mover + 2; //mover is now at first byte of 4byte displacement
	int put;
	memcpy(&put, &memory[mover], 4);  //copy displacement value into int put
	mover = mover - 1; //mvoer is now at register byte 
	int memvalue = reg[getregister(mover, 2)] + put;	
	reg[getregister(mover, 1)]	= memory[memvalue];
	
	
}

void addlf(int mover)
{
	mover = mover + 1; //mover is now at register byte
	int one = reg[getregister(mover,1)];
	int two = reg[getregister(mover,2)];
	int sum = one + two;

//	printf("register one is %d and its value is %d", getregister(mover,1), reg[getregister(mover, 1)]);
//	printf("register two is %d and its value is %d", getregister(mover, 2), reg[getregister(mover, 2)]);
	reg[getregister(mover,2)] = sum;	

	setzeroandsignflag(sum);

	if(one > 0 && two > 0 && sum < 0)
	{
		OF = 1;
	}

	else if(one < 0 && two < 0 && sum > 0)
	{
		OF = 1;
	}

	else 
	{
		OF = 0;
	}


}

void sublf(int mover)
{
	mover = mover + 1; //mover is now at register byte
	int one = reg[getregister(mover,1)];
	int two = reg[getregister(mover,2)];
	int result = two - one;
	reg[getregister(mover,2)] = result;
	
	setzeroandsignflag(result);

	if(two > 0 && one < 0 && result < 0)
	{
		OF = 1;
	}

	else if(two < 0 && one > 0 && result > 0)
	{
		OF = 1;
	}

	else
	{
		OF = 0;
	}
	
}

void andlf(int mover)
{
	mover = mover + 1; //mover is at register byte
	int value = reg[getregister(mover, 1)] & reg[getregister(mover, 2)];
	reg[getregister(mover, 2)] = value;

	setzeroandsignflag(value);

	OF = 0;
}

void xorlf(int mover)
{
	mover = mover + 1; //mover is at register byte
	int value = reg[getregister(mover, 1)] ^ reg[getregister(mover, 2)];
	reg[getregister(mover, 2)] = value;

	setzeroandsignflag(value);

	OF = 0;
}

void mullf(int mover)
{
	mover = mover + 1;  //mover is at register byte
	int one = reg[getregister(mover, 1)]; 
	int two = reg[getregister(mover, 2)];
	int value = one * two;
	reg[getregister(mover,2)] = value;

	setzeroandsignflag(value);

	if(one > 0 && two > 0 && value < 0)
	{
		OF = 1;
	}

	else if(one < 0 && two < 0 && value < 0)
	{
		OF = 1;
	}

	else if(one < 0 && two > 0 && value > 0)
	{
		OF = 1;
	}

	else if(one > 0 && two < 0 && value > 0)
	{
		OF = 1;
	}

	else
	{
		OF = 0;
	}

}

void cmplf(int mover)
{
	mover = mover + 1; //mover is now at register byte
	int one = reg[getregister(mover,1)];
	int two = reg[getregister(mover,2)];
	int result = two - one;
	
	setzeroandsignflag(result);

	if(two > 0 && one < 0 && result < 0)
	{
		OF = 1;
	}

	else if(two < 0 && one > 0 && result > 0)
	{
		OF = 1;
	}

	else
	{
		OF = 0;
	}


}

void setzeroandsignflag(int value)
{
	if(value == 0)
	{
		ZF = 1; 
	}

	else
	{
		ZF = 0;
	}

	if (value < 0)
	{
		SF = 1;
	}

	else
	{
		SF = 0;
	}
}


int hextodecimal(int mover)  //input is a pointer to the char before the 32 bit number starts
{	
	char arr[9];
	mover = mover + 1; //mover is now at first byte in 32 bit value
	int arrmover = 0;
	int i =0;
	int sum = 0;
	int num;

	while(i < 4)
	{
		sprintf(&arr[arrmover], "%02x", memory[mover]);
		arrmover = arrmover + 2;
		mover = mover + 1;	
		i = i +1;
	}

	char * pointer = &arr[0];
	pointer = pointer + 1;
	int iterator = 4;
	
	int power = 0;

	while(iterator > 0)
	{
		if(isalpha(*pointer))
		{
			if((*pointer >= 'a' && *pointer <='f') || (*pointer >= 'A' && *pointer <='F') )
			{
				num = *pointer - 'W';
			}

			else
			{
				fprintf(stderr, "ERROR: invalid 32 bit value \n");
				exit(0);
			}

		}

		else
		{
			num = *pointer - '0';
		}
		
		
		sum = sum +  ( (int)pow(16, power) * num );  //any digit that is not the most significant is positive

		pointer = pointer - 1;
		power = power + 1;
		
		if(isalpha(*pointer))
		{
			if((*pointer >= 'a' && *pointer <='f') || (*pointer >= 'A' && *pointer <='F') )
			{
				num = *pointer - 'W';
			}

			else
			{
				fprintf(stderr, "ERROR: invalid 32 bit value \n");
				exit(0);
			}

		}

		else
		{
			num = *pointer - '0';
		}



		sum = sum + ( (int)pow(16, power) * num );

		power = power + 1;

		if(iterator > 1)
		{
			pointer = pointer + 3;
		}

		iterator = iterator - 1;
		//printf("sum is %d \n", sum);

	}

	return sum;



}