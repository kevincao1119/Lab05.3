#include <stdio.h>
#include <wiringPi.h>
#include <curl/curl.h>
#include <time.h>
#include <math.h>
#include "ifttt.h"

void alarm_off();
void alarm_arming();
void alarm_armed();
int alarm_triggered();
int alarm_sounding();

enum {
	ALARM_OFF,
	ALARM_ARMING,
	ALARM_ARMED,
	ALARM_TRIGGERED,
	ALARM_SOUNDING
} state;

int main(void) {
    
	wiringPiSetup();
	pinMode(4, OUTPUT); /* BUZZER */
	pinMode(1, OUTPUT); /* LED 1 (RED) */
	pinMode(2, OUTPUT); /* LED 2 (GREEN) */
	pinMode(3, INPUT); /* BUTTON */
	pinMode(0, INPUT); /* PIR SENSOR */
	pullUpDnControl(3, PUD_UP);
  
    for (;;)
    {
        if (state == ALARM_OFF)
        {
            alarm_off();
            if (digitalRead(3) == 0)
            {
                state = ALARM_ARMING;    
            }
        }
        else if (state == ALARM_ARMING)
        {
            alarm_arming();
            state = ALARM_ARMED;
        }
        else if (state == ALARM_ARMED)
        {
            if (digitalRead(3) == 0)
            {
                state = ALARM_OFF;
            }
            else if (digitalRead(0) == 1)
            {
                state = ALARM_TRIGGERED;
            }
        }
        else if (state == ALARM_TRIGGERED)
        {
            alarm_triggered();
        }
        if (state == ALARM_SOUNDING)
        {
            alarm_sounding();
        }
    }
    return 0;
}

void alarm_off()
{
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
}

void alarm_arming()
{
    time_t start, end;
    double elapsed;  /* seconds */
    start = time(NULL);
    int terminate = 1;
    
    while (terminate)
    {
        end = time(NULL);
        elapsed = difftime(end, start);
        if (elapsed <= 10.0)
        {
            if(fmod(elapsed, 2) == 0) /* blink if remainder is 0 */
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

void alarm_armed()
{

}

int alarm_triggered()
{
    time_t start, end;
    double elapsed;  /* seconds */
    start = time(NULL);
    int terminate = 1;

    while (terminate)
    {
        end = time(NULL);
        elapsed = difftime(end, start);
        if (elapsed <= 10.0 && digitalRead(3) == 1)
        {
            if (fmod(elapsed, 2) == 0)
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
            terminate = 0;
            state = ALARM_SOUNDING;
        }
        else
        {
            terminate = 0;
            state = ALARM_OFF;
        }
    }
    return state;
}

int alarm_sounding()
{
    ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/n4lcdYxuEnhjgJ5CmGtmV3Y8_PAH4KsPNpFIO4V78Ux", "test1", "test2", "test3");

    time_t start, end;
    double elapsed; 
    start = time(NULL);
    int terminate = 1;

    while (terminate)
    {
        end = time(NULL);
        elapsed = difftime(end, start);
        if (elapsed >= 0.0 && digitalRead(3) == 1)
        {
            if (fmod(elapsed, 3) == 0)
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
            terminate = 0;
            state = ALARM_OFF;
        }
    }
    return state;
}