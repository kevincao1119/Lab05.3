CC=gcc
CFLAGS=-lWarn -pedantic

HouseAlarm: HouseAlarm.o libmyifttt.a
	$(CC) HouseAlarm.o -L. -lwiringPi -lm -lmyifttt -lcurl -o HouseAlarm

tester: tester.o libmyifttt.a
	$(CC) tester.o -L. -lmyifttt -lcurl -o tester

blink: blink.o 
	$(CC) blink.o -lwiringPi -o blink

button: button.o 
	$(CC) button.o -lwiringPi -o button

libmyifttt.a:	ifttt.o
	ar -rcs libmyifttt.a ifttt.o

ifttt.o: 	ifttt.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

tester.o:	tester.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

HouseAlarm.o:	HouseAlarm.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

all:	tester HouseAlarm blink button

clean:
	rm tester irtester *.o