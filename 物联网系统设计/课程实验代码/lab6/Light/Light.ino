
//#include <Wire.h>
//头文件
#include <string.h>
#include <ArduinoJson.h>
#include <Adafruit_BMP280.h>

// #define CLOSE_LIGHT //

//————————————————————————————————————————————————————————————————————————
#define My_JSON_PACK_1        "{\"id\":\"66666\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"Temperature\":%d.%02d,\"ColorRed\":%d,\"ColorBlue\":%d,\"ColorGreen\":%d,\"IsFlickering\":%d}}\r"



//————————————————————————————————————————————————————————————————————————
//BMP280
#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp;
//————————————————————————————————————————————————————————————————————————
//User Modified Part
//WIFI，MQTT等参数设置
#define wifi_ssid     "xm"    
#define wifi_psw      "12090217"     

//————————————————————————————————————————————————————————————————————————
// #define clientIDstr   "3color_light"
// #define timestamp     "6666"
// #define ProductKey    "k0v6olZyIU8"
// #define DeviceName    "3color_light"
// #define DeviceSecret  "ffcb212d922f0285f5cf08cc1495ed2b"
// #define password      "55203F6BA2AAEEAD5421602929B1C3F127A70323"
//————————————————————————————————————————————————————————————————————————
#define clientIDstr   "lab5device"
#define timestamp     "6666"
#define ProductKey    "k0v6olZyIU8"
#define DeviceName    "lab5device"
#define DeviceSecret  "36fb33a37215af5f81e5701446f18363"
#define password      "CBFEF6412D49F4E30853A2A40316C99E71715ADB"
//————————————————————————————————————————————————————————————————————————

//开关等宏
#define OFF           0
#define ON            1
#define MUTE          2
#define KEEP_OFF      2
#define KEEP_ON       3


//蜂鸣器宏
#define Buzzer_ON   digitalWrite(BuzzerPin,HIGH)
#define Buzzer_OFF  digitalWrite(BuzzerPin,LOW)


//AT指令宏设置
#define AT                    "AT\r"
#define AT_OK                 "OK"
#define AT_REBOOT             "AT+REBOOT\r"
#define AT_ECHO_OFF           "AT+UARTE=OFF\r"
#define AT_MSG_ON             "AT+WEVENT=ON\r"

#define AT_WIFI_START         "AT+WJAP=%s,%s\r"
#define AT_WIFI_START_SUCC    "+WEVENT:STATION_UP"

#define AT_MQTT_AUTH          "AT+MQTTAUTH=%s&%s,%s\r"
#define AT_MQTT_CID           "AT+MQTTCID=%s|securemode=3\\,signmethod=hmacsha1\\,timestamp=%s|\r"
#define AT_MQTT_SOCK          "AT+MQTTSOCK=%s.iot-as-mqtt.cn-shanghai.aliyuncs.com,1883\r"

#define AT_MQTT_AUTOSTART_OFF "AT+MQTTAUTOSTART=OFF\r"
#define AT_MQTT_ALIVE         "AT+MQTTKEEPALIVE=500\r"
#define AT_MQTT_START         "AT+MQTTSTART\r"
#define AT_MQTT_START_SUCC    "+MQTTEVENT:CONNECT,SUCCESS"
#define AT_MQTT_PUB_SET       "AT+MQTTPUB=/sys/%s/%s/thing/event/property/post,1\r"
#define AT_MQTT_PUB_ALARM_SET "AT+MQTTPUB=/sys/%s/%s/thing/event/GasAlarm/post,1\r"
#define AT_MQTT_PUB_DATA      "AT+MQTTSEND=%d\r"

#define JSON_DATA_PACK        "{\"id\":\"100\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"RoomTemp\":%d.%02d,\"AC\":%d,\"Fan\":%d,\"Buzzer\":%d,\"GasDetector\":%d}}\r"
#define JSON_DATA_PACK_2      "{\"id\":\"110\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"LightDetector\":%d,\"Curtain\":%d,\"Light\":%d,\"SoilHumi\":%d,\"Pump\":%d,\"eCO2\":%d,\"TVOC\":%d}}\r"
#define JSON_DATA_PACK_ALARM  "{\"id\":\"110\",\"version\":\"1.0\",\"method\":\"thing.event.GasAlarm.post\",\"params\":{\"GasDetector\":%d}}\r"
#define AT_MQTT_PUB_DATA_SUCC "+MQTTEVENT:PUBLISH,SUCCESS"
#define AT_MQTT_UNSUB         "AT+MQTTUNSUB=2\r"
#define AT_MQTT_SUB           "AT+MQTTSUB=2,/sys/%s/%s/thing/service/property/set,1\r"
#define AT_MQTT_SUB_SUCC      "+MQTTEVENT:2,SUBSCRIBE,SUCCESS"
#define AT_MQTT_CLOSE          "AT+MQTTCLOSE\r"

