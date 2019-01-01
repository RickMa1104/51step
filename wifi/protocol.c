/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**

**-----------------------------------------------------------------------------
******************************************************************************/

#include "wifi.h"
#include "touch.h"
#include "Common.h"
#include "light.h"

extern unsigned char  comtem,comwet;
extern signed char comfilter;
extern unsigned int comp25;

extern bit bopen;
extern unsigned char mwind;

extern bit banion;
extern bit bbac;
extern bit bauto;
extern bit bsleep ;
extern bit block;
extern bit resetflag;
extern void startmotor(unsigned char speed);
extern volatile unsigned char opentime ;
extern bit openop ;
extern bit startreset;
extern void closework(void);
extern void openwork(void);
extern void sleepwork(void);
extern void awakework (void);



/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_PM25, DP_TYPE_VALUE},
  {DPID_MODE, DP_TYPE_ENUM},
  {DPID_SPEED, DP_TYPE_ENUM},
  {DPID_FILTER, DP_TYPE_VALUE},
  {DPID_ANION, DP_TYPE_BOOL},
  {DPID_LOCK, DP_TYPE_BOOL},
  {DPID_UV, DP_TYPE_BOOL},
  {DPID_FILTERRESET, DP_TYPE_BOOL},
  {DPID_TEMP, DP_TYPE_VALUE},
  {DPID_HUMIDITY, DP_TYPE_VALUE},
  {DPID_FAULT, DP_TYPE_FAULT},
  {DPID_AIR_QUALITY_INDOOR, DP_TYPE_ENUM},
  {DPID_AIR_QUALITY_OUTDOOR, DP_TYPE_ENUM},
  {DPID_PM25_OUTDOOR, DP_TYPE_VALUE},
  {DPID_SLEEP, DP_TYPE_BOOL},
};


/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/*****************************************************************************
函数名称 : uart_transmit_output
功能描述 : 发数据处理
输入参数 : value:串口收到字节数据
返回参数 : 无
使用说明 : 请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{

    extern void  Send_Data_To_UART0(unsigned char c);
    Send_Data_To_UART0(value);

}


/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上传,实现APP和muc数据同步
输入参数 : 无
返回参数 : 无
使用说明 : 此函数SDK内部需调用;
           MCU必须实现该函数内数据上报功能;包括只上报和可上报可下发型数据
*****************************************************************************/
void all_data_update(void)
{
  mcu_dp_bool_update(DPID_SWITCH,bopen); //BOOL
//  mcu_dp_value_update(DPID_PM25,comp25); //VALUE
  mcu_dp_enum_update(DPID_MODE,bauto); //     
  mcu_dp_enum_update(DPID_SPEED,mwind-1); //enum				  
//  mcu_dp_value_update(DPID_FILTER,comfilter); //VALUE				  
  mcu_dp_bool_update(DPID_ANION,banion); //BOOL
  mcu_dp_bool_update(DPID_LOCK,block); //BOOL					   
  mcu_dp_bool_update(DPID_UV,bbac); //BOOL
//  mcu_dp_bool_update(DPID_FILTERRESET,resetflag); //BOOL		  
//  mcu_dp_value_update(DPID_TEMP,comtem); //VALUE
//  mcu_dp_value_update(DPID_HUMIDITY,comwet); //VALUE
//  mcu_dp_fault_update(DPID_FAULT,0); //fault			   N   
//  mcu_dp_enum_update(DPID_AIR_QUALITY_INDOOR,1); //enum		N meaningless ,app gets pm25 ,enough judgement
  mcu_dp_bool_update(DPID_SLEEP,bsleep); //BOOL					 

}


