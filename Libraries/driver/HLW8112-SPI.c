
/*=========================================================================================================
  * File Name	 : HLW8112-SPI.c
  * Describe 	 : HLW8112,ʹ��SPI��ͨѶ��ʽ
  * Author	   : Tuqiang
  * Version	   : V1.3
  * Record	   : 2019/04/16��V1.2
  * Record	   : 2020/04/02, V1.3	
==========================================================================================================*/
/* Includes ----------------------------------------------------------------------------------------------*/
#include "HLW8110.h"
#include "delay.h"


//extern unsigned char UART_EN;
//extern unsigned char SPI_EN;

#if HLW8112

#define HIGH	1
#define LOW		0

union IntData
{
	u16  inte;			
	u8 byte[2];		
};
union LongData
{
    u32  word;		
    u16  inte[2];		
    u8   byte[4];		
};


unsigned int    U16_TempData;	
unsigned int    U16_IFData;
unsigned int    U16_RIFData;

unsigned int 		U16_IEData;
unsigned int		U16_INTData;


unsigned int 		U16_SAGCYCData;
unsigned int 		U16_SAGLVLData;

unsigned int 		U16_SYSCONData;
unsigned int 		U16_EMUCONData;
unsigned int 		U16_EMUCON2Data;




unsigned int    U16_LineFData;
unsigned int    U16_AngleData;
unsigned int    U16_PFData;



//--------------------------------------------------------------------------------------------
unsigned int	U16_RMSIAC_RegData; 			// Aͨ������ת��ϵ��
unsigned int	U16_RMSIBC_RegData; 			// Bͨ������ת��ϵ��
unsigned int	U16_RMSUC_RegData; 				// ��ѹͨ��ת��ϵ��
unsigned int	U16_PowerPAC_RegData; 		// Aͨ������ת��ϵ��
unsigned int	U16_PowerPBC_RegData; 		// Bͨ������ת��ϵ��
unsigned int	U16_PowerSC_RegData; 			// ���ڹ���ת��ϵ��,���ѡ��Aͨ��������Aͨ�����ڹ���ת��ϵ����A��Bͨ��ֻ�ܶ���ѡ��һ
unsigned int	U16_EnergyAC_RegData; 		// Aͨ���й�����(��)ת��ϵ�� 
unsigned int	U16_EnergyBC_RegData; 		// Aͨ���й�����(��)ת��ϵ��
unsigned int	U16_CheckSUM_RegData; 		// ת��ϵ����CheckSum
unsigned int	U16_CheckSUM_Data; 				// ת��ϵ�����������CheckSum

unsigned int	U16_Check_SysconReg_Data; 						
unsigned int	U16_Check_EmuconReg_Data; 						
unsigned int	U16_Check_Emucon2Reg_Data; 			
			
//--------------------------------------------------------------------------------------------
unsigned long 	U32_RMSIA_RegData;			//Aͨ�������Ĵ���ֵ
unsigned long 	U32_RMSU_RegData;			//��ѹ�Ĵ���ֵ
unsigned long 	U32_POWERPA_RegData;		//Aͨ�����ʼĴ���ֵ
unsigned long 	U32_ENERGY_PA_RegData;		//Aͨ���й����ܣ������Ĵ���ֵ


unsigned long 	U32_RMSIB_RegData;			//Bͨ�������Ĵ���ֵ
unsigned long		U32_POWERPB_RegData;		//Bͨ�����ʼĴ���ֵ
unsigned long 	U32_ENERGY_PB_RegData;	    //Bͨ���й����ܣ������Ĵ���ֵ
//--------------------------------------------------------------------------------------------

float   F_AC_V;						//��ѹֵ��Чֵ
float   F_AC_I;						//Aͨ��������Чֵ
float   F_AC_P;						//Aͨ���й�����
float   F_AC_E;						//Aͨ���й�����(��)
float   F_AC_BACKUP_E;		//Aͨ����������	
float   F_AC_PF;						//�������أ�Aͨ����Bͨ��ֻ��ѡ��һ 
float		F_Angle;


float   F_AC_I_B;						//Bͨ��������Чֵ
float   F_AC_P_B;						//Bͨ���й�����
float 	F_AC_E_B;						//Bͨ���й�����(��)
float   F_AC_BACKUP_E_B;		//Bͨ����������	

float   F_AC_LINE_Freq;     //�е�����Ƶ��
float   F_IF_RegData;     	//IF�Ĵ���ֵ
//--------------------------------------------------------------------------------------------


