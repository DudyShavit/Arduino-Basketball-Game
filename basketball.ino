//We always have to include the library
#include "LedControlMS.h"
#include <Servo.h>

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

LedControl lc=LedControl(12,11,10, NBR_MTX);
LedControl lc2=LedControl(9,8,7, NBR_MTX);

Servo myservo;

int HitCount = 0;
int tryHitCount = 0;
int TimerCount = 0;
int BlinkCount = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
unsigned long previoushitMillis = millis();
unsigned long RestartMillis = 0;
int TimerInterval = 1000;
int HitCheckInterval = 50   ;
int NafNafInterval = 500;
int BlinkInterval = 500;
int UpdateScoreInterval = 1000;
int RestartNoDetectionInterval = 1000;
int BlinkTimeout = 5;
int pos = 0;
bool GameOver = false;
bool BlinkVar = false;
bool IsHit = false;
bool IsHitTimerTimeout = false;
bool EngagaeNafnaf = false;

byte IMAGES[][8]=
                      {
   
                          {  B00000000,  B01110111,  B01000101,  B01110111,  B00010001,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B01000101,  B01110111,  B00010101,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B01000001,  B01110001,  B00010001,  B01110001,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B01000100,  B01110111,  B00010101,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B01000100,  B01110111,  B00010001,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110101,  B01000101,  B01110111,  B00010001,  B01110001,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B01000001,  B01110111,  B00010001,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B01000001,  B01110111,  B00010100,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110010,  B01000110,  B01110010,  B00010010,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B01000101,  B01110101,  B00010101,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01010111,  B01010101,  B01110111,  B00010001,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01010111,  B01010101,  B01110111,  B00010101,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01010111, B01010001,  B01110001,  B00010001,  B00010001,  B00000000,  B00000000},
                          {  B00000000,  B01010111,  B01010100,  B01110111,  B00010101,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01010111,  B01010100,  B01110111,  B00010001,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01010101, B01010101,  B01110111,  B00010001,  B00010001,  B00000000,  B00000000},
                          {  B00000000,  B01010111,  B01010001,  B01110111,  B00010001,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01010111,  B01010001,  B01110111,  B00010100,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01010010,  B01010110,  B01110010,  B00010010,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01010111,  B01010101,  B01110101,  B00010101,  B00010111,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B00010101,  B01110111,  B00010001,  B01110111,  B00000000, B00000000},
                          {  B00000000,  B01110111,  B00010101,  B01110111,  B00010101,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B00010001,  B01110001,  B00010001,  B01110001,  B00000000, B00000000},
                          {  B00000000,  B01110111,  B00010100,  B01110111,  B00010101,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110111, B00010100,  B01110111,  B00010001,  B01110111,  B00000000,  B00000000},
                          {  B00000000,  B01110101,  B00010101,  B01110111,  B00010001,  B01110001,  B00000000,  B00000000},
                          {  B00000000,  B01110111,  B00010001,  B01110111,  B00010001,  B01110111,  B00000000,  B00000000},
                          {   B00000000,  B01110111,  B00010001,  B01110111,  B00010100,  B01110111,  B00000000,  B00000000},
                          {   B00000000,  B01110010,  B00010110,  B01110010,  B00010010,  B01110111,  B00000000,  B00000000},
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
                        { B00000000, B11100111, B10100101, B10100101, B10100101, B11100111, B00000000, B00000000}
};                       
const int num_of_Numbers  = sizeof(IMAGES)/8;              


void DegelNafnef()
{
   // myservo.write(pos);
}
/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
 

  int i1, j2;
  for (i1 = 0; i1< num_of_Numbers; i1++)
  {
    for (j2 = 0; j2< 8; j2++)
    {
      lc.setRow(0,j2,IMAGES[i1][j2]);
    }
    delay(1000); 
  }

for (i1 = num_of_Numbers - 1; i1 > 0; i1--)
  {
    for (j2 = 0; j2< 8; j2++)
    {
      lc.setRow(0,j2,IMAGES[i1][j2]);
    }
    delay(1000); 
  }
}
float microsecondsToCentimeters(long microseconds){
  // Converte o tempo de microsegundos para segundos
  float seconds = (float) microseconds / 1000000.0;
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
bool Hit()
{
  bool res = false;

   digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // O sensor calcula o tempo gasto entre o envio e o recebimento
  // do sinal e retorna um pulso com esta duração
  long duration = pulseIn(echoPin, HIGH);
//Serial.println(duration, DEC);

  // Converte o tempo para distancia em centimetros
  float cm = microsecondsToCentimeters(duration);
  
  // Informa a distancia na serial
  //Serial.println(cm, DEC);
  
  if (cm < 50)
   
    
        if (cm >= 8 && cm <= 40)
        {    
          Serial.println(cm, DEC);
          //Serial.println("********");
          res = true;
        }
      
      

  return res;
}
void DisplayHitCount()
{
    for (int j = 0; j< 8; j++)
    {
      lc2.setRow(0,j,IMAGES[num_of_Numbers - 1 - HitCount][j]);
    }
}
void DisplayTimer ()
{
  int j;
 
    for (j = 0; j< 8; j++)
    {
      lc.setRow(0,j,IMAGES[TimerCount][j]);
    }
}

#define resetPin 6 /* D3 to C9 */

void reset_hcsr04 () {
pinMode(resetPin, OUTPUT);
digitalWrite(resetPin, LOW);
delay(1);
pinMode(resetPin, INPUT);
digitalWrite(resetPin, LOW);
}

void setup()
{
    RestartMillis = millis();
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
    DisplayHitCount();
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
            reset_hcsr04();

        }
    if ((millis() - RestartMillis > RestartNoDetectionInterval) && 
        (millis() - previousMillis2 > HitCheckInterval) && !GameOver)
    {
        // save the last time  
        previousMillis2 = millis();
        if (Hit())
        {
            previoushitMillis = millis();
            EngagaeNafnaf = true;
            if (IsHitTimerTimeout)
            {
                IsHitTimerTimeout = false;
                HitCount++;
                DisplayHitCount();
            }
        }
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
    if ((millis() - previoushitMillis > UpdateScoreInterval) && !GameOver)
        {
            IsHitTimerTimeout = true;
            pos = 0;
            DegelNafnef();
            EngagaeNafnaf = false;
        }
    if ((millis() - previoushitMillis < UpdateScoreInterval) &&
        (millis() - previousMillis4 > NafNafInterval) && EngagaeNafnaf &&
        !GameOver) {
         // save the last time  
         previousMillis4 = millis();
         pos = 180;
         DegelNafnef();
        }
}
