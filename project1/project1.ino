#include <Eventually.h>//eventually library https://github.com/johnnyb/Eventually

//Ultrasonic sensor
int TRIG_PIN = 2; // TRIG_PIN
int ECHO_PIN = 4; // ECHO_PIN
double distance = 0;
//LED
int ledPins[] = {5,6,7,8,9,12}; //LED pins
int ledCount = 6; // Amount of LEDs
//RGB LED
int R_PIN = 11; //Red pin
int G_PIN = 10; //Green pin
int B_PIN = 3; // Blue pin
//Joystick
int SW_PIN = 13;
int X_PIN = 1;
int Y_PIN = 0;
int L = 505;
int R = 540;
//variables and instantiations
EvtManager mgr;
int arduinoMode = 1;

void setup() { 
  //ultrasonic sensor pin setup
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);  
  //LED setup
  for(int i=0; i<ledCount;i++)
  {
    pinMode(ledPins[i],OUTPUT); //Set all LEDs to output mode
  }
  //RGB LED setup
  pinMode(R_PIN,OUTPUT);
  pinMode(G_PIN,OUTPUT);
  pinMode(B_PIN,OUTPUT);
  //pin setup for joystick module
  pinMode(SW_PIN,INPUT);
  digitalWrite(SW_PIN,HIGH);
  Serial.begin(9600); 
  ledRGB(170,20,0);
  mgr.addListener(new EvtTimeListener(100,true,(EvtAction)joyStick));
  mgr.addListener(new EvtTimeListener(100,true,(EvtAction)choiceMode));
  mgr.addListener(new EvtPinListener(SW_PIN,(EvtAction)joyStickButton));
}
void ledRGB(int red,int green,int blue){
  analogWrite(R_PIN,red);
  analogWrite(G_PIN,green);
  analogWrite(B_PIN,blue);
}
void ultraSound(){
  double maxDistance = 24;
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(TRIG_PIN, LOW);  
// calculation sensor
  distance = pulseIn(ECHO_PIN, HIGH)/58.2; 
  Serial.print("distance :"); 
  Serial.print(distance); 
  Serial.println("cm"); 
// X LEDs will be on depending on X distance between 0 and 35
  if(distance < 5){
    for(int i=0;i<ledCount;i++)
    {
      digitalWrite(ledPins[i],HIGH);
    }
  }
  else if(distance < 10){
    for(int i=0;i<(ledCount-1);i++)
    {
      digitalWrite(ledPins[i],HIGH);
    }
    for(int j=ledCount-1;j<ledCount;j++){
      digitalWrite(ledPins[j],LOW);
    }
  }
  else if(distance < 15){
    for(int i=0;i<(ledCount-2);i++)
    {
      digitalWrite(ledPins[i],HIGH);
    }
    for(int j=ledCount-2;j<ledCount;j++){
      digitalWrite(ledPins[j],LOW);
    }
  }
  else if(distance < 20){
    for(int i=0;i<(ledCount-3);i++)
    {
      digitalWrite(ledPins[i],HIGH);
    }
    for(int j=ledCount-3;j<ledCount;j++){
      digitalWrite(ledPins[j],LOW);
    }
  }
  else if(distance < 25){
    for(int i=0;i<(ledCount-4);i++)
    {
      digitalWrite(ledPins[i],HIGH);
    }
    for(int j=ledCount-4;j<ledCount;j++){
      digitalWrite(ledPins[j],LOW);
    }
  }
  else if(distance <= 30){
    for(int i=0;i<(ledCount-5);i++)
    {
      digitalWrite(ledPins[i],HIGH);
    }
    for(int j=ledCount-5;j<ledCount;j++){
      digitalWrite(ledPins[j],LOW);
    }
  }
  if(distance > 35){
    for(int i=0;i<ledCount;i++)
    {
      digitalWrite(ledPins[i],LOW);
    }
  }
 }