/*=====================================================
 * Function : void HLW8112_SPI_WriteByte(unsigned char u8_data)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void HLW8112_SPI_WriteByte(unsigned char u8_data)
{
	unsigned char i;
	unsigned char x;
	x = u8_data;
	for(i = 0;i<8;i++)
		{
						
			if(x&(0x80>>i))
				IO_HLW8112_SDI = HIGH;
			else
				IO_HLW8112_SDI = LOW;

	
			IO_HLW8112_SCLK = HIGH;
                        delay_u(10);
			IO_HLW8112_SCLK = LOW;
                        delay_u(10);

		}
}
/*=====================================================
 * Function : unsigned char HLW8112_SPI_ReadByte(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
unsigned char HLW8112_SPI_ReadByte(void)
{
  unsigned char i;
  unsigned char u8_data;
  u8_data = 0x00;
  for(i = 0;i<8;i++)
  {
	u8_data <<= 1;
	
    IO_HLW8112_SCLK = HIGH;
    delay_u(10);
    IO_HLW8112_SCLK = LOW;
    delay_u(10);

    if (IO_HLW8112_SDO == HIGH)
       u8_data |= 0x01;
	   
  }
  
  return u8_data;
        
}

/*=====================================================
 * Function : void HLW8112_SPI_WriteCmd(unsigned char u8_cmd)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void HLW8112_SPI_ReadReg(unsigned char u8_RegAddr)
{

    HLW8112_SPI_WriteByte(u8_RegAddr);
}

/*=====================================================
 * Function : void HLW8112_SPI_WriteReg(unsigned char u8_RegAddr)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void HLW8112_SPI_WriteReg(unsigned char u8_RegAddr)
{
    unsigned char u8_tempdata;
    u8_tempdata = (u8_RegAddr | 0x80);
    HLW8112_SPI_WriteByte(u8_tempdata);
}



/*=====================================================
 * Function : void HLW8112_WriteREG_EN(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void HLW8112_WriteREG_EN(void)
{
	//��дHLW8112 Reg����
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteByte(0xea);
  HLW8112_SPI_WriteByte(0xe5);
  IO_HLW8112_CS = HIGH;
}
/*=====================================================
 * Function : void HLW8112_WriteREG_DIS(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void HLW8112_WriteREG_DIS(void)
{
	//�ر�дHLW8112 Reg����
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteByte(0xea);
  HLW8112_SPI_WriteByte(0xdc);
  IO_HLW8112_CS = HIGH;
}

/*=====================================================
 * Function : void Write_HLW8112_RegData(unsigned char ADDR_Reg,unsigned char Data_length,unsigned long u32_data)
 * Describe : д�Ĵ���
 * Input    : ADDR_Reg,Data_length��u32_data
 * Output   : 
 * Return   : 
 * Record   : 2018/05/09
=====================================================*/
void Write_HLW8112_RegData(unsigned char ADDR_Reg,unsigned char u8_reg_length,unsigned long u32_data)
{
  unsigned char i;
  union LongData u32_t_data;
  union LongData u32_p_data;
  
  u32_p_data.word = u32_data;
  u32_t_data.word = 0x00000000;
  
  for (i = 0; i < u8_reg_length; i++ )
  	{
  		u32_t_data.byte[i] = u32_p_data.byte[i];	//д��REGʱ����Ҫע��MCU��������(union)���壬�ֽ��Ǹ�λ��bytep[0]��������byte[3]
 
  	}
    
  HLW8112_SPI_WriteReg(ADDR_Reg);
  for (i = 0; i < u8_reg_length; i++ )
  	{
  		HLW8112_SPI_WriteByte(u32_t_data.byte[i]);
  	}

  
}
/*=====================================================
 * Function : unsigned long Read_HLW8112_RegData(unsigned char ADDR_Reg,unsigned char u8_reg_length)
 * Describe : ��ȡ�Ĵ���ֵ
 * Input    : ADDR_Reg,u8_Data_length
 * Output   : u8_Buf[4],�Ĵ���ֵ
 * Return   : u1(True or False)
 * Record   : 2018/05/09
=====================================================*/
unsigned long Read_HLW8112_RegData(unsigned char ADDR_Reg,unsigned char u8_reg_length)
{
  unsigned char i;
  union LongData u32_t_data1;

  
  
  IO_HLW8112_CS = LOW;
  u32_t_data1.word = 0x00000000;

//��λ��ǰ	
	HLW8112_SPI_ReadReg(ADDR_Reg);
	for (i = 0; i<u8_reg_length; i++ )
  	{
  		u32_t_data1.byte[u8_reg_length-1-i] = HLW8112_SPI_ReadByte();
  		
  	}	
	
  
   IO_HLW8112_CS = HIGH;
   
  return u32_t_data1.word;
}

/*=====================================================
 * Function : void Judge_CheckSum_HLW8112_Calfactor(void)
 * Describe : ��֤��ַ0x70-0x77��ַ��ϵ����
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
=====================================================*/
unsigned char Judge_CheckSum_HLW8112_Calfactor(void)
{
  unsigned long a;
  unsigned char d;
 
  //��ȡRmsIAC��RmsIBC��RmsUC��PowerPAC��PowerPBC��PowerSC��EnergAc��EnergBc��ֵ

  U16_RMSIAC_RegData = Read_HLW8112_RegData(REG_RMS_IAC_ADDR,2);
  U16_RMSIBC_RegData = Read_HLW8112_RegData(REG_RMS_IBC_ADDR,2);
  U16_RMSUC_RegData = Read_HLW8112_RegData(REG_RMS_UC_ADDR,2);
  U16_PowerPAC_RegData = Read_HLW8112_RegData(REG_POWER_PAC_ADDR,2);
  U16_PowerPBC_RegData = Read_HLW8112_RegData(REG_POWER_PBC_ADDR,2);
  U16_PowerSC_RegData = Read_HLW8112_RegData(REG_POWER_SC_ADDR,2);
  U16_EnergyAC_RegData = Read_HLW8112_RegData(REG_ENERGY_AC_ADDR,2);
  U16_EnergyBC_RegData = Read_HLW8112_RegData(REG_ENERGY_BC_ADDR,2);
 
  U16_CheckSUM_RegData = Read_HLW8112_RegData(REG_CHECKSUM_ADDR,2);
  
  a = 0;
  
  a = ~(0xffff+U16_RMSIAC_RegData + U16_RMSIBC_RegData + U16_RMSUC_RegData + 
        U16_PowerPAC_RegData + U16_PowerPBC_RegData + U16_PowerSC_RegData + 
          U16_EnergyAC_RegData + U16_EnergyBC_RegData  );
  
  U16_CheckSUM_Data = a & 0xffff;
  
  
  
  printf("ת��ϵ���Ĵ���\r\n");
	printf("U16_RMSIAC_RegData = %x\n " ,U16_RMSIAC_RegData);
	printf("U16_RMSIBC_RegData = %x\n " ,U16_RMSIBC_RegData);
	printf("U16_RMSUC_RegData = %x\n " ,U16_RMSUC_RegData);
	printf("U16_PowerPAC_RegData = %x\n " ,U16_PowerPAC_RegData);
	printf("U16_PowerPBC_RegData = %x\n " ,U16_PowerPBC_RegData);
	printf("U16_PowerSC_RegData = %x\n " ,U16_PowerSC_RegData);
	printf("U16_EnergyAC_RegData = %x\n " ,U16_EnergyAC_RegData);
	printf("U16_EnergyBC_RegData = %x\n " ,U16_EnergyBC_RegData);
	
	printf("U16_CheckSUM_RegData = %x\n " ,U16_CheckSUM_RegData);
	printf("U16_CheckSUM_Data = %x\n " ,U16_CheckSUM_Data);
  
  if ( U16_CheckSUM_Data == U16_CheckSUM_RegData)
  {
    d = 1;
    printf("ϵ���Ĵ���У��ok\r\n");
    
  }
  else
  {
    d = 0;
    printf("ϵ���Ĵ���У��fail\r\n");
  }
  
  return d; 
}


