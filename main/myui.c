#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#include "../lvgl/lvgl.h"

#include "i2s_dac.h"

#include "myui.h"
#include "qr_encode.h"
#include "LcdTFT22_Driver.h"
#include "i2s_dac_demo.h"

bool wifi_connected = false ;
bool server_connected = false ;


char *str1test = "http://weixin.qq.com/q/02EwdQQBjMdz01h78VxscA";

uint8_t bitdata[QR_MAX_BITDATA];

LV_IMG_DECLARE(boyulogo);
LV_FONT_DECLARE(myfont);
//LV_FONT_DECLARE(hansans_20_jp);
/*
lv_obj_t *screenmain,*lougou,*lougoutext,*startup,*ordinary ,*smart1,*score,*ptotal,*wifi_icon,*img_coverlogo;
lv_obj_t *status_bar,*Table_text,*dirtext,*remarks, *startup_bar;
static lv_style_t style_bar1,status_bar_style, startup_bar_style,startup_bar_indic_style,bottom_style,wifi_icon_stayle;
*/

lv_obj_t *screenmain,*lougou,*lougoutext,*text1,*text2 ,*text3,*text4,*text5,*wifi_icon,* text6,* text7,* text8,*img_coverlogo;
lv_obj_t *status_bar,*Table_text,*dirtext,*remarks, *startup_bar;
lv_style_t temp_style ,temp_style1 ,temp_style2,style_bar1,status_bar_style, startup_bar_style,startup_bar_indic_style,bottom_style,wifi_icon_stayle;


lv_group_t *group;

/*
static const uint8_t bin_pixel_map[] = {
	
	  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
	};
	
	
	const lv_img_t qrimg_t = {
	  .header.w = 10,			//
	  .header.h = 6,			//
	  .header.alpha_byte = 0,		//
	  .header.chroma_keyed = 0, //
	  .header.format = LV_IMG_FORMAT_INTERNAL_RAW,	//
	  .pixel_map = bin_pixel_map	//Pointer the array of image pixels.
	 
	};

*/

void displayQRCode (unsigned int startx1, unsigned int starty1,int side,uint8_t *bitdata)
{
	int i=0,j=0,a=0,l=0,n=0;
	int OUT_FILE_PIXEL_PRESCALER = 3;//3
	
	//CLEAR 
	//Lcd_Clear(WHITE);

	
	
	if(side == 21)
	{
		OUT_FILE_PIXEL_PRESCALER = 6 ;
	}
	/*

	text4 = lv_obj_create(screenmain, NULL);
	lv_obj_set_size(text4, side*OUT_FILE_PIXEL_PRESCALER+20,side*OUT_FILE_PIXEL_PRESCALER+20);
	lv_obj_set_pos(text4, startx1, starty1);
	lv_obj_set_style(text4, &lv_style_scr);




	
	uint16_t qrimgtemp = 0 ;
	 uint8_t *qrimgstr;
	qrimgstr = malloc(side*side*2+2);
	if(qrimgstr != NULL)
	{
		printf("qimgstr malloc success side = %d",side*side*2);

		//printf("sizeof(bin_pixel_map) = %d",sizeof(bin_pixel_map));
		

		
		for(qrimgtemp = 0 ;qrimgtemp<side*side*2 ; qrimgtemp++)
			qrimgstr[qrimgtemp] = 0xff ;
		for(i=0;i<side;i++)
		{
			for(j=0;j<side;j++)
			{
				a=j*side +i ;
				
				if(bitdata[a/8]&(1<<(7-a%8)))
				{
					for(l=0;l<OUT_FILE_PIXEL_PRESCALER;l++)
					{
						for(n=0;n<OUT_FILE_PIXEL_PRESCALER;n++)
						{
							qrimgstr[(OUT_FILE_PIXEL_PRESCALER*i+l)*2+(OUT_FILE_PIXEL_PRESCALER*j+n)*(side*2)] = 0x00 ;
							qrimgstr[(OUT_FILE_PIXEL_PRESCALER*i+l)*2+(OUT_FILE_PIXEL_PRESCALER*j+n)*(side*2)+1] = 0x00 ;
							//Gui_DrawPoint(startx1+(OUT_FILE_PIXEL_PRESCALER*i+l),starty1+(OUT_FILE_PIXEL_PRESCALER*j+n),BLACK);	
							//BLACK
						}
					}
				}
				
				if(j%3 == 0 )
				{
					qrimgstr[i*2+j*(side*2)]=0x00;
					qrimgstr[i*2+j*(side*2)+1]=0x00;
				}
			
			}
		}
	}


	
	
	

	
	//LV_IMG_DECLARE(qrimg_t);
	text5 = lv_img_create(screenmain, NULL);
	lv_obj_align(text5, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);//左对齐(lv_obj_get_width(img_coverlogo)/2)
	lv_obj_set_size(text5, qrimg_t.header.w, qrimg_t.header.h);
	lv_img_set_src(text5, &qrimg_t);

	//vTaskDelay(3000 / portTICK_PERIOD_MS);
	//free(qrimgstr);
	*/

	TFT22LCD_Color_Fill1_color(startx1-10,starty1-10, (startx1+side*OUT_FILE_PIXEL_PRESCALER)+10, (starty1+side*OUT_FILE_PIXEL_PRESCALER)+10,WHITE);
	
	//TFT22LCD_Color_Fill1_color(startx1,starty1, startx1+side*OUT_FILE_PIXEL_PRESCALER, starty1+side*OUT_FILE_PIXEL_PRESCALER,WHITE);
	for(i=0;i<side;i++)
	{
		for(j=0;j<side;j++)
		{
			a=j*side +i ;
			
			if(bitdata[a/8]&(1<<(7-a%8)))
			{
				for(l=0;l<OUT_FILE_PIXEL_PRESCALER;l++)
				{
					for(n=0;n<OUT_FILE_PIXEL_PRESCALER;n++)
					{
						Gui_DrawPoint(startx1+(OUT_FILE_PIXEL_PRESCALER*i+l),starty1+(OUT_FILE_PIXEL_PRESCALER*j+n),BLACK);	
					}
				}
			}
		
		}
	}
	

}

void draw_qrimg(char *str)
{
	int qrsize = 0 ;

		qrsize = qr_encode( QR_LEVEL_L, 0,str, 0, bitdata);
		printf("size=%d\r\n",qrsize);
		/*int i = 0 ;
		
		for(i=0;i<qrsize*qrsize;i++)
			printf("%02x %d ",bitdata[i],i);
		printf("size=%d\r\n",qrsize);
		*/
		displayQRCode (LV_HOR_RES/2-50,64,qrsize,bitdata);//30
		vTaskDelay(10 / portTICK_PERIOD_MS);

}




struct_majiang majiang ;

