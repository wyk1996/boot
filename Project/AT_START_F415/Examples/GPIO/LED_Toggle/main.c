/**
  ******************************************************************************
  * File   : GPIO/LED_Toggle/main.c
  * Version: V1.2.8
  * Date   : 2020-11-27
  * Brief  : Main program body
  ******************************************************************************
  */

#include <stdio.h>
#include "at32f4xx.h"
#include "main.h"
#include "flashdispos.h"

#include  <includes.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskCreate         (void);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/


SYSTEM_RTCTIME gs_SysTime;
pFunction Jump_To_LoadCode;
uint32_t JumpAddress;
#define LoadCodeAddress    0x8000000//跳转到烧写代码的位置，通过这个位置执行程序，将应用代码从FLASH里面导出来烧写到应用程序区域
/* Private functions--------------------------------------------------------------------------*/
//跳转到烧写代码部分
void   JumpToProgramCode(void)
{
    __set_FAULTMASK(1);   //跳转前必须关闭全部中断，不关闭会出现跳转错误
    JumpAddress = *(__IO uint32_t*) (LoadCodeAddress + 4);
    Jump_To_LoadCode = (pFunction) JumpAddress;

    //初始化用户程序的堆栈指针
    __set_MSP(*(__IO uint32_t*) LoadCodeAddress);
    Jump_To_LoadCode();
}


#define  SECTOR_SIZE   1024            //AT32F415CBT7 内部flash 扇区大小为1K   一共128K  其中100K为程序区
#define FLASH_SIZE   	128

#define FALSE_W_BASE    0x8000000

//从指定地址开始写入多个数据
void FLASH_WriteMoreData(uint32_t startAddress,uint8_t *writeData,uint16_t countToWrite)
{
    if(startAddress<FLASH_BASE||((startAddress+countToWrite)>=(FLASH_BASE+1024*FLASH_SIZE)))
    {
        return;//非法地址
    }
    FLASH_Unlock();         //解锁写保护
    uint32_t offsetAddress=startAddress-FLASH_BASE;               //计算去掉0X08000000后的实际偏移地址
    uint32_t sectorPosition=offsetAddress/SECTOR_SIZE;            //计算扇区地址，对于STM32F103VET6为0~255

    uint32_t sectorStartAddress=sectorPosition*SECTOR_SIZE+FLASH_BASE;    //对应扇区的首地址

    FLASH_ErasePage(sectorStartAddress);//擦除这个扇区

    uint16_t dataIndex;
    for(dataIndex=0; dataIndex<countToWrite; dataIndex++)
    {
        FLASH_ProgramByte(startAddress+dataIndex,writeData[dataIndex]);
    }
    FLASH_Lock();//上锁写保护
}


void flash_read(uint32_t read_addr, uint8_t *p_buffer, uint16_t num_read)
{
    uint16_t i;
    for(i = 0; i < num_read; i++)
    {
        p_buffer[i] = *(uint8_t *)(read_addr);
        read_addr += 1;
    }
}


OS_ERR timeerr;
uint8_t  buf[1024];			//1k读取
//uint8_t  readbuf[1024];			//1k读取
int main(void)
{
    uint32_t  applen = 0;
    uint8_t updatastate = 0;
    uint32_t wflashlen = 0;   //已经写了flash的长度
    uint8_t writesuccess = 0xaa;

    Spi_Flash_Io_Init();
    FLASH_Init();
    delay_init(144);   //  毫秒级延时  delay_m(500);
    Mcu_Io_Init();
    delay_m(1000);
    fal_partition_read(APP_CODE,0,&updatastate,sizeof(updatastate));
    fal_partition_read(APP_CODE,1,(uint8_t*)&applen,sizeof(uint32_t));
    if(updatastate != 0x55)
    {
        SystemReset();  		//程序直接复位，会直接跳转到以0x8000000  开始的程序的位置
//		JumpToProgramCode();   //使用跳转前，必须全部关闭中断
    }
    if((applen < 10*1024) || (applen > (200*1024 - 5)) )
    {
        SystemReset();  		//app程序小于10k，直接跳转到应用程序
    }

    while(1)
    {
        fal_partition_read(APP_CODE,5+wflashlen,(uint8_t*)&buf,sizeof(buf));

        FLASH_WriteMoreData(wflashlen + FALSE_W_BASE,buf,sizeof(buf));
        //flash_read(wflashlen + FALSE_W_BASE, readbuf, sizeof(buf));

        wflashlen += sizeof(buf);

        if(wflashlen + sizeof(buf) > applen)
        {
            if(wflashlen == applen)
            {
                break;
            }
            fal_partition_read(APP_CODE,5+wflashlen,(uint8_t*)&buf,applen - wflashlen);
            FLASH_WriteMoreData(wflashlen + FALSE_W_BASE,buf,applen - wflashlen);
            break;
        }
    }
    updatastate = 0xaa;
    fal_partition_write(APP_CODE,0,&updatastate,sizeof(updatastate));
    BUZZER_ON;
    delay_m(1000);
    BUZZER_OFF;
    SystemReset();
    //JumpToProgramCode();
    return 0;
}