/*=====================================================
 * Function : void Set_OIALVL(void)
 * Describe : ������ѹ��ֵ������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2020/12/04
=====================================================*/
void Set_OIALVL(void)
{
    unsigned int a;
  
  //���÷���,����100mA����,OIALVL = 0xb1
  
  
  HLW8112_WriteREG_EN();	//��д8112 Reg
  
  //�򿪹�ѹ�������ȹ���
/*  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(0x0f);          //�����Ĵ�����������
  HLW8112_SPI_WriteByte(0xff);
  IO_HLW8112_CS = HIGH;
*/
  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_OIALVL_ADDR);
  HLW8112_SPI_WriteByte(0x00);
  HLW8112_SPI_WriteByte(0xb1);
  IO_HLW8112_CS = HIGH;
  U16_TempData = Read_HLW8112_RegData(REG_OIALVL_ADDR,2);

  
  //����INT�Ĵ���, ����ͨ�����������INT = 32E9��INT2������� ,INT1�������
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_INT_ADDR);
  HLW8112_SPI_WriteByte(0x32);        
  HLW8112_SPI_WriteByte(0xE9);               
  IO_HLW8112_CS = HIGH;
  
  
  //����IE�Ĵ���, IE
  a = Read_HLW8112_RegData(REG_IE_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_IE_ADDR);
  HLW8112_SPI_WriteByte((a>>8)&0xff); //    
  HLW8112_SPI_WriteByte(a|0x80); 			//����A����ʹ�� OIAIE = 1; 
 // HLW8112_SPI_WriteByte(a&0xff);               
  IO_HLW8112_CS = HIGH;
  
  U16_TempData = Read_HLW8112_RegData(REG_IE_ADDR,2);
  
  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg
  
  
  
 
}

/*=====================================================
 * Function : void Set_OVLVL(void)
 * Describe : ��ѹ��ѹ��ֵ������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
=====================================================*/
void Set_OVLVL(void)
{
    unsigned int a;
  
  //���÷���,0x5b21,����210V��ѹ,OVLVL = 0x5a8b
  
  
  HLW8112_WriteREG_EN();	//��д8112 Reg
  
  //�򿪹�ѹ�������ȹ���
/*  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(0x0f);          //�����Ĵ�����������
  HLW8112_SPI_WriteByte(0xff);
  IO_HLW8112_CS = HIGH;
*/
  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_OVLVL_ADDR);
  HLW8112_SPI_WriteByte(0x5a);
  HLW8112_SPI_WriteByte(0x8b);
  IO_HLW8112_CS = HIGH;
   U16_TempData = Read_HLW8112_RegData(REG_OVLVL_ADDR,2);

  
  //����INT�Ĵ���, ��ѹͨ�����������INT = 3219��INT2��ѹ��� 
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_INT_ADDR);
  HLW8112_SPI_WriteByte(0x32);        
  HLW8112_SPI_WriteByte(0xc9);               
  IO_HLW8112_CS = HIGH;
  
  
  //����IE�Ĵ���, IE
  a = Read_HLW8112_RegData(REG_IE_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_IE_ADDR);
  HLW8112_SPI_WriteByte((a>>8)|0x02); //��ѹ��ѹ�ж�ʹ�ܣ�OVIE= 1      
 // HLW8112_SPI_WriteByte(0x02); //��ѹ��ѹ�ж�ʹ�ܣ�OVIE= 1   
  HLW8112_SPI_WriteByte(a&0xff);               
  IO_HLW8112_CS = HIGH;
  
  U16_TempData = Read_HLW8112_RegData(REG_IE_ADDR,2);
  
  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg
  
  
  
 
}


/*=====================================================
 * Function : void Set_underVoltage(void)
 * Describe : ��ѹǷѹ��ֵ������INT2���
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2020/08/13
=====================================================*/
void Set_underVoltage(void)
{
    unsigned int a;
  
  //���÷���,0x5b21,����210V��ѹ,OVLVL = 0x5a8b
  
  
  HLW8112_WriteREG_EN();	//��д8112 Reg
  
/*	
	IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_SAGCYC_ADDR);		//Ƿѹ����
  HLW8112_SPI_WriteByte(0x00);
//  HLW8112_SPI_WriteByte(0x05);
	HLW8112_SPI_WriteByte(0x01);
  IO_HLW8112_CS = HIGH;
   U16_TempData = Read_HLW8112_RegData(REG_SAGCYC_ADDR,2);
*/
  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_SAGLVL_ADDR);		//Ƿѹ����180V
  HLW8112_SPI_WriteByte(0x4E);
  HLW8112_SPI_WriteByte(0x1C);
	
//	HLW8112_SPI_WriteByte(0x6C);		//Ƿѹ����280V
//	HLW8112_SPI_WriteByte(0x7C);
	
	
  IO_HLW8112_CS = HIGH;
   U16_TempData = Read_HLW8112_RegData(REG_OVLVL_ADDR,2);

  
  //����INT�Ĵ���, ��ѹͨ�����������INT = 3219��INT2Ƿѹ��� 
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_INT_ADDR);
  HLW8112_SPI_WriteByte(0x32);        
  HLW8112_SPI_WriteByte(0xD9);               
  IO_HLW8112_CS = HIGH;
  
  
  //����IE�Ĵ���, IE
  a = Read_HLW8112_RegData(REG_IE_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_IE_ADDR);
  HLW8112_SPI_WriteByte((a>>8)|0x08); //��ѹǷѹ�ж�ʹ�ܣ�OVIE= 1      
 // HLW8112_SPI_WriteByte(0x02); //��ѹ��ѹ�ж�ʹ�ܣ�OVIE= 1   
  HLW8112_SPI_WriteByte(a&0xff);               
  IO_HLW8112_CS = HIGH;
  
  U16_TempData = Read_HLW8112_RegData(REG_IE_ADDR,2);
  
  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg
  
  
  
 
}


