#define ssid		"SMT_SKT"
#define password	"12345678"

#define pinLEDob  13

#define pinTemp A3
#define pinVol A2
#define pinCurrent0 A0
#define pinCurrent1 A1

#define MAX_FAN_CURRENT  473    	//unit = ma
#define MAX_LED_CURRENT  473		//unit = ma

//#define __DEBUG

#ifdef __DEBUG
#define DBUG_PRINTLN(a) mySerial.println((a))
#define DBUG_PRINTLN2(a,b) mySerial.println((a),(b))
#define DBUG_PRINT(a) mySerial.print((a))
#define DBUG_PRINT2(a,b) mySerial.print((a),(b))
#define DBUG_WRITE(a) mySerial.write((a))
#else
#define DBUG_PRINTLN(a)  
#define DBUG_PRINTLN2(a,b) 
#define DBUG_PRINT(a) 
#define DBUG_PRINT2(a,b) 
#define DBUG_WRITE(a) 
#endif	

#define myDBUG_PRINTLN(a) Serial.println((a))
#define myDBUG_PRINTLN2(a,b) Serial.println((a),(b))
#define myDBUG_PRINT(a) Serial.print((a))
#define myDBUG_PRINT2(a,b) Serial.print((a),(b))
#define myDBUG_WRITE(a) Serial.write((a))

#define TICK_DIV  250
#include <SoftwareSerial.h>
//#include "cn.c"
SoftwareSerial mySerial(2, 3);

// constants won't change. Used here to set a pin number :
const int relayPin9 = 10;      // the pin number of the relay pin
const int relayPin10 = 9;      // the pin number of the relay pin

const int pinLED8 = 8;

//loop中的循环计数值最大值，用于控制各种定时操作
//const int MAX_CIRCNT = 10000;
const int MAX_CIRCNT = 4000;//2000;
//采样间隔
//const int SAMPLE_RATE = 500;
const int SAMPLE_RATE = 2;//1;
//时间检测间隔
//const int TDPROC_RATE = 5000;
const int TDPROC_RATE = 4;//2;
//heartbeat 发送间隔
const int HBEAT_RATE = 2;//6;
//最大允许的心跳计数差值
const int maxHBdiff = 30;

int showDebugInfo = 1;
int connectOk = 0;

const char OK_str[] = "OK";
const char ERROR_str[] = "ERROR";
const char DISCNT_str[] = "DISCONNECT";
const char WIFIDISCN_str[] = "WIFI Disconnected!";
const char FAIL_str[] = "FAIL";
const char AT_CIPSTATUS_str[] = "AT+CIPSTATUS";
const char AT_CIPCLOSE_str[] = "AT+CIPCLOSE";
const char AT_CIPSTART_str[] = "AT+CIPSTART=";

//状态信息结构
typedef struct _STATUS_INFO {
  unsigned long flag;
  unsigned char st_sw1, st_sw2, alarm_sw1, alarm_sw2;
  int  cur_A1, cur_A2, cur_V, curT, curP, time;
} StatusInfo;


//定延时信息结构
typedef struct _TIMER_SETTING {
  unsigned long flag;
  unsigned int sw1_do, sw1_dc, sw1_to, sw1_tc;
  unsigned int sw2_do, sw2_dc, sw2_to, sw2_tc;
} TimerSetting;

//安全设置信息结构
typedef struct _SAFTY_SETTING {
  unsigned long flag;
  unsigned int maxA1, maxA2, minV, maxV, maxP1, maxP2, minT, maxT;
} SaftySetting;

//udp数据缓冲大小
const int udprcvBuffSize = 80;
//const int udpsndBuffSize = 64;
//udp数据缓冲区
char udprcvPakageBuffer[udprcvBuffSize];
//char udpsndPakageBuffer[udpsndBuffSize];
//缓冲区指针
int bufPointR = 0;
//int bufPointS=0;

//设备状态
int voltage, current1, current2, temperature, power, power1, power2;//mV,mA,mW,1/10摄氏度
bool state1, state2;
int last_cur1=0,last_cur2=0;

//设备名称
char name[16] = "smartSocket";

//loop中的循环计数值，用于控制各种定时操作
int circleCount = 0;

//安全设置
int max_current1 = 50;//单位10mA
int max_current2 = 50;//单位10mA
int max_power1 = 350;//单位10mW
int max_power2 = 350;
int max_voltage = 550;//单位10mV
int min_voltage = 400;
int max_temperature = 500;//单位0.1度
int min_temperature = -200;

//告警来源
int warningID1 = -1;
int warningID2 = -1;

//定时开关设置
typedef struct _TimeOpenClose {
  bool enabled;
  bool triged;
  int hour;
  int minute;
} TimeOpenClose;
//延时开关设置
typedef struct _DelayOpenClose {
  bool enabled;
  long lStartTick;
  int minutes;
} DelayOpenClose;

TimeOpenClose toInfo1, tcInfo1, toInfo2, tcInfo2;
DelayOpenClose doInfo1, dcInfo1, doInfo2, dcInfo2;

unsigned long CurrTime, PrevTime, InitTime;//TICK_DIV控制粒度

long lTicks = 0;//午夜至今的半秒数
long lInitTicks = 0;//时间同步时的Ticks,半秒数
int lstMins = 0;

long delayForbitNotifyCnt = 0;

char fatal_err = 0;

unsigned long pwmc_lastTicks = 0;
unsigned long pwmc_csum1 = 0, pwmc_csum2 = 0;
int pwmc_scnt = 0;
int pwmcnt_num = 0;

bool pinLED8_st = false;
bool pinLEDob_st = false;

int heartbeatCnt = -1;
bool linked = false;
bool vr_exists = false;

int open_close = -1;

//time to open & time to close
void initTOC()
{
	toInfo1.enabled = false;
	toInfo1.hour = 4;
	toInfo1.minute = 5;
	tcInfo1.enabled = false;
	tcInfo1.hour = 6;
	tcInfo1.minute = 7;
	toInfo2.enabled = false;
	toInfo2.hour = 12;
	toInfo2.minute = 13;
	tcInfo2.enabled = false;
	tcInfo2.hour = 14;
	tcInfo2.minute = 15;
}

//delay to open & delay to close
void initDOC()
{
	doInfo1.enabled = false;
	doInfo1.lStartTick = 0;
	doInfo1.minutes = 1;

	dcInfo1.enabled = false;
	dcInfo1.lStartTick = 0;
	dcInfo1.minutes = 123;

	doInfo2.enabled = false;
	doInfo2.lStartTick = 0;
	doInfo2.minutes = 489;

	dcInfo2.enabled = false;
	dcInfo2.lStartTick = 0;
	dcInfo2.minutes = 611;
}

int getCurrent(int ch)
{
	if (ch == 0)
		return (analogRead(pinCurrent0) / 4);
	else if (ch == 1)
		return (analogRead(pinCurrent1) / 4);
	else
		return (0);
}


