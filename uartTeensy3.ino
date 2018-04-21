#define toPi Serial2
int val = 0; //value for storing moisture value
int soilPin = A3;//Declare a variable for the soil moisture sensor
int soilPower = 5;//Variable for Soil moisture Power
int soilPower2 = 21;
int soilPin2 = A2;
String i;
int dataPi;
int pump = 15;
bool pumpPower;
int sen1 = 0;
int sen2 = 0;
String ssen1;
String ssen2;
bool newSignal = false;
bool switchSen = false;
bool autoMode = true;
int modeShow = 0;

void setup() {
  // put your setup code here, to run once:
  toPi.begin(9600);
  Serial.begin(9600);   // open serial over USB
  toPi.println("Hello from Teensy");
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  pinMode(soilPower2, OUTPUT);
  digitalWrite(soilPower2, LOW);
  pinMode(18, INPUT);
  pinMode(22, INPUT);
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  pinMode(pump, OUTPUT);
  pumpPower = true;
  digitalWrite(pump, pumpPower);
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  {
      if (toPi.available()) {
        i = toPi.readString();
        dataPi = i.toInt();
      Serial.print("from pi: ");
      Serial.println(i);
      newSignal = true;
    }
    //Serial.print("autoMode: ");
    //Serial.println(autoMode);
    //delay(200);
    //Serial.print("Soil Moisture = ");
    //get soil moisture value from the function below and print it
    //Serial.println(readSoil());
    //  if (Serial.available()) {
    //    toPi.write(Serial.read());
    //  }
    if (digitalRead(22) == 1)
    {
      autoMode = !autoMode;
      digitalWrite(23, autoMode);
      delay(100);
    }
    if (digitalRead(18) == 1)
    {
      pumpPower = !pumpPower;
      Serial.print("pin 18: ");
      Serial.println(digitalRead(18));
      delay(100);
    }
    if (newSignal == true)
    {
      switch(dataPi)
      {
        case 10: pumpPower = true;newSignal = false; break;
        case 11: 
            {
              if(sen1 < 680)
              {
                pumpPower = false; 
              }
              newSignal = false;
              break;
            }
        case 80:
        {
          autoMode = true;
          digitalWrite(23, autoMode);
          newSignal = false;
          break;
        }
        case 90:
        {
          autoMode = false;
          digitalWrite(23, autoMode);
          newSignal = false;
          break;
        }
        default: newSignal = false;
        }
    }
    //Serial.print("UART: ");
    //Serial.println(dataPi);
//    Serial.print("sen1: ");
//    Serial.println(sen1);
//    Serial.print("sen2: ");
////    Serial.println(sen2);
    if (sen1 >= 650)
    {
      pumpPower = true;
    }
    if(autoMode == true)
    {
      if (sen1 <= 300)
      {
        pumpPower = false;
      }
    }
    digitalWrite(pump, pumpPower);
    if (switchSen == true)
    {
    readSoil1();
    toPi.println(ssen1);
    delay(30);
    switchSen = false;
    }
    else
    {
    readSoil2();
    toPi.println(ssen2);
    delay(30);
    switchSen = true;
    }
  }
}

void readSoil1()
{

  digitalWrite(soilPower, HIGH);//turn D7 "On"
  delay(10);//wait 10 milliseconds
  sen1 = analogRead(soilPin);//Read the SIG value form sensor
  digitalWrite(soilPower, LOW);//turn D7 "Off"
  if (autoMode == true)
  {
    if (pumpPower == true)
      ssen1 = "1" + String(sen1);
    else
      ssen1 = "?1" + String(sen1);
  }
  else
  {
    if(pumpPower == true)
      ssen1 = "!1" + String(sen1);
    else
      ssen1 = "*1" + String(sen1);
  }
}

void readSoil2()
{
  digitalWrite(soilPower2, HIGH);//turn D7 "On"
  delay(10);//wait 10 milliseconds
  sen2 = analogRead(soilPin2);//Read the SIG value form sensor
  digitalWrite(soilPower2, LOW);//turn D7 "Off"
  if (autoMode == true)
  {
    if (pumpPower == true)
      ssen2 = "2" + String(sen2);
    else
      ssen2 = "?2" + String(sen2);
  }
  else
  {
    if(pumpPower == true)
      ssen2 = "!2" + String(sen2);
    else
      ssen2 = "*2" + String(sen2);
  }
}