/*=====================================================
 * Function : void Set_V_Zero(void)
 * Describe : ��ѹͨ����������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
=====================================================*/
void Set_V_Zero(void)
{
  unsigned int a;
  
  HLW8112_WriteREG_EN();	         //��д8112 Reg

  //����EMUCON�Ĵ���,REG_EMUCON_ADDR = REG_EMUCON_ADDR | 0x0180
  a = Read_HLW8112_RegData(REG_EMUCON_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON_ADDR);
  HLW8112_SPI_WriteByte((a>>8)|0x01);          
  HLW8112_SPI_WriteByte((a&0xff)|0x80); // ����͸�������������仯��ZXD0 = 1��ZXD1 = 1
  IO_HLW8112_CS = HIGH;
  
  
  //����EMUCON2�Ĵ���, REG_EMUCON2_ADDR = REG_EMUCON2_ADDR | 0x0024
  a = Read_HLW8112_RegData(REG_EMUCON2_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(a>>8);          
  HLW8112_SPI_WriteByte((a&0xff)|0x24); // ZxEN = 1,WaveEn = 1;
  IO_HLW8112_CS = HIGH;
    
  

  //����IE�Ĵ���
  a = Read_HLW8112_RegData(REG_IE_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_IE_ADDR);
  HLW8112_SPI_WriteByte((a>>8)|0x40); //��ѹ�����ж�ʹ�ܣ�ZX_UIE = 1         
  HLW8112_SPI_WriteByte(a&0xff);               
  IO_HLW8112_CS = HIGH;
  
  //����INT�Ĵ���, ��ѹͨ�����������INT = 3219,INT1�����ѹ����
  //a = Read_HLW8112_RegData(REG_IE_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_INT_ADDR);
  HLW8112_SPI_WriteByte(0x32);        
  HLW8112_SPI_WriteByte(0x19);               
  IO_HLW8112_CS = HIGH;
  
  
  
  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg
}

/*=====================================================
 * Function : void Set_Leakage(void)
 * Describe : ����Bͨ��©���⹦�ܣ�ֻ���ñȽ�������,INT2���
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
=====================================================*/
void Set_Leakage(void)
{
  unsigned int a;
  
   HLW8112_WriteREG_EN();	//��д8112 Reg
  

  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_SYSCON_ADDR);
  HLW8112_SPI_WriteByte(0x0a);          //-------------��8bit���ر�ADC����ͨ��B
  HLW8112_SPI_WriteByte(0x04);          //-------------��8bit��
  IO_HLW8112_CS = HIGH;

  //����comp_off = 1,��Bͨ���Ƚ���
  a = Read_HLW8112_RegData(REG_EMUCON_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON_ADDR);  
  HLW8112_SPI_WriteByte((a>>8)&0xcf);   //   �򿪱Ƚ���    
  HLW8112_SPI_WriteByte(a&0xfd);  
  IO_HLW8112_CS = HIGH;
  
  
  //����INT�Ĵ���, INT2�Ƚ���©�����
  a = Read_HLW8112_RegData(REG_INT_ADDR,2); 
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_INT_ADDR);
  HLW8112_SPI_WriteByte((a>>8));        
  HLW8112_SPI_WriteByte(0x29);               
  IO_HLW8112_CS = HIGH;
  
  
  //����IE�Ĵ���, IE
  a = Read_HLW8112_RegData(REG_IE_ADDR,2);  
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_IE_ADDR);
  HLW8112_SPI_WriteByte((a>>8)|0x80); //©���ж�ʹ�ܣ�LeakageIE= 1        
  HLW8112_SPI_WriteByte(a&0xff);               
  IO_HLW8112_CS = HIGH;
  
  U16_TempData = Read_HLW8112_RegData(REG_IE_ADDR,2);
  
  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg
}


/*=====================================================
 * Function : void Init_HLW8112(void)
 * Describe : ��ʼ��8112
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void Init_HLW8112(void)
{
	GPIO_InitType GPIO_InitStructure;
	
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
			delay_init(200);	  			//��ʱ��ʼ��
  //SPI init
  delay_m(100);
	
	
//STM32 IO init
	RCC_APB2PeriphClockCmd(	RCC_APB2PERIPH_GPIOB, ENABLE );
	   	 
	  
	  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_7 | GPIO_Pins_8 | GPIO_Pins_9;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
    

    // Port A output
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6; 
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pins_6);
	
	/*GPIOB->CTRLL&=0X00FFFFFF;  //IO״̬����,����PB7��PB6
	GPIOB->CTRLH&=0XFFFFFF00;  //IO״̬����,����PB8��PB9
	GPIOB->CTRLL|=0X38000000;  //IO״̬���ã�PB7��PB8��PB9-�����PB6-����	 
	GPIOB->CTRLL|=0X00000033;  //IO״̬���ã�PB7��PB8��PB9-�����PB6-����	 
    */
//	GPIOD->CRL&=0XFFF00FFF;  //IO״̬����,����PD11��PD12
//	GPIOD->CRL|=0X00088000;  //IO״̬���ã�PD11��PD12-����	 
	
//STM32 IO init
  
//  IO_HLW8112_EN = 1;             //ʹ��һֱ������״̬ ���ԾͲ�������
  delay_m(10);
  IO_HLW8112_CS = HIGH;
  delay_u(2);
  delay_u(2);

  IO_HLW8112_SCLK = LOW;
  delay_u(2);
  IO_HLW8112_SDI = LOW;
  HLW8112_WriteREG_EN();	//��д8112 Reg
// ea + 5a ����ͨ��A�������ָ����ǰ���ڼ������ڹ��ʡ�������������ǡ�
//˲ʱ�й����ʡ�˲ʱ���ڹ��ʺ��й����ʹ��ص��ź�ָʾ ��ͨ��Ϊͨ��A 
// ea + a5 ����ͨ��A�������ָ����ǰ���ڼ������ڹ��ʡ�������������ǡ�
//˲ʱ�й����ʡ�˲ʱ���ڹ��ʺ��й����ʹ��ص��ź�ָʾ ��ͨ��Ϊͨ��A 
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteByte(0xea);
  HLW8112_SPI_WriteByte(0x5a);
  IO_HLW8112_CS = HIGH;
  
