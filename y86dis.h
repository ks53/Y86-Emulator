#ifndef _y86dis_H_
#define _y86dis_H_

typedef enum 
{
	AOK,   //everything fine, no detected error
	HLT,   //halt instruction encountered
	ADR,   //invalid address encountered
	INS,   //invalid instruction
}statuscode;

typedef enum 
{
	nop, //00
	halt, //10
	rrmovl, //20
	irmovl, //30
	rmmovl, //40
	mrmovl, //50
	addl, //60
	subl, //61
	andl, //62
	xorl, //63
	mull, //64
	cmpl, //65
	jmp, //70
	jle, //71
	jl, //72
	je, //73
	jne, //74
	jge, //75
	jg, //76
	call, //80
	ret, //90
	pushl, //a0
	popl, //b0
	readb, //C0
	readl, //C1
	writeb, //D0
	writel, //D1
	movsbl, //e0
} cmd;

char * regname(int address, int which);
int getregister(int address, int which);
int execute(int st);
int jf(int mover);

#endif /* _y86dis_H_ */