void choiceMode(){
  if(analogRead(X_PIN) < L){
    arduinoMode = 0;
    ledRGB(0,60,0);
  }
  if(analogRead(X_PIN)> R){
    arduinoMode = 2;
    ledRGB(80,0,0);
  }
}
void joyStickButton(){
  // resets lights
    for(int i=0;i<ledCount;i++){
      digitalWrite(ledPins[i],LOW);
    }
	// Changes mode to ping pong game mode
    if(arduinoMode == 2)
    {
    mgr.resetContext();
    mgr.addListener(new EvtTimeListener(100,true,(EvtAction)pingPongReady));
    mgr.addListener(new EvtPinListener(SW_PIN,(EvtAction)joyStickButton));
    arduinoMode=1;
    }
	// Changes mode to ultrasonic sensor mode
    else if(arduinoMode == 0)
    {
//Green colored RGB LED shows that its on ultrasound sensor mode
    ledRGB(0,60,0);
    mgr.resetContext();
    mgr.addListener(new EvtTimeListener(200,true,(EvtAction)ultraSound));
    mgr.addListener(new EvtPinListener(SW_PIN,(EvtAction)joyStickButton));
    arduinoMode=1;
    }
	//Changes mode to select mode
    else if(arduinoMode == 1)
    {
//Yellow colored RGB LED shows that its on select mode
    ledRGB(170,20,0);
    mgr.resetContext();
    mgr.addListener(new EvtTimeListener(100,true,(EvtAction)joyStick));
    mgr.addListener(new EvtTimeListener(100,true,(EvtAction)choiceMode));
    mgr.addListener(new EvtPinListener(SW_PIN,(EvtAction)joyStickButton));
    
  }
}
// 540 > right
// 505 < left
void joyStick(){
  Serial.print("X-axis");
  Serial.print(analogRead(X_PIN));
  Serial.print("\n");
  Serial.print("Y-axis");
  Serial.print(analogRead(Y_PIN));
  Serial.print("\n\n");
  //Right tilted joystick
  if(analogRead(X_PIN)>R){
    for(int i=0; i< (ledCount-2);i++){
      if(analogRead(X_PIN)<1000&&ledPins[i]==ledPins[0])
        digitalWrite(ledPins[i],LOW);
      else{
       digitalWrite(ledPins[i],HIGH);
      }
    }
    for(int j=4; j<ledCount;j++){
      digitalWrite(ledPins[j],LOW);
    }
  }
  //Left tilted joystick
  else if(analogRead(X_PIN)<L){
    for(int i=2; i< ledCount;i++){
       if(analogRead(X_PIN)>60&&ledPins[i]==ledPins[5])
        digitalWrite(ledPins[i],LOW);
      else{
       digitalWrite(ledPins[i],HIGH);
      }
    }
    for(int j=0; j<(ledCount-4);j++){
      digitalWrite(ledPins[j],LOW);
    }
  }
  //Idle joystick
  else{
    for(int i=0; i<ledCount; i++){
      if(ledPins[i] == ledPins[2]||ledPins[i] == ledPins[3])
        digitalWrite(ledPins[i],HIGH);
        else{
          digitalWrite(ledPins[i],LOW);
        }
    }
  }
}
void pingPongReady(){
  digitalWrite(ledPins[0],HIGH);
//Red colored RGB LED shows that its on ping pong mode
  ledRGB(80,0,0);
  //add countdown on rgb LED
  if(analogRead(X_PIN) < L){
    // Ready, Set, GOO!!! (lights)
    ledRGB(0,60,0);
    delay(1000);
    ledRGB(255,50,0);
    delay(1000);
    ledRGB(255,0,0);
    delay(500);
    ledRGB(0,0,0);
    pingPong();
  }
  
}
void pingPong(){
  int points = 0;
  bool endGame=false;
  double difficultyRate=1;
  double difficultyDelay=500*difficultyRate;
  
  while(endGame == false){
    //Difficulty scaling for pingpong
    if(points % 2 == 0 && points != 0){
      
      
      if(difficultyRate > 0.40){
      difficultyRate -= 0.20;
      if(difficultyRate <= 0.40){
        difficultyRate = 0.40;
      }
      difficultyDelay = 500*difficultyRate;
      }
    }
    //Sent ball loop
    for(int i=0; i<ledCount;i++){
      digitalWrite(ledPins[i],HIGH);
      delay(difficultyDelay);
// If joystick wants to hit ball before end point the game ends
      if(analogRead(X_PIN)>R && ledPins[i] != ledPins[5]){
        endGame = true;
        for(int j=0;j<ledCount;j++){
          digitalWrite(ledPins[j],LOW);
        }
        break;
      }
// If joystick points to the same direction the ball is going when it reaches its end point the game ends
      if(analogRead(X_PIN)<R && ledPins[i] == ledPins[5]){
        endGame = true;
        for(int k=0;k<ledCount;k++){
          digitalWrite(ledPins[k],LOW);
        }
        break;
      }
//ball is hit score 1  point
      if(analogRead(X_PIN)>R && ledPins[i] == ledPins[5]){
        points++;
      }
      digitalWrite(ledPins[i],LOW);
    }
    if(endGame == false){
      //ball returning loop
    for(int i=5; i>-1;i--){
      digitalWrite(ledPins[i],HIGH);
      delay(difficultyDelay);
// If joystick wants to hit ball before end point the game ends
      if(analogRead(X_PIN)<L && ledPins[i] != ledPins[0]){
        endGame = true;
        for(int j=5;j>-1;j--){
          digitalWrite(ledPins[j],LOW);
        }
        break;
      }
// If joystick points to the same direction the ball is going when it reaches its end point the game ends
      if(analogRead(X_PIN)>L && ledPins[i] == ledPins[0]){
        endGame = true;
        for(int k=5;k>-1;k--){
          digitalWrite(ledPins[k],LOW);
        }
        break;
      }
//ball is hit score 1  point
      if(analogRead(X_PIN)<L && ledPins[i] == ledPins[0]){
        points++;
      }
      digitalWrite(ledPins[i],LOW);
    }
    }
  }
//When game ends the lights turn off because of while loop exit
  for(int lightsOut=0;lightsOut<ledCount;lightsOut++){
    digitalWrite(ledPins[lightsOut],LOW);
  }
  delay(300);
//Blinking of RGB to display points
  for(int i=0;i<points;i++){
    ledRGB(130,80,0);
    delay(500);
    ledRGB(0,0,0);
    delay(300);
  }
  mgr.addListener(new EvtTimeListener(100,true,(EvtAction)pingPongReady));
  
}
//Loop method replaced by library's loop method
USE_EVENTUALLY_LOOP(mgr)