#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "motor.h"
#include "sensor.h"
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 28 // 4 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SERIAL Serial
#define BTSERIAL Serial3

#define LOG_DEBUG
#ifdef LOG_DEBUG
#define M_LOG SERIAL.print
#else
#define M_LOG BTSERIAL.println
#endif

// PWM Definition
#define MAX_PWM 2000
#define MIN_PWM 300
int Motor_PWM = 1900;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int window_size = 0;

int BT_alive_cnt = 0;

unsigned long time;

Motor motor;
Sensor sensor;

void UART_Control()
{
  String tempString;
  char BT_Data = 0;
  // USB data
  /****
   * Check if USB Serial data contain brackets
   */

  if (SERIAL.available())
  {
    char inputChar = SERIAL.read();
    if (inputChar == '(')
    { // Start loop when left bracket detected
      tempString = "";
      inputChar = SERIAL.read();
      while (inputChar != ')')
      {
        tempString = tempString + inputChar;
        inputChar = SERIAL.read();
        if (!SERIAL.available())
        {
          break;
        } // Break when bracket closed
      }
    }
    int commaIndex = tempString.indexOf(','); // Split data in bracket (a, b, c)
    // Search for the next comma just after the first
    int secondCommaIndex = tempString.indexOf(',', commaIndex + 1);
    String firstValue = tempString.substring(0, commaIndex);
    String secondValue = tempString.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = tempString.substring(secondCommaIndex + 1); // To the end of the string
    // if ((firstValue.toInt() > servo_min and firstValue.toInt() < servo_max) and // Convert them to numbers
    //     (secondValue.toInt() > servo_min and secondValue.toInt() < servo_max))
    // {
    //   pan = firstValue.toInt();
    //   tilt = secondValue.toInt();
    //   window_size = thirdValue.toInt();
    // }
    SERIAL.flush();
    BTSERIAL.println(tempString);
    BTSERIAL.println("Done");
    if (tempString != "")
    {
      display.clearDisplay();
      display.setCursor(0, 0); // Start at top-left corner
      display.println("Serial_Data = ");
      display.println(tempString);
      display.display();
    }
  }

  // BT Control
  /*
    Receive data from app and translate it to motor movements
  */
  // BT Module on Serial 3 (D14 & D15)
  if (BTSERIAL.available())
  {
    BT_Data = BTSERIAL.read();
    SERIAL.print(BT_Data);
    BTSERIAL.flush();
    BT_alive_cnt = 100;
    display.clearDisplay();
    display.setCursor(0, 0); // Start at top-left corner
    display.println("BT_Data = ");
    display.println(BT_Data);
    display.display();
  }

  BT_alive_cnt = BT_alive_cnt - 1;
  if (BT_alive_cnt <= 0)
  {
    motor.STOP();
  }

  switch (BT_Data)
  {
  case 'A':
    motor.ADVANCE(Motor_PWM);
    M_LOG("Run!\r\n");
    break;
  case 'B':
    motor.ADVANCE_RIGHT(Motor_PWM);
    M_LOG("Right up!\r\n");
    break;
  case 'C':
    motor.ROTATE_CW(Motor_PWM);
    break;
  case 'D':
    motor.BACK_RIGHT(Motor_PWM);
    M_LOG("Right down!\r\n");
    break;
  case 'E':
    motor.BACK(Motor_PWM);
    M_LOG("Back!\r\n");
    break;
  case 'F':
    motor.BACK_LEFT(Motor_PWM);
    M_LOG("Left down!\r\n");
    break;
  case 'G':
    motor.ROTATE_CCW(Motor_PWM);
    break;
  case 'H':
    motor.ADVANCE_LEFT(Motor_PWM);
    M_LOG("Left up!\r\n");
    break;
  case 'Z':
    motor.STOP();
    M_LOG("Stop!\r\n");
    break;
  case 'z':
    motor.STOP();
    M_LOG("Stop!\r\n");
    break;
  case 'd':
    motor.LEFT(Motor_PWM);
    M_LOG("Left!\r\n");
    break;
  case 'b':
    motor.RIGHT(Motor_PWM);
    M_LOG("Right!\r\n");
    break;
  case 'L':
    Motor_PWM = 1500;
    break;
  case 'M':
    Motor_PWM = 500;
    break;
  }
}

// void sendVolt()
// {
//   newV = analogRead(A0);
//   if (newV != oldV)
//   {
//     if (!Serial3.available())
//     {
//       Serial3.println(newV);
//       Serial.println(newV);
//     }
//   }
//   oldV = newV;
// }

// Where the program starts
void setup()
{
  motor = Motor();
  sensor = Sensor();

  SERIAL.begin(115200); // USB serial setup
  SERIAL.println("Start");
  motor.STOP();         // Stop the robot
  BTSERIAL.begin(9600); // BT serial setup
                        // Pan=PL4=>48, Tilt=PL5=>47
  // servo_pan.attach(48);
  // servo_tilt.attach(47);
  //////////////////////////////////////////////
  // OLED Setup//////////////////////////////////
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font
  display.setCursor(0, 0);             // Start at top-left corner
  display.println("AI Robot");
  display.display();

  // Setup Voltage detector
  // pinMode(A0, INPUT);
}

void loop()
{
  // run the code in every 20ms
  if (millis() > (time + 15))
  {
    // voltCount++;
    time = millis();
    UART_Control(); // get USB and BT serial data

    sensor.read();

    Serial.print("LIGHT_L: ");
    Serial.print(sensor.lightL);
    Serial.print(" LIGHT_R: ");
    Serial.print(sensor.lightR);
    Serial.print(" DISTANCE: ");
    Serial.println(sensor.distance);

    // // constrain the servo movement
    // pan = constrain(pan, servo_min, servo_max);
    // tilt = constrain(tilt, servo_min, servo_max);

    // // send signal to servo
    // servo_pan.write(pan);
    // servo_tilt.write(tilt);
  }
  // if (voltCount >= 5)
  // {
  //   voltCount = 0;
  //   sendVolt();
  // }
}