#define AT_BUZZER_MUTE           "\"Buzzer\":2"


#define DEFAULT_TIMEOUT       10   //默认超时时间
#define BUF_LEN               100  //缓冲区长度
#define BUF_LEN_DATA          190  //缓冲区数据长度

char      ATcmd[BUF_LEN];       //AT指令缓冲区
char      ATbuffer[BUF_LEN];    //AT返回数据缓冲区
char      ATdata[BUF_LEN_DATA]; //AT数据缓冲区
#define BuzzerPin             3 //蜂鸣器引脚
int   Buzzer = OFF;             //蜂鸣器状态

String data;  //串口数据
float temperature=0; //温度
double frequency=1; 
int delay_seconds;
int ColorGreen=0;
int ColorRed=0; 
int ColorBlue=0; 
int IsFlickering=0;
int IsBreathing=0;
StaticJsonDocument<300> doc; //创建一个json文档
String inString;//串口数据

void setup() {
  //Serial Initial
  Serial3.begin(115200);
  Serial.begin(115200);
  String inString="";
  pinMode(7,OUTPUT);  //RGB引脚
  pinMode(8,OUTPUT); 
  pinMode(9,OUTPUT); 
  pinMode(13,OUTPUT); //板载led
  
  //  BMP module test
  Serial.println(F("BMP280 Forced Mode Test."));
  if (!bmp.begin(0x76,0x58)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  
  //  Pin_init();
  //BEEP(1);//蜂鸣器响一声
  
  //wifi连接和阿里云连接
  while(1)
  {
    if(!WiFi_init())continue;
   // BEEP(2);
    Serial.println("WiFi init success");
    if(!Ali_connect())continue;
    break;
  }
  //BEEP(3);
  Serial.println("Ali connect success");
}


//设置了一个解析的函数，利用库ArdunioJson版本是6.2.13
double parse(String data){

  //找到“{”，并截断到倒数第二个字符
  int commaPosition;  
  commaPosition = data.indexOf('{');
  data= data.substring(commaPosition, data.length());
  Serial.println(data);
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, data);

  // 错误处理
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // 获取数据
  const char* method  = doc["method"];
  const char* id      = doc["id"];
  frequency  = doc["params"]["Frequency"];
  IsFlickering=doc["params"]["IsFlickering"];
  IsBreathing=doc["params"]["IsBreathing"];
  ColorRed=doc["params"]["ColorRed"];
  ColorGreen=doc["params"]["ColorGreen"];
  ColorBlue=doc["params"]["ColorBlue"];
  

  // 打印数据
  Serial.println("==============Start================");
  Serial.print("Frequency:");
  Serial.println(frequency);
  Serial.print("IsFlickering:");
  Serial.println(IsFlickering);
  Serial.print("IsBreathing:");
  Serial.println(IsBreathing);
  Serial.print("ColorRed:");
  Serial.println(ColorRed);
  Serial.print("ColorGreen:");
  Serial.println(ColorGreen);
  Serial.print("ColorBlue:");
  Serial.println(ColorBlue);
  return frequency;
}
  
void loop() {
    delay(10);
    while (1)
    {
      if (bmp.takeForcedMeasurement()) {
        // can now print out the new measurements
        temperature=bmp.readTemperature();
        Serial.print(F("Temperature = "));
        Serial.print(bmp.readTemperature());
        Serial.println(" *C");

        // Serial.print(F("Pressure = "));
        // Serial.print(bmp.readPressure());
        // Serial.println(" Pa");

        // Serial.print(F("Approx altitude = "));
        // Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
        // Serial.println(" m");
      } 
      else {
        Serial.println("Forced measurement failed!");
      }
      if(Update()) Serial.println("Update success");
      Serial.println("============================");
      while(Serial3.available()){ //清楚串口缓冲区
        Serial3.read();
      }
//闪烁灯
      unsigned long starmil=millis();
      unsigned long currmil;
      do{
          if (Serial3.available() > 0) {
              // 读取并处理串口数据
              handleSerialData();
            }
          if (IsFlickering){//闪烁灯实现
            delay(delay_seconds/2);
            analogWrite(9, ColorBlue);
            analogWrite(7, ColorGreen);
            analogWrite(8, ColorRed);

            delay(delay_seconds/2);
            analogWrite(9, 0);
            analogWrite(7, 0);
            analogWrite(8, 0);   
        }
        currmil=millis();
      }while (currmil-starmil<10000);
    }
    


#ifdef CLOSE_LIGHT
    //如果串口没有收到新的数据，就一直循环
    while(Serial3.available()==0) {

      if (IsBreathing){//呼吸灯实现
        for (int i = 0; i <= 20; i++)
        {
          analogWrite(9, 0+ColorBlue*i/20);
          analogWrite(7, 0+ColorGreen*i/20);
          analogWrite(8, 0+ColorRed*i/20);
          delay(delay_seconds/20);
        }
        for (int i = 20; i >= 0; i--)
        {
          analogWrite(9, 0+ColorBlue*i/20);
          analogWrite(7, 0+ColorGreen*i/20);
          analogWrite(8, 0+ColorRed*i/20);
          delay(delay_seconds/20);
        }
      }
      else if (IsFlickering){//闪烁灯实现
          delay(delay_seconds/2);
          analogWrite(9, ColorBlue);
          analogWrite(7, ColorGreen);
          analogWrite(8, ColorRed);

          delay(delay_seconds/2);
          analogWrite(9, 0);
          analogWrite(7, 0);
          analogWrite(8, 0);   
      }
      else{//常亮灯实现
        analogWrite(9, ColorBlue);
        analogWrite(7, ColorGreen);
        analogWrite(8, ColorRed);
      }
    }
#endif
  }


