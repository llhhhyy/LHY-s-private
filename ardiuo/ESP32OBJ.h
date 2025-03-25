#ifndef ESP32OBJ_H  //避免重复定义.WIFI配网连接对象.需要指定LED指示灯IO地址和配网按键IO地址.如果不指定,则使用ESP8266默认IO地址.
  #define ESP32OBJ_H  
  #include <WiFi.h>//ESP32
/*WiFi.status()//WIFI模块状态值含义:
0：​ WL_IDLE_STATUS – 返回值为0说明正在尝试连接
1​： WL_NO_SSID_AVAIL – 返回值为1说明没有找到设定的SSID的网络
2​： WL_SCAN_COMPLETED – 返回值为2说明网络扫描完毕
3：​ WL_CONNECTED – 返回值为3说明连接成功成功
4： WL_CONNECT_FAILED – 返回值为4说明连接失败
5： WL_CONNECTION_LOST – 返回值为5说明连接丢失
6： WL_DISCONNECTED – 返回值为6说明未连接
*/
class ESP32OBJ{//ESP32板载资源连接对象(可以使用安信可公众号进行WIFI配网).
private:
  uint8_t led=2;//LED--D2(GPIO2)
  uint8_t key=0;//KEY--D0(GPIO0)
protected:
//analogRead();此函数可以读取引脚模拟量。
public:
  ESP32OBJ(void){;}
  ESP32OBJ(uint8_t _led_io,uint8_t _key_io){
    led=_led_io;//指定LED引脚IO地址.
    key=_key_io;//指定按键引脚IO地址.
  }
void begin(void){
    pinMode(led, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
    pinMode(key, INPUT); //设置内置LED引脚为输出模式以便控制LED
    //自动连接,失败则进入WIFI配网模式.
    if(autoConfig()!=0){smartConfig();}
  }
char begin(char *Ssid,char *PassWord){
    //使用指定WIFI名和密码进行连接.
    char Index=60;//设定连接等待时间30秒.
    char getKeyindex;
    pinMode(led, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
    pinMode(key, INPUT); //设置内置LED引脚为输出模式以便控制LED
    WiFi.mode(WIFI_STA);
    WiFi.begin(Ssid,PassWord);
    Serial.print("\r\n连接WIFI...");
    while(WiFi.status()!=WL_CONNECTED && Index!=0){
      Serial.print(".");Index--;
      digitalWrite(led,not digitalRead(led));//LED慢速闪烁表示正在连接WIFI.
      for(getKeyindex=0;getKeyindex<50;getKeyindex++){//有按键按下则退出。
        delayMicroseconds(10000);if(digitalRead(key)==0){delayMicroseconds(1000);if(digitalRead(key)==0){lightOff();return 1;}}
      }
    }
    if(Index!=0){
      Serial.println("成功!");
      Serial.print("\r\n本机IP地址:");Serial.print(WiFi.localIP());Serial.print("\t\t路由器IP地址:");Serial.println( WiFi.gatewayIP());
      digitalWrite(led,1);//LED常亮表示WIFI连接成功.
      return 0;
    }else{
      digitalWrite(led,0);//连接失败LED熄灭.
      Serial.println("失败!");
      return 1;
    }
}
String getSSID(){
  return WiFi.SSID().c_str();
}
String getIP(){
  uint8_t IpData[4];String TemStr="";
  uint8_t index;
  for(index=0;index<4;index++){
    IpData[index]=WiFi.localIP()[index];
    if(IpData[index]>100){TemStr=TemStr+(char)(IpData[index]/100+48);IpData[index]=IpData[index]%100;}
    if(IpData[index]>10){TemStr=TemStr+(char)(IpData[index]/10+48);IpData[index]=IpData[index]%10;}
    TemStr=TemStr+(char)(IpData[index]+48);
    if(index<3){TemStr=TemStr+".";}
  }
  return TemStr;
}
char autoConfig(void){
    //自动连接WiFi.
    char getKeyindex;
    int i=20;
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    pinMode(led, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
    pinMode(key, INPUT); //设置内置LED引脚为输出模式以便控制LED
    Serial.print("\r\n自动连接WIFI...");
    while (i>0 && WiFi.status() != WL_CONNECTED)//
    {
      i--;
      Serial.print(".");
      digitalWrite(led,not digitalRead(led));//LED慢速闪烁表示正在连接WIFI.
      for(getKeyindex=0;getKeyindex<50;getKeyindex++){//有按键按下则退出。
        delayMicroseconds(10000);if(digitalRead(key)==0){delayMicroseconds(1000);if(digitalRead(key)==0){lightOff();return 1;}}
      }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("成功!");
      Serial.printf("\r\nSSID:%s   localIP:%u.%u.%u.%u   routeIP:%u.%u.%u.%u",WiFi.SSID().c_str(),WiFi.localIP()&0x000000FF,(WiFi.localIP()>>8)&0x0000FF,(WiFi.localIP()>>16)&0x00FF,WiFi.localIP()>>24,WiFi.gatewayIP()&0x000000FF,(WiFi.gatewayIP()>>8)&0x0000FF,(WiFi.gatewayIP()>>16)&0x00FF,WiFi.gatewayIP()>>24);
      lightOn(); //灯亮表示连接成功。
      return 0;//返回0表示连接成功.
    }else{
      Serial.println("失败!" );
      lightOff(); //灯灭表示连接失败。
      return 1;//返回1表示连接失败.
    }
}
void smartConfig(){
  
  char getKeyindex;
  int WaitIndex=0;long SmartlastMs;
  //如果WIFI模块未连接,则连接WIFI,并等待连接结束.
  if(WiFi.status() == WL_DISCONNECTED){
    Serial.print("准备...");
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    for(WaitIndex=60;WaitIndex>0;WaitIndex--){
      if(WiFi.status() != WL_DISCONNECTED){break;}
      Serial.print(".");
      for(getKeyindex=0;getKeyindex<50;getKeyindex++){//有按键按下则退出。
        delayMicroseconds(10000);if(digitalRead(key)==0){delayMicroseconds(1000);if(digitalRead(key)==0){lightOff();return;}}
      }
    }
  }
  pinMode(led, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
  pinMode(key, INPUT); 
  WiFi.mode(WIFI_STA);
  Serial.println("\r\n微信WIFI配网(关注\"安信可科技\"微信公众号-->应用开发--->微信配网):");
  WiFi.beginSmartConfig();
  SmartlastMs=millis();
  WaitIndex=0;
  while (1)
  {
    Serial.print(".");if(WaitIndex++>50){WaitIndex=0;Serial.println("");}
    for(getKeyindex=0;getKeyindex<25;getKeyindex++){//有按键按下则退出。
      delayMicroseconds(10000);if(digitalRead(key)==0){delayMicroseconds(1000);if(digitalRead(key)==0){lightOff();return;}}
    }
    digitalWrite(led,not digitalRead(led));
    if (WiFi.smartConfigDone())
    {
      Serial.println("\r\n配网成功!");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      Serial.print("\r\n连接WIFI...");
      //WiFiMultiObj.addAP(WiFi.SSID().c_str(),WiFi.psk().c_str());WiFiMultiObj.run();
      //等待WIFI连接.
      for(WaitIndex=60;WaitIndex>0;WaitIndex--){
        if(WiFi.status() == WL_CONNECTED){break;}
        Serial.print(".");
        for(getKeyindex=0;getKeyindex<50;getKeyindex++){//有按键按下则退出。
          delayMicroseconds(10000);if(digitalRead(key)==0){delayMicroseconds(1000);if(digitalRead(key)==0){lightOff();return;}}
        }
      }
      if(WiFi.status() == WL_CONNECTED){
        Serial.println("成功!");
        lightOn();
      }else{
        Serial.println("失败!");
        lightOff();
      }
      break;
    }
    if (millis() - SmartlastMs >= 60000)
    {
      //60秒内配网不成功,则退出.
      Serial.println("\r\n配网超时!");
      lightOff();
      break;
    }
  }
  WiFi.beginSmartConfig();//退出smartConfig.必须加上此句,否则二次使用smartConfig会不正常.
}//微信配网.
void lightOff(void){lightOff(led);}//熄灭LED.
void lightOff(uint8_t LED_IO_NUM){pinMode(LED_IO_NUM, OUTPUT);digitalWrite(LED_IO_NUM,0);}
void lightOn(void){lightOn(led);}//点亮LED.
void lightOn(uint8_t LED_IO_NUM){pinMode(LED_IO_NUM, OUTPUT);digitalWrite(LED_IO_NUM,1);}
void twinkle(void){twinkle(1);}
void twinkle(uint8_t Repeat){twinkle(Repeat,5);}
void twinkle(uint8_t Repeat,uint8_t Mode){twinkle(Repeat,Mode,led);}
void twinkle(uint8_t Repeat,uint8_t Mode,uint8_t LED_IO_NUM){uint8_t Index;if(Mode>10){Mode=10;}Mode=10-Mode;pinMode(LED_IO_NUM, OUTPUT);for(;Repeat>0;Repeat--){digitalWrite(LED_IO_NUM,1);for(Index=0;Index<=Mode*10;Index++){delayMicroseconds(10000);if(digitalRead(key)==0){delayMicroseconds(1000);if(digitalRead(key)==0){lightOff();return;}}}digitalWrite(LED_IO_NUM,0);for(Index=0;Index<=Mode*10;Index++){delayMicroseconds(10000);if(digitalRead(key)==0){delayMicroseconds(1000);if(digitalRead(key)==0){lightOff();return;}}}}}//Repeat:闪烁次数;Mode--闪烁快慢（0~10）。
uint8_t getLed(void){return digitalRead(led);}//读取LED状态:0--灭;1--亮.
uint8_t getKey(void){if(digitalRead(key)!=0){return 0;}unsigned long KeyMillis;KeyMillis=millis();delayMicroseconds(1000);if(digitalRead(key)!=0){return 0;}while(millis()-KeyMillis<1000){delayMicroseconds(1000);if(digitalRead(key)!=0){delayMicroseconds(1000);if(digitalRead(key)!=0){return 1;}}}while(millis()-KeyMillis<10000){delayMicroseconds(1000);if(digitalRead(key)!=0){delayMicroseconds(1000);if(digitalRead(key)!=0){return 2;}}}while(millis()-KeyMillis<20000){delayMicroseconds(1000);if(digitalRead(key)!=0){delayMicroseconds(1000);if(digitalRead(key)!=0){return 3;}}}while(millis()-KeyMillis<60000){delayMicroseconds(1000);if(digitalRead(key)!=0){delayMicroseconds(1000);if(digitalRead(key)!=0){return 4;}}}return 255;}//按键状态:0--未按下;1--短按(模拟左键);2--表示长按(模拟右键);3--超长按(用于WIFI配网);4--特长按(用于系统复位);255--短路损坏。
void loop(void){if(WiFi.status()==WL_CONNECTED){return;}else{autoConfig();}}//检查WIFI连接,如果未连接,则自动进行连接.
};
#endif
