#include "IRremote.h"
int receiver = 10; 
// S = pin receiver//
// - = ground of Arduino//
// Middle one = 5 volts//

IRrecv irrecv(receiver);           
decode_results results;           

void setup()    
{
  Serial.begin(9600);  
  irrecv.enableIRIn(); 
  pinMode(13,OUTPUT);
}

void loop()   
{
  if (irrecv.decode(&results)) 
  {
    translateIR(); 
    irrecv.resume(); 
  } 
 delay(500); 
}

void translateIR() 
{
   Serial.println(results.value,HEX);
   if(results.value==0xFF629D){
     digitalWrite(13,HIGH);
   }
   if(results.value==0xFFA857){
     digitalWrite(13,LOW);
   }
   delay(100);
}

