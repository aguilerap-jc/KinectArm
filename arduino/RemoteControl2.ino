#include "IRremote.h"
int receiver = 26; 
int countLed = 0; int led=13;
int grabMotor1 = 22; int grabMotor2 = 24;
int wristMotor1 = 5; int wristMotor2 = 6;  
int foreArmMotor1 = 8; int foreArmMotor2 = 7;
int armMotor1 = 4; int armMotor2 = 3;
int spinMotor1 = 12; int spinMotor2 = 11; 

char matrix[5][9]={{'a','b','c','d','e','A','B','C','D'},
                   {'f','g','h','i','j','F','G','H','I'},
                   {'k','l','m','n','o','K','L','M','N'},
                   {'p','q','r','s','t','P','Q','R','S'},
                   {'u','v','w','x','y','U','V','W','X'}}; //Position array of all Motors
int posI0=4; int posI1=4; int posI2=9; int posI3=4;
int posI4=9;

int posF0=4; int posF1=4; int posF2=4; int posF3=4;
int posF4=9;

//boolean once=false;
//int test1=22;
//int test2=24;

IRrecv irrecv(receiver);           
decode_results results;           

void setup()    
{
  Serial.begin(9600);  
  irrecv.enableIRIn(); 
  pinMode(led,OUTPUT); pinMode(grabMotor1,OUTPUT); pinMode(grabMotor2,OUTPUT); pinMode(wristMotor1,OUTPUT); pinMode(wristMotor2,OUTPUT); 
  pinMode(foreArmMotor1,OUTPUT); pinMode(foreArmMotor2,OUTPUT); pinMode(armMotor1,OUTPUT); pinMode(armMotor2,OUTPUT); pinMode(spinMotor1,OUTPUT); pinMode(spinMotor2,OUTPUT);
  //pinMode(test1,OUTPUT); pinMode(test2,OUTPUT);
}

void loop()   
{
  if(Serial.available()>0) //Checking which motor is moving and where it's initial position is located in
  {
    char movement = Serial.read();
    if(movement=='a'||movement=='b'||movement=='c'||movement=='d'||movement=='e'||movement=='A'||movement=='B'||movement=='C'||movement=='D'){
      movementMethod(0,movement);}
    if(movement=='f'||movement=='g'||movement=='h'||movement=='i'||movement=='j'||movement=='F'||movement=='G'||movement=='H'||movement=='I'){
      movementMethod(1,movement);}
    if(movement=='k'||movement=='l'||movement=='m'||movement=='n'||movement=='o'||movement=='K'||movement=='L'||movement=='M'||movement=='N'){
      movementMethod(2,movement);}
    if(movement=='p'||movement=='q'||movement=='r'||movement=='s'||movement=='t'||movement=='P'||movement=='Q'||movement=='R'||movement=='S'){
      movementMethod(3,movement);}
    if(movement=='u'||movement=='v'||movement=='w'||movement=='x'||movement=='y'||movement=='U'||movement=='V'||movement=='W'||movement=='X'){
      movementMethod(4,movement);}
    if(movement=='Z'){digitalWrite(led,HIGH);}
    if(movement=='z'){digitalWrite(led,LOW);}
  }
        
  if (irrecv.decode(&results)) 
  {
    translateIR(); 
    irrecv.resume(); 
  } 
 delay(500); 
  //if(!once){analogWrite(test1,200); analogWrite(test2,0); delay(200); analogWrite(test1,0); once=true;}
  //else{analogWrite(test1,0);analogWrite(test2,0);}
}

void translateIR() //Calibration for Motor in Remote Control
{
   Serial.println(results.value);
   if(results.value==691090||results.value==2415644012){countLed++;} //On Button
   if(results.value==1168||results.value==3024981499){ analogWrite(grabMotor2,200);analogWrite(grabMotor1,0); delay(100); analogWrite(grabMotor2,0);}  //Vol + Button (OPEN)
   if(results.value==3216||results.value==1133087936){ analogWrite(grabMotor1,200);analogWrite(grabMotor2,0); delay(100); analogWrite(grabMotor1,0);}  //Vol - Button (CLOSE)
   
   if(results.value==912274||results.value==119733513){ analogWrite(spinMotor2,250);analogWrite(spinMotor1,0); delay(400); analogWrite(spinMotor2,0);} //<- Button (SPIN LEFT)
   if(results.value==256914||results.value==14279632){ analogWrite(spinMotor1,250); analogWrite(spinMotor2,0); delay(400); analogWrite(spinMotor1,0);} //-> Button (SPIN RIGHT)
 
   if(results.value==527250||results.value==936730692){ digitalWrite(armMotor2,HIGH);digitalWrite(armMotor1,LOW); delay(400); digitalWrite(armMotor2,LOW);} //2 Button (GO FORWARD BELOW)
   if(results.value==134034||results.value==1544747263){ digitalWrite(armMotor1,HIGH);digitalWrite(armMotor2,LOW); delay(400); digitalWrite(armMotor1,LOW);} //5 Button (GO BACKWARD BELOW)
   
   if(results.value==265106){ digitalWrite(foreArmMotor2,HIGH);digitalWrite(foreArmMotor1,LOW); delay(400); digitalWrite(foreArmMotor2,LOW);} //3 Button (GO UP FOREARM)
   if(results.value==658322){ digitalWrite(foreArmMotor1,HIGH);digitalWrite(foreArmMotor2,LOW); delay(400); digitalWrite(foreArmMotor1,LOW);} //6 Button (GO DOWN FOREARM)
   
   if(results.value==2962){ digitalWrite(wristMotor2,HIGH);digitalWrite(wristMotor1,LOW); delay(400); digitalWrite(wristMotor2,LOW);} //1 Button (GO UPWARD WRIST BELOW)
   if(results.value==789394){ digitalWrite(wristMotor1,HIGH);digitalWrite(wristMotor2,LOW); delay(400); digitalWrite(wristMotor1,LOW);} //4 Button (GO DOWNWARD WRIST BELOW)
   
   if(countLed%2==1){digitalWrite(led,HIGH);} //turn on LED
   if(countLed%2==0){digitalWrite(led,LOW);} // turn off LED
}

