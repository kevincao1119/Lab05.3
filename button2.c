#include <stdio.h>
#include <wiringPi.h>
int main (int argc, char *argv[]) 
{
  wiringPiSetup();
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
  pullUpDnControl(3, PUD_UP);

  for(;;)
  {
    int x = digitalRead(1);
    if (x == 0) 
    {
      digitalWrite(1, HIGH);
    }
    else 
    {
      digitalWrite(1, LOW);
    } 
  }
  return 0;
}
