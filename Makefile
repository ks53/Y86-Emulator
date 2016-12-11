all: y86emul y86dis

y86emul: y86emul.c  
	gcc -Wall -o y86emul y86emul.c -lm

y86dis: y86dis.c
	gcc -Wall -o y86dis y86dis.c -lm

clean:
	rm -f *.o y86emul y86dis