void getPwmCurrent_1()
{
  if ( millis() > pwmc_lastTicks+50 )
  {
    pwmc_lastTicks = millis();
    int c0 = getCurrent(0);
	delay(5);
    int c1 = getCurrent(1);
	if( (c0-last_cur1>20) || (last_cur1-c0>20) ||
		(c1-last_cur2>20) || (last_cur2-c1>20) )
	{
		DBUG_PRINT2(c0,DEC);
		DBUG_PRINT('~');
		DBUG_PRINT2(c1,DEC);
		DBUG_PRINTLN(',');
	}
	last_cur1 = c0;
	last_cur2 = c1;
      // DBUG_PRINT('.');
/*    if ( pwmc_csum1 + c0 < pwmc_csum1 || pwmc_csum1 + c0 >= 0xffffffff / MAX_LED_CURRENT )
    {
      DBUG_PRINT('x');
      DBUG_PRINT('1');
      DBUG_PRINTLN('x');
    }
    if ( pwmc_csum2 + c1 < pwmc_csum2 || pwmc_csum2 + c1 >= 0xffffffff / MAX_FAN_CURRENT )
    {
      DBUG_PRINT('x');
      DBUG_PRINT('2');
      DBUG_PRINTLN('x');
    }*/
    pwmc_csum1 += c0;
    pwmc_csum2 += c1;
    pwmc_scnt++;
    if ( pwmc_scnt > 8 )
    {
      // DBUG_PRINT(pwmc_csum1,DEC);
      // DBUG_PRINT('.');
      // DBUG_PRINTLN(pwmc_csum1/pwmc_scnt,DEC);
      // DBUG_PRINT(pwmc_csum2,DEC);
      // DBUG_PRINT('_');
      // DBUG_PRINTLN(pwmc_csum2/pwmc_scnt,DEC);
      current1 = pwmc_csum1 * MAX_LED_CURRENT / 256 / pwmc_scnt;
      current2 = pwmc_csum2 * MAX_FAN_CURRENT / 256 / pwmc_scnt;
      pwmc_scnt = 0;
      pwmc_csum1 = 0;
      pwmc_csum2 = 0;
	  pwmcnt_num++;
      //DBUG_PRINTLN2(current1,DEC);
    }
  }
}



unsigned char ChkSum(unsigned char* buf,int size){
	int i;
	long sum = 0;
	for(int i=0;i<size;i++)
		sum += buf[i];
	return (unsigned char)(sum%256);
}

void showDebugMessage(String data){
	int i,len;
	unsigned long start;
	char c;
	if( (data.length()>0) && (showDebugInfo>0) ){
		myDBUG_PRINT('[');
		myDBUG_PRINT2(data.length(),DEC);
		myDBUG_PRINT(':');
		for(i=0;i<data.length();i++){
			myDBUG_WRITE((char)data[i]);
		}
		myDBUG_PRINTLN(']');
		start = millis();
		i = 0;
		len = data.length()+3+((data.length()<10)?1:((data.length()<100)?2:3));
		while((i<len)?millis()-start<25*len:millis()-start<50) {
			if(Serial.available()>0 ){
				c = Serial.read();
				if(i>=len)
					start = millis();
				else
					i++;
			}
			getPwmCurrent_1();
		}
	}
}

bool mySerialFind(char* dest,int to_ms){
	unsigned long start = millis();
	char c;
	int dest_len = strlen(dest);
	int dest_ptr=0;
	while(millis()-start<to_ms){
		if(mySerial.available()>0 ){
			c = mySerial.read();
			if( c==dest[dest_ptr] ){
				dest_ptr++;
				if( dest_ptr>=dest_len )	return true;
			}
			else{
				dest_ptr = 0;
			}
		}
		getPwmCurrent_1();
	}
	return false;
}


bool SerialFind(char* dest,int to_ms){
	unsigned long start = millis();
	String data;
	char c;
	int i;
	int dest_len = strlen(dest);
	int dest_ptr=0;
	while(millis()-start<to_ms){
		if(Serial.available()>0 ){
			c = Serial.read();
			if( c!=0x0d && c!=0x0a )
				data = data+c;
			else
				data = data+'|';
			if( c==dest[dest_ptr] ){
				dest_ptr++;
				if( dest_ptr>=dest_len )	return true;
			}
			else{
				dest_ptr = 0;
			}
		}
		getPwmCurrent_1();
	}
	showDebugMessage(data);
	return false;
}

