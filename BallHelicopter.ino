
///////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
// Ball Helicopter
//////////////////////////////////////////////////////////////////////// 



//  pin 0 RX from Bluetooth TX
//  Pin2 SDA Display
//  Pin 3 SCL Display
//  Pin 4 LED out
//  pin 5 Attention Analogue out
//  pin 6 Mediation Analogue out
//  
//
//   pin 9 Servo Dial
//   pin 8 Servo Throttle 
//




#include <Servo.h> 
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>  // Move any other LCD libraries to another folder or delete them
                                // See Library "Docs" folder for possible commands etc.

// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address



Servo DialServo;  //Servo 1
Servo ThrottleServo;  // Servo 2


// This for Micro 32U4 chip
// Serial is USB
// Serial1 is 0,1 = Bluetooth 

int LED = 13;                    // onboard on
int LED1 = 4;                   //offboard one
int BAUDRATE = 115200;
String CurAttStr,CurMedStr;




// checksum variables
byte payloadChecksum = 0;
byte CalculatedChecksum;
byte checksum = 0;              //data type byte stores an 8-bit unsigned number, from 0 to 255
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

byte ServoValDial = 0;
byte ServoValThrottle = 0;

byte analogAtt, analogMed;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;
boolean brainwave = false;





byte ReadOneByte() {
   int ByteRead;
  // Wait until there is data
  while(!Serial1.available());
  //Get the number of bytes (characters) available for reading from the serial port.
  //This is data that's already arrived and stored in the serial receive buffer (which holds 64 bytes)
  ByteRead = Serial1.read();
  return ByteRead; // read incoming serial data
  }

unsigned int delta_wave = 0;
unsigned int theta_wave = 0;
unsigned int low_alpha_wave = 0;
unsigned int high_alpha_wave = 0;
unsigned int low_beta_wave = 0;
unsigned int high_beta_wave = 0;
unsigned int low_gamma_wave = 0;
unsigned int mid_gamma_wave = 0;

void read_waves(int i) {
  delta_wave = read_3byte_int(i);
  i+=3;
  theta_wave = read_3byte_int(i);
  i+=3;
  low_alpha_wave = read_3byte_int(i);
  i+=3;
  high_alpha_wave = read_3byte_int(i);
  i+=3;
  low_beta_wave = read_3byte_int(i);
  i+=3;
  high_beta_wave = read_3byte_int(i);
  i+=3;
  low_gamma_wave = read_3byte_int(i);
  i+=3;
  mid_gamma_wave = read_3byte_int(i);
}

int read_3byte_int(int i) {
  return ((payloadData[i] << 16) + (payloadData[i+1] << 8) + payloadData[i+2]);
}


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED, HIGH);   // hello sequence
  digitalWrite(LED1, HIGH);   // hello sequence
  delay(100);
  digitalWrite(LED1, LOW); // LED (blank) held high until 1st debug message sent
  Serial1.begin(115200);       // Bluetooth
  Serial.begin(57600);      // USB serial
  Serial.print("Communicating... ");
  Serial.println();

  DialServo.attach(9); // servo pin 
  ThrottleServo.attach(8); // Throttle servo pin 

  CurAttStr = String (attention);
  CurMedStr = String (meditation);

  
// initialise Display

// LCD

lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
lcd.backlight(); // backlight on  

// NOTE: Cursor Position: Lines and Characters start at 0  
lcd.clear();
lcd.setCursor(5,0); //Start at character 4 on line 0
lcd.print("Brainwaves");
delay(1000);
lcd.setCursor(0,2);
lcd.print("Attention Meditation");
lcd.setCursor(4,3);
lcd.print(CurAttStr);
lcd.setCursor(14,3);
lcd.print(CurMedStr);


// Servo Init
  DialServo.write(55); // Last part of the scale 55
  delay (5);
  DialServo.write(145); // First Part of the scale 145
   
  ThrottleServo.write(5); // Last part of the scale
   delay (5);
   ThrottleServo.write(150);

 
   analogWrite(5, 0);
   analogWrite(6, 0);   


}


