
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//    SDI(MOSI)      接          PB5          //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB4          //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PB13         //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB3          //液晶屏SPI总线时钟信号
//      DC/RS        接          PB14         //液晶屏数据/命令控制信号
//       RST         接          PB12         //液晶屏复位控制信号
//       CS          接          PB15         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PB1          //触摸屏触摸中断信号
//      T_DO         接          PB2          //触摸屏SPI总线读信号
//      T_DIN        接          PF11         //触摸屏SPI总线写信号
//      T_CS         接          PC5          //触摸屏片选控制信号
//      T_CLK        接          PB0          //触摸屏SPI总线时钟信号
**************************************************************************************************/		
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "lcdTFT32.h"
#include "stdlib.h"

 
#define	LCD_CS_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_CS, 1)   
#define	LCD_SDA_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SDA, 1)   
#define	LCD_SCL_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SCL, 1)   

#define	LCD_RS_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_RS, 1)  
#define	LCD_BLK_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_BLK, 1)   
#define	  LCD_RST_SET	gpio_set_level(GPIO_OUTPUT_IO_LCD_RST, 1)   

//娑叉櫠鎺у埗鍙ｇ疆0鎿嶄綔璇彞瀹忓畾涔?
#define	LCD_CS_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_CS, 0)   
#define	LCD_SDA_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SDA, 0)  
#define	LCD_SCL_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SCL, 0)  

#define	LCD_RS_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_RS, 0)
#define	LCD_BLK_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_BLK, 0)

#define   LCD_RST_CLR gpio_set_level(GPIO_OUTPUT_IO_LCD_RST, 0)

//#define LCD_DATAOUT(x) gpio_set_level(GPIO_OUTPUT_IO_LCD_CS, x); //鏁版嵁杈撳嚭


//static spi_device_handle_t mylcdspi;
	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


void  SPIv_WriteData(u8 Data)
{
	/*
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
	  if(Data&0x80)	
	  SPI_MOSI_SET; //输出数据
      else SPI_MOSI_CLR;
	   
      SPI_SCLK_CLR;       
      SPI_SCLK_SET;
      Data<<=1; 
	}
	*/
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
	  LCD_SDA_SET; //输出数据
      else LCD_SDA_CLR;
	   
      LCD_SCL_CLR;       
      LCD_SCL_SET;
      Data<<=1; 
	}
}


/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************
void LCD_WR_REG(u8 data)
{ 
   LCD_CS_CLR;     
	 LCD_RS_CLR;	  
   SPIv_WriteData(data);
   LCD_CS_SET;	
}

*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/

void LCD_WR_REG(u8 data)
{ 
LCD_CS_CLR;     
	 LCD_RS_CLR;	  
   SPIv_WriteData(data);
   LCD_CS_SET;
   //lcd_cmd(mylcdspi, data);
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
	 LCD_CS_CLR;
	 LCD_RS_SET;
   SPIv_WriteData(data);
   LCD_CS_SET;
   //lcd_data( mylcdspi, &data, 1);
}

/*
void LCD_WR_DATA(u8 data)
{
   LCD_CS_CLR;
	 LCD_RS_SET;
   SPIv_WriteData(data);
   LCD_CS_SET;
}

****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
	/*unsigned char data[2];
	data[0]=Data>>8;
	data[1]=Data;
	lcd_data( mylcdspi, data, 2);
	*/

   LCD_CS_CLR;
   LCD_RS_SET;  
   SPIv_WriteData(Data>>8);
	 SPIv_WriteData(Data);
   LCD_CS_SET;
   
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
			Lcd_WriteData_16Bit(Color);
		}
	}
	 LCD_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	
//液晶IO初始化配置


	gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
	
	LCD_BLK_SET;//开启背光
	LCD_BLK_CLR;//关闭背光
	LCD_BLK_SET;//开启背光
      


}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	
	LCD_RST_CLR;
	//delay_ms(100);
	vTaskDelay(100 / portTICK_RATE_MS);
	LCD_RST_SET;
	//delay_ms(50);
	vTaskDelay(50 / portTICK_RATE_MS);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{  
//led32_spi_master_init(mylcdspi);
	LCD_GPIOInit();//LCD GPIO初始化	
 	LCD_RESET(); //LCD 复位
//************* ILI9341初始化**********//	
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x48); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	//delay_ms(120);
	vTaskDelay(120 / portTICK_RATE_MS);
	LCD_WR_REG(0x29); //display on	

  LCD_direction(USE_HORIZONTAL);//设置LCD显示方向
	//LCD_LED=1;//点亮背光	 
	LCD_Clear(WHITE);//清全屏白色
}


/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 


void TFT32LCD_Color_Fill1_color(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int Color)
{  
	unsigned char picH,picL; 
	unsigned int height,width;
	unsigned int i,m;
	width=x2-x1+1; 			//
	height=y2-y1+1;			//
	LCD_SetWindows((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2);
 	for(m=0;m<width*height;m++)
    {	
	  	Lcd_WriteData_16Bit(Color);
    }   

}


void TFT32LCD_Color_Fill1(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int16_t *color)
{  
	unsigned char picH,picL; 
	unsigned int height,width;
	unsigned int i,m;
	width=x2-x1+1; 			//寰楀埌濉厖鐨勫搴?
	height=y2-y1+1;			//楂樺害

	LCD_SetWindows((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2);
 	for(m=0;m<width*height;m++)
    {	
		//picL=*(color+m*2);	//数据低位在前
		//picH=*(color+m*2+1);				
		//Lcd_WriteData_16Bit(picH<<8|picL);
	  	Lcd_WriteData_16Bit((unsigned int)(color[m]));
    }   
   //for(i=0;i<height;i++)
    //for(m=0;m<width;m++)
    //{	
	  	//Lcd_WriteData_16Bit((unsigned int)color[i*width+m]);
		//Gui_DrawPoint(x1+m,y1+i,(unsigned int)color[i*width+m]);
    //}   
	 

}

void TFT32LCD_Color_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t  color)
{  
	unsigned int height,width;
	unsigned int i,m;
	width=x2-x1+1; 			//寰楀埌濉厖鐨勫搴?
	height=y2-y1+1;			//楂樺害

	LCD_SetWindows((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2);


	for(m=0;m<width*height;m++)
    {	
	  	Lcd_WriteData_16Bit((unsigned int)color.full);
    }   
   //for(i=0;i<height;i++)
   // for(m=0;m<width;m++)
   // {	
	//  	Lcd_WriteData_16Bit((unsigned int)color.full);
   // }   
	 
}
void TFT32lcd_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_map)
{
	TFT32LCD_Color_Fill1((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2,color_map);
	
	lv_flush_ready();

}


void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	//Lcd_SetXY(x,y);
	//Lcd_WriteData_16Bit(Data);
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(Data); 
}  



