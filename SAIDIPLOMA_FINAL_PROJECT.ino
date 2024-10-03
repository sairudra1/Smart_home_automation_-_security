///CODE DONE BY SAIRUDRA GUDUR///
///FINAL YEAR ELECTRONIC///
#include <EEPROM.h>
#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(2, 3); // RX, TX

#define Relay1 4 // Load1 Pin Out
#define Relay2 5 // Load2 Pin Out
#define Relay3 6 // Load3 Pin Out
#define Relay4 7 // Load4 Pin Out

#define led  A1
#define buzzer1  A3
#define buzzer  A4

#define trigPin 10 //Trigger pin
#define echoPin 11 //echo pin

#define vcc 12 // ultrasound vcc
#define fire  13 //flame sensor digital pin

float sensor = A0;
float gas_value;

char bt_data; // variable to receive data from the serial port
int load1, load2, load3, load4, power, gas_g, flames_f;
int val = 0;
int lpg = 0;

float time;
float distance_cm;

void setup() {
  Serial.begin(9600);
  BT_Serial.begin(9600);

  pinMode(vcc, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(fire, INPUT);
  pinMode(sensor, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(buzzer1, OUTPUT);

  pinMode(led, OUTPUT);

  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, HIGH);

  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay2, 1);

  pinMode(Relay3, OUTPUT);
  digitalWrite(Relay3, 1);

  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay4, 1);

  load1 = EEPROM.read(1);
  load2 = EEPROM.read(2);
  load3 = EEPROM.read(3);
  load4 = EEPROM.read(4);

  power = EEPROM.read(5);
//  safe = EEPROM.read(6);
  delay(500);
}

void loop()
{
//  digitalWrite(vcc, safe);
  digitalWrite(trigPin, LOW); //PULSE ___|---|___
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  time = pulseIn(echoPin, HIGH);

  distance_cm = (time / 2) / 29.1; // distance for centimeter

  gas_value = analogRead(sensor);

  val = digitalRead(fire);

  /////////////////////////////
  if (gas_value >= 250)
  {
    digitalWrite(Relay1, 1);
    digitalWrite(Relay2, 1);
    digitalWrite(Relay3, 1);
    digitalWrite(Relay4, 1);

    power = 1;
    gas_g = 1;
    EEPROM.write(5, power);
    digitalWrite(buzzer, HIGH);
    delay(4000);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
  ///////////////////
  if (val == LOW)
  {
    digitalWrite(Relay1, 1);
    digitalWrite(Relay2, 1);
    digitalWrite(Relay3, 1);
    digitalWrite(Relay4, 1);

    power = 1;
    flames_f = 1;
    EEPROM.write(5, power);

    digitalWrite(buzzer, HIGH);
    delay(5000);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
  /////////////
  if (distance_cm <= 2)
  {
    digitalWrite(buzzer1, LOW);
  }
  else if (distance_cm <= 30)
  {
    digitalWrite(buzzer1, HIGH);
    delay(4000);
  }
  else
  {
    digitalWrite(buzzer1, LOW);
  }
  ////////////////home automation/////////////
  if (BT_Serial.available() > 0)
  {
    bt_data = BT_Serial.read();
  }

  if (bt_data == 'A')
  {
    load1 = 0;
    EEPROM.write(1, load1);
  }
  if (bt_data == 'a')
  {
    load1 = 1;
    EEPROM.write(1, load1);
  }

  if (bt_data == 'B')
  {
    load2 = 0;
    EEPROM.write(2, load2);
  }
  if (bt_data == 'b')
  {
    load2 = 1;
    EEPROM.write(2, load2);
  }

  if (bt_data == 'C')
  {
    load3 = 0;
    EEPROM.write(3, load3);
  }
  if (bt_data == 'c')
  {
    load3 = 1;
    EEPROM.write(3, load3);
  }

  if (bt_data == 'D')
  {
    load4 = 0;
    EEPROM.write(4, load4);
  }
  if (bt_data == 'd')
  {
    load4 = 1;
    EEPROM.write(4, load4);
  }

  if (bt_data == 'E')
  {
    flames_f = 0;
    gas_g = 0;
    power = 0;
    EEPROM.write(5, power);
  }
  if (bt_data == 'e')
  {
    power = 1;
    EEPROM.write(5, power);
  }
  ////////////

  

  
  //////////////////////
  bt_data = '0';

  if (power == 1)///off
  {
    load1 = 1;
    load2 = 1;
    load3 = 1;
    load4 = 1;
    digitalWrite(Relay1, 1);
    digitalWrite(Relay2, 1);
    digitalWrite(Relay3, 1);
    digitalWrite(Relay4, 1);
  }

  else //on
  {
    digitalWrite(Relay1, load1);
    digitalWrite(Relay2, load2);
    digitalWrite(Relay3, load3);
    digitalWrite(Relay4, load4);
  }

  ///////////BLUETOOTH DATA TRANSFER/////////////

  BT_Serial.print(power);//1
  BT_Serial.print(";");
  BT_Serial.print(load1);//2
  BT_Serial.print(";");
  BT_Serial.print(load2);//3
  BT_Serial.print(";");
  BT_Serial.print(load3);//4
  BT_Serial.print(";");
  BT_Serial.print(load4);//5
  BT_Serial.println(";");

  BT_Serial.print(gas_g);//6
  BT_Serial.println(";");
  BT_Serial.print(flames_f);//7
  BT_Serial.println(";");

  delay(100);
}
