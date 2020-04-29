/*
 Name:		basketball.ino
 Created:	4/29/2020 9:14:58 PM
 Author:	דודיש
*/
//We always have to include the library
#include "LedControlMS.h"
#include <Servo.h>

int pos = 0;
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 We have only a single MAX72XX.
 */
#define NBR_MTX 1 //Number of matrix connected.
#define triggerPin 5
#define echoPin 6
#define servoPin 4

LedControl lc = LedControl(12, 11, 10, NBR_MTX);
LedControl lc2 = LedControl(9, 8, 7, NBR_MTX);

Servo myservo;
/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

int HitCount = 0;
int TimerCount = 0;
int BlinkCount = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long hitMillis = 0;
int TimerInterval = 1000;
int HitCheckInterval = 50;
int BlinkInterval = 500;
int BlinkTimeout = 5;
bool GameOver = false;
bool BlinkVar = false;

const int num_of_Numbers = 31;
byte Zero_One_two[num_of_Numbers][8] =
{
  { B00000000, B01110111, B00010101, B01110101, B00010101, B01110111, B00000000, B00000000},
  { B00000000, B01110111, B00010101, B01110111, B01000001, B01110111, B00000000, B00000000},
  { B00000000, B01110111, B00010101, B01110111, B01000101, B01110111, B00000000, B00000000},
  { B00000000, B01110111, B00010001, B01110001, B01000001, B01110001, B00000000, B00000000},
  { B00000000, B01110111, B00010100, B01110111, B01000101, B01110111, B00000000, B00000000},
  { B00000000, B01110111, B00010100, B01110111, B01000001, B01110111, B00000000, B00000000},
  { B00000000, B01110101, B00010101, B01110111, B01000001, B01110001, B00000000, B00000000},
  { B00000000, B01110111, B00010001, B01110111, B01000001, B01110111, B00000000, B00000000},
  { B00000000, B01110111, B00010001, B01110111, B01000100, B01110111, B00000000, B00000000},
  { B00000000, B01110010, B00010110, B01110010, B01000010, B01110111, B00000000, B00000000},
  { B00000000, B01110111, B00010101, B01110101, B01000101, B01110111, B00000000, B00000000},
  { B00000000, B00100111, B01100101, B00100111, B00100001, B01110111, B00000000, B00000000},
  { B00000000, B00100111, B01100101, B00100111, B00100101, B01110111, B00000000, B00000000},
  { B00000000, B00100111, B01100001, B00100001, B00100001, B01110001, B00000000, B00000000},
  { B00000000, B00100111, B01100100, B00100111, B00100101, B01110111, B00000000, B00000000},
  { B00000000, B00100111, B01100100, B00100111, B00100001, B01110111, B00000000, B00000000},
  { B00000000, B00100101, B01100101, B00100111, B00100001, B01110001 ,B00000000, B00000000},
  { B00000000, B00100111, B01100001, B00100111, B00100001, B01110111, B00000000, B00000000},
  { B00000000, B00100111, B01100001, B00100111, B00100100, B01110111, B00000000, B00000000},
  { B00000000, B00100010, B01100110, B00100010, B00100010, B01110111, B00000000, B00000000},
  { B00000000, B00100111, B01100101, B00100101, B00100101, B01110111, B00000000, B00000000},
  { B00000000, B11100111, B10100101, B10100111, B10100001, B11100111, B00000000, B00000000},
  { B00000000, B11100111, B10100101, B10100111, B10100101, B11100111, B00000000, B00000000},
  { B00000000, B11100111, B10100001, B10100001, B10100001, B11100001, B00000000, B00000000},
  { B00000000, B11100111, B10100100, B10100111, B10100101, B11100111, B00000000, B00000000},
  { B00000000, B11100111, B10100100, B10100111, B10100001, B11100111, B00000000, B00000000},
  { B00000000, B11100101, B10100101, B10100111, B10100001, B11100001, B00000000, B00000000},
  { B00000000, B11100111, B10100001, B10100111, B10100001, B11100111, B00000000, B00000000},
  { B00000000, B11100111, B10100001, B10100111, B10100100, B11100111, B00000000, B00000000},
  { B00000000, B11100010, B10100110, B10100010, B10100010, B11100111, B00000000, B00000000},
  { B00000000, B11100111, B10100101, B10100101, B10100101, B11100111, B00000000, B00000000},
  //{ B01111110, B10000001, B10100101, B10000001, B10000001, B10111101, B10000001, B01111110},
  //{ B01111110, B10000001, B10100101, B10000001, B10100101, B10111101, B10000001, B01111110}

};

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    Serial.println("Setup");
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);

    for (int i = 0; i < NBR_MTX; i++)
    {
        lc.shutdown(i, false); //true - Sleep  , false -Wake_Up.
        lc2.shutdown(i, false); //true - Sleep  , false -Wake_Up.

      /* Set the brightness to a medium values 0-15 */
        lc.setIntensity(i, 8);
        lc2.setIntensity(i, 8);

        /* and clear the display */
        lc.clearDisplay(i);
        lc2.clearDisplay(i);
    }

    myservo.attach(servoPin);
}