void sys_variableinit(void)
{
    unsigned char i = 0 ,j=0;
    for(i=0;i<sizeof(majiang.Table_number);i++)
        majiang.Table_number[i] = 0;

      sprintf((char*)majiang.Table_number ,"Z0007");
//    majiang.Table_number[0] = '0';
//    majiang.Table_number[1] = '0';
//    majiang.Table_number[2] = '3';

    majiang.position = 5 ;//方位
    majiang.game_mode = 0; //单机
    majiang.wifi_connect_state = true ;
    majiang.server_connect_state = false;
    for(i=0;i<sizeof(majiang.QR_link);i++)
        majiang.QR_link[i] = 0;
    for(j=0;j<4;j++)
        for(i=0;i<4;i++)
            majiang.standard_score[j][i] = 0;

    for(j=0;j<4;j++)
        for(i=0;i<4;i++)
            majiang.match_score[j][i] = 0;
	for(j=0;j<4;j++)
        for(i=0;i<4;i++)
            majiang.current_game_score[j][i] = 0;

    majiang.inform_nums = 0 ;
    majiang.current_informnum = 1 ;//第一页
    majiang.games = 0;
    majiang.current_game = 0 ;
    majiang.current_page = 0;
    majiang.confirm_num  = 0 ;
    majiang.current_audio = AUDIO_NULL ;

}

void sys_variabletest(void)
{
    majiang.standard_score[0][0] = '-';
    majiang.standard_score[0][1] = '2';
    majiang.standard_score[0][2] = '3';

    majiang.standard_score[1][0] = '1';
    majiang.standard_score[1][1] = '0';
    majiang.standard_score[1][2] = '0';

    majiang.standard_score[2][0] = '-';
    majiang.standard_score[2][1] = '2';
    //majiang.standard_score[2][2] = '0';
    majiang.standard_score[3][0] = '4';
    majiang.standard_score[3][1] = '5';
    //majiang.standard_score[3][2] = '0';

    majiang.match_score[0][0] = '-';
    majiang.match_score[0][1] = '5';
    majiang.match_score[0][2] = '4';

    majiang.match_score[1][0] = '-';
    majiang.match_score[1][1] = '5';
    majiang.match_score[1][2] = '7';

    majiang.match_score[2][0] = '6';
    majiang.match_score[2][1] = '2';
    //majiang.match_score[2][2] = '0';
    majiang.match_score[3][0] = '3';
    majiang.match_score[3][1] = '4';
	
    majiang.current_game_score[0][0] = '-';
    majiang.current_game_score[0][1] = '2';
    majiang.current_game_score[0][2] = '3';
	
	majiang.current_game_score[1][0] = '+';
    majiang.current_game_score[1][1] = '8';

	
	majiang.current_game_score[2][0] = '-';
    majiang.current_game_score[2][1] = '5';
    majiang.current_game_score[2][2] = '4';
	
	majiang.current_game_score[3][0] = '-';
    majiang.current_game_score[3][1] = '9';

   
    majiang.inform_nums = 6 ;
    majiang.current_informnum = 1 ;//第一页
	majiang.position = DIR_NORTH ;//方位
	majiang.games = 8;
    majiang.current_game = 3 ;
}


void style_init(void)
{
	
	lv_style_copy(&style_bar1, &lv_style_scr);
	style_bar1.body.main_color = LV_COLOR_MAKE(0xEE,0x40,0x10);
	style_bar1.body.grad_color = LV_COLOR_MAKE(0xEE,0x40,0x10);

	//lv_font_add(&hansans_20_cn, &lv_font_dejavu_20);


	lv_style_copy(&status_bar_style, &lv_style_plain);
	status_bar_style.text.color = LV_COLOR_HEX(0xFFFFFF);
	status_bar_style.text.font = &myfont;
	status_bar_style.body.main_color = LV_COLOR_MAKE(0xEE,0x40,0x10);
	status_bar_style.body.grad_color = LV_COLOR_MAKE(0xEE,0x40,0x10);
	
	///lv_font_add(&hansans_20_cn, &lv_font_dejavu_20);
	//lv_font_add(&hansans_20_jp, &lv_font_dejavu_20);
	/*
    lv_style_copy(&status_bar_style,&lv_style_scr);
	status_bar_style.body.main_color = LV_COLOR_MAKE(0xEE,0x40,0x10);
	status_bar_style.body.grad_color = LV_COLOR_MAKE(0xEE,0x40,0x10);
	status_bar_style.text.color = LV_COLOR_WHITE;
	//status_bar_style.image.color = LV_COLOR_MAKE(0xEE,0x40,0x10);
	//status_bar_style.line.color = LV_COLOR_MAKE(0xEE,0x40,0x10);
    status_bar_style.text.font = &myfont20;

*/
	lv_style_copy(&wifi_icon_stayle,&status_bar_style);
	wifi_icon_stayle.text.font = &lv_font_dejavu_20;
	wifi_icon_stayle.text.color = LV_COLOR_WHITE;

    lv_style_copy(&bottom_style, &lv_style_scr);
    bottom_style.body.main_color = LV_COLOR_MAKE(0x39,0x38,0x39);
    bottom_style.body.grad_color = LV_COLOR_MAKE(0x39,0x38,0x39);

    lv_style_copy(&startup_bar_style, &lv_style_pretty);
    startup_bar_style.body.main_color = LV_COLOR_BLACK;
    startup_bar_style.body.grad_color = LV_COLOR_GRAY;
    startup_bar_style.body.radius = LV_RADIUS_CIRCLE;
    startup_bar_style.body.border.color = LV_COLOR_WHITE;

    lv_style_copy(&startup_bar_indic_style, &lv_style_pretty);
    startup_bar_indic_style.body.grad_color =  LV_COLOR_RED;
    startup_bar_indic_style.body.main_color=  LV_COLOR_RED;//LV_COLOR_LIME
    startup_bar_indic_style.body.radius = LV_RADIUS_CIRCLE;
    startup_bar_indic_style.body.shadow.width = 10;
    startup_bar_indic_style.body.shadow.color = LV_COLOR_RED;
    startup_bar_indic_style.body.padding.hor = 3;           /*Make the indicator a little bit smaller*/
    startup_bar_indic_style.body.padding.ver = 3;

	sys_variableinit();
    sys_variabletest();//测试
    //bottom_style.text.font = &myfont20;
}
void drawimglogo(void) {

	img_coverlogo = lv_img_create(screenmain, NULL);
	lv_obj_align(img_coverlogo, NULL, LV_ALIGN_IN_TOP_MID, -(boyulogo.header.w/2)+10, 10);//左对齐(lv_obj_get_width(img_coverlogo)/2)
	lv_obj_set_size(img_coverlogo, boyulogo.header.w, boyulogo.header.h);
	lv_img_set_src(img_coverlogo, &boyulogo);
	//lv_obj_set_pos(img_coverlogo, 50, 10);
	
	
	
}