void movementMethod(int motor, char movement)
{
  Serial.println(motor +" "+ movement);
  int posTemp=0;
  int difference=0;
  boolean quit=false;
  
  for(int i=0;i<9&&!quit;i++)
  {
   if(matrix[motor][i]==movement)
    {
     posTemp=i;
     quit=true; 
    } 
  }
  switch(motor)
  {
    case 0:
            posF0=posTemp; //SPINNING MOVMENT
            difference = posF0-posI0;
            if(difference<0){ //SPIN TO THE LEFT
              while(posF0!=posI0){
                analogWrite(spinMotor2,250);analogWrite(spinMotor1,0); delay(400); analogWrite(spinMotor2,0); 
                posI0--;}}
            if(difference>0){ //SPIN TO THE RIGHT
              while(posF0!=posI0){
                analogWrite(spinMotor1,250); analogWrite(spinMotor2,0); delay(400); analogWrite(spinMotor1,0);
                posI0++;}}
            //delay of 500
            break;
    case 1:
            posF1=posTemp; //FORWARD OF BACKWARD ARM MOVEMENT
            difference = posF1-posI1;
            if(difference<0){ //GO FORWARD
              while(posF1!=posI1){
                digitalWrite(armMotor2,HIGH);digitalWrite(armMotor1,LOW); delay(200); digitalWrite(armMotor2,LOW);
                posI1--;}}
            if(difference>0){ //GO BACKWARD
              while(posF1!=posI1){
                digitalWrite(armMotor1,HIGH);digitalWrite(armMotor2,LOW); delay(200); digitalWrite(armMotor1,LOW);
                posI1++;}}
            //delay of 300
            break;
    case 2:
            posF2=posTemp; //UP OR DOWN FOREARM MOVEMENT
            difference = posF2-posI2;
            if(difference<0){ //GO UPWARD FOREARM
              while(posF2!=posI2){
                digitalWrite(foreArmMotor2,HIGH);digitalWrite(foreArmMotor1,LOW); delay(250); digitalWrite(foreArmMotor2,LOW);
                posI2--;}}
            if(difference>0){ //GO DOWNWARD FOREWARM
              while(posF2!=posI2){
                digitalWrite(foreArmMotor1,HIGH);digitalWrite(foreArmMotor2,LOW); delay(250); digitalWrite(foreArmMotor1,LOW);
                posI2++;}}
            //3 down 6 up
            break;
    case 3:
            posF3=posTemp; //WRIST MOVEMENT
            difference = posF3-posI3;
             if(difference<0){ //GO UPWARD WRIST
              while(posF3!=posI3){
                digitalWrite(wristMotor2,HIGH);digitalWrite(wristMotor1,LOW); delay(300); digitalWrite(wristMotor2,LOW);
                posI3--;}}
            if(difference>0){ //GO DOWNWARD WRIST
              while(posF3!=posI3){
                digitalWrite(wristMotor1,HIGH);digitalWrite(wristMotor2,LOW); delay(300); digitalWrite(wristMotor1,LOW);
                posI3++;}}
            //delay of 300
            break;
    case 4:
            posF4=posTemp; //OPENING AND CLOSING OF HAND MOVEMENT
            difference = posF4-posI4;
            if(difference>0){ //GO UPWARD WRIST
              while(posF4!=posI4){
                analogWrite(grabMotor2,200);analogWrite(grabMotor1,0); delay(50); analogWrite(grabMotor2,0);
                posI4++;}}
            if(difference<0){ //GO DOWNWARD WRIST
              while(posF4!=posI4){
                analogWrite(grabMotor1,200);analogWrite(grabMotor2,0); delay(50); analogWrite(grabMotor1,0);
                posI4--;}}
            //delay of 30
            break;
    default:
            Serial.println("Hello");
            break;
  }
}