//дSYSCON REG, �رյ���ͨ��B����ѹͨ�� PGA = 1������ͨ��A PGA =  16;
//��·ͨ������ֵ�����ֵ800ma,��Чֵ/1.414 = 565mV��
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_SYSCON_ADDR);
//HLW8112_SPI_WriteByte(0x0a);         //��������ͨ��A,PGA = 16   ------��8bit
  HLW8112_SPI_WriteByte(0x0f);          //��������ͨ��A�͵���ͨ��B PGA =  16;��Ҫ��EMUCON�йرձȽ���------��8bit
  HLW8112_SPI_WriteByte(0x04);          //-------------��8bit
  IO_HLW8112_CS = HIGH;
 


//дEMUCON REG, ʹ��Aͨ��PF����������й��ܵ��ܼĴ����ۼ�
   IO_HLW8112_CS = LOW;
   HLW8112_SPI_WriteReg(REG_EMUCON_ADDR);
    HLW8112_SPI_WriteByte(0x10);        //���λһ�����ó�0001���رձȽ���,��Bͨ��  
    HLW8112_SPI_WriteByte(0x03);        //��Aͨ����Bͨ�������ۼƹ���
    IO_HLW8112_CS = HIGH;
  
//дEMUCON2 REG, 3.4HZ���������ѡ���ڲ���׼,�򿪹�ѹ�������ȹ���
//3.4HZ(300ms) 6.8HZ(150ms) 13.65HZ(75ms) 27.3HZ(37.5ms)
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(0x0f);          //�����Ĵ�����������
  HLW8112_SPI_WriteByte(0xff);
  IO_HLW8112_CS = HIGH;
	
/*	delay_m(10);
Write_HLW8112_RegData(REG_RMS_UC_ADDR,2,0xa4c3);   //????Ϊʲôд���ɹ�
  delay_m(10);*/
  
//�ر������ж�
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_IE_ADDR);
  HLW8112_SPI_WriteByte(0x00);                          
  HLW8112_SPI_WriteByte(0x00);               
  IO_HLW8112_CS = HIGH;

  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg




//��ȡRmsIAC��RmsIBC��RmsUC��PowerPAC��PowerPBC��PowerSC��EnergAc��EnergBc��ֵ
  U16_RMSIAC_RegData = Read_HLW8112_RegData(REG_RMS_IAC_ADDR,2);        //0xcad1
  U16_RMSIBC_RegData = Read_HLW8112_RegData(REG_RMS_IBC_ADDR,2);        //0xcacf
  U16_RMSUC_RegData = Read_HLW8112_RegData(REG_RMS_UC_ADDR,2);          //0xa4c3
  U16_PowerPAC_RegData = Read_HLW8112_RegData(REG_POWER_PAC_ADDR,2);    //0xab47
  U16_PowerPBC_RegData = Read_HLW8112_RegData(REG_POWER_PBC_ADDR,2);    //0xab49
  U16_PowerSC_RegData = Read_HLW8112_RegData(REG_POWER_SC_ADDR,2);      //0xab41
  U16_EnergyAC_RegData = Read_HLW8112_RegData(REG_ENERGY_AC_ADDR,2);    //0xe9cc
  U16_EnergyBC_RegData = Read_HLW8112_RegData(REG_ENERGY_BC_ADDR,2);    //0xe9ce
 //printf("U16_RMSUC_RegData = %x\r\n " ,U16_RMSUC_RegData);
 Judge_CheckSum_HLW8112_Calfactor();

  
  Set_V_Zero();         //����INT1
  
// Set_OVLVL();          //����INT2
 Set_underVoltage();
  
//  Set_Leakage();        //����INT2����Bͨ��B����


}

/*=========================================================================================================
 * Function : void Check_WriteReg_Success(void)
 * Describe : ����д���REG�Ƿ���ȷд��
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2020/04/02
==========================================================================================================*/
void Check_WriteReg_Success(void)
{
	U16_Check_SysconReg_Data = Read_HLW8112_RegData(REG_SYSCON_ADDR,2);
	U16_Check_EmuconReg_Data = Read_HLW8112_RegData(REG_EMUCON_ADDR,2); 
	U16_Check_Emucon2Reg_Data = Read_HLW8112_RegData(REG_EMUCON2_ADDR,2); 
	
	printf("д���SysconReg�Ĵ���:%lx\n " ,U16_Check_SysconReg_Data); 
	printf("д���EmuconReg�Ĵ���:%lx\n " ,U16_Check_EmuconReg_Data); 
	printf("д���Emucon2Reg�Ĵ����Ĵ���:%lx\n " ,U16_Check_Emucon2Reg_Data);      
	
}



/*=====================================================
 * Function : void Read_HLW8112_PA_I(void)
 * Describe : ��ȡAͨ������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void Read_HLW8112_PA_I(void)
{ 
   float a;
   //���㹫ʽ,U16_AC_I = (U32_RMSIA_RegData * U16_RMSIAC_RegData)/(����ϵ��* 2^23��
   U32_RMSIA_RegData = Read_HLW8112_RegData(REG_RMSIA_ADDR,3);
   
	 if ((U32_RMSIA_RegData &0x800000) == 0x800000)
	 {
			F_AC_I = 0;
	 }
	 else
	 {
		a = (float)U32_RMSIA_RegData;
		a = a * U16_RMSIAC_RegData;
		a  = a/0x800000;                     //������������ĸ�������λ��mA,����5003.12 
		a = a/1;  								// 1 = ����ϵ��
		a = a/1000;              //a= 5003ma,a/1000 = 5.003A,��λת����A
		a = a * D_CAL_A_I;				//D_CAL_A_I��У��ϵ����Ĭ����1
		F_AC_I = a;
	 }
    
}


/*=====================================================
 * Function : void Read_HLW8112_PB_I(void)
 * Describe : ��ȡBͨ������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/28
=====================================================*/
void Read_HLW8112_PB_I(void)
{ 
   float a;
	
   //����,U16_AC_I_B = (U32_RMSIB_RegData * U16_RMSIBC_RegData)/2^23
   U32_RMSIB_RegData = Read_HLW8112_RegData(REG_RMSIB_ADDR,3);
   if ((U32_RMSIB_RegData &0x800000) == 0x800000)
	 {
			F_AC_I_B = 0;
	 }
	 else
	 {
   a = (float)U32_RMSIB_RegData;
   a = a * U16_RMSIBC_RegData;
   a  = a/0x800000;                     //������������ĸ�������λ��mA,����5003.12
   a = a/1;  				// 1 = ����ϵ��
	a = a/1000;								//a= 5003ma,a/1000 = 5.003A,��λת����A
   a = a * D_CAL_B_I;									//D_CAL_B_I��У��ϵ����Ĭ����1
   F_AC_I_B = a;
	 }
    
}

