
#include "LcdTFT22_Driver.h"
//#include "Picture.h"


#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


//#;include "delay.h"


//STM32F103核心板例程
//库函数版本例程
/************** 嵌入式开发网  **************/
/********** mcudev.taobao.com 出品  ********/

//static void TFT144lcd_send_cmd(unsigned char cmd);
//static void TFT144lcd_send_data(unsigned char * data, unsigned int length);

//本测试程序使用的是模拟SPI接口驱动
//可自由更改接口IO配置，使用任意最少4 IO即可完成本款液晶驱动显示
//娑叉櫠鎺у埗鍙ｇ疆1鎿嶄綔璇彞瀹忓畾涔?
#define	LCD_CS_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_CS, 1)   
#define	LCD_SDA_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SDA, 1)   
#define	LCD_SCL_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SCL, 1)   

#define	LCD_RS_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_RS, 1)  
#define	LCD_BLK_SET  	gpio_set_level(GPIO_OUTPUT_IO_LCD_BLK, 1)   
#define	  LCD_RST_SET	gpio_set_level(GPIO_OUTPUT_IO_LCD_SDO, 1)   

//娑叉櫠鎺у埗鍙ｇ疆0鎿嶄綔璇彞瀹忓畾涔?
#define	LCD_CS_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_CS, 0)   
#define	LCD_SDA_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SDA, 0)  
#define	LCD_SCL_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_SCL, 0)  

#define	LCD_RS_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_RS, 0)
#define	LCD_BLK_CLR  	gpio_set_level(GPIO_OUTPUT_IO_LCD_BLK, 0)

#define   LCD_RST_CLR gpio_set_level(GPIO_OUTPUT_IO_LCD_SDO, 0)

//#define LCD_DATAOUT(x) gpio_set_level(GPIO_OUTPUT_IO_LCD_CS, x); //鏁版嵁杈撳嚭






