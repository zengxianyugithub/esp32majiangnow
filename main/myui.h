#ifndef _MYUI_H_
#define _MYUI_H_
#include "lvgl/lvgl.h"

#define STATUS_BAR_VER  50

#define PAGE_LOUGOU    1
#define PAGE_STARTUP   2
#define PAGE_ORDINARY  3
#define PAGE_SMART1    4
#define PAGE_SCORE     5
#define PAGE_MODIFY    6
#define PAGE_MODIFY1   7
#define PAGE_PTOTAL    8
#define PAGE_RANKING   9
#define PAGE_CWAITING  10
#define PAGE_TIPs      11
#define PAGE_POINT12   12

#define KEYV_Y          1
#define KEYV_N          2
#define KEYV_U          3

#define AUDIO_NULL      0
#define AUDIO_1         1
#define AUDIO_2         2
#define AUDIO_3         3
#define AUDIO_4         4



#define DIR_EAST       1
#define DIR_SOUTH      2
#define DIR_WEST       3
#define DIR_NORTH      4

//流水最长长度
#define MAXCHARNUM     30

typedef struct {
    char Table_number[10];//桌号
    char Table_num;//桌号
    char position;//方位  1:东  2:南    3:西  4:北
    char game_mode;//游戏模式  0:单机    1:智能
    bool wifi_connect_state;//wifi连接状态
    bool server_connect_state;//server连接状态
	char wifissid[30];
	
	bool wifi_datasave ;
    char QR_link[100] ;//二维码链接
    char standard_score[4][4];//东南西北标准分
    char match_score[4][4];//东南西北比赛分
    char inform_nums;//总流水条数
    char current_informnum; //当前显示的流水数
    char games;//总盘数
    char current_game;//当前盘数
    char current_page;//当前页面   0:初始化页码
    unsigned int confirm_num :4 ;//已确认分数个数  1111 全部确认 0000 都没确认
    char current_game_score[4][4];//当前盘数的分数
	char current_keyV[4];//当前各个方位的按键状态  1 确认  2 有异议 3 不确定还没有按键  
	char point1;//拿牌顺序1
	char point2;//拿牌顺序2
	char current_audio;
	char volume;//音频音量大小 0-100
	double volumeMultiplier;//声音系数

}struct_majiang;


extern lv_font_t myfont;
extern bool wifi_connected  ;
extern bool server_connected;
extern struct_majiang majiang ;

void wifi_set_stat(bool c);
void server_set_stat(bool c);
void draw_qrimg(char *str);


void lv_restart_test1(void);//sotp

void lv_restart_test(void);//run


void drawordinary (void);
void drawsmart1 (void);
void drawptotal (void);
void drawranking (void);
void drawscore (char t1[][MAXCHARNUM]);
void drawwaiting (void);
void drawmodify1 (void);
void drawmodify(void);
void drawpoint1_point2 (unsigned char point1, unsigned char point2);

void clear_screenmain();
void clear_status_bar();



void taskUI_Char(void *parameter);


#endif
