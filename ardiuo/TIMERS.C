#ifndef USER_TIMERS_H  //避免重复定义.
  #define USER_TIMERS_H
  class USER_TIMERS{
  private:
    struct tm timeInfo;//声明时间结构体.注意其中timeInfo.tm_year是表示从1900年以来经过的年数;而月份timeInfo.tm_mon是从0开始的.
  protected:
    void init(void){configTime(8*3600,0,"ntp1.aliyun.com","ntp2.aliyun.com","ntp2.aliyun.com");}/*最初的设置很重要,因为这会影响到时区设置*/
    boolean getTimeData(void){if(!getLocalTime(&timeInfo)){return 0;}else{return 1;}}/*检查读取时间信息是否正常*/
  public:
    USER_TIMERS(void){init();}
    void begin(void){init();}
    uint16_t year(void){tm info;info=getTime();return info.tm_year;}
    uint16_t mon(void){tm info;info=getTime();return info.tm_mon;}
    uint16_t mday(void){tm info;info=getTime();return info.tm_mday;}
    uint16_t hour(void){tm info;info=getTime();return info.tm_hour;}
    uint16_t min(void){tm info;info=getTime();return info.tm_min;}
    uint16_t sec(void){tm info;info=getTime();return info.tm_sec;}
    uint16_t wday(void){tm info;info=getTime();return info.tm_wday;}
    time_t getSysStamp(void){time_t now;time(&now);return now;}/*获取系统时间戳.(如:1657625000)*/
    void   setSysStamp(time_t time_stamp){struct timeval tv;tv.tv_sec=time_stamp;tv.tv_usec=0;/*记录时间的结构体,包含两个成员变量(tv_sec:秒数;tv_usec:微秒数).*/struct timezone tz;tz.tz_minuteswest = 0;tz.tz_dsttime = 0;/*记录时间的结构体,包含两个成员变量(tz_minuteswest:时区修正分钟数;tz_dsttime:夏时制修正.).*/settimeofday(&tv,&tz);}/*更新系统时间戳.(如:1657625000)*/
    void   setTime(struct tm info){time_t now;info.tm_year-=1900;/*将年号转换为系统需要的数字.*/info.tm_mon-=1;/*将月号转换为为系统需要的数字(0~11).*/now=mktime(&info);setSysStamp(now);}
    struct tm getTime(void){struct tm info;time_t now;time(&now);/*time获取时间戳。*/localtime_r(&now,&info);info.tm_year+=1900;info.tm_mon+=1;return info;}/*获取时间信息*/
    void   ntpUpdate(void){configTime(8*3600,0,"ntp1.aliyun.com","ntp2.aliyun.com","ntp2.aliyun.com");}/*使用联网NPT服务器更新时间*/
  };
#endif