//液晶IO初始化配置
void LCD_GPIO_Init(void)
{

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


/****************************************************************************
* 名    称：void  SPIv_WriteData(u8 Data)
* 功    能：STM32_模拟SPI写一个字节数据底层函数
* 入口参数：Data
* 出口参数：无
* 说    明：STM32_模拟SPI读写一个字节数据底层函数
****************************************************************************/
void  SPIv_WriteData(u8 Data)
{
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


/****************************************************************************
* 名    称：Lcd_WriteIndex(u8 Index)
* 功    能：向液晶屏写一个8位指令
* 入口参数：Index   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
****************************************************************************/
void Lcd_WriteIndex(u8 Index)
{
   LCD_CS_CLR;
   LCD_RS_CLR;

   SPIv_WriteData(Index);
 
   LCD_CS_SET;
}

/****************************************************************************
* 名    称：Lcd_WriteData(u8 Data)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;

   SPIv_WriteData(Data);
 
   LCD_CS_SET;
}

/****************************************************************************
* 名    称：void LCD_WriteReg(u8 Index,u16 Data)
* 功    能：写寄存器数据
* 入口参数：Index,Data
* 出口参数：无
* 说    明：本函数为组合函数，向Index地址的寄存器写入Data值
****************************************************************************/
void LCD_WriteReg(u8 Index,u16 Data)
{
	Lcd_WriteIndex(Index);
  	Lcd_WriteData_16Bit(Data);
}

/****************************************************************************
* 名    称：void Lcd_WriteData_16Bit(u16 Data)
* 功    能：向液晶屏写一个16位数据
* 入口参数：Data
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void Lcd_WriteData_16Bit(u16 Data)
{	
	
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);
}

/****************************************************************************
* 名    称：void Lcd_Reset(void)
* 功    能：液晶硬复位函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化前需执行一次复位操作
****************************************************************************/
void Lcd_Reset(void)
{
	LCD_RST_CLR;
	//delay_ms(100);
	vTaskDelay(100 / portTICK_RATE_MS);
	LCD_RST_SET;
	//delay_ms(50);
	vTaskDelay(50 / portTICK_RATE_MS);
}
/****************************************************************************
* 名    称：void Lcd_Init(void)
* 功    能：液晶初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化_ILI9225_176X220
****************************************************************************/
void Lcd_Init(void)
{	

	LCD_GPIO_Init();//使用模拟SPI

	Lcd_Reset(); //Reset before LCD Init.

	//LCD Init For 2.2inch LCD Panel with ILI9225.	
	LCD_WriteReg(0x10, 0x0000); // Set SAP,DSTB,STB
	LCD_WriteReg(0x11, 0x0000); // Set APON,PON,AON,VCI1EN,VC
	LCD_WriteReg(0x12, 0x0000); // Set BT,DC1,DC2,DC3
	LCD_WriteReg(0x13, 0x0000); // Set GVDD
	LCD_WriteReg(0x14, 0x0000); // Set VCOMH/VCOML voltage
	vTaskDelay(40 / portTICK_RATE_MS);// Delay 20 ms
	
	// Please follow this power on sequence
	LCD_WriteReg(0x11, 0x0018); // Set APON,PON,AON,VCI1EN,VC
	LCD_WriteReg(0x12, 0x1121); // Set BT,DC1,DC2,DC3
	LCD_WriteReg(0x13, 0x0063); // Set GVDD
	LCD_WriteReg(0x14, 0x3961); // Set VCOMH/VCOML voltage
	LCD_WriteReg(0x10, 0x0800); // Set SAP,DSTB,STB
	vTaskDelay(10 / portTICK_RATE_MS); // Delay 10 ms
	LCD_WriteReg(0x11, 0x1038); // Set APON,PON,AON,VCI1EN,VC
	vTaskDelay(30 / portTICK_RATE_MS);// Delay 30 ms
	
	
	LCD_WriteReg(0x02, 0x0100); // set 1 line inversion

#if USE_HORIZONTAL//如果定义了横屏
	//R01H:SM=0,GS=0,SS=0 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x01, 0x001C); // set the display line number and display direction
	//R03H:BGR=1,ID0=1,ID1=1,AM=1 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x03, 0x1038); // set GRAM write direction .
#else//竖屏
	//R01H:SM=0,GS=0,SS=1 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x01, 0x011C); // set the display line number and display direction 
	//R03H:BGR=1,ID0=1,ID1=1,AM=0 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x03, 0x1030); // set GRAM write direction.
#endif

	LCD_WriteReg(0x07, 0x0000); // Display off
	LCD_WriteReg(0x08, 0x0808); // set the back porch and front porch
	LCD_WriteReg(0x0B, 0x1100); // set the clocks number per line
	LCD_WriteReg(0x0C, 0x0000); // CPU interface
	LCD_WriteReg(0x0F, 0x0501); // Set Osc
	LCD_WriteReg(0x15, 0x0020); // Set VCI recycling
	LCD_WriteReg(0x20, 0x0000); // RAM Address
	LCD_WriteReg(0x21, 0x0000); // RAM Address
	
	//------------------------ Set GRAM area --------------------------------//
	LCD_WriteReg(0x30, 0x0000); 
	LCD_WriteReg(0x31, 0x00DB); 
	LCD_WriteReg(0x32, 0x0000); 
	LCD_WriteReg(0x33, 0x0000); 
	LCD_WriteReg(0x34, 0x00DB); 
	LCD_WriteReg(0x35, 0x0000); 
	LCD_WriteReg(0x36, 0x00AF); 
	LCD_WriteReg(0x37, 0x0000); 
	LCD_WriteReg(0x38, 0x00DB); 
	LCD_WriteReg(0x39, 0x0000); 
	
	
	// ---------- Adjust the Gamma 2.2 Curve -------------------//
	LCD_WriteReg(0x50, 0x0603); 
	LCD_WriteReg(0x51, 0x080D); 
	LCD_WriteReg(0x52, 0x0D0C); 
	LCD_WriteReg(0x53, 0x0205); 
	LCD_WriteReg(0x54, 0x040A); 
	LCD_WriteReg(0x55, 0x0703); 
	LCD_WriteReg(0x56, 0x0300); 
	LCD_WriteReg(0x57, 0x0400); 
	LCD_WriteReg(0x58, 0x0B00); 
	LCD_WriteReg(0x59, 0x0017); 
	
	
	
	LCD_WriteReg(0x0F, 0x0701); // Vertical RAM Address Position
	LCD_WriteReg(0x07, 0x0012); // Vertical RAM Address Position
	vTaskDelay(50 / portTICK_RATE_MS); // Delay 50 ms
	LCD_WriteReg(0x07, 0x1017); // Vertical RAM Address Position  
	
}



/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(u16 Xpos, u16 Ypos)
{	
#if USE_HORIZONTAL//如果定义了横屏  	    	
	LCD_WriteReg(0x21,Xpos);
	LCD_WriteReg(0x20,Ypos);
#else//竖屏	
	LCD_WriteReg(0x20,Xpos);
	LCD_WriteReg(0x21,Ypos);
#endif
	Lcd_WriteIndex(0x22);		
} 
/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
//设置显示窗口
void Lcd_SetRegion(u8 xStar, u8 yStar,u8 xEnd,u8 yEnd)
{
#if USE_HORIZONTAL//如果定义了横屏	
	LCD_WriteReg(0x38,xEnd);
	LCD_WriteReg(0x39,xStar);
	LCD_WriteReg(0x36,yEnd);
	LCD_WriteReg(0x37,yStar);
	LCD_WriteReg(0x21,xStar);
	LCD_WriteReg(0x20,yStar);
#else//竖屏	
	LCD_WriteReg(0x36,xEnd);
	LCD_WriteReg(0x37,xStar);
	LCD_WriteReg(0x38,yEnd);
	LCD_WriteReg(0x39,yStar);
	LCD_WriteReg(0x20,xStar);
	LCD_WriteReg(0x21,yStar);
#endif
	Lcd_WriteIndex(0x22);	
}

	
/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：xy坐标和颜色数据
返回值：无
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetXY(x,y);
	Lcd_WriteData_16Bit(Data);

}    

/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	Lcd_WriteData_16Bit(Color);
    }   
}