/*=====================================================
 * Function : void Read_HLW8112_U(void)
 * Describe : ��ȡ��ѹ
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void Read_HLW8112_U(void)
{
   float a;
   //��ȡ��ѹ�Ĵ���ֵ
   U32_RMSU_RegData = Read_HLW8112_RegData(REG_RMSU_ADDR,3);
  
	//����:U16_AC_V = (U32_RMSU_RegData * U16_RMSUC_RegData)/2^23
	
	 if ((U32_RMSU_RegData &0x800000) == 0x800000)
	 {
			F_AC_V = 0;
	 }
  else
	{
	a =  (float)U32_RMSU_RegData;
	a = a*U16_RMSUC_RegData;  
	a = a/0x400000;     
	a = a/1;  						// 1 = ��ѹϵ��
	a = a/100; 				 		//�����a = 22083.12mV,a/100��ʾ220.8312V����ѹת����V
	a = a*D_CAL_U;				//D_CAL_U��У��ϵ����Ĭ����1		
	F_AC_V = a;
	}
   
}


/*=====================================================
 * Function : void Read_HLW8112_PA(void)
 * Describe : ��ȡAͨ������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void Read_HLW8112_PA(void)
{
     float a;
	unsigned long b;

   //��ȡ���ʼĴ���ֵ
    U32_POWERPA_RegData = Read_HLW8112_RegData(REG_POWER_PA_ADDR,4);
  
  //����,U16_AC_P = (U32_POWERPA_RegData * U16_PowerPAC_RegData)/(2^31*��ѹϵ��*����ϵ��)
	//��λΪW,���������5000.123����ʾ5000.123W


   
   if (U32_POWERPA_RegData > 0x80000000)
   {
     b = ~U32_POWERPA_RegData;
     a = (float)b;
   }
   else
     a =  (float)U32_POWERPA_RegData;
     
   
    a = a*U16_PowerPAC_RegData;
    a = a/0x80000000;            
    a = a/1;  												// 1 = ����ϵ��
    a = a/1;  												// 1 = ��ѹϵ��
   	a = a * D_CAL_A_P;								//D_CAL_A_P��У��ϵ����Ĭ����1   
    F_AC_P = a;									 			//��λΪW,���������5000.123����ʾ5000.123W 
}
/*=====================================================
 * Function : void Read_HLW8112_PB(void)
 * Describe : ��ȡBͨ������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/28
=====================================================*/

void Read_HLW8112_PB(void)
{
     float a;
	unsigned long b;


   //��ȡ���ʼĴ���ֵ
    U32_POWERPB_RegData = Read_HLW8112_RegData(REG_POWER_PB_ADDR,4);
  
  //����,U16_AC_P_B = (U32_POWERPB_RegData * U16_PowerPBC_RegData)/(1000*2^31)
   
   if (U32_POWERPB_RegData > 0x80000000)
   {
     b = ~U32_POWERPB_RegData;
     a = (float)b;
   }
   else
   {
     a =  (float)U32_POWERPB_RegData;
   }
   
    a = a*U16_PowerPBC_RegData;
    a = a/0x80000000;    
    a = a/1;  			    // 1 = ����ϵ��
    a = a/1;  				// 1 = ��ѹϵ��  
		a = a * D_CAL_B_P;						//D_CAL_A_P��У��ϵ����Ĭ����1  
    F_AC_P_B = a; 
}


/*=====================================================
 * Function : void void Read_HLW8112_EA(void)
 * Describe : ��ȡAͨ���й�����
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/09
=====================================================*/
void Read_HLW8112_EA(void)
{
//��Ϊ�������ݲ����棬����ϵͳ�ϵ��Ӧ��������EEPROM�ڵĵ������ݣ��ܵ��� = EEPROM�ڵ�������+�˴μ���ĵ���	
  float a;

   //��ȡ���ʼĴ���ֵ
  U32_ENERGY_PA_RegData = Read_HLW8112_RegData(REG_ENERGY_PA_ADDR,3);
  
	//��������,���� = (U32_ENERGY_PA_RegData * U16_EnergyAC_RegData * HFCONST) /(K1*K2 * 2^29 * 4096)
	//HFCONST:Ĭ��ֵ��0x1000, HFCONST/(2^29 * 4096) = 0x20000000
  a =  (float)U32_ENERGY_PA_RegData;	
  
  a = a*U16_EnergyAC_RegData;
  a = a/0x20000000;             //������λ��0.001KWH,�����������2.002,��ʾ2.002KWH
//��ΪK1��K2����1������a/(K1*K2) = a      
   a = a/1;  					// 1 = ����ϵ��
   a = a/1;  				// 1 = ��ѹϵ��
	a = a * D_CAL_A_E;     				//D_CAL_A_E��У��ϵ����Ĭ����1
  F_AC_E = a;
  
  
  if (F_AC_E >= 1)    //ÿ����1�ȵ������
  {
    F_AC_BACKUP_E += F_AC_E;
    
    
  //IO_HLW8112_SDI = LOW;
  HLW8112_WriteREG_EN();	//��д8112 Reg
  
  //���� REG_ENERGY_PA_ADDR�Ĵ���
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(0x0b);          //�����Ĵ�����������
  HLW8112_SPI_WriteByte(0xff);
  IO_HLW8112_CS = HIGH;  
  
  
  U32_ENERGY_PA_RegData = Read_HLW8112_RegData(REG_ENERGY_PA_ADDR,3);   //��������
  U32_ENERGY_PA_RegData = Read_HLW8112_RegData(REG_ENERGY_PA_ADDR,3);   //��������
  F_AC_E = 0;
  //ÿ����0.001�ȵ������,Ȼ�������ö�������
   
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(0x0f);          ////�����Ĵ���B��������
  HLW8112_SPI_WriteByte(0xff);
  IO_HLW8112_CS = HIGH;
  
  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg
  
  
  }
}