void drawlogo (void)
{

	clear_screenmain();
	
    startup_bar  = lv_bar_create(screenmain, NULL);
    lv_obj_set_size(startup_bar, lv_obj_get_width(screenmain)-20, 10);
    //lv_obj_align(startup_bar, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 30);
    lv_bar_set_value(startup_bar, 0);

    lv_bar_set_style(startup_bar, LV_BAR_STYLE_BG, &startup_bar_style);
    lv_bar_set_style(startup_bar, LV_BAR_STYLE_INDIC, &startup_bar_indic_style);
    //lv_obj_align(startup_bar, bar1, LV_ALIGN_OUT_BOTTOM_MID, 0, 30); /*Align below 'bar1'*/
    lv_bar_set_value_anim(startup_bar, 100, 3000);//进度条动画

    lougoutext = lv_label_create(screenmain, NULL);
    lv_obj_set_size(lougoutext, lv_obj_get_width(screenmain)-20, STATUS_BAR_VER*2);//文本框大小
    //lv_obj_set_pos(lougoutext, 10, lv_obj_get_height(screenmain)-30);
    lv_obj_align(lougoutext, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
    lv_obj_align(startup_bar, lougoutext, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
    lv_label_set_style(lougoutext, &status_bar_style);

    //lv_label_set_long_mode(lougoutext, LV_LABEL_LONG_EXPAND);
    lv_label_set_text(lougoutext,"智能麻将机启动中...");
    lv_label_set_align(lougoutext, LV_LABEL_ALIGN_CENTER);
    /*
    img_cover = lv_img_create(screen, NULL);
	lv_img_set_src(img_cover, &boyulogo);
	lv_obj_set_pos(img_cover, 50, 10);
	lv_obj_set_size(img_cover, 102, 128);
	*/
	drawimglogo();
}



void drawBottom (void)
{
    

    status_bar = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(status_bar, LV_HOR_RES, STATUS_BAR_VER);//状态栏大小
	lv_obj_set_pos(status_bar, 0, 0);
	lv_obj_set_style(status_bar, &style_bar1);

	Table_text = lv_label_create(status_bar, NULL);
    //lv_obj_set_size(Table_text, 100, STATUS_BAR_VER);//文本框大小
    //lv_obj_set_pos(Table_text, 0, 2);
    lv_obj_align(Table_text, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);//左对齐
    lv_label_set_style(Table_text, &status_bar_style);
    lv_label_set_text(Table_text,(char *)majiang.Table_number);
    //lv_label_set_align(Table_text, LV_LABEL_ALIGN_RIGHT);

    dirtext = lv_label_create(status_bar, NULL);
    //lv_obj_set_size(dirtext, STATUS_BAR_VER, STATUS_BAR_VER);//文本框大小
    //lv_obj_set_pos(dirtext, 0, 2);
    lv_obj_align(dirtext, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);//左对齐
    lv_label_set_style(dirtext, &status_bar_style);
    if(majiang.position==DIR_EAST)
        lv_label_set_text(dirtext,"东");
    else if(majiang.position==DIR_SOUTH)
        lv_label_set_text(dirtext,"南");
    else if(majiang.position==DIR_WEST)
        lv_label_set_text(dirtext,"西");
    else if(majiang.position==DIR_NORTH)
        lv_label_set_text(dirtext,"北");
    else
        lv_label_set_text(dirtext,"err");

    remarks = lv_label_create(status_bar, NULL);
    //lv_obj_set_size(remarks, 100, STATUS_BAR_VER);//文本框大小
    //lv_obj_set_pos(remarks, 0, 2);
    lv_obj_align(remarks, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, 0);//左对齐
    lv_label_set_style(remarks, &status_bar_style);
    //lv_label_set_long_mode(remarks, LV_LABEL_LONG_DOT);
    lv_label_set_text(remarks," ");
    //lv_label_set_align(dirtext, LV_LABEL_ALIGN_CENTER);


    wifi_icon = lv_label_create(status_bar, NULL);
	//
	lv_label_set_style(wifi_icon, &wifi_icon_stayle);
	//lv_obj_set_pos(wifi_icon, LV_HOR_RES-30, 5);
    lv_obj_align(wifi_icon, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 4);//左对齐

    //lv_label_set_text(wifi_icon,SYMBOL_WIFI""SYMBOL_WIFI);
    //lv_label_set_text(wifi_icon, majiang.wifi_connect_state ?  (majiang.server_connect_state ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
	lv_label_set_text(wifi_icon, wifi_connected ?  (server_connected ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
		

    screenmain = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_size(screenmain, LV_HOR_RES, LV_VER_RES-STATUS_BAR_VER);
	lv_obj_set_pos(screenmain, 0, STATUS_BAR_VER);
	lv_obj_set_style(screenmain, &bottom_style);

    //drawlogo ();

}

void draw_screenmain() {
	/*screenmain = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_size(screenmain, LV_HOR_RES, LV_VER_RES-STATUS_BAR_VER);
	lv_obj_set_pos(screenmain, 0, STATUS_BAR_VER);
	lv_obj_set_style(screenmain, &bottom_style);
*/
    drawlogo ();
}

void clear_screenmain() {
	//lv_group_del(group);
	lv_obj_clean(screenmain);
	//lv_obj_del(status_bar);
	//group = lv_group_create();
	//lv_indev_set_group(keypad_indev, group);
	//lv_group_set_style_mod_cb(group, style_mod_cb);
}

void clear_status_bar() {
	lv_obj_clean(status_bar);
}

void startus_bar_set(void)
{
    /*
    lv_label_set_text(Table_text,"桌号:24");
    lv_label_set_text(dirtext,"东");
	lv_label_set_text(wifi_icon,"   "SYMBOL_WARNING);
	lv_label_set_text(remarks,"智能");
	*/
	lv_label_set_text(Table_text,(char *)majiang.Table_number);
    if(majiang.position==DIR_EAST)
        lv_label_set_text(dirtext,"东");
    else if(majiang.position==DIR_SOUTH)
        lv_label_set_text(dirtext,"南");
    else if(majiang.position==DIR_WEST)
        lv_label_set_text(dirtext,"西");
    else if(majiang.position==DIR_NORTH)
        lv_label_set_text(dirtext,"北");
    else
        lv_label_set_text(dirtext,"err");
	lv_label_set_text(wifi_icon, majiang.wifi_connect_state ?  (majiang.server_connect_state ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
	lv_label_set_text(remarks,"智能");
}

void drawordinary (void)
{
    clear_screenmain();
    lv_label_set_text(dirtext,"");
    lv_label_set_text(Table_text,"桌号:--");
    lv_label_set_text(dirtext,"普通模式");
	lv_label_set_text(wifi_icon,"   "SYMBOL_WARNING);
	lv_label_set_text(remarks,"");
    /*
    画二维码

    */

    text1 = lv_label_create(screenmain, NULL);
    //lv_obj_set_size(text1,100,30);
    //lv_obj_set_width(text1,100);
    lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, 20);//左对齐
    lv_label_set_style(text1, &status_bar_style);
    lv_label_set_long_mode(text1,LV_LABEL_LONG_BREAK);
    lv_label_set_text(text1,"扫码联系客服");

    majiang.current_page = PAGE_ORDINARY ;

}

void drawsmart1 (void)
{
    clear_screenmain();

    text1 = lv_label_create(screenmain, NULL);

	lv_label_set_style(text1, &status_bar_style);
	//lv_obj_set_pos(text1, LV_HOR_RES-30, 5);
    lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_MID, -50, 0);//左对齐

    lv_label_set_text(text1,"请扫码加入竞赛");

    //画二维码
    /*

	
    */
    
    
	
    majiang.current_page = PAGE_SMART1 ;
}


void lv_restart_test(void)
{
	lv_task_enable(true);
	//vTaskDelay(20 / portTICK_PERIOD_MS);
	//lv_init();//初始化lv
	vTaskDelay(1 / portTICK_PERIOD_MS);
}


void lv_restart_test1(void)
{
	
	//vTaskDelay(100 / portTICK_PERIOD_MS);
	//lv_init();//初始化lv
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	lv_task_enable(false);
}




void drawptotal (void)
{

	lv_obj_del(screenmain);
	lv_obj_del(status_bar);

	drawBottom ();

    //clear_screenmain();

    text1 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text1, &status_bar_style);
	//lv_obj_set_pos(text1, LV_HOR_RES-30, 5);
    lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 0);//左对齐
    lv_label_set_text(text1,"统计标准分");


    text3 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text3, &status_bar_style);
	//lv_obj_set_pos(text3, LV_HOR_RES-30, 5);
    lv_obj_align(text3, NULL, LV_ALIGN_IN_TOP_LEFT, lv_obj_get_width(screenmain)/5, lv_obj_get_height(screenmain)/3);
/*
    switch(majiang.position)
    {
        case DIR_EAST  : lv_label_set_text(text3,(char *)majiang.standard_score[0]);
            break;
        case DIR_SOUTH : lv_label_set_text(text3,(char *)majiang.standard_score[1]);
            break;
        case DIR_WEST  : lv_label_set_text(text3,(char *)majiang.standard_score[2]);
            break;
        case DIR_NORTH : lv_label_set_text(text3,(char *)majiang.standard_score[3]);
            break;
        default        : lv_label_set_text(text3,(char *)"dir err");
            break;
    }

    */

    text2 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text2, &status_bar_style);
	//lv_obj_set_pos(text2, LV_HOR_RES-30, 5);
    lv_obj_align(text2, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, 0);
    lv_label_set_text(text2,"累计得分");

    text4 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text4, &status_bar_style);
	//lv_obj_set_pos(text4, LV_HOR_RES-30, 5);
    lv_obj_align(text4, NULL, LV_ALIGN_IN_TOP_RIGHT, -lv_obj_get_width(screenmain)/5, lv_obj_get_height(screenmain)/3);
    switch(majiang.position)
    {
        case DIR_EAST  :
            lv_label_set_text(text3,(char *)majiang.standard_score[0]);
            lv_label_set_text(text4,(char *)majiang.match_score[0]);
            break;
        case DIR_SOUTH :
            lv_label_set_text(text3,(char *)majiang.standard_score[1]);
            lv_label_set_text(text4,(char *)majiang.match_score[1]);
            break;
        case DIR_WEST  :
            lv_label_set_text(text3,(char *)majiang.standard_score[2]);
            lv_label_set_text(text4,(char *)majiang.match_score[2]);
            break;
        case DIR_NORTH :
            lv_label_set_text(text3,(char *)majiang.standard_score[3]);
            lv_label_set_text(text4,(char *)majiang.match_score[3]);
            break;
        default        :
            lv_label_set_text(text3,(char *)"dir err");
            lv_label_set_text(text4,(char *)"dir err");
            break;
    }

    text5 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text5, &status_bar_style);
	//lv_obj_set_pos(text5, LV_HOR_RES-30, 5);
    lv_obj_align(text5, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

	
    char str11[5];
    if(majiang.current_game)
        sprintf(str11 ,"%d/%d",majiang.current_game,majiang.games);
    else
        sprintf(str11 ,"-/-");
    lv_label_set_text(text5,(char *)str11);

    text6 = lv_label_create(screenmain, NULL);
    lv_label_set_style(text6, &status_bar_style);
    lv_label_set_text(text6,"统计");//
    lv_obj_align(text6, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -5, -5);

    majiang.current_page = PAGE_PTOTAL ;

}