void handleSerialData() {
  delay(10);
  inString = Serial3.readString();  // 读取串口数据
  if (inString != "") {
    // 在这里处理接收到的串口数据
    data=inString;
    Serial.println(inString);
  }
    frequency=parse(data);   //解析数据并打印
  if (frequency==0) { frequency=1;}
  Serial.print("Frequency:");
  Serial.println(frequency);  
  delay_seconds=1.0/frequency*1000; //单位为毫秒
  Serial.print("delay_seconds(ms):");
  Serial.println(delay_seconds);
  if(IsBreathing && IsFlickering){//防止两个同时为真
      IsBreathing==0;
      Serial.println("IsBreathing and IsFlickering can't be true at the same time,set IsBreathing to false");
  }
}

bool Update()
{
  bool flag;
  int  frac;
  int zs;
  int len;
  cleanBuffer(ATcmd,BUF_LEN);
  snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_SET,ProductKey,DeviceName);
  flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
  if(!flag) {
    Serial.println("send AT_MQTT_PUB_SET false");
    return false;
  }

  cleanBuffer(ATdata,BUF_LEN_DATA);
  zs=(int) temperature;
  frac=(temperature-zs)*100;
  len = snprintf(ATdata,BUF_LEN_DATA,My_JSON_PACK_1,zs,frac,ColorRed,ColorBlue,ColorGreen,IsFlickering);

  cleanBuffer(ATcmd,BUF_LEN);
  snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_DATA,len-1);
  flag=0;
  flag = check_send_cmd(ATcmd,">",DEFAULT_TIMEOUT);
  if(flag) {
    flag = check_send_cmd(ATdata,AT_MQTT_PUB_DATA_SUCC,20);
    
  } 
  else {
    Serial.println("> accept wrong");
    return false;
  }
    return true;
//   inte1 = (int)(RoomTemp);
//   frac1 = (RoomTemp - inte1) * 100;
//    len = snprintf(ATdata,BUF_LEN_DATA,JSON_DATA_PACK,inte1,frac1,AC,Fan,Buzzer,GasDetector);
  
//   cleanBuffer(ATcmd,BUF_LEN);
//   snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_DATA,len-1);
//   flag = check_send_cmd(ATcmd,">",DEFAULT_TIMEOUT);
//   if(flag) flag = check_send_cmd(ATdata,AT_MQTT_PUB_DATA_SUCC,20);
  
  
// //  delay(500);
  
//   cleanBuffer(ATdata,BUF_LEN_DATA);
//   len = snprintf(ATdata,BUF_LEN_DATA,JSON_DATA_PACK_2,LightDetector,Curtain,Light,SoilHumi,Pump,eCO2,TVOC);

//   cleanBuffer(ATcmd,BUF_LEN);
//   snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_DATA,len-1);
//   flag = check_send_cmd(ATcmd,">",DEFAULT_TIMEOUT);
//   if(flag) flag = check_send_cmd(ATdata,AT_MQTT_PUB_DATA_SUCC,20);

//  return flag;
}



