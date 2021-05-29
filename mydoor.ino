#include <IRremote.h>

int RECV_PIN = 3;
const char type ='W';// W for white, B for black. Must keep single quotes like 'B' or 'W'
const boolean PCB = 0;// if receiver is PCB set to 1, if not set to 0. See video for details
boolean displayCode = true;// to display remote code. if not, set to false

//***** Servo settings begins
#include <Servo.h>
Servo RJservo;  // create servo object to control a servo
const int SERVO_PIN = 2;  // analog pin used to connect the potentiometer
int angle =90;    // initial angle  for servo
int angleStep =45;
const int ANGLE_CENTRE =90;// the centre/reset angle of your servo
//**** Servo settings settings ends

// remote settings 
const String RIGHT=">>|";// move servo to the right with this key on remote
const String LEFT ="|<<";// move servo to the left with this key on remote
const String CENTRE ="OK";// move servo to centre with this key on remote

const String OPEN="open";// move servo to the right with this key on remote
const String OPEN1="open1";// move servo to the right with this key on remote
const String OPEN2="open2";// move servo to the right with this key on remote
const String OPEN3="open3";// move servo to the right with this key on remote
const String CLOSE="close";// move servo to the right with this key on remote
const String CLOSE1="close1";// move servo to the right with this key on remote
const String CLOSE2="close2";// move servo to the right with this key on remote
const String CLOSE3="close3";// move servo to the right with this key on remote
const String LOCK=">||";// move servo to the right with this key on remote
// remote settings end

IRrecv irrecv(RECV_PIN);

// this is array holding codes for White Remote when used with PCB version of receiver
unsigned int whiteRemotePCB[] ={
            0xE318261B, // CH-
            0x511DBB,   // CH
            0xEE886D7F,  // CH+

            0x52A3D41F, // |<<
            0xD7E84B1B, // >>|
            0x20FE4DBB, // >||          

            0xF076C13B, // -
            0xA3C8EDDB, // +
            0x12CEA6E6, // EQ

            0xC101E57B, // 0
            0x97483BFB, // 100+
            0xF0C41643, // 200+

            0x9716BE3F, // 1
            0x3D9AE3F7, // 2
            0x6182021B, // 3           

            0x8C22657B, // 4 
            0x488F3CBB, // 5
            0x449E79F,  // 6

            0x32C6FDF7, // 7
            0x1BC0157B, // 8
            0x3EC3FC1B  // 9                          
            };

// this is array holding codes for White Remote when used with non-PCB version of receiver            
unsigned int whiteRemote[] ={
            0xFFA25D, // CH-
            0xFF629D,   // CH
            0xFFE21D,  // CH+

            0xEF3B295B, //open
            0x1FE48B7, //open1
            0xBCFA7A74, //open2
            0x1FE, //open3
            
            0xF1FFE9FB, //close
            0x1FE7887, //close1
            0x49C673B8, //close2
            0xFBC9E3C5, //close3

            0x1FE40BF, // |<<
            0x1FEC03F, // >>|
            0x1FE807F, // >||          

            0xFFE01F, // -
            0xFFA857, // +
            0xFF906F, // EQ

            0xFF6897, // 0
            0xFF9867, // 100+
            0xFFB04F, // 200+

            0xFF30CF, // 1
            0xFF18E7, // 2
            0xFF7A85, // 3           

            0xFF10EF, // 4 
            0xFF38C7, // 5
            0xFF5AA5,  // 6

            0xFF42BD, // 7
            0xFF4AB5, // 8
            0xFF52AD  // 9                          
            };
// key lables of white remote
 String whiteRemoteKey[] ={
            "CH-",
            "CH",
            "CH+",

            "open",
            "open1",
            "open2",
            "open3",
            
            "close",
            "close1",
            "close2",
            "close3",

            "|<<",
            ">>|",
            ">||",

            "-",
            "+",
            "EQ",

            "0",
            "100+",
            "200+",

            "1",
            "2",
            "3",

            "4",
            "5",
            "6",

            "7",
            "8",
            "9"
            };

