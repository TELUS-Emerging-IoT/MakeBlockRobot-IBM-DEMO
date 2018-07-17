#include <MeMegaPi.h>
#include <MeLEDMatrix.h>
#include <MePort.h>
#include <SoftwareSerial.h>

uint8_t motorSpeed = 200;
char val;
float dist;

void face(char emotion);

MeMegaPiDCMotor motor1(PORT3B);
MeMegaPiDCMotor motor2(PORT1B);
MeMegaPiDCMotor motor3(PORT2B);
MeMegaPiDCMotor motor4(PORT4B);
MeUltrasonicSensor ultraSensor(PORT_6);

MeLEDMatrix Matrix_1(PORT_5);

uint8_t Heart[16]= {0x00,0x38,0x3c,0x1e,0x3c,0x38,0x00,0x00,0x00,0x00,0x38,0x3c,0x1e,0x3c,0x38,0x00}; 
uint8_t Happy[16] = {0x00,0x08,0x10,0x20,0x10,0x08,0x00,0x00,0x00,0x00,0x08,0x10,0x20,0x10,0x08,0x00}; 
uint8_t Sad[16] = {0x00,0x04,0x08,0x08,0x18,0x00,0x00, 0x00,0x00,0x00,0x00,0x18,0x08,0x08,0x04,0x00}; 
uint8_t Mad[16] = {0x00,0x20,0x10,0x08,0x04,0x00,0x00,0x00, 0x00, 0x00,0x00,0x04,0x08,0x10,0x20,0x00}; 
uint8_t Dead[16] = {0x00,0x22,0x14,0x08,0x14,0x22,0x00,0x00,0x00,0x00,0x22,0x14,0x08,0x14,0x22,0x00};
uint8_t RIP[16] = {0x00,0x7e,0x48,0x4c,0x32,0x00,0x00,0x7e,0x00,0x00,0x7e,0x48,0x48,0x48,0x30,0x00};
uint8_t RestL[16] = {0x00,0x1c,0x2e,0x2e,0x22,0x1c,0x00,0x00,0x00,0x00,0x1c,0x2e,0x2e,0x22,0x1c,0x00};
uint8_t RestR[16] = {0x00,0x1c,0x22,0x2e,0x2e,0x1c,0x00,0x00,0x00,0x00,0x1c,0x22,0x2e,0x2e,0x1c,0x00};

void setup()
{
 Serial.begin(9600);
 Serial3.begin(9600);
 Serial.println("Ready!");
}

