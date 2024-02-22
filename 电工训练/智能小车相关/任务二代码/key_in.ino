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
const int keyPin11 = 11;  //the pin number of the key pin

float t1,t2,t;
float max=0.00000,min=0.00000,aver=0.00000;
int acc;
bool keyin, lastkey;

void setup()
{
  // set the digital pin as output:
  pinMode( relayPin9, OUTPUT );
  pinMode( relayPin10, OUTPUT );
  pinMode( ledPin8, OUTPUT );
  pinMode(keyPin11, INPUT);

  // start serial port at 9600 bps:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  digitalWrite(ledPin8, HIGH);
  digitalWrite(relayPin9, LOW);
  digitalWrite(relayPin10, LOW);

  Serial.println("UART ready...");
  acc = 0;
  keyin = 1;
  lastkey = keyin;
}

void loop()
{
  keyin = digitalRead(keyPin11);
  if (Serial.available() > 0) {
    // get incoming byte:
    char inChar = (char)Serial.read();
    switch (inChar)
    {
      case '0':
        acc = 0;
        t1=t2=t=0;
        aver=0;
        Serial.print("Keystrokes: ");
        Serial.print(acc);
        Serial.print("  Key input: ");
        Serial.println(keyin);
        break;
      case 'r':
        Serial.print("Keystrokes: ");
        Serial.print(acc);
        Serial.print("  Key input: ");
        Serial.println(keyin);
        break;
      case 't':
        Serial.print("max: ");
        Serial.print(max);
        Serial.print("us ");
        Serial.print("min: ");
        Serial.print(min);
        Serial.print("us ");
        Serial.print("average: ");
        Serial.print(aver/acc);
        Serial.print("us ");
      default:
        break;
    }
  }

if(keyin != lastkey)
  {  
    t1=micros();
    delay(20);
    if (keyin !=1) {
    t2=micros();
    t=t2-t1;
    if(min==0 || min>t) min=t;
    if(max==0 || max<t)  max=t;
    aver+=t;
    acc++;
    }
    lastkey = keyin;
  }

}