void TFT22LCD_Color_Fill1_color(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int Color)
{  
	unsigned char picH,picL; 
	unsigned int height,width;
	unsigned int i,m;
	width=x2-x1+1; 			//
	height=y2-y1+1;			//
	Lcd_SetRegion((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2);
 	for(m=0;m<width*height;m++)
    {	
	  	Lcd_WriteData_16Bit(Color);
    }   

}


void TFT22LCD_Color_Fill1(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, int16_t *color)
{  
	unsigned char picH,picL; 
	unsigned int height,width;
	unsigned int i,m;
	width=x2-x1+1; 			//寰楀埌濉厖鐨勫搴?
	height=y2-y1+1;			//楂樺害

	Lcd_SetRegion((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2);
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

void TFT22LCD_Color_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t  color)
{  
	unsigned int height,width;
	unsigned int i,m;
	width=x2-x1+1; 			//寰楀埌濉厖鐨勫搴?
	height=y2-y1+1;			//楂樺害

	Lcd_SetRegion((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2);


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
void TFT22lcd_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_map)
{
	TFT22LCD_Color_Fill1((unsigned int)x1,(unsigned int)y1,(unsigned int)x2,(unsigned int)y2,color_map);
	
	lv_flush_ready();

}
void showimage(const unsigned char *p) //显示40*40 QQ图片
{
  	int i,j,k; 
	unsigned char picH,picL; 
	Lcd_Clear(GRAY0);
	//Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"图片显示测试");
	vTaskDelay(1000 / portTICK_RATE_MS);

	Lcd_Clear(GRAY0);
	for(k=0;k<4;k++)
	{
	   	for(j=0;j<4;j++)
		{	
			Lcd_SetRegion(40*j,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				Lcd_WriteData_16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}
unsigned int arrtest[1600];

void  tempfun(const unsigned char *p)
{
	unsigned int i;
	unsigned char picH,picL; 
			for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				arrtest[i]=(picH<<8|picL);  						
			 }	
	
}

void testfill(void)
{

	lv_color_t  color_map;
	//color_map.full = RED ;

	//TFT144lcd_fill(0, 0, 20, 20,color_map );
	color_map.full = BLUE ;
	//TFT144lcd_fill(20, 20, 60, 60,color_map );
	//TFT144lcd_flush(60, 60, 100, 100,(lv_color_t*)gImage_qqlog );
	Lcd_Clear(RED);

	//showimage(gImage_qq);
	Lcd_Clear(RED);
	
	//tempfun(gImage_qq)	;
	//TFT22LCD_Color_fill(10,10,40,40,color_map);
	//TFT22LCD_Color_Fill1(60,60,100,100,arrtest);
	//TFT22lcd_flush(110,110,150,150,(lv_color_t *)gImage_qq);
	vTaskDelay(5000 / portTICK_RATE_MS);
}