int mySerialFind2(char* destok1,char* destok2,char* desterr1,char* desterr2,char* Info,int to_ms){
	unsigned long start = millis();
	char c;
	int destok1_len = strlen(destok1);
	int destok1_ptr=0;
	int destok2_len = strlen(destok2);
	int destok2_ptr=0;
	int desterr1_len = strlen(desterr1);
	int desterr1_ptr=0;
	int desterr2_len = strlen(desterr2);
	int desterr2_ptr=0;
	int ret = 0;
	bool ok1 = false;
	bool ok2 = false;
	bool err1 = false;
	bool err2 = false;
	while(millis()-start<to_ms){
		if(mySerial.available()>0 ){
			c = mySerial.read();
			if( c==destok1[destok1_ptr] ){
				destok1_ptr++;
				if( destok1_ptr>=destok1_len )	ok1 = true;
			}
			else{
				destok1_ptr = 0;
			}
			if( c==destok2[destok2_ptr] ){
				destok2_ptr++;
				if( destok2_ptr>=destok2_len )	ok2 = true;
			}
			else{
				destok2_ptr = 0;
			}
			if( c==desterr1[desterr1_ptr] ){
				desterr1_ptr++;
				if( desterr1_ptr>=desterr1_len )	err1 = true;
			}
			else{
				desterr1_ptr = 0;
			}
			if( c==desterr2[desterr2_ptr] ){
				desterr2_ptr++;
				if( desterr2_ptr>=desterr2_len )	err2 = true;
			}
			else{
				desterr2_ptr = 0;
			}
		}
		getPwmCurrent_1();
		if( ok1 || ok2 ){
			if( Info!=NULL ){
				DBUG_PRINT(OK_str);
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			return 1;
		}
		if( err1 || err2 )	{
			if( Info!=NULL ){
				DBUG_PRINT("Fail");
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			ret = -1;
			break;
		}
	}
	return ret;
}

int SerialFind2(char* destok1,char* destok2,char* desterr1,char* desterr2,char* Info,int to_ms){
	unsigned long start = millis();
	String data;
	char c;
	int i;
	int destok1_len = strlen(destok1);
	int destok1_ptr=0;
	int destok2_len = strlen(destok2);
	int destok2_ptr=0;
	int desterr1_len = strlen(desterr1);
	int desterr1_ptr=0;
	int desterr2_len = strlen(desterr2);
	int desterr2_ptr=0;
	int ret = 0;
	bool ok1 = false;
	bool ok2 = false;
	bool err1 = false;
	bool err2 = false;
	//LEDNotify(7);
	while(millis()-start<to_ms){
		if(Serial.available()>0 ){
			c = Serial.read();
			if( c!=0x0d && c!=0x0a )
				data = data+c;
			else
				data = data+'|';
			if( c==destok1[destok1_ptr] ){
				destok1_ptr++;
				if( destok1_ptr>=destok1_len ){
					ok1 = true;
				}
			}
			else{
				destok1_ptr = 0;
			}
			if( c==destok2[destok2_ptr] ){
				destok2_ptr++;
				if( destok2_ptr>=destok2_len ){
					ok2 = true;
				}
			}
			else{
				destok2_ptr = 0;
			}
			if( c==desterr1[desterr1_ptr] ){
				desterr1_ptr++;
				if( desterr1_ptr>=desterr1_len ){
					err1 = true;
				}
			}
			else{
				desterr1_ptr = 0;
			}
			if( c==desterr2[desterr2_ptr] ){
				desterr2_ptr++;
				if( desterr2_ptr>=desterr2_len ){
					err2 = true;
				}
			}
			else{
				desterr2_ptr = 0;
			}
		}
		getPwmCurrent_1();
		if( (ok1!=false) || (ok2!=false) ){
			if( Info!=NULL ){
				DBUG_PRINT(OK_str);
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			//LEDNotify(0);
			return (ok2==false)?1:((ok1==false)?2:3);
		}
		if( (err1!=false) || (err2!=false) )	{
			if( Info!=NULL ){
				DBUG_PRINT(FAIL_str);
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			ret = -1;
			break;
		}
	}
	showDebugMessage(data);
	//LEDNotify(0);
	delay(500);
	return ret;
}


//获取udp数据包
int getUdpDatagram_content(char* buf, int size)
{
	char strTmp[16];
	int nLen = 0;
	char cTmp;
	int i;
	Serial.setTimeout(300);
	nLen = Serial.parseInt();
	if (nLen > size)
	{
		DBUG_PRINTLN("!get UDP1 x!");
		fatal_err = 1;
		return -1;//error!!!
	}
	cTmp = Serial.readBytesUntil(':', strTmp, 15);
	Serial.setTimeout(300);

	Serial.readBytes(buf, nLen);      //get data
	Serial.readBytesUntil('\x1', strTmp, 15);
	return nLen;
}

//**************************************************************
//通过udp发送缓冲区数据,返回发送成功与否
bool putUdpDatagram(char* buf, int size)
{
	int nTmp;
	char strTmp[16];
	int cnt = 0;
	int i;

	i=0;
	while(i++<3){
		DBUG_PRINT("S");
		DBUG_PRINT2(size,DEC);

		Serial.print("AT+CIPSEND=");
		Serial.println(size+1, DEC);
		flipLED13_2();
		cnt++;
		if ( SerialFind2((char*)OK_str,(char*)OK_str,(char*)ERROR_str,"usy",NULL,200)>=1 )
		{
			if( SerialFind2(">",">",(char*)ERROR_str,"ERR",NULL,200)>=1 )
			{
				Serial.write(buf, size);
				Serial.write(ChkSum((unsigned char*)buf,size));
				if( SerialFind2("SEND OK","SEND OK",(char*)ERROR_str,"ERR",NULL,500)>=1 )
				{
					DBUG_PRINT("S");
					return true;
				}
			}
		}
	}
	DBUG_PRINT("!");
	return false;
}
//****************************************************************

//以下函数需要实现
int getVoltage()
{
	return (analogRead(pinVol) / 4);
}


int getTemperature()
{
	return (analogRead(pinTemp) / 4);
}


//获取udp数据到缓冲区中，udp数据包来源填写到hi中，返回得到的数据长度，没有返回0，失败返回-1
int getUdpDatagram(char* buf, int size)
{
	int nLen = 0,i;
	char cTmp;
	do {
		if( lTicks<4 ){
			DBUG_PRINT("Init");
			if( SerialFind((char*)ERROR_str,2000) ){
				DBUG_PRINTLN("!get UDP x!");	
				fatal_err = 1;
			}
			break;
		}
		if ( Serial.available() < 13 )	//wait +IPD,x,x[x]:xxxx\r\nOK
			break;
		if( !SerialFind("+IPD,",80) )
			break;
		DBUG_PRINT("R");
		//获取udp数据包并将链路号设置到hi->id
		nLen = getUdpDatagram_content(buf, size);
		DBUG_PRINT2(nLen,DEC);
		if( nLen>0 ){
			SerialFind("OK\r\n",50); //read to skip "OK\r\n"

			DBUG_PRINT("R");
			break;
		}
		DBUG_PRINT("!");
	} while (0);
	return nLen;
}

int strToInt(char *str)
{
	int ret = 0;
	int sig = 1;
	if ( str[0] == '-' )
	{
		sig = -1;
		str++;
	}
	while (*str != 0)
	{
		ret *= 10;
		if ( *str < '0' || *str > '9' )  return 0; //invalid
		ret += *str - '0';
		str++;
	}
	return ret * sig;
}

int intToStr(char* str, int size, int value)
{
	char *str1 = str;
	int size1 = size;
	char cTemp;
	if ( value < 0 )
	{
		*str = '-';
		str++;
		str1 = str;
		size--;
		size1--;
		value = -value;
	}
	if (value == 0 && size1 > 0)
	{
		*str1 = '0';
		*(str1 + 1) = 0;
		size1--;
		return size - size1;
	}
	while (value > 0 && size1 > 0)
	{
		*str1 = (value % 10) + '0';
		str1++;
		size1--;
		value /= 10;
	}
	if ( size1 == 0 )  str1--;
	*str1 = 0;
	str1--;
	//翻转
	while (str1 > str)
	{
		cTemp = *str;
		*str = *str1;
		*str1 = cTemp;
		str++;
		str1--;
	}
	return size - size1;
}

void speakTime()
{
	if( !vr_exists ) return;
	int ubVal = (lTicks/120)%60;
	String str = String(lTicks/7200)+"."+String(ubVal);
	ttsSpeak(str.c_str(),2);
}

//处理udp数据缓冲
void udpDatagramProcess()
{
	char pkgBuf[48];
	_STATUS_INFO* si;
	_SAFTY_SETTING* ss;
	_TIMER_SETTING* ts;
	unsigned long flag = 0, ltime,ldate;
	int dst = 0;
	int value = 0;
	int hb_tmp = 0;
	if ( bufPointR < 4 )  return;
	DBUG_PRINT("P");
	DBUG_PRINT2(bufPointR,DEC);
	//DBUG_PRINTLN(udprcvPakageBuffer);
	flag = ((unsigned long*)udprcvPakageBuffer)[0];
	flag = (flag % 256) * 256 * 65536 + (flag % 65536) / 256 * 65536 + ((flag / 65536) % 256) * 256 + flag / (256 * 65536);
	//DBUG_PRINT2(flag,HEX);
	switch (flag)
	{
		case 0xFF0000FF://link
		case 0xFF0000EE://force link
			//if ( !linked )	{
			//	ttsSpeak("sound117",0);
			//	delay(300);
			//}
			break;
		default:
			if ( !linked )	{
				bufPointR = 0;
				DBUG_PRINTLN("!");
				if( heartbeatCnt>0 )
					heartbeatCnt--;
				ttsSpeak("sound117",0);
				delay(300);
				return;
			}
	}
  
	hb_tmp = heartbeatCnt;
	heartbeatCnt = 0;
	switch (flag) {
		case 0xFF0000EE://force link
		case 0xFF0000FF://link
			// if ( !linked )
			// {
				DBUG_PRINT('+');
				DBUG_PRINT('1');
				flag = 0xff0000ff;
				putUdpDatagram((char*)&flag, 4);
				linked = true;
				if( vr_exists ){
					//mySerial.println("{ZZ}");
					mySerial.println("{Z1}");
					delay(500);
				}
			// }
			// else
			// {
				// flag = 0xff0000aa;
				// putUdpDatagram((char*)&flag, 4);
				// heartbeatCnt = hb_tmp;
				// if( vr_exists ){
				//	mySerial.println("{ZZ}");
					// mySerial.println("{Z2}");
					// delay(500);
				// }
			// }
			break;
		case 0xFF000001://sync-1
			si = (_STATUS_INFO*)pkgBuf;
			si->flag = 0xff000001;
			si->st_sw1 = state1 ? 0xff : 0;
			si->st_sw2 = state2 ? 0xff : 0;
			si->alarm_sw1 = warningID1;
			si->alarm_sw2 = warningID2;
			si->cur_A1 = current1;
			si->cur_A2 = current2;
			si->cur_V = voltage;
			si->curT = temperature+1000;
			si->curP = power;
			strncpy(pkgBuf + sizeof(struct _STATUS_INFO), name, 16);
			putUdpDatagram(pkgBuf, strlen(name) + sizeof(struct _STATUS_INFO));
			if (  bufPointR >= 12 ) {
				ldate = ((unsigned long*)udprcvPakageBuffer)[1];
				ltime = ((unsigned long*)udprcvPakageBuffer)[2];
				lTicks = 2*((ltime & 0xff) + (((ltime >> 8) & 0xff) * 60) + (((ltime >> 16) & 0xff) * 3600));
				lInitTicks = lTicks;
				InitTime = millis()/TICK_DIV;			
				speakTime();
			}	  
			if( vr_exists ){
				mySerial.println("{Z3}");
				delay(500);
			}
			break;
		case 0xFF000002://sync-2
			ss = (_SAFTY_SETTING*)pkgBuf;
			ss->flag = 0xff000002;
			ss->maxA1 = max_current1;
			ss->maxA2 = max_current2;
			ss->maxV = max_voltage;
			ss->minV = min_voltage;
			ss->maxP1 = max_power1;
			ss->maxP2 = max_power2;
			ss->maxT = max_temperature+1000;
			ss->minT = min_temperature+1000;
			putUdpDatagram(pkgBuf, sizeof(struct _SAFTY_SETTING));
			if( vr_exists ){
				mySerial.println("{Z5}");
				delay(500);
			}
			break;
		case 0xFF000003://sync-3
			ts = (_TIMER_SETTING*)pkgBuf;
			ts->flag = 0xff000003;
			if( doInfo1.minutes>(lTicks - doInfo1.lStartTick)/120 )
				doInfo1.minutes -= (lTicks - doInfo1.lStartTick)/120;
			if( doInfo2.minutes>(lTicks - doInfo2.lStartTick)/120 )
				doInfo2.minutes -= (lTicks - doInfo2.lStartTick)/120;
			if( dcInfo1.minutes>(lTicks - dcInfo1.lStartTick)/120 )
				dcInfo1.minutes -= (lTicks - dcInfo1.lStartTick)/120;
			if( dcInfo2.minutes>(lTicks - dcInfo2.lStartTick)/120 )
				dcInfo2.minutes -= (lTicks - dcInfo2.lStartTick)/120;
			ts->sw1_do = (doInfo1.minutes % 60) + doInfo1.minutes / 60 * 256 + (doInfo1.enabled ? 0x8000 : 0);
			ts->sw1_dc = (dcInfo1.minutes % 60) + dcInfo1.minutes / 60 * 256 + (dcInfo1.enabled ? 0x8000 : 0);
			ts->sw2_do = (doInfo2.minutes % 60) + doInfo2.minutes / 60 * 256 + (doInfo2.enabled ? 0x8000 : 0);
			ts->sw2_dc = (dcInfo2.minutes % 60) + dcInfo2.minutes / 60 * 256 + (dcInfo2.enabled ? 0x8000 : 0);
			ts->sw1_to = toInfo1.minute + toInfo1.hour * 256 + (toInfo1.enabled ? 0x8000 : 0);
			ts->sw1_tc = tcInfo1.minute + tcInfo1.hour * 256 + (tcInfo1.enabled ? 0x8000 : 0);
			ts->sw2_to = toInfo2.minute + toInfo2.hour * 256 + (toInfo2.enabled ? 0x8000 : 0);
			ts->sw2_tc = tcInfo2.minute + tcInfo2.hour * 256 + (tcInfo2.enabled ? 0x8000 : 0);
			putUdpDatagram(pkgBuf, sizeof(struct _TIMER_SETTING));
			if( vr_exists ){
				mySerial.println("{Z6}");
				delay(500);
			}
			break;
		case 0xFF000004://query
			if( bufPointR>4 )
			{
				state1 = (udprcvPakageBuffer[4]&0x0f)!=0;
				state2 = (udprcvPakageBuffer[4]&0xf0)!=0;
			}
			si = (_STATUS_INFO*)pkgBuf;
			si->flag = 0xff000004;
			si->st_sw1 = state1 ? 0xff : 0;
			si->st_sw2 = state2 ? 0xff : 0;
			si->alarm_sw1 = warningID1;
			si->alarm_sw2 = warningID2;
			si->cur_A1 = current1;
			si->cur_A2 = current2;
			si->cur_V = voltage;
			si->curT = temperature+1000;
			si->curP = power;
			si->time = lTicks/6;
			putUdpDatagram(pkgBuf, sizeof(struct _STATUS_INFO));
			break;
		case 0xFF000005://set name
			if ( bufPointR > 4 ) {
				memset(name, 0, 16);
				strncpy(name, udprcvPakageBuffer + 4, bufPointR - 4);
				flag = 0xff000005;
				putUdpDatagram((char*)&flag, 4);
				DBUG_PRINT('=');
				DBUG_PRINT(name);
				DBUG_PRINTLN('=');
			}
			break;
		case 0xFF000006://set safty setting
			if ( bufPointR >= sizeof(struct _SAFTY_SETTING) )
			{
				ss = (struct _SAFTY_SETTING*)udprcvPakageBuffer;
				max_current1 = ss->maxA1;
				max_current2 = ss->maxA2;
				max_voltage = ss->maxV;
				min_voltage = ss->minV;
				max_temperature = ss->maxT-1000;
				min_temperature = ss->minT-1000;
				max_power1 = ss->maxP1;
				max_power2 = ss->maxP2;
				flag = 0xff000006;
				putUdpDatagram((char*)&flag, 4);
				if( vr_exists ){
					mySerial.println("{Z5}");
					delay(500);
				}
			}
			break;
		case 0xFF000007://set timer setting
			if ( bufPointR >= sizeof(struct _TIMER_SETTING) )
			{
				ts = (struct _TIMER_SETTING*)udprcvPakageBuffer;
				doInfo1.enabled = ts->sw1_do > 0x7fff;
				doInfo1.lStartTick = lTicks;
				doInfo1.minutes = (ts->sw1_do & 0x7f00) / 256 * 60 + (ts->sw1_do % 256) % 60;
				doInfo2.enabled = ts->sw2_do > 0x7fff;
				doInfo2.lStartTick = lTicks;
				doInfo2.minutes = (ts->sw2_do & 0x7f00) / 256 * 60 + (ts->sw2_do % 256) % 60;
				dcInfo1.enabled = ts->sw1_dc > 0x7fff;
				dcInfo1.lStartTick = lTicks;
				dcInfo1.minutes = (ts->sw1_dc & 0x7f00) / 256 * 60 + (ts->sw1_dc % 256) % 60;
				dcInfo2.enabled = ts->sw2_dc > 0x7fff;
				dcInfo2.lStartTick = lTicks;
				dcInfo2.minutes = (ts->sw2_dc & 0x7f00) / 256 * 60 + (ts->sw2_dc % 256) % 60;

				toInfo1.enabled = ts->sw1_to > 0x7fff;
				toInfo1.minute = (ts->sw1_to % 256) % 60;
				toInfo1.hour = (ts->sw1_to & 0x7f00) / 256;
				toInfo2.enabled = ts->sw2_to > 0x7fff;
				toInfo2.minute = (ts->sw2_to % 256) % 60;
				toInfo2.hour = (ts->sw2_to & 0x7f00) / 256;
				tcInfo1.enabled = ts->sw1_tc > 0x7fff;
				tcInfo1.minute = (ts->sw1_tc % 256) % 60;
				tcInfo1.hour = (ts->sw1_tc & 0x7f00) / 256;
				tcInfo2.enabled = ts->sw2_tc > 0x7fff;
				tcInfo2.minute = (ts->sw2_tc % 256) % 60;
				tcInfo2.hour = (ts->sw2_tc & 0x7f00) / 256;
				flag = 0xff000007;
				putUdpDatagram((char*)&flag, 4);
				if( vr_exists ){
					mySerial.println("{Z6}");
					delay(500);
				}
			}
			break;
		case 0xFF000010://open sw1
			state1 = true;
			flag = 0xff000010;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000011://close sw1
			state1 = false;
			flag = 0xff000011;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000012://open sw2
			state2 = true;
			flag = 0xff000012;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000013://close sw2
			state2 = false;
			flag = 0xff000013;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000018://clear alarm1
			warningID1 = -1;
			flag = 0xff000018;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000019://clear alarm2
			warningID2 = -1;
			flag = 0xff000019;
			putUdpDatagram((char*)&flag, 4);
			break;
		default:
			heartbeatCnt = hb_tmp;
			break;
	}
	bufPointR = 0;
	DBUG_PRINT("P");
}

void flipLED13()
{
  if ( pinLED8_st )
    digitalWrite(pinLED8, LOW);
  else
    digitalWrite(pinLED8, HIGH);
  pinLED8_st = !pinLED8_st;
}

void flipLED13_2()
{
    digitalWrite(pinLED8, LOW);
	pinLED8_st = 0;
	delay(50);
	flipLED13();//亮
	delay(100);
	flipLED13();//灭
    digitalWrite(pinLED8, LOW);
}

void flipLED13_3()
{
    digitalWrite(pinLED8, LOW);
	pinLED8_st = 0;
	delay(50);
	flipLED13();//亮
	delay(100);
	flipLED13();//灭
	delay(50);
	flipLED13();//亮
	delay(100);
	flipLED13();//灭
    digitalWrite(pinLED8, LOW);
}


void flipLEDob()
{
  if ( pinLEDob_st )
    digitalWrite(pinLEDob, LOW);
  else
    digitalWrite(pinLEDob, HIGH);
  pinLEDob_st = !pinLEDob_st;
}

void LEDNotify(unsigned char flag){
	digitalWrite(relayPin9, (flag&0x02)>0?HIGH:LOW);
	delay(20);
	digitalWrite(relayPin10, (flag&0x04)>0?HIGH:LOW);
}

bool removeHostChain()
{
	int i;
	int result;
	long start = millis();
	DBUG_PRINT("<rHC:");
	while(millis()-start<3000){
		Serial.println(AT_CIPCLOSE_str);
		result = SerialFind2((char*)OK_str,"UNLINK","ER@!","ER@!",NULL,900);
		if ( result==1 )
		{
			DBUG_PRINT(OK_str);
			DBUG_PRINTLN(" /rHC>");
			return true;
		}
	}
	DBUG_PRINT("!rHC x!");
	DBUG_PRINTLN(" /rHC>");
	//fatal_err = 1;  
	return false;
}

void createHostChain()
{
	do {
		delay(50);
		Serial.flush();
		Serial.print(AT_CIPSTART_str); //open udp #0
		Serial.print('"');
		Serial.print("UDP");
		Serial.print('"');
		Serial.print(",");
		Serial.print('"');
		Serial.print("255.255.255.255");
		Serial.print('"');
		Serial.println(",7761,7762,2");

		if( SerialFind2((char*)OK_str,(char*)OK_str,(char*)ERROR_str,"busy","UDP 0 listen!",3000)>=1 )
			break;
		removeHostChain();
	} while (1);
}

void Init8266()
{
	long cnt = 0;
	boolean result;
	int nret = 0;
	String data;
	bool ipOk = false;
	//const char* okmsg = "sound121";
	//const char* errmsg = "sound122";
	//const char* toutmsg = "sound111";
	//const char* initWifi = "\xb3\xf5\xca\xbc\xbb\xaf[n1]8266";
	//const char* connWifi = "\xc1\xac\xbd\xd3\xc8\xc8\xb5\xe3";
	//bool rstReady = false;
	//bool skip9600 = false;
	LEDNotify(0);
	Serial.flush();
	Serial.print("+++");  
	delay(50);
	Serial.print("+++");  
	delay(50);
	Serial.print("+++");  
	delay(50);
	
	connectOk = 0;
	showDebugInfo = 1;
	if( vr_exists ){
		mySerial.println("{Y3}");
		delay(1500);
	}
	//ttsSpeak(initWifi,3);//初始化无线模块	
	do {//set into station mode
		delay(50);
		//Serial.flush();
		Serial.println("AT+CWMODE=1");  
		if( SerialFind2((char*)OK_str,"no change",(char*)ERROR_str,"busy","station mode!",2000)>=1 )
			break;
		flipLED13();
		
		//ttsSpeak(errmsg,1);
		if( vr_exists ){
			mySerial.println("{Y5}");	
			delay(200);
		}
	}while(1);

	while(1){
		//Serial.flush();
		DBUG_PRINTLN("try RST..");
		Serial.println("AT+RST");
		if( SerialFind((char*)OK_str,2000) ){
			//rstReady = true;
			DBUG_PRINT("8266 ready on ");
			DBUG_PRINTLN("115200...");
			break;
		}
		flipLED13();
	}
	
	LEDNotify(2);
	//print 8266 startup info
	DBUG_PRINTLN('.');
	DBUG_PRINTLN("..8266_info..");

	cnt = 0;
	while (Serial.available() <= 128 && cnt < 3000)
	{
		delay(1);
		while (Serial.available() > 0)
		{
			char c = (char)Serial.read();
			DBUG_WRITE(c);
			flipLED13();
		}
		cnt++;
	}
  	DBUG_PRINTLN('.');

	LEDNotify(4);

	do {//set into station mode
		delay(50);
		//Serial.flush();
		Serial.println("AT+CWMODE=1");  
		if( SerialFind2((char*)OK_str,"no change",(char*)ERROR_str,"busy","station mode!",2000)>=1 )
			break;
		flipLED13();
	}while(1);

	//Serial.flush();
	do//attach to the wifi
	{
		delay(500);
		//ttsSpeak(connWifi,3);//开始连接热点
		if( vr_exists ){
			//mySerial.println("{YA}");
			//delay(800);
			mySerial.println("{Y8}");
			delay(1500);
			//mySerial.println("{o9|0}");
		}
		DBUG_PRINT("connect to wifi:");
		DBUG_PRINT(ssid);
		DBUG_PRINT(" - ");
		DBUG_PRINTLN(password);
		delay(50);
		Serial.print("AT+CWJAP=");  //attach to the wifi
		Serial.print("\"");     //"ssid"
		Serial.print(ssid);
		Serial.print("\"");

		Serial.print(",");

		Serial.print("\"");      //"pwd"
		Serial.print(password);
		Serial.println("\"");

		char strMyIP[16];
		int nLen;

		cnt = 0;
		while(cnt<3){
			nret = SerialFind2("WIFI CONNECT",(char*)OK_str,(char*)DISCNT_str,(char*)FAIL_str,NULL,6600);
			if( nret>0 ){
				if( SerialFind2("WIFI GOT IP",(char*)OK_str,(char*)DISCNT_str,(char*)FAIL_str,NULL,3000)>0 ){
					ipOk = true;
				}
			}
			// if( vr_exists ){
				// mySerial.println("{o9|0}");
				// delay(20);
			// }
			if (nret<0)
			{
				//ttsSpeak(errmsg,1);
				if( vr_exists ){
					mySerial.println("{Y6}");
					delay(500);
				}
				break;
			}		
			if (nret==0)
			{
				//ttsSpeak(toutmsg,1);
				if( vr_exists ){
					mySerial.println("{Y7}");
					delay(500);
				}
				cnt++;
				continue;
			}
			break;
		}
		if( nret<0 ){
			showDebugMessage(WIFIDISCN_str);
			continue;
		}
		if (nret==0)
		{
			showDebugMessage("WIFI_timeout!");
			continue;
		}
		
		showDebugMessage("GET IP");
		//ttsSpeak(okmsg,1);
		if( vr_exists ){
			mySerial.println("{Y5}");
			delay(500);
		}
		cnt = 0;
		while(cnt<3){
			if( ipOk ){
				nret = 1;
				break;
			}
			nret = SerialFind2("WIFI GOT IP",(char*)OK_str,(char*)DISCNT_str,(char*)FAIL_str,NULL,6600);
			// if( vr_exists ){
				// mySerial.print("{o9|0}");
				// delay(20);
			// }
			if (nret<0)
			{
				//ttsSpeak(errmsg,1);
				if( vr_exists ){
					mySerial.println("{Y6}");
					delay(500);
				}
				break;
			}		
			if (nret==0)
			{
				//ttsSpeak(toutmsg,1);
				if( vr_exists ){
					mySerial.println("{Y7}");
					delay(500);
				}
				cnt++;
				continue;
			}
			break;
		}		
		if (nret<0)
		{
			showDebugMessage("IP fail!");
			if( vr_exists ){
				mySerial.println("{YD}");//取地址
				delay(1000);
				mySerial.println("{YB}");//失败
				delay(500);
			}
			continue;
		}		
		if (nret==0)
		{
			showDebugMessage("IP tout!");
			if( vr_exists ){
				mySerial.println("{YD}");//取地址
				delay(1000);
				mySerial.println("{YC}");//超时
				delay(500);
			}
			continue;
		}
		
		//ttsSpeak(okmsg,1);
		if( vr_exists ){
			mySerial.println("{Y5}");
			delay(500);
		}
		
		unsigned long start = millis();
		//while(millis()-start<2000)
		//{
		//	if( Serial.available()>0 )
		//		Serial.read();			
		//}
		while(millis()-start<30000)
		{
			Serial.println("AT+CIFSR"); //get IP address
			result = SerialFind("TAIP,",500);
			if (result)
			{
				break;
			}		
			// if( vr_exists )
				// mySerial.println("{o9|0}");
			DBUG_PRINTLN("failure for IP Address!");
		}
		if( !result ){
			Serial.println("Err IP!");
			break;
		}

		//ttsSpeak(okmsg,1);
		if( vr_exists ){
			mySerial.println("{Y5}");
			delay(200);
		}
		
		int i;
		unsigned int ucsum=0;
		for (i = 0; i < 16; i++)
			strMyIP[i] = '\0';

		Serial.setTimeout(200);
		Serial.readBytesUntil('"', strMyIP, 15); //read to skip the prev '"'
		Serial.setTimeout(200);
		Serial.readBytesUntil('"', strMyIP, 15); //read the IP addr
		data = "IP:";
		data = data+strMyIP;
		i = 0;
		while (i < 16 && strMyIP[i] != 0)
		{
			DBUG_PRINT(strMyIP[i]);
			i++;
		}
		if( strMyIP[0]=='0' && strMyIP[1]=='.' ){
			showDebugMessage(data+" err ip!");
			continue;
		}
		DBUG_PRINTLN('.');
		if( SerialFind("AMAC,",200) ){
			Serial.setTimeout(200);
			Serial.readBytesUntil('"', strMyIP, 15); //read to skip the prev '"'
			Serial.setTimeout(200);
			Serial.readBytesUntil('"', strMyIP, 15); //read the MAC addr
			for(i=0;i<17;i++)
				ucsum += strMyIP[i];
			String strName = String("SMTSKT-")+String(ucsum,HEX);
			strName.toCharArray(name,16);
			name[15] = 0;
			data = data+" MAC:";
			data = data+strMyIP;
			data = data+" NAME:";
			data = data+name;
			showDebugMessage(data);
			//DBUG_PRINT("NAME:");
			//DBUG_PRINTLN(name);
		}
		else{
			if( vr_exists ){
				mySerial.println("{Y6}");
				delay(500);
			}
			//ttsSpeak(errmsg,1);
			//if( vr_exists )	delay(200);
			showDebugMessage("fail for MAC!");
			continue;
		}
		connectOk = 1;
		//if( vr_exists )	delay(300);
		//ttsSpeak("\xb3\xc9\xb9\xa6",1);//成功	
		break;
	}
	while(1);
  
	LEDNotify(6);
	Serial.flush();
  
	if( connectOk>0 ){
		// 设置 单连接
		do{
			delay(50);
			Serial.println("AT+CIPMUX=0");
			if( SerialFind2((char*)OK_str,(char*)OK_str,(char*)ERROR_str,"busy","Mux mode!",2000)>=1 )
				break;
			flipLED13();
		} while (1);
	}
	LEDNotify(2);

	//建立UDP连接，准备广播至7762端口。
	if( connectOk>0 )
		createHostChain();
	LEDNotify(0);

	delay(100);
	Serial.flush();
	Serial.setTimeout(300);
	
	showDebugInfo = 0;
	//myDBUG_PRINTLN("Init8266 ended");
	if( vr_exists ){
		//mySerial.print("{Y3}");
		//delay(500);
		mySerial.println("{Y4}");
		delay(500);
	}
}


void delayTimeControl()
{
  //延时和定时控制需要结合实时钟
  if (toInfo1.enabled && (lTicks/60 == (toInfo1.minute +toInfo1.hour * 60) * 2) )
  {
    //toInfo1.enabled = false;
    state1 = true;
    DBUG_PRINTLN("to1");
  }
  if (tcInfo1.enabled && (lTicks/60 == (tcInfo1.minute +tcInfo1.hour * 60) * 2) )
  {
    //tcInfo1.enabled = false;
    state1 = false;
    DBUG_PRINTLN("tc1");
  }

  if (toInfo2.enabled && (lTicks/60 == (toInfo2.minute +toInfo2.hour * 60) * 2) )
  {
    //toInfo2.enabled = false;
    state2 = true;
    DBUG_PRINTLN("to2");
  }
  if (tcInfo2.enabled && (lTicks/60 == (tcInfo2.minute +tcInfo2.hour * 60) * 2) )
  {
    //tcInfo2.enabled = false;
    state2 = false;
    DBUG_PRINTLN("tc2");
  }

  
  if (doInfo1.enabled && ((lTicks - doInfo1.lStartTick)/60 >= doInfo1.minutes * 2) )
  {
    doInfo1.enabled = false;
    state1 = true;
    DBUG_PRINTLN("do1");
  }
  if (dcInfo1.enabled && ((lTicks - dcInfo1.lStartTick)/60 >= dcInfo1.minutes * 2) )
  {
    dcInfo1.enabled = false;
    state1 = false;
    DBUG_PRINTLN("dc1");
  }

  if (doInfo2.enabled && ((lTicks - doInfo2.lStartTick)/60 >= doInfo2.minutes * 2) )
  {
    doInfo2.enabled = false;
    state2 = true;
    DBUG_PRINTLN("do2");
  }
  if (dcInfo2.enabled && ((lTicks - dcInfo2.lStartTick)/60 >= dcInfo2.minutes * 2) )
  {
    dcInfo2.enabled = false;
    state2 = false;
    DBUG_PRINTLN("dc2");
  }
}

void safetyCheck()
{
  //安全检测检测失败立刻断开电源!!!!!!!!!!!!!!
  do{
	  if ((voltage > max_voltage*10 || voltage < min_voltage*10) && (state1 || state2) )
	  {
		state1 = state2 = false;
		digitalWrite(relayPin9, LOW);
		digitalWrite(relayPin10, LOW);
		warningID1 = 0;
		warningID2 = 0;
		DBUG_PRINT('V');
		DBUG_PRINTLN2(voltage, DEC);
		//ttsSpeak("sound304",2);
		//if( vr_exists )	delay(200);
		if( vr_exists ){
			mySerial.println("{ZC}");
			delay(500);
			mySerial.println("{ZE}");
			delay(500);
		}
		
		break;
		//notifyState();
	  }
	  if ( (state1 || state2 ) && (temperature > max_temperature || temperature < min_temperature ) )
	  {
		state1 = state2 = false;
		digitalWrite(relayPin9, LOW);
		digitalWrite(relayPin10, LOW);
		warningID1 = 3;
		warningID2 = 3;
		DBUG_PRINT('T');
		DBUG_PRINTLN2(temperature, DEC);
		//ttsSpeak("sound315",2);
		//if( vr_exists )	delay(200);
		if( vr_exists ){
			mySerial.println("{ZH}");
			delay(500);
			mySerial.println("{ZE}");
			delay(500);
		}
		break;;
		//notifyState();
	  }
	  if ( state1 && (current1 > max_current1*10 || power1 > max_power1*10) )
	  {
		state1 = false;
		digitalWrite(relayPin9, LOW);
		warningID1 = ((current1 > max_current1*10 ? 1 : 0) + (power1 > max_power1*10 ? 10 : 0));
		DBUG_PRINT('C');
		DBUG_PRINT('a');
		DBUG_PRINTLN2(current1, DEC);
		//ttsSpeak("sound316",2);
		//if( vr_exists )	delay(200);
		if( vr_exists ){
			mySerial.println("{ZD}");
			delay(500);
			mySerial.println("{ZE}");
			delay(500);
		}
		//notifyState();
	  }
	  if ( state2 && (current2 > max_current2*10 || power2 > max_power2*10) )
	  {
		state2 = false;
		digitalWrite(relayPin10, LOW);
		warningID2 = ((current2 > max_current2*10 ? 2 : 0) + (power2 > max_power2*10 ? 20 : 0));
		DBUG_PRINT('C');
		DBUG_PRINT('b');
		DBUG_PRINTLN2(current2, DEC);
		//ttsSpeak("sound320",2);
		//if( vr_exists )	delay(200);
		if( vr_exists ){
			mySerial.println("{ZD}");
			delay(500);
			mySerial.println("{ZE}");
			delay(500);
		}
		//notifyState();
	  }
  }while(0);
}

String stringToHex(const char* sMsg)
{
	int len = strlen(sMsg);
	//mySerial.println(sMsg);
	//mySerial.print("len=");
	//mySerial.println(len,DEC);
	String sRet;
	for(int i=0;i<len;i++){
		if( sMsg[i]<16 && sMsg[i]>=0 )	sRet = sRet+"0";
		sRet = sRet+String(sMsg[i]>=0?int(sMsg[i]):int(sMsg[i])+256,HEX);
	}
	return sRet;
}

void ttsSpeak(const char* sMsg, int waitSecs){
	if( !vr_exists )	return;
	String msg = stringToHex(sMsg);
	int len = msg.length()/4;
	msg = msg+String("4F");
	if( len<16 )	msg = msg+String(0);
	msg = msg+String(len,HEX);
	len = msg.length()/2;
	mySerial.print("{x1");
	if( len+3<16 )	mySerial.print('0');
	mySerial.print(String(len+3,HEX));
	mySerial.print("FD00");
	//len = strlen(sMsg)+2;
	if( len<16 )	mySerial.print("0");
	mySerial.print(len,HEX);
	mySerial.print("01");
	mySerial.print("00");
	mySerial.print(msg);
	mySerial.print("}");
}


void setup()
{
	int cfgOk = 0;
	char c;
	int i;
	// put your setup code here, to run once:
	//.....
	pinMode(pinLED8, OUTPUT);
	digitalWrite(pinLED8, LOW); //turn off led13

	// set the digital pin as output:
	pinMode( relayPin9, OUTPUT );
	pinMode( relayPin10, OUTPUT );
	digitalWrite(relayPin9, LOW);
	digitalWrite(relayPin10, LOW);


	Serial.begin(115200);
	while (!Serial);
	mySerial.begin(9600);
	delay(100);
	//ttsSpeak("\xc9\xe8\xb1\xb8\xc6\xf4\xb6\xaf",1);//设备启动
	//DBUG_PRINTLN("Debug UART ready...");
	//delay(100);
	//退出透传模式
	//mySerial.print("+++");
	//delay(100);
	//测试是否存在debug用的WIFI模块
	LEDNotify(7);

	showDebugMessage("check vr...");
	delay(200);
	mySerial.print("{r000}");
	// delay(200);
	// mySerial.print("{r000}");
	// delay(200);
	// mySerial.print("{r000}");

	PrevTime = millis();
	while (millis() - PrevTime < 8000)
	{
		if(mySerial.available()>0 ){
			c = mySerial.read();
			if( c=='\xFE' ){
				showDebugMessage("...hb...");
				//mySerial.print("{o9|0}");
				//delay(100);
				vr_exists = true;
				continue;
			}
			if( c=='\x9D' ){
				cfgOk = 1;
				showDebugMessage(OK_str);
				vr_exists = true;
				break;
			}
			else{
				showDebugMessage("ret="+String(c));
				vr_exists = true;
				continue;
			}
		}
	}

	if( vr_exists ){	
		mySerial.print("{o7|0}");//关闭看门狗检测
		delay(100);
		mySerial.println("{Y0}");
		delay(500);
	}
	
	if( (cfgOk!=1) && vr_exists ){
		//mySerial.println("{YA}");
		delay(800);
		mySerial.println("{Y2}");
		//ttsSpeak("\xd3\xef\xd2\xf4\xc5\xe4\xd6\xc3",1);//语音配置
		delay(1000);
		mySerial.print("{c0}");
		delay(200);
		mySerial.print("{c0}");
		delay(200);
		mySerial.print("{c0}");
		delay(200);
		mySerial.print("{a0jin shi wei kai}");
		delay(500);
		mySerial.print("{a0bi men si guo}");
		delay(500);
		mySerial.print("{a0zuo xian feng}");
		delay(500);
		mySerial.print("{a0you jiang jun}");
		delay(500);
		mySerial.print("{a0quan ti du you}");
		delay(500);
		mySerial.print("{a0sheng kong bang zhu}");
		delay(500);
		mySerial.print("{f09D}");
		delay(200);
		mySerial.print("{f09D}");
		delay(200);
		mySerial.print("{f09D}");
		delay(200);
		//ttsSpeak("\xb3\xc9\xb9\xa6",1);//成功	
		mySerial.println("{Y4}");
		//ttsSpeak("sound204",1);
		delay(600);
	}
	
	LEDNotify(0);
	//showDebugMessage("start Init8266");

	PrevTime = millis();
	while (millis() - PrevTime < 500)
	{
		while (Serial.available() > 0)
		{
			char c = (char)Serial.read();
			DBUG_PRINT(c);
			PrevTime = millis();
		}
		// if( mySerialFind("\xFE",1) )
			// mySerial.println("{o9|0}");
	}
	
	Init8266();
	//mySerial.println("{o9|0}");
	showDebugInfo = 0;
	
	initTOC();
	initDOC();

	PrevTime = millis()/TICK_DIV;
	InitTime = millis()/TICK_DIV;			

	if( connectOk ){	
		LEDNotify(0);
		delay(200);
		LEDNotify(1);
		delay(200);
		LEDNotify(2);
		delay(200);
		LEDNotify(4);
		delay(200);
		LEDNotify(0);
		delay(200);
		LEDNotify(4);
		delay(200);
		LEDNotify(2);
		delay(200);
		LEDNotify(1);
		delay(200);
		LEDNotify(0);
	}
	if( !vr_exists ){
		mySerial.print("{f0FF}");
		delay(100);
		mySerial.print("{f0FF}");
		delay(100);
		mySerial.print("{f0FF}");
		delay(100);
	}

	LEDNotify(6);
	pinLED8_st = 0;
	digitalWrite(pinLED8, LOW); //turn off led13
	state1 = state2 = true;

	lTicks = 0;
	lInitTicks = 0;

	//myDBUG_PRINTLN("Setup ended");
	if( vr_exists ){
		mySerial.println("{Y1}");
		for(i=0;i<90;i++){
			if(mySerial.available()>0 ){
				c = mySerial.read();
				// if( c=='\xFE' )
					// mySerial.print("{o9|0}");
			}
			delay(100);
		}
	}
	//ttsSpeak("\xbf\xaa\xca\xbc\xc1\xcb",1);//开始了
}

void loop()
{
	// put your main code here, to run repeatedly:
	char c;
	int i;
	int udp_rcvSize = 0;
	unsigned long flag;
	String spkStr;
	if ( fatal_err )
	{
		if( vr_exists ){
			mySerial.println("{ZG}");
			delay(200);
		}
		digitalWrite(pinLED8,HIGH);
		LEDNotify(7);
		delay(500);
		digitalWrite(pinLED8,LOW);
		LEDNotify(0);
		delay(500);
		showDebugMessage("re-setup...");
		setup();
		fatal_err = 0;
	}

	getPwmCurrent_1();

	CurrTime = millis();

	CurrTime /= TICK_DIV;

	if (CurrTime > PrevTime )
	{
		if(mySerial.available()>0 ){
			c = mySerial.read();
			// if( c=='\xFE' )
				// mySerial.print("{o9|0}");
			if( c>=0 && c<5 && (c<2 || open_close>=0) ){
				if( c==0 || open_close==1 )
					spkStr = "\xb4\xf2\xbf\xaa";//打开
				if( c==1 || open_close==0 )
					spkStr = "\xb9\xd8\xb1\xd5";//关闭
			}
			else{
				if( c==5 && vr_exists ){
					mySerial.println("{Y1}");
					for(i=0;i<90;i++){
						if(mySerial.available()>0 ){
							c = mySerial.read();
							// if( c=='\xFE' )
								// mySerial.print("{o9|0}");
						}
						delay(100);
					}
					//ttsSpeak("\xbd\xf0\xca\xaf\xce\xaa\xbf\xaa,\xb1\xd5\xc3\xc5\xcb\xbc\xb9\xfd",3);//金石为开，闭门思过
					//delay(3000);
					//ttsSpeak("\xd7\xf3\xcf\xc8\xb7\xe6",1);//左先锋
					//delay(1000);
					//ttsSpeak("\xd3\xd2\xbd\xab\xbe\xfc",1);//右将军
					//delay(1000);
					//ttsSpeak("\xc8\xab\xcc\xe5\xb6\xbc\xd3\xd0",3);//全体都有
					//delay(1000);
				}
			}
			switch(c){
				case 0:	
				case 1:	open_close = 1-c;
						ttsSpeak((spkStr+"\xc4\xc4\xd2\xbb\xc2\xb7").c_str(),2);//哪一路
						break;
				case 2:	if(open_close>=0){
							ttsSpeak((spkStr+"\xd7\xf3\xc2\xb7").c_str(),1);//左路
							state1 = open_close;
						}
						open_close = -1;
						break;
				case 3:	if(open_close>=0){
							ttsSpeak((spkStr+"\xd3\xd2\xc2\xb7").c_str(),1);//右路
							state2 = open_close;
						}
						open_close = -1;						
						break;
				case 4:	if(open_close>=0){
							ttsSpeak((spkStr+"\xc8\xab\xb2\xbf").c_str(),1);//全部
							state1 = state2 = open_close;
						}
						open_close = -1;						
						break;
			}
		}
		DBUG_PRINTLN("}");
		DBUG_PRINTLN("...");
		DBUG_PRINT("{");
		//lTicks = millis()/500;
		lTicks = lInitTicks+(CurrTime-InitTime)*TICK_DIV/500;
		if (lTicks >= ((long)24 * 60 * 60 * 2))
			lTicks = 0;
		if( (lTicks/120)!=lstMins ){
			speakTime();
			lstMins = lTicks/120;
		}

		circleCount++;
		PrevTime = CurrTime;

		if ( circleCount > MAX_CIRCNT )
			circleCount = 0;

		DBUG_PRINT("~");
		//获取udp数据包到接收缓冲区
		udp_rcvSize = getUdpDatagram(udprcvPakageBuffer + bufPointR, udprcvBuffSize - bufPointR);
		if (udp_rcvSize != 0)
		{
			//DBUG_PRINT('R');
			//DBUG_PRINTLN2(udp_rcvSize, DEC);
			//flipLED13_2();
		}
		if (udp_rcvSize < 0)
		{
			//出错处理
			DBUG_PRINTLN("!get UDP3 x!");
			fatal_err = 1;
			//while(1); //deadlock here!!!
		}
		else
		{
			bufPointR += udp_rcvSize;
		}
		udprcvPakageBuffer[bufPointR] = 0;
		DBUG_PRINT("~");

		//处理接收数据
		if( bufPointR>0 )
		{
			while ( bufPointR > 0 )
			{
				udpDatagramProcess();
				bufPointR = 0;
			}
		}
		DBUG_PRINT("~");

		if ( circleCount % SAMPLE_RATE == 0 )
		{
			voltage = (long)getVoltage() * 39.0625; //标定5000mv为128
			temperature = (getTemperature() * 114) / 64; //单位0.1度
			power1 = (long)(voltage/10) * (long)(current1/10) / 10;//单位mW
			power2 = (long)(voltage/10) * (long)(current2/10) / 10;//单位mW
			power = power1 + power2;

			safetyCheck();
		}
		if ( circleCount % TDPROC_RATE == 0 )
		{
			//延时和定时控制
			delayTimeControl();

			//delievery data to each app
			//notifyState();
		}
		DBUG_PRINT("~");

		//心跳变量处理
		if ( circleCount % HBEAT_RATE == 0 )
		{
			DBUG_PRINT('-');
			if ( linked )
			{
				heartbeatCnt++;
				if ( heartbeatCnt > maxHBdiff )
				{
					//移除主机信息
					removeHostChain();
					DBUG_PRINT('x');
					createHostChain();
					linked = false;
					heartbeatCnt = 0;
					if( vr_exists ){
						mySerial.println("{ZZ}");
						delay(1200);
						mySerial.println("{ZB}");
						delay(200);
					}
				}
				// else{
					// if( vr_exists ){
						// mySerial.println("{Y7}");
						// delay(500);
					// }
				// }
			}
			else{
				DBUG_PRINT('.');
				flag = 0xFFAA00FF|((99&0xff)<<8);
				//if( (heartbeatCnt%3)==0 )
				putUdpDatagram((char*)&flag, 4);
				heartbeatCnt++;
				if ( heartbeatCnt==0 || heartbeatCnt > maxHBdiff*5 )
				{
/* 					if( vr_exists ){
						mySerial.println("{ZZ}");
						delay(1200);
						mySerial.println("{Z0}");
						delay(200);
					}
 */					//移除主机信息
					removeHostChain();
					DBUG_PRINT('x');
					createHostChain();
					heartbeatCnt = 0;
				}
				else{
					//ttsSpeak("sound122",0);
				}
			}
			DBUG_PRINT2(heartbeatCnt,DEC);
			DBUG_PRINT('-');
		}

		if (state1)
			digitalWrite(relayPin9, HIGH);
		else
			digitalWrite(relayPin9, LOW);
		if (state2)
			digitalWrite(relayPin10, HIGH);
		else
			digitalWrite(relayPin10, LOW);

		DBUG_PRINT(";");
	}
}