void drawranking (void)
{
    lv_obj_t * txt1[8];
    uint16_t screenheight = lv_obj_get_height(screenmain);
    uint16_t screenwidth = lv_obj_get_width(screenmain);

    clear_screenmain();

    text1 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text1, &status_bar_style);
	//lv_obj_set_pos(text1, LV_HOR_RES-30, 5);
    lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 0);//左对齐
    lv_label_set_text(text1,"标准分");


    text3 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text3, &status_bar_style);
	lv_label_set_long_mode(text3,LV_LABEL_LONG_BREAK);
	//lv_obj_set_pos(text3, LV_HOR_RES-30, 5);
    lv_obj_align(text3, NULL, LV_ALIGN_IN_TOP_LEFT, 10, screenheight/5);
    lv_label_set_text(text3,"东\n南\n西\n北");

    text2 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text2, &status_bar_style);
	//lv_obj_set_pos(text2, LV_HOR_RES-30, 5);
    lv_obj_align(text2, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, 0);
    lv_label_set_text(text2,"比赛分");


	text7 = lv_label_create(screenmain, NULL);
    lv_label_set_style(text7, &status_bar_style);
    lv_obj_align(text7, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -5, -5);
	lv_label_set_text(text7,"返回");//统计


    txt1[0] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[0], &status_bar_style);
	//lv_obj_set_pos(txt1[0], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[0], NULL, LV_ALIGN_IN_TOP_LEFT, 50, screenheight/5);
    lv_label_set_text(txt1[0],(char*)majiang.standard_score[0]);
    txt1[1] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[1], &status_bar_style);
	//lv_obj_set_pos(txt1[1], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[1], NULL, LV_ALIGN_IN_TOP_LEFT, screenwidth-80, screenheight/5);
    lv_label_set_text(txt1[1],(char*)majiang.match_score[0]);

     txt1[2] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[2], &status_bar_style);
	//lv_obj_set_pos(txt1[2], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[2], NULL, LV_ALIGN_IN_TOP_LEFT, 50, screenheight/5*2);
    lv_label_set_text(txt1[2],(char*)majiang.standard_score[1]);
    txt1[3] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[3], &status_bar_style);
	//lv_obj_set_pos(txt1[3], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[3], NULL, LV_ALIGN_IN_TOP_LEFT, screenwidth-80, screenheight/5*2);
    lv_label_set_text(txt1[3],(char*)majiang.match_score[1]);

    txt1[4] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[4], &status_bar_style);
	//lv_obj_set_pos(txt1[2], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[4], NULL, LV_ALIGN_IN_TOP_LEFT, 50, screenheight/5*3);
    lv_label_set_text(txt1[4],(char*)majiang.standard_score[2]);
    txt1[5] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[5], &status_bar_style);
	//lv_obj_set_pos(txt1[3], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[5], NULL, LV_ALIGN_IN_TOP_LEFT, screenwidth-80, screenheight/5*3);
    lv_label_set_text(txt1[5],(char*)majiang.match_score[2]);

    txt1[6] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[6], &status_bar_style);
	//lv_obj_set_pos(txt1[2], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[6], NULL, LV_ALIGN_IN_TOP_LEFT, 50,  screenheight/5*4);
    lv_label_set_text(txt1[6],(char*)majiang.standard_score[3]);
    txt1[7] = lv_label_create(screenmain, NULL);
	lv_label_set_style(txt1[7], &status_bar_style);
	//lv_obj_set_pos(txt1[3], LV_HOR_RES-30, 5);
    lv_obj_align(txt1[7], NULL, LV_ALIGN_IN_TOP_LEFT, screenwidth -80 ,  screenheight/5*4);
    lv_label_set_text(txt1[7],(char*)majiang.match_score[3]);


    

    lv_style_copy(&temp_style,&status_bar_style);
	//temp_style.body.main_color =LV_COLOR_RED ;
	//temp_style.body.grad_color = LV_COLOR_RED;
	//temp_style.body.border.color = LV_COLOR_WHITE ;
	//temp_style.body.border.width = 2 ;
	temp_style.text.color = LV_COLOR_RED;

    switch(majiang.position)
    {
        case DIR_EAST :
            lv_label_set_style(txt1[0], &temp_style);
            lv_label_set_style(txt1[1], &temp_style);
        break;
        case DIR_SOUTH :
            lv_label_set_style(txt1[2], &temp_style);
            lv_label_set_style(txt1[3], &temp_style);
        break;
        case DIR_WEST :
            lv_label_set_style(txt1[4], &temp_style);
            lv_label_set_style(txt1[5], &temp_style);
        break;
        case DIR_NORTH :
            lv_label_set_style(txt1[6], &temp_style);
            lv_label_set_style(txt1[7], &temp_style);
        break;
        default        :
            break;

    }

    majiang.current_page = PAGE_RANKING ;
}