void loop()
{
  face('1');
  val = 'z';
  
  if (Serial.available() > 0){
    val = Serial.read(); 
    Serial.println(val);
  }

  if (Serial3.available() > 0){
    val = Serial3.read();
    Serial.println(val);
  }
   
  if (val== 'x') { //move forwards
      motor2.run(-motorSpeed);
      motor1.run(-motorSpeed);
      delay(1500); 
      motor2.stop();
      motor1.stop(); 
  }
  else if (val== 'y') { // move backwards
 
      motor2.run(motorSpeed);
      motor1.run(motorSpeed);
      delay(1500); 
      motor2.stop();
      motor1.stop(); 
  }
    else if (val == 'l') { //turn left
      motor2.run(motorSpeed);
      motor1.run(-motorSpeed);
      delay(1100);
      motor2.stop();
      motor1.stop();
  }
    else if (val == 'r') { //turn right
      motor2.run(-motorSpeed);
      motor1.run(motorSpeed);
      delay(1100);
      motor2.stop();
      motor1.stop();
  }
    else if (val == 't') { //pickup
      dist = ultraSensor.distanceCm();
      while(dist > 15.80){
        motor2.run(-motorSpeed);
        motor1.run(-motorSpeed);
        delay(250);
        motor2.stop();
        motor1.stop();
        dist = ultraSensor.distanceCm();
        Serial.print(ultraSensor.distanceCm());
      }
      motor3.run(motorSpeed);
      delay(500);
      motor3.stop();
      motor4.run(motorSpeed);
      delay(1250);
      motor4.stop();
      delay(300);
      motor3.run(-motorSpeed);
      delay(500);
      motor3.stop();
      motor2.run(-motorSpeed);
      motor1.run(-motorSpeed);
      delay(600);
      motor2.stop();
      motor1.stop();
      face('2');
      delay(3000);
  }
    else if (val == 'd') { //drop down 
      face('4');
      motor3.run(motorSpeed);
      delay(600);
      motor3.stop();
      delay(500);
      motor4.run(-motorSpeed);
      delay(1250);
      motor4.stop();
      delay(500);
      motor3.run(-motorSpeed);
      delay(600);
      motor3.stop();
      delay(1000);
      motor2.run(motorSpeed);
      motor1.run(motorSpeed);
      delay(500);
      motor2.stop();
      motor1.stop();
  }
  else if (val== 'q') { //turn left and move forward
      motor2.run(-motorSpeed);
      motor1.run(-motorSpeed);
      delay(2200);
      motor2.stop();
      motor1.stop();
      motor2.run(motorSpeed);
      motor1.run(-motorSpeed);
      delay(1000); 
      motor2.stop();
      motor1.stop(); 
  }
  else if (val == 'i') { //Initialization sequence
      face('2');
      motor3.run(motorSpeed);
      delay(1500);
      motor3.stop();
      motor3.run(-motorSpeed);
      delay(1400);
      motor3.stop();
      motor4.run(motorSpeed);
      delay(1500);
      motor4.stop();
      delay(200);
  }
  else if (val== 'f') { //move forwards until you reach can
      
      dist = ultraSensor.distanceCm();
      while(dist > 23.80){
        motor2.run(motorSpeed);
        motor1.run(-motorSpeed);
        delay(250);
        motor2.stop();
        motor1.stop();
        dist = ultraSensor.distanceCm();
        Serial.print(ultraSensor.distanceCm());
      }
      
  } 
  else if (val == 'o') {
      motor2.run(-motorSpeed);
      motor1.run(-motorSpeed);
      delay(2200);
      motor2.stop();
      motor1.stop();
  }
  else if (val == 'p') {
        face('1');
        motor3.run(-motorSpeed);
        delay(500);
        motor3.stop();
        motor3.run(motorSpeed);
        delay(550);
        motor3.stop();
        motor3.run(-motorSpeed);
        delay(500);
        motor3.stop();
        motor3.run(motorSpeed);
        delay(550);
        motor3.stop();
        motor3.run(-motorSpeed);
        delay(500);
        motor3.stop();
        motor3.run(motorSpeed);
        delay(550);
        motor3.stop();
     
  }
  else if (val=='z') {
      motor1.stop();
      motor2.stop();
      motor3.stop();
      motor4.stop();
  }
  else if (val == 'u'){
      face('1');
      motor3.run(motorSpeed);
      delay(700);
      motor3.stop();
  }
  else if (val == 'v'){
      face('1');
      motor3.run(-motorSpeed);
      delay(500);
      motor3.stop();
  }
  else if (val == '0'){
      face('2');
      delay(2000);
  }
  else if (val != 1){
      face(val);
      delay(3000);
  }
}

void face(char emotion){
  /*case : 1 = rest, 2 = happy, 3 = mad, 4 = sad, 5 = dead, 6 = love*/
  switch(emotion){
    case '1': Matrix_1.drawBitmap(0,0,sizeof(RestL), RestL);
              delay(1000);
              Matrix_1.drawBitmap(0,0,sizeof(RestR), RestR); 
              delay(1000); 
              break;
    case '2': Matrix_1.drawBitmap(0,0,sizeof(Happy), Happy);
              break;
    case '3': Matrix_1.drawBitmap(0,0,sizeof(Mad), Mad);
              break;
    case '4': Matrix_1.drawBitmap(0,0,sizeof(Sad), Sad);
              break;
    case '5': Matrix_1.drawBitmap(0,0,sizeof(Dead), Dead);
              delay(1000);
              Matrix_1.drawBitmap(0,0,sizeof(RIP), RIP);
              delay(1000);
              Matrix_1.drawBitmap(0,0,sizeof(Dead), Dead);
              delay(1000);
              Matrix_1.drawBitmap(0,0,sizeof(RIP), RIP);
              delay(1000);
              break;
    case '6': Matrix_1.drawBitmap(0,0,sizeof(Heart), Heart);
              break;
    default:  break;
    
      }
  }

void blink(){
    
  for(uint8_t i=4;i>0;i--)
  {
    Matrix_1.setBrightness(i);
    delay(50);
  }
  
  for(uint8_t i=0;i<4;i++)
  {
    Matrix_1.setBrightness(i);
    delay(50);
  }
}