/*=====================================================
 * Function : void void Read_HLW8112_EB(void)
 * Describe : ��ȡBͨ���й�����
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/28
=====================================================*/

void Read_HLW8112_EB(void)
{
//��Ϊ�������ݲ����棬����ϵͳ�ϵ��Ӧ��������EEPROM�ڵĵ������ݣ��ܵ��� = EEPROM�ڵ�������+�˴μ���ĵ���	
  float a;

   //��ȡ���ʼĴ���ֵ
  U32_ENERGY_PB_RegData = Read_HLW8112_RegData(REG_ENERGY_PB_ADDR,3);
  
	//��������,���� = (U32_ENERGY_PA_RegData * U16_EnergyAC_RegData * HFCONST) /(K1*K2 * 2^29 * 4096)
	//HFCONST:Ĭ��ֵ��0x1000, HFCONST/(2^29 * 4096) = 0x20000000
  a =  (float)U32_ENERGY_PB_RegData;	
  a = a*U16_EnergyBC_RegData;
 //HFConst(Ĭ��ֵ1000H = 2^12),0x1000/(2^29*2^12) = 0x20000000
  a = a/0x20000000;             //������λ��0.001KWH,�����������2.002,��ʾ2.002KWH 
	//��ΪK1��K2����1������a/(K1*K2) = a 
  a = a/1;  										// 1 = ����ϵ��,ϵ��������Բο�����
  a = a/1;  										// 1 = ��ѹϵ��,ϵ��������Բο�����
	a = a * D_CAL_B_E;     				//D_CAL_B_E��У��ϵ������У׼Ӧ��Ĭ����1
  F_AC_E_B = a;
  
  
  if (F_AC_E_B >= 1)    		//ÿ����1�ȵ������
  {
    F_AC_BACKUP_E_B += F_AC_E_B;
    
    
  //IO_HLW8112_SDI = LOW;
  HLW8112_WriteREG_EN();	//��д8112 Reg
  
  //���� REG_ENERGY_PB_ADDR�Ĵ���
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(0x07);          //�����Ĵ���B��������,A������
  HLW8112_SPI_WriteByte(0xff);
  IO_HLW8112_CS = HIGH;  
  
  
  U32_ENERGY_PB_RegData = Read_HLW8112_RegData(REG_ENERGY_PB_ADDR,3);   //��������
  U32_ENERGY_PB_RegData = Read_HLW8112_RegData(REG_ENERGY_PB_ADDR,3);   //��������
  F_AC_E_B = 0;
  //ÿ����0.001�ȵ������,Ȼ�������ö�������
   
  IO_HLW8112_CS = LOW;
  HLW8112_SPI_WriteReg(REG_EMUCON2_ADDR);
  HLW8112_SPI_WriteByte(0x0f);          //�����Ĵ�����������
  HLW8112_SPI_WriteByte(0xff);
  IO_HLW8112_CS = HIGH;
  
  HLW8112_WriteREG_DIS();	//�ر�д8112 Reg
  
  
  }
}


/*=====================================================
 * Function : void Read_HLW8112_Linefreq(void)
 * Describe : ��ȡ�е�����Ƶ��50HZ,60HZ,������ʹ�õ������þ���
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
=====================================================*/
void Read_HLW8112_Linefreq(void)
{
  float a;
  unsigned long b;
  b = Read_HLW8112_RegData(REG_UFREQ_ADDR,2);
  U16_LineFData = b;
  a = (float)b;
  a = 3579545/(8*a);       
  
  F_AC_LINE_Freq = a;
  
}
/*=====================================================
 * Function : void Read_HLW8112_PF(void)
 * Describe : ��ȡ��������
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
=====================================================*/
void Read_HLW8112_PF(void)
{
  float a;
  unsigned long b;
  b = Read_HLW8112_RegData(REG_PF_ADDR,3);
	U16_PFData = b;
 	
  
  if (b>0x800000)       //Ϊ�������Ը���
  {
      a = (float)(0xffffff-b + 1)/0x7fffff;
  }
  else
  {
      a = (float)b/0x7fffff;
  }
  
  // ������ʺ�С���ӽ�0����ô��PF = �й�/���ڹ��� = 1����ô�˴�Ӧ��PF ��� 0��
  
  if (F_AC_P < 0.3) // С��0.3W
	  a = 0; 
  
  F_AC_PF = a;
  
}


/*=====================================================
 * Function : void Read_HLW8112_Angle(void)
 * Describe : ��ȡ��λ��
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/12
=====================================================*/
void Read_HLW8112_Angle(void)
{

	float a;
	unsigned long b;
	b = Read_HLW8112_RegData(REG_ANGLE_ADDR,2);
	U16_AngleData = b;
	
	if ( F_AC_PF < 55)	//����Ƶ��50HZ
	{
		a = b;
		a = a * 0.0805;
		F_Angle = a;
	}
	else
	{
		//����Ƶ��60HZ
		a = b;
		a = a * 0.0965;
		F_Angle = a;
	}
	
	if (F_AC_P < 0.5)		//����С��0.5ʱ��˵��û�и��أ����Ϊ0
	{
		F_Angle = 0;
	}
	
	if (F_Angle < 90)
	{
		a = F_Angle;
		printf("������ǰ��ѹ:%f\n " ,a);
	}
	else if (F_Angle < 180)
	{
		a = 180-F_Angle;
		printf("�����ͺ��ѹ:%f\n " ,a);	
	}
	else if (F_Angle < 360)
	{
		a = 360 - F_Angle;
		printf("�����ͺ��ѹ:%f\n " ,a);	
	}
	else
	{
			a = F_Angle -360;
			printf("������ǰ��ѹ:%f\n " ,a);	
	}
	
}