// this is array holding codes for Black Remote when used with non-PCB version of receiver
 unsigned int blackRemote[] ={
            0xFF629D, // ^
            0xFF22DD,   // <
            0xFF02FD,  // OK
            0xFFC23D, // >
            0xFFA857, // v

            0xFF6897, // 1
            0xFF9867, // 2
            0xF0C41643, // 3           

            0xFF30CF, // 4 
            0xFF18E7, // 5
            0xFF7A85,  // 6

            0xFF10EF, // 7
            0xFF38C7, // 8
            0xFF5AA5,  // 9 

            0xFF42BD, // *
            0xFF4AB5, // 0
            0xFF52AD  // #                                      
            };

// this is array holding codes for Black Remote when used with PCB version of receiver
 unsigned int blackRemotePCB[] ={
            0x511DBB, // ^
            0x52A3D41F,   // <
            0xD7E84B1B,  // OK
            0x20FE4DBB, // >
            0xA3C8EDDB, // v


            0xC101E57B, // 1
            0x97483BFB, // 2
            0xF0C41643, // 3           

            0x9716BE3F, // 4 
            0x3D9AE3F7, // 5
            0x6182021B,  // 6

            0x8C22657B, // 7
            0x488F3CBB, // 8
            0x449E79F,  // 9 

            0x32C6FDF7, // *
            0x1BC0157B, // 0
            0x3EC3FC1B  // #                                      
            };

// Black remote key names
 String blackRemoteKey[] ={
            "^",
            "<",
            "OK",
            ">",
            "v",

            "1",
            "2",
            "3",

            "4",
            "5",
            "6",

            "7",
            "8",
            "9",
            
            "*",
            "0",
            "#"
            };

decode_results results;


void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Robojax IR Decode");
  Serial.println("Servo Control with Remote");
  irrecv.enableIRIn(); // Start the receiver

   RJservo.attach(SERVO_PIN);  // attaches the servo on pin servoPin to the servo object

}

void loop() {

  if (irrecv.decode(&results)) {
    if(displayCode)Serial.println(results.value, HEX);
    robojaxValidateCode(results.value);// used the "robojaxValidateCode" bellow
    irrecv.resume(); // Receive the next value
  }
  delay(50);// 50 melisecond delay
}

/*
 * function: robojaxValidateCode
 * validates the remote code and prints correct key name
 * cd is code poassed from the loop
 * Written by A. S. for Robojax
 */
void robojaxValidateCode(int cd)
{

  // Robojax IR Remote decoder
  int found=0;

 if(type =='W' && !PCB)
 {
    // Robojax IR White Remote decoder
    // if tyepe is set to 'W' (white remote) and PCB=0 then check Black remote code
      for(int i=0; i< sizeof(whiteRemote)/sizeof(int); i++)
      {
        if(whiteRemote[i] ==cd)
        {
          
          Serial.print("Key pressed:");
          Serial.println(whiteRemoteKey[i]);
          servoAction(whiteRemoteKey[i]);// take action
          found=1;
        }// if matched
      }// for
 }else if(type =='W' && PCB){
    // Robojax IR White Remote decoder
    // if tyepe is set to 'W' (white remote) and PCB=1 then check Black remote code
      for(int i=0; i< sizeof(whiteRemotePCB)/sizeof(int); i++)
      {
        if(whiteRemotePCB[i] ==cd)
        {
          
          Serial.print("Key pressed:");
          Serial.println(whiteRemoteKey[i]);
          servoAction(whiteRemoteKey[i]);// take action 
          found=1;
        }// if matched
      }// for  
 }else if(type =='B' && PCB){
    // Robojax IR White Remote decoder
       // if tyepe is set to 'B' (black remote) and PCB=1 then check Black remote code
       for(int i=0; i< sizeof(blackRemotePCB)/sizeof(int); i++)
      {
        // Robojax IR black Remote decoder
        if(blackRemotePCB[i] ==cd)
        {

          Serial.print("Key pressed:");
          Serial.println(blackRemoteKey[i]);
         servoAction(blackRemoteKey[i]);// take action     
          found=1;
        }// if matched
      }// for   
 }else{

      // if tyepe is set to 'B' (black remote) and PCB =0 then check Black remote code
       for(int i=0; i< sizeof(blackRemote)/sizeof(int); i++)
      {
        // Robojax IR black Remote decoder
        if(blackRemote[i] ==cd)
        {

          Serial.print("Key pressed:");
          Serial.println(blackRemoteKey[i]);
          servoAction(blackRemoteKey[i]);// take action          
    
          found=1;
        }// if matched
      }// for  
 }// else
  if(!found){
    if(cd !=0xFFFFFFFF)
      {
    Serial.println("Key unkown");
      }
  }// found
}// robojaxValidateCode end

