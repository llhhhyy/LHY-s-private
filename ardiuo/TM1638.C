#ifndef TM1638_H  //避免重复定义.
  #define TM1638_H
  class TM1638{
  private:
    //在我的ESP32模块上,所有”delayMicroseconds(TM1637_DELAY_US)”（延时）语句都是可以删除的,但为了使更高主频的MCU可以适应总线速度,建议还是加上延时.
    #define TM1637_DELAY_US 1
    uint8_t STB = 17;uint8_t CLK = 16;uint8_t DIO = 4;//TM1638模块引脚定义.
    //LED_CODE_COUNT表示LED数码管可显示字符编码的总量.如要扩展编码字符,修改下面两行就可以了.
    #define LED_CODE_COUNT 20
    uint8_t LED_CODE[LED_CODE_COUNT]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40,0x74,0x5C,0x00};//数字编码0~f加"-""h""o",最后一个字符为熄灭状态.
    uint8_t brightnessData=0;//亮度值.
    uint8_t ledData=0;//LED显存(每一个Bit位对应一只LED灯珠),默认全部熄灭.
    uint8_t pointData=0;//小数点显存(第一个Bit位对应一只数码管的小数点),默认全部熄灭.
    uint8_t displayData[8];//数码管显存.
  protected:
    void init(void){uint8_t i;pinMode(STB,OUTPUT);pinMode(CLK,OUTPUT);digitalWrite(STB,HIGH);digitalWrite(CLK,HIGH);com_Write(0x88);com_Write(0x40);digitalWrite(STB,LOW);bus_Write(0xc0);for(i=0;i<16;i++){bus_Write(0x00);}digitalWrite(STB,HIGH);}//初始化.
    uint8_t bus_Read(void){uint8_t i;uint8_t temp=0;pinMode(DIO,INPUT);delayMicroseconds(TM1637_DELAY_US);for(i=0;i<8;i++){temp>>=1;digitalWrite(CLK,LOW);delayMicroseconds(TM1637_DELAY_US);digitalWrite(CLK,HIGH);delayMicroseconds(TM1637_DELAY_US);if(digitalRead(DIO)==HIGH){temp|=0x80;}}return temp;}//从总线上读一个字节.
    void bus_Write(uint8_t Data){uint8_t i;pinMode(DIO,OUTPUT);for(i=0;i<8;i++){delayMicroseconds(TM1637_DELAY_US);digitalWrite(CLK,LOW);digitalWrite(DIO,Data&0x01);delayMicroseconds(TM1637_DELAY_US);digitalWrite(CLK,HIGH);Data>>=1;}}//向总线上写一个字节.
    void com_Write(uint8_t in_Com){digitalWrite(STB,LOW);delayMicroseconds(TM1637_DELAY_US);bus_Write(in_Com);delayMicroseconds(TM1637_DELAY_US);digitalWrite(STB,HIGH);}//写命令字.
    void add_Write(uint8_t Add,uint8_t Data){com_Write(0x44);digitalWrite(STB,LOW);bus_Write(0xc0|Add);bus_Write(Data);digitalWrite(STB,HIGH);}//指定地址写入数据.
  public:
    TM1638(void){init();}
    TM1638(uint8_t in_STB,uint8_t in_CLK,uint8_t in_DIO){STB=in_STB;CLK=in_CLK;DIO=in_DIO;init();}
    void    begin(void){init();}
    void    begin(uint8_t in_STB,uint8_t in_CLK,uint8_t in_DIO){STB=in_STB;CLK=in_CLK;DIO=in_DIO;init();}
    void    test(void){uint8_t index=0;uint8_t count=0;uint8_t tem=0;com_Write(0x8f);for(index=0;index<16;index+=2){com_Write(0xc0);add_Write(0xc0|index,0xff);add_Write(0xc0|index+1,0xff);delay(50);}for(count=0;count<6;count++){com_Write(0x87|(count%2)<<3);delay(100);}delay(500);}//测试(用于展示LED是否可以点亮,该过程会破坏先前的显示内容).
    void    brightness(uint8_t in_Data){brightnessData=in_Data&0x07;com_Write(0x88|brightnessData);}//亮度设置(0~7).
    uint8_t brightness(void){return brightnessData;}//返回亮度设置(0~7).
    uint8_t key(void){uint8_t c[4],i,key_value=0;digitalWrite(STB,LOW);bus_Write(0x42);for(i=0;i<4;i++){c[i]=bus_Read();}digitalWrite(STB,HIGH);for(i=0;i<4;i++){key_value|=c[i]<<i;}for(i=0;i<8;i++){if((0x01<<i)==key_value){break;}}return i;}//获取按键值(0~8,8表示未按键),本系统8根按键线都接在K3端口,不支持组合键.
    void    led(uint8_t index,boolean state){if(index>7){return;}if(state){ledData=ledData|(0x01<<index);}else{ledData=ledData&(~(0x01<<index));}add_Write(2*index+1,state);}//控制指定的LED亮/灭状态.
    void    led(uint8_t in_ledData){uint8_t i;ledData=in_ledData;for(i=0;i<8;i++){if(ledData&(0x01<<i)){add_Write(2*i+1,1);}else{add_Write(2*i+1,0);}}}//以一个字节数据控制8个LED灯珠,参数的每一位控制一个LED灯珠的亮/灭状态.
    uint8_t led(void){return ledData;}//返回一个字节的数据,其每个Bit位表示相应LED的亮/灭状态.
    void    point(uint8_t index,boolean state){if(index>=8){return;}if(state){pointData=pointData|(0x01<<index);add_Write(2*index,LED_CODE[displayData[index]]|0x80);}else{pointData=pointData&(~(0x01<<index));add_Write(2*index,LED_CODE[displayData[index]]&0x7f);}}//改变指定LED数码管小数点状态.
    void    point(uint8_t in_pointData){uint8_t i;pointData=in_pointData;for(i=0;i<8;i++){if(pointData&(0x01<<i)){add_Write(2*i,LED_CODE[displayData[i]]|0x80);}else{add_Write(2*i,LED_CODE[displayData[i]]&0x7f);}}}//设置所有小数点的亮/灭,参数的每一个Bit位对应一个数码管的小数点.
    uint8_t point(void){return pointData;}//以单字节数据返回小数点显示状态.每个位对应一个小数点.
    void    display(uint8_t index,int in_Data){uint8_t Data;Data=uint8_t(in_Data);if(index>=8||in_Data>=LED_CODE_COUNT){return;}if(in_Data<0){add_Write(2*index,0x00);return;}displayData[index]=Data;if(pointData&(0x01<<index)){add_Write(2*index,LED_CODE[displayData[index]]|0x80);}else{add_Write(2*index,LED_CODE[displayData[index]]&0x7f);}}//指定数码管显示数据,负数参数表示熄灭.
    uint8_t display(uint8_t index){if(index>=8){return 0;}return displayData[index];}//返回指定LED数码管的十六进制值(含小数点).
    void    close(){com_Write(0x80);}//关闭显示.
    void    open(){com_Write(0x88|brightnessData);}//打开显示.
  };
#endif
