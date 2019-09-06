CC=avr-gcc
CPPFLAGS=-DF_CPU=10000000UL -DBAUD=115200UL 
CFLAGS=-std=c99 -Os -mmcu=atmega328p -fshort-enums 
LDFLAGS=-mmcu=atmega328p

gestirq.o: gestirq.h
spibus.o: spibus.h
dds.o: dds.h spibus.h
queue.o: queue.h
serial.o: serial.h queue.h
cli.o: cli.h dds.h 
shell.o: shell.h cli.h gestirq.h

main.o: serial.h shell.h 
main: serial.o queue.o shell.o cli.o dds.o spibus.o gestirq.o 


prova.o: serial.h dds.h
prova: serial.o queue.o dds.o spibus.o

%.hex : %
	avr-objcopy -Oihex $< $@

%_c: %.hex
	avr-size -C --mcu=atmega328p $*			
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -U $<
	rm *.o 


%_d: %.hex
	avrdude -c arduino -p atmega328p -b 57600 -P /dev/ttyUSB0 -U $<
	rm *.o 

%_c1: %.hex
	avrdude -c arduino -p atmega328p -P /dev/ttyACM1 -U $<
	rm *.o 

%_isp: %.hex
	avr-size -C --mcu=atmega328p $*			
	avrdude -c avrisp -b 19200 -p atmega328p -P /dev/ttyACM0 -U $<
	rm *.o 	

.PHONY: clean veryclean
clean:
	\rm -f *~ *.o *.s *.hex *#
#	$(MAKE) -C libpbn clean
veryclean: clean
	\rm -f main
#	$(MAKE) -C libpbn veryclean




