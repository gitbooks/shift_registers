#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 

int SER_Pin = 9;   //pin 14 on the 75HC595
int RCLK_Pin = 10;  //pin 12 on the 75HC595
int SRCLK_Pin = 11; //pin 11 on the 75HC595
int headlightsPin = 2; //pin D2 on Nano

int countThrough = 0;

//How many of the shift registers - change this
#define number_of_74hc595s 3 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

void setup(){

  myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
  
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);
  pinMode(headlightsPin, OUTPUT);


  //reset all register pins
  clearRegisters();
  writeRegisters();
}               


//set all register pins to LOW
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;}

void allLow(){
  for(int i = 0; i < 24; i++){
    setRegisterPin(i, LOW);
    writeRegisters();}}

void allHigh(){
    for(int i = 0; i < 24; i++){
    setRegisterPin(i, HIGH);
    writeRegisters();}}

void flash(int delayTime, int delayReduction, int finishPause){
  while(delayTime > 0){
    allHigh();
    delay(delayTime-= delayReduction);
    allLow();
    delay(delayTime);
  }//end while
    allHigh();
    delay(finishPause);
    allLow();
}//end flash()

void snake(int speedDelay, int LEDLength, int loops, int completionPause){

  
    LEDLength = 24 - LEDLength;
    int iMath=speedDelay*24*loops;    
  
   for(int i=iMath; i >= 0; i-=speedDelay){
      static uint8_t hi_pos = 0;
      static uint8_t lo_pos = LEDLength;
    
      setRegisterPin(hi_pos, HIGH);
      setRegisterPin(lo_pos, LOW);
      writeRegisters();
    
      // ensure it won't overflow:
      if (++hi_pos == 24) hi_pos = 0;
      if (++lo_pos == 24) lo_pos = 0;
      delay(speedDelay);
    }//end for
      delay(completionPause*.5);
      allLow();
      delay(completionPause*.5);
}//end snake

void line(int speedDelay, int midDelay, int endDelay, bool backwards){
  for(int i = 0; i < 24; i++){
    setRegisterPin(i, HIGH);
    writeRegisters();
    delay(speedDelay);
  }//end for
  delay(midDelay);
if(backwards == false) {
  for(int i = 0; i < 24; i++){
    setRegisterPin(i, LOW);
    writeRegisters();
    delay(speedDelay);}}//end if backwards
else{
    for(int i = 24; i > -1; i--){
    setRegisterPin(i, LOW);
    writeRegisters();
    delay(speedDelay);
    }//end for
  }//end else
delay(endDelay);
}//end snake

void randomize(int speedDelay, int iterations, int completionPause){
  while(iterations > 0){
    for(int i = 0; i < 24; i++){
    int randomNumber = random(0, 2);
    setRegisterPin(i, randomNumber);
    writeRegisters();}
    iterations--;
    delay(speedDelay);
  }//end while
  delay(completionPause*.5);
  allLow();
  delay(completionPause*.5);
}//end randomize

void blinker(int delaySpeed, int iterations, int completionPause){
while(iterations > 0){   
    for(int i = 0; i < 24; i+=2){
    setRegisterPin(i, HIGH);
    writeRegisters();}

    for(int i = 1; i < 24; i+=2){
    setRegisterPin(i, LOW);
    writeRegisters();}
delay(delaySpeed);
    for(int i = 1; i < 24; i+=2){
    setRegisterPin(i, HIGH);
    writeRegisters();}

    for(int i = 0; i < 24; i+=2){
    setRegisterPin(i, LOW);
    writeRegisters();}    
delay(delaySpeed);
iterations--;
}//end while
  delay(completionPause*.5);
  allLow();
  delay(completionPause*.5);
}//end alternate

void loop(){


myservo.write(0);                  // sets the servo position according to the scaled value 


digitalWrite(headlightsPin, HIGH);

//flash(200, 10, 0);//takes 3 arguments: (delay, delay increment, completion pause)
snake(50,5,3,0); //takes 4 arguments: (speed, LED length, loops, completion pause);
//line(100, 0, 0, false);//takes 4 arguments (speed, mid delay, completion pause, end backwards?);
//randomize(50,40,0);//takes 3 arguments (delay between sets, number of iternation, completion pause);
//blinker(500, 5, 0);//takes 3 arguments (delay between sets, number of iternation, completion pause);

myservo.write(176);  
delay(1000);
digitalWrite(headlightsPin, LOW);
delay(1000);
}//end loop