void drawscore (char t1[][MAXCHARNUM])
{
    lv_obj_t * txtobj[5];
    lv_obj_t * txt1[5];

    uint16_t screenheight = lv_obj_get_height(screenmain);
    uint16_t screenwidth = lv_obj_get_width(screenmain);

    clear_screenmain();


    text2 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text2, &status_bar_style);
	//lv_obj_set_pos(text2, LV_HOR_RES-30, 5);
    lv_obj_align(text2, NULL, LV_ALIGN_IN_BOTTOM_LEFT,10, -10);
    lv_label_set_text(text2,"确认分数");

    text3 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text3, &status_bar_style);
	//lv_obj_set_pos(text3, LV_HOR_RES-30, 5);
    lv_obj_align(text3, NULL, LV_ALIGN_IN_BOTTOM_RIGHT,-20, -10);
    lv_label_set_text(text3,"改分");


    text4 = lv_label_create(screenmain, NULL);
	lv_label_set_style(text4, &status_bar_style);
	//lv_obj_set_pos(text4, LV_HOR_RES-30, 5);
    lv_obj_align(text4, NULL, LV_ALIGN_IN_BOTTOM_MID,10, -10);

    char str11[5];
    unsigned char temp = 0 ,temp1 = 0 ;
    unsigned char tempi=0;//临时变量
                //temp  有多少页   temp1 最后一页有几条
    switch(majiang.position)
    {
        case DIR_EAST :
            lv_label_set_text(text4,majiang.current_game_score[0]);
        break;
        case DIR_SOUTH :
            lv_label_set_text(text4,majiang.current_game_score[1]);
        break;
        case DIR_WEST :
            lv_label_set_text(text4,majiang.current_game_score[2]);
        break;
        case DIR_NORTH :
            lv_label_set_text(text4,majiang.current_game_score[3]);
        break;
        default        :
            break;

    }

    if(majiang.inform_nums)
    {
        temp1 = majiang.inform_nums%4;//最后一页有几条
        temp = majiang.inform_nums/4 + ((majiang.inform_nums%4)?1:0);//有多少页
        if(majiang.current_informnum>temp)majiang.current_informnum=1;
        if(temp>majiang.current_informnum||((temp==majiang.current_informnum)&&(temp1==0)))//当前显示页面小于总页面数
        {
            for(tempi=0;tempi<4;tempi++)
            {
                txtobj[tempi] = lv_obj_create(screenmain, NULL);
                lv_obj_set_style(txtobj[tempi], &status_bar_style);
                lv_obj_set_size(txtobj[tempi], screenwidth/3*2, screenheight/6);
                lv_obj_align(txtobj[tempi], NULL,  LV_ALIGN_IN_TOP_MID,10,10+2*tempi+screenheight/6 * tempi);
                //printf("%d\n",10+2*tempi+screenheight/6 * tempi);
                //printf("%d\n",majiang.current_informnum*4-4+tempi);
                txt1[tempi] = lv_label_create(txtobj[tempi], NULL);
                lv_label_set_style(txt1[tempi], &status_bar_style);
                lv_label_set_text(txt1[tempi],t1[majiang.current_informnum*4-4+tempi]);
            }
            /*
            txtobj[0] = lv_obj_create(screenmain, NULL);
            lv_obj_set_style(txtobj[0], &status_bar_style);
            lv_obj_set_size(txtobj[0], screenwidth/3*2, screenheight/6);
            lv_obj_align(txtobj[0], NULL, LV_ALIGN_IN_TOP_MID,10, 10);

            txt1[0] = lv_label_create(txtobj[0], NULL);
            lv_label_set_style(txt1[0], &status_bar_style);
            lv_label_set_text(txt1[0],t1[majiang.current_informnum*4-4]);

            txtobj[1] = lv_obj_create(screenmain, txtobj[0]);
            lv_obj_set_style(txtobj[1], &status_bar_style);
            lv_obj_set_size(txtobj[1], screenwidth/3*2, screenheight/6);
            lv_obj_align(txtobj[1], NULL, LV_ALIGN_IN_TOP_MID,10, 10+2 +screenheight/6);

            txt1[1] = lv_label_create(txtobj[1], NULL);
            lv_label_set_style(txt1[1], &status_bar_style);
            lv_label_set_text(txt1[1],t1[majiang.current_informnum*4-3]);

            txtobj[2] = lv_obj_create(screenmain, txtobj[0]);
            lv_obj_set_style(txtobj[2], &status_bar_style);
            lv_obj_set_size(txtobj[2], screenwidth/3*2, screenheight/6);
            lv_obj_align(txtobj[2], NULL, LV_ALIGN_IN_TOP_MID,10, 10+2+2 +screenheight/6 * 2);

            txt1[2] = lv_label_create(txtobj[2], NULL);
            lv_label_set_style(txt1[2], &status_bar_style);
            lv_label_set_text(txt1[2],t1[majiang.current_informnum*4-2]);

            txtobj[3] = lv_obj_create(screenmain, txtobj[0]);
            lv_obj_set_style(txtobj[3], &status_bar_style);
            lv_obj_set_size(txtobj[3], screenwidth/3*2, screenheight/6);
            lv_obj_align(txtobj[3], NULL, LV_ALIGN_IN_TOP_MID,10, 10+2+2+2 +screenheight/6 * 3);

            txt1[3] = lv_label_create(txtobj[3], NULL);
            lv_label_set_style(txt1[3], &status_bar_style);
            lv_label_set_text(txt1[3],t1[majiang.current_informnum*4-1]);
            */

        }

        else if((temp==majiang.current_informnum)&&(temp1>0))
        {

            for(tempi=0;tempi<temp1;tempi++)
            {
                txtobj[tempi] = lv_obj_create(screenmain, NULL);
                lv_obj_set_style(txtobj[tempi], &status_bar_style);
                lv_obj_set_size(txtobj[tempi], screenwidth/3*2, screenheight/6);
                lv_obj_align(txtobj[tempi], NULL,  LV_ALIGN_IN_TOP_MID,10,10+2*tempi+screenheight/6 * tempi);
                //printf("%d\n",10+2*tempi+screenheight/6 * tempi);
                //printf("%d\n",majiang.current_informnum*4-4+tempi);
                txt1[tempi] = lv_label_create(txtobj[tempi], NULL);
                lv_label_set_style(txt1[tempi], &status_bar_style);
                lv_label_set_text(txt1[tempi],t1[majiang.current_informnum*4-4+tempi]);
            }
        }

        if(temp>1)
        {

            text1 = lv_label_create(screenmain, NULL);
            //lv_obj_set_pos(text1, LV_HOR_RES-30, 5);
            lv_obj_set_size(text1,30,50);
            lv_label_set_style(text1, &status_bar_style);
            lv_label_set_long_mode(text3,LV_LABEL_LONG_BREAK);
            lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 10);//左对齐
            lv_label_set_text(text1,"下\n一\n页");

            text6 = lv_obj_create(screenmain, txtobj[0]);
            lv_obj_set_style(text6, &status_bar_style);
            lv_obj_set_size(text6,20, 20);
            lv_obj_align(text6, NULL, LV_ALIGN_IN_BOTTOM_RIGHT,0,-35);

            text7 = lv_label_create(text6, NULL);
            lv_label_set_style(text7, &status_bar_style);

            sprintf(str11,"%d",temp);//流水总条数
            lv_label_set_text(text7,str11);

            text5 = lv_label_create(screenmain, NULL);
            lv_label_set_style(text5, &status_bar_style);
            //lv_obj_set_pos(text5, LV_HOR_RES-30, 5);
            lv_obj_align(text5, NULL, LV_ALIGN_IN_BOTTOM_RIGHT,30, -55);
            sprintf(str11,"%d",majiang.current_informnum);//当前流水页面
            lv_label_set_text(text5,str11);
        }
    }
    majiang.current_page = PAGE_SCORE ;

}


