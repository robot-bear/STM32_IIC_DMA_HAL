//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖÐ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼þ Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : Evk123
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üÐÞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : 0.69´çOLED ½Ó¿ÚÑÝÊ¾Àý³Ì(STM32F103ZEÏµÁÐIIC)
//              ËµÃ÷: 
//              ----------------------------------------------------------------
//              GND   µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              SCL   ½ÓPD6£¨SCL£©
//              SDA   ½ÓPD7£¨SDA£©            
//              ----------------------------------------------------------------
//Copyright(C) ÖÐ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////£

#include "oled.h"

#include "oledfont.h"  	 

#include "stm32f1xx_hal.h"			   
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
#define OLED_ADDRESS	0x78 //Í¨¹ýµ÷Õû0Rµç×è,ÆÁ¿ÉÒÔÑ¡Ôñ0x78  0x7AÁ½¸öµØÖ·   Ä¬ÈÏÎª0x78

uint8_t OLED[8][128]={0};
uint8_t CMD_Data[]={
0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xDB, 0x30, 0x8D, 0x14, 0xAF,0x20,0x00};
void WriteCmd(void)
{
	HAL_I2C_Mem_Write_DMA(&hi2c1,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data,29);
}


//ÏòÉè±¸Ð´Êý¾Ý
void OLED_Refreash(void)
{
	//HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0xff);
	HAL_I2C_Mem_Write_DMA(&hi2c1,0x78,0x40,I2C_MEMADD_SIZE_8BIT,OLED[0],1024);
}
//³õÊ¼»¯oledÆÁÄ»
void OLED_Init(void)
{ 	
	HAL_Delay(200);//ÕâÀïµÄÑÓÊ±ºÜÖØÒª
	 WriteCmd();
}
//ÇåÆÁ
void OLED_Clear(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		for(n=0;n<128;n++)
		OLED[i][n]=0;
	} 
}

unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}
//ÏÔÊ¾2¸öÊý×Ö
//x,y :Æðµã×ø±ê	 
//len :Êý×ÖµÄÎ»Êý
//size:×ÖÌå´óÐ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µþ¼ÓÄ£Ê½
//num:ÊýÖµ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2,0); 
	}
} 
//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//mode:0,·´°×ÏÔÊ¾;1,Õý³£ÏÔÊ¾				 
//size:Ñ¡Ôñ×ÖÌå 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t Is_Reverse)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
				if(Is_Reverse==0){
			for(i=0;i<8;i++){
					OLED[y][x+i] = F8X16[c*16+i];		
				  OLED[y+1][x+i] = F8X16[c*16+i+8];	
			}				
			}else{
			    OLED[y][x+i] = ~F8X16[c*16+i];		
				  OLED[y+1][x+i] = ~F8X16[c*16+i+8];				
			}
		}
			else {	
				if(Is_Reverse==0){
				for(i=0;i<6;i++)
				OLED[y][x+i] = F6x8[c][i];
				}
				else{
				for(i=0;i<6;i++)
				OLED[y][x+i] = ~F6x8[c][i];
				}
			}
}
 
//ÏÔÊ¾Ò»¸ö×Ö·ûºÅ´®
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size,0);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//ÏÔÊ¾ºº×Ö
//hzk ÓÃÈ¡Ä£Èí¼þµÃ³öµÄÊý×é
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t;

    for(t=0;t<16;t++)
		{
			OLED[y][x+t]=Hzk[2*no][t];
     }		
    for(t=0;t<16;t++)
			{	
				OLED[y+1][x+t]=Hzk[2*no+1][t];
      }					
}
