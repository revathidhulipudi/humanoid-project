#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3hbUMxE56"
#define BLYNK_TEMPLATE_NAME "robot"
#define BLYNK_AUTH_TOKEN "nmmf_PAsb0vIbZUG15LmGR_izdm4CqsP"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "nmmf_PAsb0vIbZUG15LmGR_izdm4CqsP";
char ssid[] = "ABCDEFGH";
char pass[] = "abcdefgh";

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define trigPin 5
#define echoPin 18

Servo leftLeg;
Servo servo2;
Servo rightLeg;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo leftjoint;
Servo rightjoint;

long duration;
int distance;

bool walkSwitch = false;
bool stopSwitch = false;
bool handSwitch = false;

void setup()
{

Serial.begin(9600);

Blynk.begin(auth, ssid, pass);

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

// Servo Connections 
leftLeg.attach(13);
rightjoint.attach(12);
rightLeg.attach(14);
leftjoint.attach(27);
servo5.attach(26);
servo7.attach(25);

// OLED I2C 
Wire.begin(21,22);

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();

display.setTextSize(2);
display.setCursor(0,20);
display.println("Robot Start");
display.display();

delay(2000);

}

BLYNK_WRITE(V0)   // WALK SWITCH
{
walkSwitch = param.asInt();
}

BLYNK_WRITE(V1)   // STOP SWITCH
{
stopSwitch = param.asInt();

if(stopSwitch == 1)
{
stopRobot();
walkSwitch = false;
}
}

BLYNK_WRITE(V2)   // HAND SWITCH
{
handSwitch = param.asInt();

if(handSwitch == 1)
{
raiseHands();
}
}

int getDistance()
{

digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH);

distance = duration * 0.034 / 2;

Blynk.virtualWrite(V3, distance);

return distance;

}

void walkForward()
{

leftLeg.write(60);
delay(300);
leftjoint.write(100);
delay(300);
leftLeg.write(90);
delay(300);

rightLeg.write(60);
delay(300);
rightjoint.write(100);
delay(300);
rightLeg.write(90);
delay(300);

}

void stopRobot()
{

leftLeg.write(90);
rightLeg.write(90);
leftjoint.write(90);
rightjoint.write(90);

}

void raiseHands()
{

servo5.write(150);
servo7.write(150);

delay(500);

}

void loop()
{

Blynk.run();

distance = getDistance();

display.clearDisplay();

display.setTextSize(1);
display.setCursor(0,0);
display.print("Distance: ");
display.print(distance);
display.println(" cm");

if(distance <= 5)
{

stopRobot();

display.setCursor(0,30);
display.println("Obstacle Detected");
display.print("At ");
display.print(distance);
display.println(" cm");

display.display();

raiseHands();

while(true);

}

if(walkSwitch == true)
{
walkForward();
}

display.display();

}