void loop() {

  
  // Look for sync bytes
  // Byte order: 0xAA, 0xAA, payloadLength, payloadData,
  // Checksum (sum all the bytes of payload, take lowest 8 bits, then bit inverse on lowest
  if(ReadOneByte() == 0xAA) {
  if(ReadOneByte() == 0xAA) {
  payloadLength = ReadOneByte();
  if(payloadLength > 169) //Payload length can not be greater than 169
  return;
  
  payloadChecksum = 0;
      for(int i = 0; i < payloadLength; i++) {      //loop until payload length is complete
        payloadData[i] = ReadOneByte();             //Read payload
        payloadChecksum += payloadData[i];
      }
      checksum = ReadOneByte();                     //Read checksum byte from stream
      payloadChecksum = 255 - payloadChecksum;      //Take one’s compliment of generated checksum
      if(checksum == payloadChecksum) {
        poorQuality = 200;
        attention = 0;
        meditation = 0;

     }
     brainwave = false;
     for(int i = 0; i < payloadLength; i++) { // Parse the payload
          switch (payloadData[i]) {
          case 02:
            i++;
            poorQuality = payloadData[i];
            bigPacket = true;
            break;
          case 04:
            i++;
            attention = payloadData[i];
            break;
          case 05:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:                         // ASIC EEG POWER INT
            i++;
            brainwave = true;
            byte vlen = payloadData[i];
            //Serial.print(vlen, DEC);
            //Serial.println();
            read_waves(i+1);
            i += vlen; // i = i + vlen
            break;
          }                                 // switch
        }                                   // for loop

        if(bigPacket) {
          if(poorQuality == 0){
        
          }
        
          else{                             // do nothing
          
           }
         }


            if(brainwave && attention > 0 && attention < 100) {
            Serial.print("Attention value is: ");
            Serial.print(attention, DEC);
            Serial.println();
            Serial.print("Delta value is: ");
            Serial.print(delta_wave, DEC);
            Serial.println();
            Serial.print("Theta value is: ");
            Serial.print(theta_wave, DEC);
            Serial.println();
            Serial.print("Low Alpha value is: ");
            Serial.print(low_alpha_wave, DEC);
            Serial.println();
            Serial.print("High Alpha value is: ");
            Serial.print(high_alpha_wave, DEC);
            Serial.println();            
            Serial.print("Alertness value1 is: ");
            Serial.print(low_beta_wave, DEC);
            Serial.println();
            Serial.print("Alertness value2 is: ");
            Serial.print(high_beta_wave, DEC);
            Serial.println();           
            Serial.print(low_gamma_wave, DEC);
            Serial.println();
            Serial.print(mid_gamma_wave, DEC);
            Serial.println();

          
            if(attention > 40){
               digitalWrite(LED1, HIGH);
            }
            else
               digitalWrite(LED1, LOW);

            constrain(attention,0,100);
            constrain(meditation,0,100);           

            digitalWrite(LED, LOW);
            CurAttStr = String (attention);
            Serial.print("CurrAttStr = ");
            Serial.print(CurAttStr);
            Serial.println();
            CurMedStr = String (meditation);
            Serial.print("CurMedStr = ");
            Serial.print(CurMedStr);
            Serial.println();

//LCD
            lcd.setCursor(0,3);
            lcd.print("                    ");
            lcd.setCursor(4,3);
            lcd.print(CurAttStr);
            lcd.setCursor(14,3);
            lcd.print(CurMedStr);


//          Output Functions
            // may need timing loop here
            ServoValDial = map (attention, 100, 0, 55, 145); //55 Max to 145 Min
           ServoValThrottle = map (attention, 100, 0, 5, 150); //5 Max to 150 Min



            DialServo.write(ServoValDial);
            ThrottleServo.write(ServoValThrottle);

    
        

            analogAtt = map(attention, 0, 100, 0, 255);
            analogWrite(5, analogAtt);

            analogMed = map(meditation, 0, 100, 0, 255);
            analogWrite(6, analogMed);

         
           }
        } 
        }
      }


/*
 * // Controlling a servo position using a potentiometer (variable resistor) 

#include <Servo.h> 
 
Servo Servo1;  // create servo object to control a servo 1
Servo Servo2;  // create servo object to control a servo 2
 
int PotPin = 2;  // analog pin 2 used to connect the potentiometer
int PotVal;    // variable to read the value from the analog pin 

int ServoPos1 = 1;    // variable to store the servo position
int ServoPos2 = 1;    // variable to store the servo position

void setup() 
{ 
  // Set up Serial Monitor Bit rate and Print Serial Ready
  Serial.begin(9600);
  delay(100);                        
  Serial.flush();

  Servo1.attach(8);  // attaches the servo on pin 9 to the servo object 
  Servo2.attach(7);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 
  PotVal = analogRead(PotPin);            // reads the value of the potentiometer (value between 0 and 1023) 
  ServoPos1 = map(PotVal, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 

if (ServoPos2 > ServoPos1) {
  ServoPos2=ServoPos2-1;
}
else if (ServoPos2 < ServoPos1) {
  ServoPos2=ServoPos2+1;
}
else if (ServoPos2 == ServoPos1) {
}
  
  Servo1.write(ServoPos1);                  // sets the servo position according to the scaled value 
  Servo2.write(ServoPos2);                  // sets the servo position according to the scaled value 

 
Serial.print(ServoPos1);  // Write with the carrage return and line feed
Serial.print(" ");  // Write with the carrage return and line feed
Serial.println(ServoPos2);  // Write with the carrage return and line feed


  delay(30);                           // waits for the servo to get there 
}

 */

















