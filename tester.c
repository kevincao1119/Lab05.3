#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#include <math.h>
#include "ifttt.h"

enum s {ALARM_OFF, ALARM_ARMING, ALARM_ARMED, ALARM_TRIGGERED, ALARM_SOUNDING};

int main(int argc, char *argv[])
{ 
	enum s state = ALARM_OFF;
	
	wiringPiSetup () ;
	/*LED1*/pinMode (1, OUTPUT) ;
	/*LED2*/pinMode (2, OUTPUT) ;
	/*Button*/pinMode(3, INPUT) ;
			  pullUpDnControl (3, PUD_UP) ;
	/*Alarm*/pinMode (4, OUTPUT) ;
	
	time_t start, end;
    double elapsed;  /* seconds */
    start = time(NULL);
	int terminate = 1;
	
	for(;;)
	{	
		int x = digitalRead(3);
		
		if (state == ALARM_OFF) 
		{
			digitalWrite(1, HIGH);
			digitalWrite(2, LOW);
			digitalWrite(4, LOW);
			if (x == 0)
			{
				state = ALARM_ARMING;
				printf("System Arming");
			}
		}
		
		if (state == ALARM_ARMING) 
		{	
			while (terminate) 
			{
				end = time(NULL);
				elapsed = difftime(end, start);
				
				if (elapsed <= 10.0) 
				{
					if (fmod(elapsed,2) == 0) 
					{
						digitalWrite(1, HIGH);
					}
					else 
					{
						digitalWrite(1, LOW);
					}
				}
				else 
				{
					terminate = 0;
				}
			}
			
			digitalWrite(1, LOW);
			digitalWrite(2, HIGH);
		}
		
		if (state == ALARM_ARMED) 
		{
			if (x == 0) 
			{
				state = ALARM_OFF;
			}
			if (digitalRead(0) == 0) 
			{
				state == ALARM_TRIGGERED;
			}
		}
		
		if (state == ALARM_TRIGGERED) 
		{
			while (terminate) 
			{
				end = time(NULL);
				elapsed = difftime(end, start);
				
				if (elapsed <= 10.0) 
				{
					if (fmod(elapsed,2) == 0) 
					{
						digitalWrite(1, HIGH);
						digitalWrite(2, HIGH);
					}
					else 
					{
						digitalWrite(1, LOW);
						digitalWrite(2, LOW);
					}
				}
				else if (elapsed >= 10.0) 
				{
					state = ALARM_SOUNDING;
					terminate = 0;
				}
				else 
				{
					state = ALARM_OFF;
					terminate = 0;
				}
			}
		}
		
		if (state == ALARM_SOUNDING) 
		{
			ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/n4lcdYxuEnhjgJ5CmGtmV3Y8_PAH4KsPNpFIO4V78Ux", "my1", "my 2", "my 33333");
			while (terminate) 
			{
				end = time(NULL);
				elapsed = difftime(end, start);
				
				if (elapsed <= 10.0) 
				{
					if (fmod(elapsed,3) == 0) 
					{
						digitalWrite(1, HIGH);
						digitalWrite(2, HIGH);
						digitalWrite(4, HIGH);
					}
					else 
					{
						digitalWrite(1, LOW);
						digitalWrite(2, LOW);
						digitalWrite(4, LOW);
					}
				}
				else 
				{
					state = ALARM_OFF;
					terminate = 0;
				}
			}
		}
	}
	printf("Trying to connect to server\n");
	
	return 0;
}
