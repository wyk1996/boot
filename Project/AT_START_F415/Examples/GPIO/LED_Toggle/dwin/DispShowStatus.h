/*******************************************************************************
 *          Copyright (c) 2020-2050,  Co., Ltd.
 *                              All Right Reserved.
 * @file
 * @note
 * @brief
 *
 * @author
 * @date     2021-05-02
 * @version  V1.0.0
 *
 * @Description
 *
 * @note History:
 * @note     <author>   <time>    <version >   <desc>
 * @note
 * @warning
 *******************************************************************************/
#ifndef __DISPSTATUS_H_
#define __DISPSTATUS_H_

#include "stdint.h"
//#include "DwinProtocol.h"
#include "ch_port.h"

//��ʾ״��
typedef enum
{
    SHOW_START_APP,  		//APP����
    SHOW_START_CARD,		//ˢ������
    SHOW_BILL,				//�ѽ���
    SHOW_NOTBIL,			//δ����
    SHOW_STOP_ERR_NONE,		//����ֹͣ
    SHOW_STOP_FAIL,			//����ֹͣ
    SHOW_STOP_LESS,   //����ֹͣ
    SHOW_XY_HY,   //����
    SHOW_XY_AP,
    SHOW_XY_YKC,  //�ƿ��
    SHOW_XY_HFQG,
    SHOW_XY_YL1,
    SHOW_XY_YL2,
    SHOW_XY_YL3,
    SHOW_XY_YL4,
    SHOW_GUNA, //Aǹ
    SHOW_GUNB,	//Bǹ
    SHOW_MAX,
} _SHOW_NUM;


uint8_t Dis_ShowStatus(uint16_t add_show,_SHOW_NUM show_num);
uint8_t Dis_ShowCopy(uint8_t* pdata,_SHOW_NUM show_num);
uint8_t Dis_Showstop_reason(uint8_t* pdata,STOP_REASON Stop_reasonnum);
#endif