/*=====================================================
 * Function : void Read_HLW8112_State(void)
 * Describe : ��8112 �ж�״̬λ
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
=====================================================*/
void Read_HLW8112_State(void)
{
   //��ȡ��ѹ״̬λ,�����ȡRIF״̬�����ܽ�����һ���ж�
 // U16_IFData = Read_HLW8112_RegData(REG_IF_ADDR,2);
 // U16_RIFData = Read_HLW8112_RegData(REG_RIF_ADDR,2);
	
	
	U16_IEData = Read_HLW8112_RegData(REG_IE_ADDR,2);  
	U16_INTData= Read_HLW8112_RegData(REG_INT_ADDR,2); 
	
	
	U16_SAGCYCData = Read_HLW8112_RegData(REG_SAGCYC_ADDR,2); 
	U16_SAGLVLData = Read_HLW8112_RegData(REG_SAGLVL_ADDR,2); 
	
	
	U16_SYSCONData = Read_HLW8112_RegData(REG_SYSCON_ADDR,2); 
	U16_EMUCONData = Read_HLW8112_RegData(REG_EMUCON_ADDR,2); 
	U16_EMUCON2Data = Read_HLW8112_RegData(REG_EMUCON2_ADDR,2); 
}
/*=====================================================
 * Function : void HLW8012_Measure(void);
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/10
=====================================================*/

void HLW8112_Measure(void)
{
	
	Check_WriteReg_Success();
	
	Read_HLW8112_PA_I();
	Read_HLW8112_U();
	Read_HLW8112_PA();
	Read_HLW8112_EA();    
  Read_HLW8112_Linefreq();
	Read_HLW8112_Angle();
  Read_HLW8112_State();
  Read_HLW8112_PF();   
  Read_HLW8112_PB_I();
	Read_HLW8112_PB();
	Read_HLW8112_EB();
	
	
//	printf("��������-SPIͨѶ \r\n");
//	
//	printf("\r\n\r\n");//���뻻��
//	printf("Aͨ�����ܲ���\r\n");
//	printf("F_AC_V = %f V \n " ,F_AC_V);		//��ѹ
//	printf("F_AC_I = %f A \n " ,F_AC_I);		//Aͨ������
//	printf("F_AC_P = %f W \n " ,F_AC_P);		//Aͨ������
//	printf("F_AC_E = %f KWH \n " ,F_AC_E);		//Aͨ������


//	printf("\r\n\r\n");//���뻻��
//	printf("Bͨ�����ܲ���\r\n");
//	printf("F_AC_I_B = %f A \n " ,F_AC_I_B);		//Bͨ������
//	printf("F_AC_P_B = %f W \n " ,F_AC_P_B);		//Bͨ������
//	printf("F_AC_E_B = %f KWH \n " ,F_AC_E_B);		//Bͨ������
//	
//	printf("\r\n\r\n");//���뻻��
//	printf("F_AC_PF = %f\n " ,F_AC_PF);		//Aͨ����������
//	printf("F_AC_LINE_Freq = %f Hz \n " ,F_AC_LINE_Freq);		//F_AC_LINE_Freq	
//	printf("F_Angle = %f\n " ,F_Angle);

//	
//	printf("\r\n\r\n");//���뻻��
//	printf("���ܲ���\r\n");
//	printf("U32_RMSIA_RegData = %x\n " ,U32_RMSIA_RegData);
//	printf("U32_RMSIB_RegData = %x\n " ,U32_RMSIB_RegData);
//	printf("U32_RMSU_RegData = %x\n " ,U32_RMSU_RegData);
//	printf("U32_POWERPA_RegData = %x\n " ,U32_POWERPA_RegData);
//	printf("U32_POWERPB_RegData = %x\n " ,U32_POWERPB_RegData);
//	printf("U16_AngleData = %x\n " ,U16_AngleData);
//	printf("U16_PFData = %x\n " ,U16_PFData);
//	printf("U16_LineFData = %x\n " ,U16_LineFData);
//	printf("U16_IFData = %x\n " ,U16_IFData);
//	printf("U16_RIFData = %x\n " ,U16_RIFData);
//	printf("U16_IEData = %x\n " ,U16_IEData);
//	printf("U16_INTData = %x\n " ,U16_INTData);
//	printf("U16_SAGCYCData = %x\n " ,U16_SAGCYCData);
//	printf("U16_SAGLVLData = %x\n " ,U16_SAGLVLData);
//	
//	printf("U16_SYSCONData = %x\n " ,U16_SYSCONData);
//	printf("U16_EMUCONData = %x\n " ,U16_EMUCONData);
//	printf("U16_EMUCON2Data = %x\n " ,U16_EMUCON2Data);
//	
//	
//	
//	printf("\r\n\r\n");//���뻻��
//	printf("Aͨ������ת��ϵ��:%x\n " ,U16_RMSIAC_RegData);
//	printf("Bͨ������ת��ϵ��:%x\n " ,U16_RMSIBC_RegData);
//	printf("��ѹͨ��ת��ϵ��:%x\n " ,U16_RMSUC_RegData);
//	printf("Aͨ������ת��ϵ��:%x\n " ,U16_PowerPAC_RegData);
//	printf("Bͨ������ת��ϵ��:%x\n " ,U16_PowerPBC_RegData);
//	printf("Aͨ������ת��ϵ��:%x\n " ,U16_EnergyAC_RegData);
//	printf("Bͨ������ת��ϵ��:%x\n " ,U16_EnergyBC_RegData);
//	printf("U16_CheckSUM_RegData = %x\n " ,U16_CheckSUM_RegData);
//	printf("U16_CheckSUM_Data = %x\n " ,U16_CheckSUM_Data);
//	


//	printf("----------------------------------------------\r\n");	
//	printf("----------------------------------------------\r\n");	
}


#endif



