/*
 * 
 * servoAction()
 * receives string "value" as input and based on the settings, 
 * sends translates it to servo value and controls servo
 * either 
 * rotates servo to right
 * rotates servo to left
 * moves the servo to middle
 * 
 */
void servoAction(String value)
{
  // Servo button demo with IR remote by Robojax.com
  while(value == CLOSE||value == CLOSE1||value == CLOSE2||value == CLOSE3){

    int angleStep1=180;
    if (angle > 0 && angle <= 180) {
      angle = angle - angleStep1;
       if(angle < 0){
        angle = 0;
       }else{
      RJservo.write(angle); // move the servo to desired angle
      Serial.print("Moved to: ");
      Serial.print(angle);   // print the angle
      Serial.println(" degree");
       }
    }// if
    value =".";
    delay(100);
  }// while for OPEN
  while(value == OPEN||value == OPEN1||value == OPEN2||value == OPEN3){

    // Servo button demo with IR remote by Robojax.com
    int angleStep1=180;
    if (angle >= 0 && angle <= 180) {
      angle = angle + angleStep1;
      if(angle >180){
        angle =180;
       }else{
      RJservo.write(angle); // move the servo to desired angle
      Serial.print("Moved to: ");
      Serial.print(angle);   // print the angle
      Serial.println(" degree");
       }
    }
    value =".";
    delay(100);
  }// while for CLOSE
  while(value == LOCK){

    // Servo button demo with IR remote by Robojax.com
    if (angle >= 0 && angle <= 180) {
      angle = angle + angleStep;
      if(angle >180){
        angle =180;
       }else{
      RJservo.write(angle); // move the servo to desired angle
      Serial.print("Moved to: ");
      Serial.print(angle);   // print the angle
      Serial.println(" degree");
       }
    }
    value =".";
    delay(100);
  }// while for LOCK
  
  while(value == RIGHT){

    if (angle > 0 && angle <= 180) {
      angle = angle - angleStep;
       if(angle < 0){
        angle = 0;
       }else{
      RJservo.write(angle); // move the servo to desired angle
      Serial.print("Moved to: ");
      Serial.print(angle);   // print the angle
      Serial.println(" degree");
       }
    }// if
    value =".";
    delay(100);
  }// while for RIGHT
  while(value == LEFT){

    // Servo button demo with IR remote by Robojax.com
    if (angle >= 0 && angle <= 180) {
      angle = angle + angleStep;
      if(angle >180){
        angle =180;
       }else{
      RJservo.write(angle); // move the servo to desired angle
      Serial.print("Moved to: ");
      Serial.print(angle);   // print the angle
      Serial.println(" degree");
       }
    }
    value =".";
    delay(100);
  }// while for LEFT

  if(value == CENTRE)
  {
    angle = ANGLE_CENTRE;
    RJservo.write(angle); // move the servo to centre at 90 degree angle
  }
    // Robojax IR Servo control
}//relayAction end
