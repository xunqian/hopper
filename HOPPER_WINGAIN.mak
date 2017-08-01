CC = iccavr
LIB = ilibw
CFLAGS =  -e -D__ICC_VERSION="7.21A" -DATMega64  -l -g -MLongJump -MHasMul -MEnhanced -Wf-use_elpm 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -g -e:0x10000 -ucrtatmega.o -bfunc_lit:0x8c.0x10000 -dram_end:0x10ff -bdata:0x100.0x10ff -dhwstk_size:30 -beeprom:0.2048 -fihx_coff -S2
FILES = main.o 

HOPPER_WINGAIN:	$(FILES)
	$(CC) -o HOPPER_WINGAIN $(LFLAGS) @HOPPER_WINGAIN.lk   -lcatmega
main.o: C:\iccv7avr\include\iom64v.h C:\iccv7avr\include\macros.h C:\iccv7avr\include\AVRdef.h .\main.h .\command.h
main.o:	main.c
	$(CC) -c $(CFLAGS) main.c
