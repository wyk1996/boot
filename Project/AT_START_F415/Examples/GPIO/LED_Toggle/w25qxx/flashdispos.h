/**
  **************************************************************************
  * @file     spi_flash.h
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    header file of spi_flash
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#ifndef __SPI_FLASH_DISPOSE_H
#define __SPI_FLASH_DISPOSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#define  CHARGE_RATE   	0
#define  DWIN_INFO   	1
#define  ONLINE_BILL    2
#define  OFFLINE_BILL   3
#define  OFFFSLINE_BILL 4
#define  RECORD_QUERY   5
#define  CARD_WL    	6
#define  APP_CODE    	7      //����  1  0x55 ��ʾ��Ҫ����  �� 0x55��ʾ��Ҫ������2 - 5  ��ʾ���򳤶�    6�Ժ�ȫ���ǳ���  

/**
  * @brief  spi configuration.
  * @param  none
  * @retval none
  */
int fal_partition_read(uint8_t flash_type,uint32_t offsetadd, uint8_t*pdata,uint32_t len);

/**
  * @brief  spi configuration.
  * @param  none
  * @retval none
  */
int fal_partition_write(uint8_t flash_type,uint32_t offsetadd, uint8_t*pdata,uint32_t len);

#endif