void drawwaiting (void)
{
    lv_obj_t * txtobj[4];
    lv_obj_t * txt1[4];
    char * str11[10];

    uint16_t screenheight = lv_obj_get_height(screenmain);
    uint16_t screenwidth = lv_obj_get_width(screenmain);

    clear_screenmain();

    lv_style_copy(&temp_style,&status_bar_style);
	temp_style.body.main_color =LV_COLOR_BLACK ;
	temp_style.body.grad_color = LV_COLOR_BLACK;
	temp_style.body.border.color = LV_COLOR_WHITE ;
	temp_style.body.border.width = 2 ;

    txtobj[0] = lv_obj_create(screenmain, NULL);
	lv_obj_set_style(txtobj[0], &temp_style);
	lv_obj_set_size(txtobj[0], screenwidth/4, screenheight/4*3);
    lv_obj_align(txtobj[0], NULL, LV_ALIGN_IN_TOP_LEFT,0, 0);

    txt1[0] = lv_label_create(txtobj[0], NULL);
    lv_label_set_long_mode(txt1[0],LV_LABEL_LONG_BREAK);
	lv_label_set_style(txt1[0], &temp_style);

	sprintf(str11,"  东\n   %c\n  %s",majiang.current_game_score[0][0],&majiang.current_game_score[0][1]);
	//sprintf(str11,"  东\n\n  %s",majiang.current_game_score[0]);
	lv_label_set_text(txt1[0],str11);

	txtobj[1] = lv_obj_create(screenmain, NULL);
	lv_obj_set_style(txtobj[1], &temp_style);
	lv_obj_set_size(txtobj[1], screenwidth/4, screenheight/4*3);
    lv_obj_align(txtobj[1], NULL, LV_ALIGN_IN_TOP_LEFT,screenwidth/4, 0);

    txt1[1] = lv_label_create(txtobj[1], NULL);
    lv_label_set_long_mode(txt1[1],LV_LABEL_LONG_BREAK);
	lv_label_set_style(txt1[1], &temp_style);

	//sprintf(str11,"  南\n\n  %s",majiang.current_game_score[1]);
	sprintf(str11,"  南\n   %c\n  %s",majiang.current_game_score[1][0],&majiang.current_game_score[1][1]);
	
	lv_label_set_text(txt1[1],str11);

    txtobj[2] = lv_obj_create(screenmain, NULL);
	lv_obj_set_style(txtobj[2], &temp_style);
	lv_obj_set_size(txtobj[2], screenwidth/4, screenheight/4*3);
    lv_obj_align(txtobj[2], NULL, LV_ALIGN_IN_TOP_LEFT,(screenwidth/4)*2, 0);

    txt1[2] = lv_label_create(txtobj[2], NULL);
    lv_label_set_long_mode(txt1[2],LV_LABEL_LONG_BREAK);
	lv_label_set_style(txt1[2], &temp_style);
	
	//sprintf(str11,"  西\n\n  %s",majiang.current_game_score[2]);
	sprintf(str11,"  西\n   %c\n  %s",majiang.current_game_score[2][0],&majiang.current_game_score[2][1]);
	
	lv_label_set_text(txt1[2],str11);

	txtobj[3] = lv_obj_create(screenmain, NULL);
	lv_obj_set_style(txtobj[3], &temp_style);
	lv_obj_set_size(txtobj[3], screenwidth/4, screenheight/4*3);
    lv_obj_align(txtobj[3], NULL, LV_ALIGN_IN_TOP_LEFT,(screenwidth/4)*3, 0);

    txt1[3] = lv_label_create(txtobj[3], NULL);
    lv_label_set_long_mode(txt1[3],LV_LABEL_LONG_BREAK);
	lv_label_set_style(txt1[3], &temp_style);
	
	//sprintf(str11,"  北\n\n  %s",majiang.current_game_score[3]);
	sprintf(str11,"  北\n   %c\n  %s",majiang.current_game_score[3][0],&majiang.current_game_score[3][1]);
	
	lv_label_set_text(txt1[3],str11);

    lv_style_copy(&temp_style1,&status_bar_style);
	temp_style1.body.main_color =LV_COLOR_GREEN ;
	temp_style1.body.grad_color = LV_COLOR_GREEN;
	temp_style1.body.border.color = LV_COLOR_WHITE ;
	temp_style1.body.border.width = 2 ;
	
	lv_style_copy(&temp_style2,&status_bar_style);
	temp_style2.body.main_color =LV_COLOR_RED ;
	temp_style2.body.grad_color = LV_COLOR_RED;
	temp_style2.body.border.color = LV_COLOR_WHITE ;
	temp_style2.body.border.width = 2 ;


	if(majiang.current_keyV[0] == KEYV_Y)
	{
		text1 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text1, &temp_style1);
		lv_obj_set_size(text1, screenwidth/4, screenheight/4);
		lv_obj_align(text1, NULL, LV_ALIGN_IN_BOTTOM_LEFT,0, 0);

		text2 = lv_label_create(text1, NULL);
		lv_label_set_long_mode(text2,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text2, &temp_style1);
		lv_obj_align(text2, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text2,"已确");
	}
	else if(majiang.current_keyV[0] == KEYV_N)
	{
		text1 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text1, &temp_style2);
		lv_obj_set_size(text1, screenwidth/4, screenheight/4);
		lv_obj_align(text1, NULL, LV_ALIGN_IN_BOTTOM_LEFT,0, 0);

		text2 = lv_label_create(text1, NULL);
		lv_label_set_long_mode(text2,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text2, &temp_style2);
		lv_obj_align(text2, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text2,"异议");
		
	}
	else if(majiang.current_keyV[0] == KEYV_U)
	{
		text1 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text1, &status_bar_style);
		lv_obj_set_size(text1, screenwidth/4, screenheight/4);
		lv_obj_align(text1, NULL, LV_ALIGN_IN_BOTTOM_LEFT,0, 0);

		text2 = lv_label_create(text1, NULL);
		lv_label_set_long_mode(text2,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text2, &status_bar_style);
		lv_obj_align(text2, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text2,"待确");
		
	}
	
	if(majiang.current_keyV[1] == KEYV_Y)
	{
		text3 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text3, &temp_style1);
		lv_obj_set_size(text3, screenwidth/4, screenheight/4);
		lv_obj_align(text3, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*1, 0);

		text4 = lv_label_create(text3, NULL);
		lv_label_set_long_mode(text4,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text4, &temp_style1);
		lv_obj_align(text4, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text4,"已确");
	}
	else if(majiang.current_keyV[1] == KEYV_N)
	{
		text3 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text3, &temp_style2);
		lv_obj_set_size(text3, screenwidth/4, screenheight/4);
		lv_obj_align(text3, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*1, 0);

		text4 = lv_label_create(text3, NULL);
		lv_label_set_long_mode(text4,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text4, &temp_style2);
		lv_obj_align(text4, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text4,"异议");
		
	}
	else if(majiang.current_keyV[1] == KEYV_U)
	{
		
		text3 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text3, &status_bar_style);
		lv_obj_set_size(text3, screenwidth/4, screenheight/4);
		lv_obj_align(text3, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*1, 0);

		text4 = lv_label_create(text3, NULL);
		lv_label_set_long_mode(text4,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text4, &status_bar_style);
		lv_obj_align(text4, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text4,"待确");
	}
	
	if(majiang.current_keyV[2] == KEYV_Y)
	{
		text5 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text5, &temp_style1);
		lv_obj_set_size(text5, screenwidth/4, screenheight/4);
		lv_obj_align(text5, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*2, 0);

		text6 = lv_label_create(text5, NULL);
		lv_label_set_long_mode(text6,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text6, &temp_style1);
		lv_obj_align(text6, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text6,"已确");
	}
	else if(majiang.current_keyV[2] == KEYV_N)
	{
		text5 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text5, &temp_style2);
		lv_obj_set_size(text5, screenwidth/4, screenheight/4);
		lv_obj_align(text5, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*2, 0);

		text6 = lv_label_create(text5, NULL);
		lv_label_set_long_mode(text6,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text6, &temp_style2);
		lv_obj_align(text6, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text6,"异议");
		
	}
	else if(majiang.current_keyV[2] == KEYV_U)
	{
		
		text5 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text5, &status_bar_style);
		lv_obj_set_size(text5, screenwidth/4, screenheight/4);
		lv_obj_align(text5, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*2, 0);

		text6 = lv_label_create(text5, NULL);
		lv_label_set_long_mode(text6,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text6, &status_bar_style);
		lv_obj_align(text6, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text6,"待确");
	}
	

	if(majiang.current_keyV[3] == KEYV_Y)
	{
		text7 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text7, &temp_style1);
		lv_obj_set_size(text7, screenwidth/4, screenheight/4);
		lv_obj_align(text7, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*3, 0);

		text8 = lv_label_create(text7, NULL);
		lv_label_set_long_mode(text8,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text8, &temp_style1);
		lv_obj_align(text8, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text8,"已确");
	}
	else if(majiang.current_keyV[3] == KEYV_N)
	{
		text7 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text7, &temp_style2);
		lv_obj_set_size(text7, screenwidth/4, screenheight/4);
		lv_obj_align(text7, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*3, 0);

		text8 = lv_label_create(text7, NULL);
		lv_label_set_long_mode(text8,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text8, &temp_style2);
		lv_obj_align(text8, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text8,"待确");
		
	}
	else if(majiang.current_keyV[3] == KEYV_U)
	{
		
		text7 = lv_obj_create(screenmain, NULL);
		lv_obj_set_style(text7, &status_bar_style);
		lv_obj_set_size(text7, screenwidth/4, screenheight/4);
		lv_obj_align(text7, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/4)*3, 0);

		text8 = lv_label_create(text7, NULL);
		lv_label_set_long_mode(text8,LV_LABEL_LONG_BREAK);
		lv_label_set_style(text8, &status_bar_style);
		lv_obj_align(text8, NULL, LV_ALIGN_CENTER,0, 0);
		lv_label_set_text(text8,"待确");
	}
	

	

	

	majiang.current_page = PAGE_CWAITING ;
  //
}

