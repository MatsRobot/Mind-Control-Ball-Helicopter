/**
 * Brainwave Controlled Helicopter
 * Purpose: Translates Mindwave EEG data (Attention/Meditation) into Servo movements.
 * Hardware: Arduino Micro (32U4), HC-05 Bluetooth, 2x Servos, I2C LCD.
 */

#include <Servo.h> 
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

// --- Hardware Definitions ---
const int PIN_LED_ONBOARD = 13;   // Standard Arduino Micro LED 
const int PIN_LED_EXTERNAL = 4;   // Status LED 
const int PIN_SERVO_DIAL = 9;     // Servo for UI Dial [cite: 20]
const int PIN_SERVO_THROTTLE = 8; // Servo for Helicopter Throttle [cite: 20]
const int PIN_ANALOG_ATT = 5;     // PWM out for Attention [cite: 51]
const int PIN_ANALOG_MED = 6;     // PWM out for Meditation [cite: 51]

// LCD Setup: Address 0x27, 20 columns, 4 rows [cite: 2, 3]
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo DialServo;
Servo ThrottleServo;

// --- Data Variables ---
byte payloadData[64] = {0};       // Buffer for incoming Mindwave packets [cite: 7]
byte attention = 0;               // Focus level (0-100) [cite: 31]
byte meditation = 0;              // Calmness level (0-100) [cite: 32]
byte poorQuality = 0;             // Signal strength (0 is good) [cite: 30]

// Brainwave frequencies (extracted from ASIC EEG Power) [cite: 13-16]
unsigned int delta_wave, theta_wave, low_alpha_wave, high_alpha_wave;
unsigned int low_beta_wave, high_beta_wave, low_gamma_wave, mid_gamma_wave;

/**
 * Reads a single byte from Bluetooth (Serial1)
 * Note: Arduino Micro uses Serial1 for hardware RX/TX pins.
 */
byte readOneByte() {
  while (!Serial1.available());   // Wait for data [cite: 10]
  return Serial1.read();          // Return byte from buffer [cite: 12]
}

/**
 * Helper to combine 3 bytes into a single integer for wave values
 */
int read_3byte_int(int i) {
  return ((payloadData[i] << 16) + (payloadData[i+1] << 8) + payloadData[i+2]); [cite: 17]
}

void parseWaves(int i) {
  delta_wave      = read_3byte_int(i); i += 3;
  theta_wave      = read_3byte_int(i); i += 3;
  low_alpha_wave  = read_3byte_int(i); i += 3;
  high_alpha_wave = read_3byte_int(i); i += 3;
  low_beta_wave   = read_3byte_int(i); i += 3;
  high_beta_wave  = read_3byte_int(i); i += 3;
  low_gamma_wave  = read_3byte_int(i); i += 3;
  mid_gamma_wave  = read_3byte_int(i); [cite: 15, 16]
}

void setup() {
  pinMode(PIN_LED_ONBOARD, OUTPUT);
  pinMode(PIN_LED_EXTERNAL, OUTPUT);
  
  // Startup visual feedback [cite: 18]
  digitalWrite(PIN_LED_ONBOARD, HIGH);
  digitalWrite(PIN_LED_EXTERNAL, HIGH);
  delay(100);
  digitalWrite(PIN_LED_EXTERNAL, LOW);

  // Initialize Serial Ports
  Serial1.begin(115200); // Bluetooth input [cite: 19]
  Serial.begin(57600);   // USB Debugging [cite: 19]
  
  // Servo Initialization
  DialServo.attach(PIN_SERVO_DIAL);
  ThrottleServo.attach(PIN_SERVO_THROTTLE);
  
  // Wipe servos through range as a test [cite: 24-26]
  DialServo.write(145);
  ThrottleServo.write(150);
  delay(500);

  // LCD Initialization [cite: 21, 23]
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Brainwaves");
  lcd.setCursor(0, 2);
  lcd.print("Attention Meditation");
}

void loop() {
  // Sync Bytes: Mindwave packets start with 0xAA 0xAA [cite: 28]
  if (readOneByte() == 0xAA) {
    if (readOneByte() == 0xAA) {
      
      int payloadLength = readOneByte();
      if (payloadLength > 169) return; // Error check

      // Read payload and calculate checksum [cite: 28]
      byte checksumCalc = 0;
      for (int i = 0; i < payloadLength; i++) {
        payloadData[i] = readOneByte();
        checksumCalc += payloadData[i];
      }
      
      byte checksumReceived = readOneByte();
      checksumCalc = 255 - checksumCalc; // One's complement [cite: 28]

      if (checksumReceived == checksumCalc) {
        boolean hasBrainwaveData = false;
        boolean hasBigPacket = false;

        // Parse Payload Data [cite: 30-36]
        for (int i = 0; i < payloadLength; i++) {
          switch (payloadData[i]) {
            case 0x02: // Poor Signal Quality
              poorQuality = payloadData[++i];
              hasBigPacket = true;
              break;
            case 0x04: // Attention Value
              attention = payloadData[++i];
              break;
            case 0x05: // Meditation Value
              meditation = payloadData[++i];
              break;
            case 0x83: // EEG Power (Waves)
              hasBrainwaveData = true;
              byte vlen = payloadData[++i];
              parseWaves(i + 1);
              i += vlen;
              break;
          }
        }

        // Action: Only update if we have a valid attention value [cite: 38-44]
        if (hasBrainwaveData && attention > 0 && attention < 100) {
          
          // Toggle LED based on focus threshold [cite: 42]
          digitalWrite(PIN_LED_EXTERNAL, (attention > 40) ? HIGH : LOW);

          // Update LCD [cite: 46, 47]
          lcd.setCursor(4, 3);
          lcd.print("   "); // Clear old value
          lcd.setCursor(4, 3);
          lcd.print(attention);
          
          lcd.setCursor(14, 3);
          lcd.print("   ");
          lcd.setCursor(14, 3);
          lcd.print(meditation);

          // Map attention (0-100) to Servo angles and PWM [cite: 48-51]
          int servoValDial = map(attention, 0, 100, 145, 55); 
          int servoValThrottle = map(attention, 0, 100, 150, 5);
          
          DialServo.write(servoValDial);
          ThrottleServo.write(servoValThrottle);

          // Output to analog pins for external monitoring [cite: 51]
          analogWrite(PIN_ANALOG_ATT, map(attention, 0, 100, 0, 255));
          analogWrite(PIN_ANALOG_MED, map(meditation, 0, 100, 0, 255));
        }
      }
    }
  }
}