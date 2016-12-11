#ifndef _y86emul_H_
#define _y86emul_H_

void rrmovlf(int mover);
void irmovlf(int mover);
void rmmovlf(int mover);
void mrmovlf(int mover);
void addlf(int mover);
void sublf(int mover);
void andlf(int mover);
void xorlf(int mover);
void mullf(int mover);
void cmplf(int mover);
int jf(int mover);
void readbf(int mover);
void readlf(int mover);
void writebf(int mover);
void writelf(int mover);
void movsblf(int mover);
void pushlf(int mover);
void poplf(int mover);
void callf(int mover);
int getregister(int address, int which);
void execute(int st);
int hextodecimal(int mover);
void setzeroandsignflag(int mover);

typedef enum 
{
	AOK,   //everything fine, no detected error
	HLT,   //halt instruction encountered
	ADR,   //invalid address encountered
	INS,   //invalid instruction
}statuscode;

statuscode status;

typedef enum 
{
	nop, //00
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

#endif /* _y86emul_H_ */