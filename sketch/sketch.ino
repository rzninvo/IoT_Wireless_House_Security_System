/*************************************************************
  

  This sketch shows how to read values from Virtual Pins

  App project setup:
    Slider widget (0...100) on Virtual Pin V1
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLo51sI1a1"
#define BLYNK_DEVICE_NAME           "SecuritySystem"
#define BLYNK_AUTH_TOKEN            "AKUyyWzf_eOIA5x_pcwe8W1ezA_1inmg"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial1
#define alarm_led 24
#define trigPin 45
#define echoPin 46
#define buzzer 25


#include <BlynkSimpleStream.h>
#include <Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
Servo camera;
int distance;
int last_distance = 0;
bool alarm_flag = false;

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  long dur;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  dur = pulseIn(echoPin, HIGH);
  distance = dur * 0.034 / 2;
  Blynk.virtualWrite(V1, distance);
  if (last_distance == 0)
  {
    last_distance = distance;
    Blynk.virtualWrite(V3, "No Threat Detected!");
  }
  else{
    if ((abs(last_distance - distance) >= 5) || (distance <= 10))
    {
      alarm_flag = true;
      Blynk.virtualWrite(V3, "Threat Detected!");
    }
  }
  if (alarm_flag == false)
    Blynk.virtualWrite(V3, "No Threat Detected!");
  last_distance = distance;
}

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V0
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V0 to a variable
  camera.write(pinValue);
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  if (pinValue == 1){
    alarm_flag = false;
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial2.begin(9600);
  Blynk.begin(Serial2, auth);
  timer.setInterval(1000L, myTimerEvent);
  camera.attach(12,1000,2000);
  camera.write(0);
  pinMode(alarm_led, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop()
{
  Blynk.run();
  timer.run();
  if (alarm_flag == false){
    digitalWrite(alarm_led, LOW);
    noTone(buzzer);
  }
  else
  {
    digitalWrite(alarm_led, HIGH);
    tone(buzzer, 1000);
  }
}