void loop()
{

    if (millis() - previousMillis1 > TimerInterval)
    {
        // save the last time  
        previousMillis1 = millis();
        DisplayTimer();
        if (GameOver == false)
            TimerCount++;
        if (TimerCount == num_of_Numbers - 1)
        {
            GameOver = true;
        }
        //reset_hcsr04 ();
    }

    if ((millis() - previousMillis2 > HitCheckInterval) && !GameOver && (millis() - hitMillis > 1000))
    {
        // if (HitCount == 0)
        //   hitMillis = millis(); 

           // save the last time  
        previousMillis2 = millis();
        unsigned long temp = previousMillis2 - hitMillis;
        //Serial.println(temp );
        float MeasuredDistanceCm = MesureDistanceCm();
        
        if (MeasuredDistanceCm > 20 && MeasuredDistanceCm < 40)// && ((temp > 100) || (temp == 0)))
        {
            hitMillis = millis();
            HitCount++;
          // Serial.println("Hit , "); 
          //   DegelNafnef();   
          //   Serial.println(temp);
        }
        DisplayHitCount();
    }

    if ((millis() - previousMillis3 > BlinkInterval) && GameOver)
    {
        // save the last time  
        previousMillis3 = millis();
        if (BlinkVar == true)
            lc2.clearDisplay(0);
        else
            DisplayHitCount();

        BlinkVar = !BlinkVar;
        BlinkCount++;
    }

    if ((BlinkCount > 10))
    {
        BlinkCount = 0;
        HitCount = 0;
        TimerCount = 0;
        GameOver = false;
    }

}


void DegelNafnef()
{
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(1);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(1);                       // waits 15ms for the servo to reach the position
    }
}

void writeArduinoOnMatrix() {


    int i1, j2;
    for (i1 = 0; i1 < num_of_Numbers; i1++)
    {
        for (j2 = 0; j2 < 8; j2++)
        {
            lc.setRow(0, j2, Zero_One_two[i1][j2]);
        }
        //delay(1000); 
    }

    for (i1 = num_of_Numbers - 1; i1 > 0; i1--)
    {
        for (j2 = 0; j2 < 8; j2++)
        {
            lc.setRow(0, j2, Zero_One_two[i1][j2]);
        }
        //delay(1000); 
    }
}

float microsecondsToCentimeters(long microseconds) {
    // Converte o tempo de microsegundos para segundos
    float seconds = (float)microseconds / 1000000.0;
    // Com a velocidade do som de 340m/s calcula-se a
    // distancia percorrida
    float distance = seconds * 340;
    // Divide o resultado por dois pois o tempo é calculado
    // considerando a ida e a volta do sinal  
    distance = distance / 2;
    // Converte o resultado em metros para centimetros
    distance = distance * 100;

    return distance;
}

int MesureDistanceCm()
{
    bool res = false;
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    //Serial.println(duration, DEC);
    float cm = microsecondsToCentimeters(duration);
    Serial.println(cm, DEC);
    //Serial.println("after");
    //if (cm >= 18 && cm < 30)
    //{    
      //Serial.print(cm);
      //res = true;
    //}
    return cm;
}

void DisplayHitCount()
{
    for (int j = 0; j < 8; j++)
    {
        lc2.setRow(0, j, Zero_One_two[num_of_Numbers - 1 - HitCount][j]);
    }
}
void DisplayTimer()
{
    int j;

    for (j = 0; j < 8; j++)
    {
        lc.setRow(0, j, Zero_One_two[TimerCount][j]);
    }
}

#define resetPin 6 /* D3 to C9 */

void reset_hcsr04() {
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, LOW);
    delay(1);
    pinMode(resetPin, INPUT);
    digitalWrite(resetPin, LOW);
}