void drawmodify (void)
{
    uint16_t screenheight = lv_obj_get_height(screenmain);
    uint16_t screenwidth = lv_obj_get_width(screenmain);
    char str11[15];
    clear_screenmain();

    text1 = lv_label_create(screenmain, NULL);
    lv_label_set_recolor(text1,true);

    lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_MID, -30, 10);//左对齐
    lv_label_set_style(text1, &status_bar_style);
    lv_label_set_text(text1,"#ff0000 分数已修改#");

    text2 = lv_label_create(screenmain, NULL);
    lv_label_set_recolor(text2,true);

    lv_obj_align(text2, NULL, LV_ALIGN_IN_TOP_MID, 0, screenheight/3);//左对齐
    lv_label_set_style(text2, &status_bar_style);

    switch(majiang.position)
    {
        case DIR_EAST  :
			sprintf(str11,"#ff0000 %s #",majiang.current_game_score[0]);
			
            break;
        case DIR_SOUTH :
            sprintf(str11,"#ff0000 %s #",majiang.current_game_score[1]);
            break;
        case DIR_WEST  :
            sprintf(str11,"#ff0000 %s #",majiang.current_game_score[2]);
            break;
        case DIR_NORTH :
            sprintf(str11,"#ff0000 %s #",majiang.current_game_score[3]);
            break;
        default        :
            sprintf(str11,"dir err");
            break;
    }

    lv_label_set_text(text2,str11);

    lv_style_copy(&temp_style,&status_bar_style);
	//temp_style.body.main_color =LV_COLOR_BLACK ;
	//temp_style.body.grad_color = LV_COLOR_BLACK;
	temp_style.body.border.color = LV_COLOR_WHITE ;
	temp_style.body.border.width = 2 ;

	text5 = lv_obj_create(screenmain, NULL);
	lv_obj_set_style(text5, &temp_style);
	lv_obj_set_size(text5, screenwidth/4, screenheight/4);
    lv_obj_align(text5, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/6), -10);

    text6 = lv_label_create(text5, NULL);
    lv_label_set_long_mode(text6,LV_LABEL_LONG_BREAK);
	lv_label_set_style(text6, &temp_style);
	lv_obj_align(text6, NULL, LV_ALIGN_CENTER,0, 0);
	lv_label_set_text(text6,"确认");

	text7 = lv_obj_create(screenmain, NULL);
	lv_obj_set_style(text7, &temp_style);
	lv_obj_set_size(text7, screenwidth/4, screenheight/4);
    lv_obj_align(text7, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/5)*3, -10);

    text8 = lv_label_create(text7, NULL);
    lv_label_set_long_mode(text8,LV_LABEL_LONG_BREAK);
	lv_label_set_style(text8, &temp_style);
	lv_obj_align(text8, NULL, LV_ALIGN_CENTER,0, 0);
	lv_label_set_text(text8,"异议");

	majiang.current_page = PAGE_MODIFY ;

}

