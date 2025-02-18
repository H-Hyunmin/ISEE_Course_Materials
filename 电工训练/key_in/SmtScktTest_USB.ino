/*
  Serial Call and Response
 Language: Wiring/Arduino

 This program sends an ASCII A (byte of value 65) on startup
 and repeats that until it gets some data in.
 Then it waits for a byte in the serial port, and
 sends three sensor values whenever it gets a byte in.

 Thanks to Greg Shakar and Scott Fitzgerald for the improvements

   The circuit:
 * potentiometers attached to analog inputs 0 and 1
 * pushbutton attached to digital I/O 2

 Created 26 Sept. 2005
 by Tom Igoe
 modified 24 April 2012
 by Tom Igoe and Scott Fitzgerald

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialCallResponse

 */

// constants won't change. Used here to set a pin number :
const int relayPin9 =  9;      // the pin number of the relay pin
const int relayPin10 =  10;      // the pin number of the relay pin
const int ledPin8 = 8;      //the pin number of the led pin

unsigned int Sensor0 = 0;    // 6 analog sensors
unsigned int Sensor1 = 0;
unsigned int Sensor2 = 0;
unsigned int Sensor3 = 0;
float fSensor2;
void setup()
{
  // set the digital pin as output:
  pinMode( relayPin9, OUTPUT );
  pinMode( relayPin10, OUTPUT );
  pinMode( ledPin8, OUTPUT );

  // start serial port at 9600 bps:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //digitalWrite(ledPin10,HIGH);
  //  pinMode(2, INPUT);   // digital sensor is on digital pin 2
  //  establishContact();  // send a byte to establish contact until receiver responds

  digitalWrite(ledPin8, HIGH);
  digitalWrite(relayPin9, LOW);
  digitalWrite(relayPin10, LOW);

  Serial.println("UART ready...");
}

void loop()
{
  //    inByte = Serial.read();





  if(Sensor3>=24)  digitalWrite(relayPin10, HIGH);      //温度测控代码
 else digitalWrite(relayPin10, LOW);





  // read analog input, divide by 4 to make the range 0-255:
  Sensor0 = analogRead(A0) / 4; //i0
  ///    Serial.println(Sensor0);
  //    Sensor0 =Sensor0 * 0.042;
  Sensor0 = Sensor0 * 4.2 / 5 * 220;
  //    Sensor0 =Sensor0 * 2.6/5*220;
  Sensor1 = analogRead(A1) / 4; //i1
  ///    Serial.println(Sensor1);
  //    Sensor1 = Sensor1 * 0.026;
  Sensor1 = Sensor1 * 4.2 / 5 * 220;
  //    Sensor1 = Sensor1 * 2.6/5*220;
  Sensor2 = analogRead(A2) / 4;
  fSensor2 = Sensor2 * 1.718 / 220 * 5; //V
  Sensor3 = analogRead(A3) / 4;
  Sensor3 = Sensor3 * 0.178;

  // send sensor values:
  Serial.print("Socket #1 current: ");
  Serial.print((float)Sensor0 / 100);
  Serial.print(" mA");
  Serial.print("\t\t");
  Serial.print("Socket #2 current: ");
  Serial.print((float)Sensor1 / 100);
  Serial.println(" mA");
  Serial.print("Voltage input: ");
  Serial.print(fSensor2);
  Serial.println(" V");
  Serial.print("Temperature: ");
  Serial.print(Sensor3, DEC);
  Serial.println(" Deg Cel");
  Serial.println();
  Serial.println();
  delay(500);
if(fSensor2<4.84) digitalWrite(ledPin8,HIGH);
else digitalWrite(ledPin8,LOW);

/*

  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    char inChar = (char)Serial.read();
    Serial.println(inChar);
    switch (inChar)
    {
      case 'a':
        digitalWrite(relayPin10, LOW);
        break;
      case 'A':
        digitalWrite(relayPin10, HIGH);
        break;
      case 'b':
        digitalWrite(relayPin9, LOW);
        break;
      case 'B':
        digitalWrite(relayPin9, HIGH);
        break;
      default:
        break;
    }
  }*/
}



