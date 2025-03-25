#include <Arduino.h>
#include <WiFi.h>
#include "ESP32OBJ.h"
#include "TM1638.h"
#include "TIMERS.h"
ESP32OBJ esp32;
TM1638 tm1638;
USER_TIMERS timers;
uint8_t days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
String dingShi="";//形如:"0000,1200,1530,2300";/*四个数字字符,前两个表示时,后两个表示分,定时之间用逗号隔开,默认没有定时*/
struct tm info;/*时间结构体*/
uint32_t keyUpTime=0;/*按键释放时间*/
uint32_t keyDownTime=0;/*按键按下时间*/
uint8_t dispMode=0;/*显示模式(0--显示时间;1--显示日期;2--显示星期)*/
uint8_t setMode=0;/*调整模式(0--正常状态;1--低位调整状态;2--中位调整状态;3--高位调整状态)*/
int buzzerPin=2;
void setup ()
{
  Serial.begin(115200);
  esp32.begin();//wifi
  timers.begin();
  tm1638.begin(4,6,8);/*显示模块引脚设置*/
  pinMode(buzzerPin,OUTPUT);
 }
void loop(){
  uint8_t key;uint8_t nowKey;
  if(millis()-keyUpTime>10000){dispMode=0;setMode=0;}/*10秒未操作自动转入时间显示状态,并取消调整状态。*/
  key=tm1638.key();
  if(key!=8){keyDownTime=millis();}/*如按下按键则记录按键时的系统时间*/
  if(dispMode!=0||setMode!=1){info=timers.getTime();}/*获取时间信息(校秒时不刷新)*/
  keySet(key);
  ScreenDisplay(false);
  while(key!=8){
    keyUpTime=millis();
    nowKey=tm1638.key();
    if(nowKey==8&&(key==1||setMode>0&&key>1&&key<4)){timers.setTime(info);break;}else{key=nowKey;}/*与调整有关的按键释放时更新时间,写入RTC*/
    if(keyUpTime-keyDownTime>1500){delay(100);if(key==2||key==3){keySet(key);}}/*按键1.5秒未释放,则进行长按键(只支持"加键"和"减键")连续处理)*/
    if(setMode==0){info=timers.getTime();}/*非调校状态即使按键未释放也会刷新时间*/
    ScreenDisplay(true);/*按键未释放状态不会闪烁*/
    ;
  }/*按键阻塞*/
}
void keySet(uint8_t key){
  /*按键处理函数*/
  switch(key){
    case 0:/*模式键*/
      dispMode=(dispMode+1)%2;
      setMode=0;
      break;  
    case 1:/*调整键*/
      if(dispMode<2){setMode=(setMode+1)%4;}
      break;
    case 2:/*减号键*/
      switch(setMode){
        case 1:/*调整低位*/
          switch(dispMode){
            case 0:/*秒减一*/
              if(info.tm_sec>0){info.tm_sec--;}else{info.tm_sec=59;}break;
            case 1:/*日减一*/
              if(info.tm_mday>1){info.tm_mday--;}else{info.tm_mday=days[info.tm_mon-1];if(info.tm_mon==2&&info.tm_year%4==0){info.tm_mday++;}}break;
          }break;
        case 2:/*调整中位*/
          switch(dispMode){
            case 0:/*分减一*/
              if(info.tm_min>0){info.tm_min--;}else{info.tm_min=59;}break;
            case 1:/*月减一*/
              if(info.tm_mon>1){info.tm_mon--;}else{info.tm_mon=12;}if(info.tm_mday>days[info.tm_mon-1]){info.tm_mday=days[info.tm_mon-1];if(info.tm_mon==2&&info.tm_year%4==0){info.tm_mday++;}}break;
          }break;
        case 3:/*调整高位*/
          switch(dispMode){
            case 0:/*时减一*/
              if(info.tm_hour>0){info.tm_hour--;}else{info.tm_hour=23;}break;
            case 1:/*年减一*/
              if(info.tm_year>1970){info.tm_year--;if(info.tm_year%4!=0&&info.tm_mon==2&&info.tm_mday>days[info.tm_mon-1]){info.tm_mday=days[info.tm_mon-1];}}break;
          }break;
      }
      break;  
    case 3:/*加号键*/
      switch(setMode){
        case 1:/*调整低位*/
          switch(dispMode){
            case 0:/*秒加1*/
              info.tm_sec=(info.tm_sec+1)%60;break;
            case 1:/*日加1*/
              info.tm_mday++;if(info.tm_mday>days[info.tm_mon-1]&&(info.tm_mon!=2||info.tm_year%4!=0||info.tm_mday>29)){info.tm_mday=1;}break;
          }break;
        case 2:/*调整中位*/
          switch(dispMode){
            case 0:/*分加一*/  
              info.tm_min=(info.tm_min+1)%60;break;
            case 1:/*月加一*/
              info.tm_mon=info.tm_mon%12+1;if(info.tm_mday>days[info.tm_mon-1]){info.tm_mday=days[info.tm_mon-1];if(info.tm_mon==2&&info.tm_year%4==0){info.tm_mday++;}}break;
          }break;
        case 3:/*调整高位*/
          switch(dispMode){
            case 0:/*时加一*/
              info.tm_hour=(info.tm_hour+1)%24;break;
           case 1:/*年加一*/
              if(info.tm_year<2037){info.tm_year++;}if(info.tm_mon==2&&info.tm_year%4!=0&&info.tm_mday>days[info.tm_mon-1]){info.tm_mday=days[info.tm_mon-1];}break;
          }break;
      }
      break;  
    case 4:/*响铃设置*/alarmClock(info.tm_hour,info.tm_min,1);break;
    case 5:/*响铃清空*/dingShi="";break;
    case 6:/*配网键*/esp32.smartConfig();break;
    case 7:/*校时键*/timers.ntpUpdate();dispMode=0;setMode=0;break;
    default:/*默认操作*/break;  
  }
}
void ScreenDisplay(boolean mode){
  /*显示处理函数*/
  uint16_t tem;uint8_t i;uint16_t temtime;
  temtime=millis()%1000;
  switch(dispMode){
    case 1:/*显示日期*/
      tem=info.tm_year;
      for(i=0;i<4;i++){
        if(setMode!=3||temtime<500||mode){tm1638.display(3-i,tem%10);tem=tem/10;}else{tm1638.display(3-i,-1);tem=tem/10;}
      }
      tem=info.tm_mon;
      for(i=0;i<2;i++){
        if(setMode!=2||temtime<500||mode){tm1638.display(5-i,tem%10);tem=tem/10;}else{tm1638.display(5-i,-1);tem=tem/10;}
      }
      tem=info.tm_mday;
      for(i=0;i<2;i++){
        if(setMode!=1||temtime<500||mode){ tm1638.display(7-i,tem%10);tem=tem/10;}else{tm1638.display(7-i,-1);tem=tem/10;}
      }
      break;
    default:/*显示时间*/
      tm1638.display(2,16);tm1638.display(5,16);/*显示横线.*/


      tem=info.tm_hour;
      if(setMode!=3||temtime<500||mode){tm1638.display(0,tem/10);tm1638.display(1,tem%10);}else{tm1638.display(0,-1);tm1638.display(1,-1);}
      tem=info.tm_min;
      if(setMode!=2||temtime<500||mode){tm1638.display(3,tem/10);tm1638.display(4,tem%10);}else{tm1638.display(3,-1);tm1638.display(4,-1);}
      tem=info.tm_sec;
      if(setMode!=1||temtime<500||mode){tm1638.display(6,tem/10);tm1638.display(7,tem%10);}else{tm1638.display(6,-1);tm1638.display(7,-1);}
      break;
  }
  /*使用独立LED提示星期几,第一个LED表示星期日*/
  for(i=0;i<7;i++){
    if(info.tm_wday==i){tm1638.led(i,1);}else{tm1638.led(i,0);}
  }
  alarmClock(info.tm_hour,info.tm_min,0);
}
void alarmClock(uint16_t in_hour,uint16_t in_min,uint8_t in_mode){
  //根据小时和分钟数判断是否打开闹铃(每次响铃一分钟).
  //由于使用字符串保存定时数据,实际使用场景基本不用考虑定时控制的数量限制问题(可以使用24*60=1440个定时,字符串最长可达1440*5=7200个字节.).
  //由于没有使用闪存保存,所以断电或重启后定时数据将丢失.
  String temStr="";int i;
  temStr=temStr+char(in_hour/10+48)+char(in_hour%10+48)+char(in_min/10+48)+char(in_min%10+48);/*四个字符代表一个定时时间*/
  switch(in_mode){
    case 0:/*响铃判断*/
      if(dingShi.indexOf(temStr)==-1){tm1638.led(7,0);}else{tm1638.led(7,1);digitalWrite(buzzerPin,HIGH);delay(30000);digitalWrite(buzzerPin,LOW);}/*这里可以更改定时控制方式,示例以最后一个LED亮灭为标志*/
      break;
    case 1:/*响铃设置*/
      i=dingShi.indexOf(temStr);
      if(i!=-1){
        /*已有此定时则删除*/
        if(i==0){
          if(dingShi==temStr){
            dingShi="";
          }else{
            dingShi=dingShi.substring(5);
          }
        }else{
          if(i==dingShi.length()-4){
            dingShi=dingShi.substring(0,dingShi.length()-5);
          }else{
            dingShi=dingShi.substring(0,i-1)+dingShi.substring(i+4);
          }
        }
      }else{
        /*没有此定时则添加*/
        if(dingShi==""){dingShi=temStr;}else{dingShi=dingShi+","+temStr;}
      }
      break;
  }
}
