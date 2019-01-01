/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
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
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
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
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/*****************************************************************************
�������� : uart_transmit_output
�������� : �����ݴ���
������� : value:�����յ��ֽ�����
���ز��� : ��
ʹ��˵�� : �뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{

    extern void  Send_Data_To_UART0(unsigned char c);
    Send_Data_To_UART0(value);

}


/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺���SDK�ڲ������;
           MCU����ʵ�ָú����������ϱ�����;����ֻ�ϱ��Ϳ��ϱ����·�������
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
�������� : dp_download_switch_handle
�������� : ���DPID_SWITCH�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_mode_handle
�������� : ���DPID_MODE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
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
�������� : dp_download_speed_handle
�������� : ���DPID_SPEED�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_anion_handle
�������� : ���DPID_ANION�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_lock_handle
�������� : ���DPID_LOCK�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_uv_handle
�������� : ���DPID_UV�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_filterreset_handle
�������� : ���DPID_FILTERRESET�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_air_quality_outdoor_handle
�������� : ���DPID_AIR_QUALITY_OUTDOOR�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ֻ�·�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_pm25_outdoor_handle
�������� : ���DPID_PM25_OUTDOOR�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ֻ�·�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�������� : dp_download_sleep_handle
�������� : ���DPID_SLEEP�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
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
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  /*
  time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
  time[1] Ϊ �� �� , 0x00 �� ʾ2000 ��
  time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
  time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
  time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
  time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[7]Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
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
�������� : wifi_test_result
�������� : wifi���ܲ��Է���
������� : result:wifi���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{

  if(result == 0)
  {
    //����ʧ��
    if(rssi == 0x00)
    {
      //δɨ�赽����Ϊtuya_mdev_test·����,����
	  writeruntime(0,0,0,0,0,0,0);
	  writeno();
    }
    else if(rssi == 0x01)
    {
	  writeruntime(0,0,0,0,0,0,0);
	  writeun();
      //ģ��δ��Ȩ
    }
  }
  else
  {
    //���Գɹ� ;rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
    writeruntime(0,0,0,0,rssi/100,rssi/10%10,rssi%10);
  }
  
}


#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
�������� : mcu_firm_update_handle
�������� : MCU����̼�����ģʽ
������� : value:�̼�������
           position:��ǰ���ݰ����ڹ̼�λ��
           length:��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
  #error "���������MCU�̼���������,��ɺ���ɾ������"
  if(length == 0)
  {
    //�̼����ݷ������
    
  }
  else
  {
    //�̼����ݴ���
  }
  
  return SUCCESS;
}
#endif
/******************************************************************************
                                WARNING!!!                     
���º����û������޸�!!
******************************************************************************/

/*****************************************************************************
�������� : dp_download_handle
�������� : dp�·�������
������� : dpid:DP���
           value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERRO
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  ��ǰ����������·�/���ϱ����ݵ���                    
  ���庯������Ҫʵ���·����ݴ���
  �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {
    case DPID_SWITCH:
      //���ش�����
      ret = dp_download_switch_handle(value,length); 
      break;
    case DPID_MODE:
      //ģʽ������
      ret = dp_download_mode_handle(value,length);	keytone();
      break;
    case DPID_SPEED:
      //���ٴ�����
      ret = dp_download_speed_handle(value,length);	bauto =0 ;mcu_dp_enum_update(DPID_MODE,bauto); keytone();
      break;
    case DPID_ANION:
      //�����Ӵ�����
      ret = dp_download_anion_handle(value,length);	keytone();
      break;
    case DPID_LOCK:
      //ͯ��������
      ret = dp_download_lock_handle(value,length);	keytone();
      break;
    case DPID_UV:
      //UVɱ��������
      ret = dp_download_uv_handle(value,length);  keytone();
      break;
    case DPID_FILTERRESET:
      //��о��λ������
      ret = dp_download_filterreset_handle(value,length);
      break;
    case DPID_AIR_QUALITY_OUTDOOR:
      //�����������������
      ret = dp_download_air_quality_outdoor_handle(value,length);
      break;
    case DPID_PM25_OUTDOOR:
      //����PM2.5������
      ret = dp_download_pm25_outdoor_handle(value,length);
      break;
    case DPID_SLEEP:
      //˯�ߴ�����
      ret = dp_download_sleep_handle(value,length);	  
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
�������� : get_download_cmd_total
�������� : ��ȡ����dp�����ܺ�
������� : ��
���ز��� : �·������ܺ�
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