void drawmodify1 (void)
{
    uint16_t screenheight = lv_obj_get_height(screenmain);
    uint16_t screenwidth = lv_obj_get_width(screenmain);

    clear_screenmain();
    text1 = lv_label_create(screenmain, NULL);
    lv_label_set_recolor(text1,true);

    lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_MID, -30, 10);//左对齐
    lv_label_set_style(text1, &status_bar_style);
    lv_label_set_text(text1,"#ff0000 也呼叫裁判#");

    text2 = lv_label_create(screenmain, NULL);
    lv_label_set_recolor(text2,true);

    lv_obj_align(text2, NULL, LV_ALIGN_IN_TOP_MID, 0, screenheight/3);//左对齐
    lv_label_set_style(text2, &status_bar_style);
    lv_label_set_text(text2,"#ffffff 请等待...#");

    lv_style_copy(&temp_style,&status_bar_style);
	//temp_style.body.main_color =LV_COLOR_BLACK ;
	//temp_style.body.grad_color = LV_COLOR_BLACK;
	temp_style.body.border.color = LV_COLOR_WHITE ;
	temp_style.body.border.width = 2 ;

	text7 = lv_obj_create(screenmain, NULL);
	lv_obj_set_style(text7, &temp_style);
	lv_obj_set_size(text7, screenwidth/4*3, screenheight/4);
    lv_obj_align(text7, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(screenwidth/7), -10);

    text8 = lv_label_create(text7, NULL);
    //lv_label_set_long_mode(text8,LV_LABEL_LONG_BREAK);
	lv_label_set_style(text8, &temp_style);
	lv_obj_align(text8, NULL, LV_ALIGN_IN_TOP_MID,-20, 5);
	lv_label_set_text(text8,"取消呼叫");

	majiang.current_page = PAGE_MODIFY1 ;

}



void drawwificonTips (void)
{

	char num = 0 ;
	char strtemp[40];

    clear_screenmain();
	majiang.current_page = PAGE_TIPs ;
	num = 0 ;
    while(!wifi_connected)
    {
    	if(num==1)
		{
	        if(majiang.wifi_datasave)
	        {
	            text1 = lv_label_create(screenmain, NULL);

	            lv_label_set_style(text1, &status_bar_style);
	            //lv_obj_set_pos(text1, LV_HOR_RES-30, 5);
	            lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 0);//左对齐
				sprintf(strtemp,"连接WIFI:%s" ,majiang.wifissid);
				lv_label_set_text(text1,strtemp);
	        }
	        else
	        {
	            text2 = lv_label_create(screenmain, NULL);
	            lv_label_set_style(text2, &status_bar_style);
	            //lv_obj_set_pos(text2, LV_HOR_RES-30, 5);
	            lv_obj_align(text2, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 0);//左对齐
	            lv_label_set_text(text2,"请用手机配置wifi");
				num = 6 ;
			}
		}
		else
		{
			if(num == 5)
			{
				text3 = lv_label_create(screenmain, NULL);

		        lv_label_set_style(text3, &status_bar_style);
		        lv_obj_align(text3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 30);//左对齐
				//sprintf(strtemp,"" ,majiang.wifissid);
				lv_label_set_text(text3,"连接失败,请检查路由器");
				text4 = lv_label_create(screenmain, NULL);

		        lv_label_set_style(text4, &status_bar_style);
		        lv_obj_align(text4, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 60);//左对齐
				//sprintf(strtemp,"" ,majiang.wifissid);
				lv_label_set_text(text4,"或者重新配置WiFi");
			}
		}
		num++;
		lv_label_set_text(wifi_icon, wifi_connected ?  (server_connected ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
	
		vTaskDelay(2000 / portTICK_RATE_MS);

    }

    clear_screenmain();
	lv_label_set_text(wifi_icon, wifi_connected ?  (server_connected ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
		
	text1 = lv_label_create(screenmain, NULL);

    lv_label_set_style(text1, &status_bar_style);
    lv_obj_align(text1, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 0);//左对齐

	sprintf(strtemp,"连接成功:%s" ,majiang.wifissid);
	
	lv_label_set_text(text1,strtemp);
	
	/*
	num = 0 ;
    while(!server_connected)
    {
    		num++;
			if(num == 1)
			{
		        text2 = lv_label_create(screenmain, NULL);

		        lv_label_set_style(text2, &status_bar_style);
		        lv_obj_align(text2, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 30);//左对齐
		        lv_label_set_text(text2,"等待连接服务器");
			}
			else if(num==5)
			{
				text3 = lv_label_create(screenmain, NULL);

		        lv_label_set_style(text3, &status_bar_style);
		        lv_obj_align(text3, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 60);//左对齐
		        lv_label_set_text(text3,"请检查WIFI网络连接");

				text4 = lv_label_create(screenmain, NULL);

		        lv_label_set_style(text4, &status_bar_style);
		        lv_obj_align(text4, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 90);//左对齐
		        lv_label_set_text(text4,"请检查服务器");
			
			}

		if(!wifi_connected)
		{
			lv_label_set_text(text1,"WIFI 断开");
			lv_label_set_text(wifi_icon, wifi_connected ?  (server_connected ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
		}
		vTaskDelay(2000 / portTICK_RATE_MS);
    }
	lv_label_set_text(wifi_icon, wifi_connected ?  (server_connected ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
		
    lv_label_set_text(text2,"连接服务器成功");
	lv_label_set_text(text3," ");
	*/
    majiang.current_page = PAGE_TIPs ;
}



void wifi_set_stat(bool c) {
	wifi_connected = c;
	majiang.wifi_connect_state =c ;
}
void server_set_stat(bool c) {
	server_connected = c;
	majiang.server_connect_state = c ;
}


void taskUI_Char(void *parameter) {
	//th = lv_theme_material_init(210, NULL);
	//lv_theme_set_current(th);
	//ESP_LOGE(TAG, "enter taskui");
	//char arrtext[6][MAXCHARNUM]={"胡：自摸 -2","杠：点炮 -22","碰： +8 东","胡： +2 西","好： +2 东","不阿： +2 东"};

	//vTaskDelay(2000 / portTICK_RATE_MS);
	style_init();
	drawBottom ();

	drawwificonTips ();
	
	drawlogo ();
	vTaskDelay(3000 / portTICK_RATE_MS);
	//drawwaiting ();
	while(1) {//TEST
	
		printf("uitask is run\r\n");
		/*
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		drawordinary ();
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		startus_bar_set();
		drawsmart1 ();
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		drawptotal ();
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		drawranking ();
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		drawscore (arrtext);
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		drawwaiting ();
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		drawmodify1 ();
		printf("page %d\r\n",majiang.current_page);
		vTaskDelay(2000 / portTICK_RATE_MS);
		drawmodify();
		printf("page %d\r\n",majiang.current_page);
		*/
		ESP_LOGI("== ==", "[APP] Free memory: %d bytes", esp_get_free_heap_size());
		
		vTaskDelay(2000 / portTICK_RATE_MS);
		lv_label_set_text(wifi_icon, wifi_connected ?  (server_connected ? SYMBOL_WIFI""SYMBOL_OK : SYMBOL_WIFI""SYMBOL_WARNING) : SYMBOL_WARNING""SYMBOL_WARNING);
		/*
		if(wifi_connected&server_connected)
			lv_label_set_text(wifi_icon,SYMBOL_WIFI""SYMBOL_OK);
		else if((wifi_connected&(!server_connected)))
			lv_label_set_text(wifi_icon,SYMBOL_WIFI);
		else
			lv_label_set_text(wifi_icon,SYMBOL_WARNING);
		*/
		
		/*
		clear_screenmain();
		vTaskDelay(2000 / portTICK_RATE_MS);
		//lv_task_enable(false);
		//draw_qrimg(str1test);
		//lv_task_enable(true);
		vTaskDelay(5000 / portTICK_RATE_MS);
		draw_screenmain();
		//clear_screenmain();
		//drawlogo ();
		*/
	}
}