//该函数通过调用check_send_cmd函数发送AT指令并检查返回值，进行阿里云连接
bool Ali_connect()
{
  bool flag;
  bool flag1;

  cleanBuffer(ATcmd,BUF_LEN);
  snprintf(ATcmd,BUF_LEN,AT_MQTT_AUTH,DeviceName,ProductKey,password);
  flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
  if(!flag) {
    Serial.println("send AT_MQTT_AUTH false");
    return false;
  }


  cleanBuffer(ATcmd,BUF_LEN);
  snprintf(ATcmd,BUF_LEN,AT_MQTT_CID,clientIDstr,timestamp);
  flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
  if(!flag){
    Serial.println("send AT_MQTT_CID false");
    return false;
  }
  cleanBuffer(ATcmd,BUF_LEN);
  snprintf(ATcmd,BUF_LEN,AT_MQTT_SOCK,ProductKey);
  flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
  if(!flag){
    Serial.println("send AT_MQTT_SOCK false");
    return false;
  }
  flag = check_send_cmd(AT_MQTT_AUTOSTART_OFF,AT_OK,DEFAULT_TIMEOUT);
  if(!flag){
    Serial.println("send AT_MQTT_AUTOSTART_OFF false");
    return false;
  }
  flag = check_send_cmd(AT_MQTT_ALIVE,AT_OK,DEFAULT_TIMEOUT);
  if(!flag){
    Serial.println("send AT_MQTT_ALIVE false");
    return false;
  }
  flag = check_send_cmd(AT_MQTT_START,AT_MQTT_START_SUCC,50);
  if(!flag){
    Serial.println("send AT_MQTT_START false");
    return false;
  }
  cleanBuffer(ATcmd,BUF_LEN);
  snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_SET,ProductKey,DeviceName);
  flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
  if(!flag){
    Serial.println("send AT_MQTT_PUB_SET false");
    return false;
  }
  cleanBuffer(ATcmd,BUF_LEN);
  snprintf(ATcmd,BUF_LEN,AT_MQTT_SUB,ProductKey,DeviceName);
  flag = check_send_cmd(ATcmd,AT_MQTT_SUB_SUCC,DEFAULT_TIMEOUT);
  if(!flag){ BEEP(4);flag1 = check_send_cmd(AT_MQTT_CLOSE,AT_OK,DEFAULT_TIMEOUT);}
  return flag;
}


//该函数通过调用check_send_cmd函数发送AT指令并检查返回值，初始化WiFi模块
bool WiFi_init()
{
  bool flag;

  flag = check_send_cmd(AT,AT_OK,DEFAULT_TIMEOUT);
  if(!flag)return false;
  
  flag = check_send_cmd(AT_REBOOT,AT_OK,20);
  if(!flag)return false;
  delay(5000);

  flag = check_send_cmd(AT_ECHO_OFF,AT_OK,DEFAULT_TIMEOUT);
  if(!flag)return false;

  flag = check_send_cmd(AT_MSG_ON,AT_OK,DEFAULT_TIMEOUT);
  if(!flag)return false;
  
  cleanBuffer(ATcmd,BUF_LEN);//清理缓冲区，然后使用snprintf函数将WiFi启动的AT命令、WiFi的SSID和密码格式化到ATcmd缓冲区。
  snprintf(ATcmd,BUF_LEN,AT_WIFI_START,wifi_ssid,wifi_psw);//snprintf是一个C语言中的函数，用于将格式化的数据写入字符串
  flag = check_send_cmd(ATcmd,AT_WIFI_START_SUCC,20);
  return flag;
}


//发送AT指令并检查返回值
bool check_send_cmd(const char* cmd,const char* resp,unsigned int timeout)
{
  int i = 0;
  unsigned long timeStart;
  timeStart = millis();
  cleanBuffer(ATbuffer,BUF_LEN);
  Serial3.print(cmd);
  Serial3.flush();//阻塞程序，等到所有发送完后程序再继续
  while(1)
  {
    while(Serial3.available())//如果串行端口有可用数据
    {
      ATbuffer[i++] = Serial3.read();//就读取数据并存入缓冲区
      if(i >= BUF_LEN)i = 0;//如果缓冲区满了，就从头开始覆盖。
    }
    if(NULL != strstr(ATbuffer,resp))break;
    if((unsigned long)(millis() - timeStart > timeout * 1000)) break;//超时
  }
  
  if(NULL != strstr(ATbuffer,resp))return true;
  return false;
}

void cleanBuffer(char *buf,int len)
{
  for(int i = 0;i < len;i++)
  {
    buf[i] = '\0';
  } 
}

void BEEP(int b_time)
{
  for(int i = 1;i <= b_time;i++)
  { 
    digitalWrite(BuzzerPin,HIGH);
    delay(100);
    digitalWrite(BuzzerPin,LOW);
    delay(100);
  }
}
void Buzzer_mute()
{
  Buzzer_OFF;
  Buzzer = MUTE;
}
  