/*****************************************************************************
函数名称 : dp_download_switch_handle
功能描述 : 针对DPID_SWITCH的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_switch_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret;
    unsigned char Switch;
  
  Switch = mcu_get_dp_download_bool(value,length);
  if(Switch == 0)
  {
    closework();//switch off
  }
  else
  {
    openwork();//switch on
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_bool_update(DPID_SWITCH,Switch);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_mode_handle
功能描述 : 针对DPID_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //示例:当前DP类型为ENUM
  unsigned char ret;
  unsigned char mode;
  
  mode = mcu_get_dp_download_enum(value,length);
  switch(mode)
  {
    case 0:	bauto = 0;
      
      break;
      
    case 1:	bauto = 1;
      
      break;
      
    case 2:	 bsleep =0 ;
      
      break;
      
    case 3:	 bsleep =1;
      
      break;
      
    default:
      
      break;
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_enum_update(DPID_MODE,mode);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_speed_handle
功能描述 : 针对DPID_SPEED的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_speed_handle(const unsigned char value[], unsigned short length)
{

  unsigned char ret;
  unsigned char speed;
  
  speed = mcu_get_dp_download_enum(value,length);
  switch(speed)
  {
    case 0:	  mwind = 1;  	  bauto = 0;
      
      break;
      
    case 1:	  mwind = 2; 	bauto = 0;
      
      break;
      
    case 2:	  mwind = 3; 	 bauto = 0;
      
      break;
      
    case 3:	  mwind = 4;   bauto = 0;
      
      break;
      
    default:
      
      break;
  }
  if(!openop&&!startreset)
  startmotor(mwind); 
  //finish DP reflect to sever;
  ret = mcu_dp_enum_update(DPID_SPEED,speed);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_anion_handle
功能描述 : 针对DPID_ANION的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_anion_handle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char anion;
  
  anion = mcu_get_dp_download_bool(value,length);
  if(anion == 0)
  {
    banion = 0;//switch off
  }
  else
  {
    banion = 1;//switch on
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_bool_update(DPID_ANION,anion);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_lock_handle
功能描述 : 针对DPID_LOCK的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_lock_handle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char lock;
  
  lock = mcu_get_dp_download_bool(value,length);
  if(lock == 0)
  {
    //switch off
  }
  else
  {
    //switch on
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_bool_update(DPID_LOCK,lock);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_uv_handle
功能描述 : 针对DPID_UV的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_uv_handle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char uv;
  
  uv = mcu_get_dp_download_bool(value,length);
  if(uv == 0)
  {
    bbac = 0;//switch off
  }
  else
  {
    bbac = 1;//switch on
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_bool_update(DPID_UV,uv);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_filterreset_handle
功能描述 : 针对DPID_FILTERRESET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_filterreset_handle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char filterreset;
  
  filterreset = mcu_get_dp_download_bool(value,length);
  if(filterreset == 0)
  {
    //switch off
  }
  else
  {
    //switch on
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_bool_update(DPID_FILTERRESET,filterreset);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_air_quality_outdoor_handle
功能描述 : 针对DPID_AIR_QUALITY_OUTDOOR的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 只下发类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_air_quality_outdoor_handle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char air_quality_outdoor;
  
  air_quality_outdoor = mcu_get_dp_download_enum(value,length);
  switch(air_quality_outdoor)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    case 3:
      
      break;
      
    case 4:
      
      break;
      
    case 5:
      
      break;
      
    default:
      
      break;
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_enum_update(DPID_AIR_QUALITY_OUTDOOR,air_quality_outdoor);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_pm25_outdoor_handle
功能描述 : 针对DPID_PM25_OUTDOOR的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 只下发类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_pm25_outdoor_handle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned long pm25_outdoor;
  
  pm25_outdoor = mcu_get_dp_download_value(value,length);

  
  //finish DP reflect to sever;
  ret = mcu_dp_value_update(DPID_PM25_OUTDOOR,pm25_outdoor);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_sleep_handle
功能描述 : 针对DPID_SLEEP的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_sleep_handle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char sleep;
  
  sleep = mcu_get_dp_download_bool(value,length);
  if(sleep == 0)
  {

   awakework ();//switch off
  }
  else
  {
   sleepwork();//switch on
  }
  
  //finish DP reflect to sever;
  ret = mcu_dp_bool_update(DPID_SLEEP,sleep);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : MCU校对本地RTC时钟
输入参数 : 无
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  /*
  time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
  time[1] 为 年 份 , 0x00 表 示2000 年
  time[2]为月份，从 1 开始到12 结束
  time[3]为日期，从 1 开始到31 结束
  time[4]为时钟，从 0 开始到23 结束
  time[5]为分钟，从 0 开始到59 结束
  time[6]为秒钟，从 0 开始到59 结束
  time[7]为星期，从 1 开始到 7 结束，1代表星期一
 */
  if(time[0] == 1)
  {
    //get wifi local rtctime 
	 
  }
  else
  {
  	//get wrong,maybe wifi no net
  }
}
#endif


/*****************************************************************************
函数名称 : wifi_test_result
功能描述 : wifi功能测试反馈
输入参数 : result:wifi功能测试结果;0:失败/1:成功
           rssi:测试成功表示wifi信号强度/测试失败表示错误类型
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{

  if(result == 0)
  {
    //测试失败
    if(rssi == 0x00)
    {
      //未扫描到名称为tuya_mdev_test路由器,请检查
	  writeruntime(0,0,0,0,0,0,0);
	  writeno();
    }
    else if(rssi == 0x01)
    {
	  writeruntime(0,0,0,0,0,0,0);
	  writeun();
      //模块未授权
    }
  }
  else
  {
    //测试成功 ;rssi为信号强度(0-100, 0信号最差，100信号最强)
    writeruntime(0,0,0,0,rssi/100,rssi/10%10,rssi%10);
  }
  
}


#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
函数名称 : mcu_firm_update_handle
功能描述 : MCU进入固件升级模式
输入参数 : value:固件缓冲区
           position:当前数据包在于固件位置
           length:当前固件包长度(固件包长度为0时,表示固件包发送完成)
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
  #error "请自行完成MCU固件升级代码,完成后请删除该行"
  if(length == 0)
  {
    //固件数据发送完成
    
  }
  else
  {
    //固件数据处理
  }
  
  return SUCCESS;
}
#endif
/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地址
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  当前函数处理可下发/可上报数据调用                    
  具体函数内需要实现下发数据处理
  完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_SWITCH:
      //开关处理函数
      ret = dp_download_switch_handle(value,length); 
      break;
    case DPID_MODE:
      //模式处理函数
      ret = dp_download_mode_handle(value,length);	keytone();
      break;
    case DPID_SPEED:
      //风速处理函数
      ret = dp_download_speed_handle(value,length);	bauto =0 ;mcu_dp_enum_update(DPID_MODE,bauto); keytone();
      break;
    case DPID_ANION:
      //负离子处理函数
      ret = dp_download_anion_handle(value,length);	keytone();
      break;
    case DPID_LOCK:
      //童锁处理函数
      ret = dp_download_lock_handle(value,length);	keytone();
      break;
    case DPID_UV:
      //UV杀菌处理函数
      ret = dp_download_uv_handle(value,length);  keytone();
      break;
    case DPID_FILTERRESET:
      //滤芯复位处理函数
      ret = dp_download_filterreset_handle(value,length);
      break;
    case DPID_AIR_QUALITY_OUTDOOR:
      //室外空气质量处理函数
      ret = dp_download_air_quality_outdoor_handle(value,length);
      break;
    case DPID_PM25_OUTDOOR:
      //室外PM2.5处理函数
      ret = dp_download_pm25_outdoor_handle(value,length);
      break;
    case DPID_SLEEP:
      //睡眠处理函数
      ret = dp_download_sleep_handle(value,length);	  
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
