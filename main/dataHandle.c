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
#include "dataHandle.h"
#include "cJSON.h"
#include "i2s_dac_demo.h"

static const char *TAG = "cJSON";
QueueHandle_t Queue_Tcprecvdata;

QueueHandle_t Queue_Tcpsenddata;

esp_err_t TcprecvdataQueueCreate() {
	Queue_Tcprecvdata = xQueueCreate(10, 512);
	if(Queue_Tcprecvdata == 0) return ESP_FAIL;

	return ESP_OK;
}

esp_err_t TcpsenddataQueueCreate() {
	Queue_Tcpsenddata = xQueueCreate(10, 512);
	if(Queue_Tcpsenddata == 0) return ESP_FAIL;

	return ESP_OK;
}

void tcpdataretrun(char *str)
{
    
    cJSON *root;
	char dir[2];
	dir[0]='A'+majiang.position-1;
	dir[1]=0;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "clientId", cJSON_CreateString(majiang.Table_number));
    cJSON_AddItemToObject(root, "actionCode", cJSON_CreateString(str));
	cJSON_AddItemToObject(root, "pos", cJSON_CreateString(dir));
    
	char *buf = cJSON_PrintUnformatted(root);

 	//printf("return  json data\r\n");
	xQueueSend(Queue_Tcpsenddata, (void*)buf, (TickType_t) 10);

	free(buf); //释放内存
	
    cJSON_Delete(root);
}

void printJson(cJSON * root)//以递归的方式打印json的最内层键值对
{
    for(int i=0; i<cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
    {
        cJSON * item = cJSON_GetArrayItem(root, i);        
        if(cJSON_Object == item->type)      //如果对应键的值仍为cJSON_Object就递归调用printJson
            printJson(item);
        else                                //值不为json对象就直接打印出键和值
        {
            printf("%s->", item->string);
            printf("%s\n", cJSON_Print(item));
        }
    }
}


/*解析json数据 只处理 解析 
* @param[in]   text  		       :json字符串
* @retval      void                 :无
* 
*               
*/



void cjson_to_struct_info(char *text)
{
    cJSON *root,*psub;
	cJSON *tempobj ,*tempobj1 ;
	char *tempstr ;
	char *pjsonprint;
    //cJSON *arrayItem;
	int arrnum =0,i=0;
	// //ESP_LOGI(TAG,"===%s===",text);
    //截取有效json

    //char *index=strchr(text,'{');
    //strcpy(text,index);

    root = cJSON_Parse(text);
    
    if(root!=NULL)
    {
		//printJson(root);
		//printf("this is a json data\r\n");
		pjsonprint = cJSON_Print(root) ;
		if(strstr(pjsonprint, "\"actionCode\"") > 0)
		{
			psub = cJSON_GetObjectItem(root, "actionCode");//动作
			if(psub!=NULL)
			{
				
				//char *tempstr ;
				ESP_LOGI(TAG,"===%s===",psub->valuestring );
				if(strcmp(psub->valuestring, "A001") == 0)     //返回二维码
				{
					//画登录二维码界面
					switch(majiang.position )
					{
						case DIR_EAST :
						//arrayItem = cJSON_GetObjectItem(root,"version");
						if(strstr(pjsonprint, "\"A\"") > 0)
						{
							ESP_LOGI(TAG,"A:%s\r\n",cJSON_GetObjectItem(root, "A")->valuestring);
							drawsmart1 ();
							lv_restart_test1();
							draw_qrimg((cJSON_GetObjectItem(root, "A")->valuestring));
							lv_restart_test();
							
						}
						break ;
						case DIR_SOUTH :
						//arrayItem = cJSON_GetObjectItem(root,"version");
						if(strstr(pjsonprint, "\"B\"") > 0)
						{
							ESP_LOGI(TAG,"B:%s\r\n",cJSON_GetObjectItem(root, "B")->valuestring);
							drawsmart1 ();
							lv_restart_test1();
							draw_qrimg((cJSON_GetObjectItem(root, "B")->valuestring));
							lv_restart_test();
							
						}
						break ;
						case DIR_WEST :
						//arrayItem = cJSON_GetObjectItem(root,"version");
						if(strstr(pjsonprint, "\"C\"") > 0)
						{
							ESP_LOGI(TAG,"C:%s\r\n",cJSON_GetObjectItem(root, "C")->valuestring);
							drawsmart1 ();
							lv_restart_test1();
							draw_qrimg((cJSON_GetObjectItem(root, "C")->valuestring));
							lv_restart_test();
							
						}
						break ;
						case DIR_NORTH :
						//arrayItem = cJSON_GetObjectItem(root,"version");
						if(strstr(pjsonprint, "\"D\"") > 0)
						{
							ESP_LOGI(TAG,"D:%s\r\n",cJSON_GetObjectItem(root, "D")->valuestring);
							drawsmart1 ();
							lv_restart_test1();
							draw_qrimg((cJSON_GetObjectItem(root, "D")->valuestring));
							lv_restart_test();
							
						}
						break ;
						
						default :
						
						break;
						
					}
					
					
				}
				else if(strcmp(psub->valuestring, "A002") == 0) //扫码成功
				{
					//ESP_LOGI(TAG,"cJSON_Print(root) %s",cJSON_Print(root));
					
					//ESP_LOGI(TAG,"cJSON_GetObjectItem(root, \"pos\")->valuestring %s",cJSON_GetObjectItem(root, "pos")->valuestring);
					
					
					if(strstr(pjsonprint, "\"pos\"") > 0)
					{
						switch(majiang.position )
						{
							case DIR_EAST :
							if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "A") == 0) 
							{
								//跳转扫码成功界面
								ESP_LOGI(TAG,"A scanning QRcode success");
								drawptotal ();
								
							}
							break ;
							case DIR_SOUTH :
							if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "B") == 0) 
							{
								//跳转扫码成功界面
								ESP_LOGI(TAG,"B scanning QRcode success");
								drawptotal ();
							}
							break ;
							case DIR_WEST :
							if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring) , "C") == 0)
							{
								//跳转扫码成功界面
								ESP_LOGI(TAG,"C scanning QRcode success");
								drawptotal ();
							}
							break ;
							case DIR_NORTH :
							if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "D") == 0) 
							{
								//跳转扫码成功界面
								ESP_LOGI(TAG,"D scanning QRcode success");
								drawptotal ();
							}
							break ;
							
							default :
								ESP_LOGI(TAG,"err  position %d " ,majiang.position);
							break;
						
						}
					}
					else
					{
						ESP_LOGI(TAG,"NOT  pos OBJ ");
					}
					
				}
				else if(strcmp(psub->valuestring, "A003") == 0) //全局同步统计
				{

					cJSON *tempobj2 ,*tempobj3 ,*tempobj4 ,*tempobj5;
					if(strstr(pjsonprint, "\"current\"") > 0)
					{
						tempobj2 = cJSON_GetObjectItem(root, "current");
						if(tempobj2->valueint<100)
							majiang.current_game =  tempobj2->valueint;
					}
					if(strstr(pjsonprint, "\"total\"") > 0)
					{
						tempobj3 = cJSON_GetObjectItem(root, "total");
						if(tempobj3->valueint<100)
							majiang.games =  tempobj3->valueint;
					}
					
					if(strstr(pjsonprint, "\"A\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "A");
						//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(pjsonprint, "\"Score\"") > 0)
						{

							tempobj4 = cJSON_GetObjectItem(tempobj, "Score") ;
							tempstr = tempobj4->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							
							//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
							memset(majiang.match_score[0],0,sizeof(majiang.match_score[0]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.match_score[0], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"A Score =%s",tempstr);
							}
								
							else
							{
								majiang.match_score[0][0]='\\';
								ESP_LOGI(TAG,"A Score too long");
							}
								
						}
						if(strstr(pjsonprint, "\"rank\"") > 0)
						{
							tempobj5 =cJSON_GetObjectItem(tempobj, "rank") ;
							tempstr = tempobj5->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"A rank len = %d  ",strlen(tempstr));
							memset(majiang.standard_score[0],0,sizeof(majiang.standard_score[0]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"A rank =%s",tempstr);
							}
								
							else
							{
								majiang.standard_score[0][0] = '\\';
								ESP_LOGI(TAG,"A rank too long");
							}
								
						}
						
					}


					if(strstr(pjsonprint, "\"B\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "B");
						//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(pjsonprint, "\"Score\"") > 0)
						{

							tempobj4 = cJSON_GetObjectItem(tempobj, "Score") ;
							tempstr = tempobj4->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							
							//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
							memset(majiang.match_score[1],0,sizeof(majiang.match_score[1]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.match_score[1], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"B Score =%s",tempstr);
							}
								
							else
							{
								majiang.match_score[1][0]='\\';
								ESP_LOGI(TAG,"B Score too long");
							}
								
						}
						if(strstr(pjsonprint, "\"rank\"") > 0)
						{
							tempobj5 =cJSON_GetObjectItem(tempobj, "rank") ;
							tempstr = tempobj5->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"A rank len = %d  ",strlen(tempstr));
							memset(majiang.standard_score[1],0,sizeof(majiang.standard_score[1]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.standard_score[1], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"B rank =%s",tempstr);
							}
								
							else
							{
								majiang.standard_score[1][0] = '\\';
								ESP_LOGI(TAG,"B rank too long");
							}
								
						}
						
					}
				

					if(strstr(pjsonprint, "\"C\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "C");
						//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(pjsonprint, "\"Score\"") > 0)
						{

							tempobj4 = cJSON_GetObjectItem(tempobj, "Score") ;
							tempstr = tempobj4->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							
							//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
							memset(majiang.match_score[2],0,sizeof(majiang.match_score[2]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.match_score[2], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"C Score =%s",tempstr);
							}
								
							else
							{
								majiang.match_score[1][0]='\\';
								ESP_LOGI(TAG,"C Score too long");
							}
								
						}
						if(strstr(pjsonprint, "\"rank\"") > 0)
						{
							tempobj5 =cJSON_GetObjectItem(tempobj, "rank") ;
							tempstr = tempobj5->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"A rank len = %d  ",strlen(tempstr));
							memset(majiang.standard_score[2],0,sizeof(majiang.standard_score[2]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.standard_score[2], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"C rank =%s",tempstr);
							}
								
							else
							{
								majiang.standard_score[2][0] = '\\';
								ESP_LOGI(TAG,"C rank too long");
							}
								
						}
						
					}
					
					if(strstr(pjsonprint, "\"D\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "D");
						//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(pjsonprint, "\"Score\"") > 0)
						{

							tempobj4 = cJSON_GetObjectItem(tempobj, "Score") ;
							tempstr = tempobj4->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							
							//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
							memset(majiang.match_score[3],0,sizeof(majiang.match_score[3]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.match_score[3], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"D Score =%s",tempstr);
							}
								
							else
							{
								majiang.match_score[3][0]='\\';
								ESP_LOGI(TAG,"D Score too long");
							}
								
						}
						if(strstr(pjsonprint, "\"rank\"") > 0)
						{
							tempobj5 =cJSON_GetObjectItem(tempobj, "rank") ;
							tempstr = tempobj5->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"A rank len = %d  ",strlen(tempstr));
							memset(majiang.standard_score[3],0,sizeof(majiang.standard_score[3]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.standard_score[3], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"D rank =%s",tempstr);
							}
								
							else
							{
								majiang.standard_score[3][0] = '\\';
								ESP_LOGI(TAG,"D rank too long");
							}
								
						}
						
					}

					
					drawranking ();
					//drawptotal ();
					
					//ESP_LOGI(TAG,"current =    %d",cJSON_GetObjectItem(root, "current")->valueint);
					//ESP_LOGI(TAG,"current =    %d",cJSON_GetObjectItem(root, "total")->valueint);
					majiang.current_keyV[0] = KEYV_U;
					majiang.current_keyV[1] = KEYV_U;
					majiang.current_keyV[2] = KEYV_U;
					majiang.current_keyV[3] = KEYV_U;
					
				}
				
				else if(strcmp(psub->valuestring, "A005") == 0) //结算页面
				{
					if(strstr(pjsonprint, "\"current\"") > 0)
					{
						if(cJSON_GetObjectItem(root, "current")->valueint<100)
							majiang.current_game =  cJSON_GetObjectItem(root, "current")->valueint;
					}
					switch(majiang.position )
					{
						case DIR_EAST :
						if(strstr(pjsonprint, "\"A\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "A");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(pjsonprint, "\"Score\"") > 0)
							{
								tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
								if(tempstr == NULL) 
								{
									ESP_LOGI(TAG,"json data format err");
									return;
								}
								
								//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
								memset(majiang.current_game_score[0],0,sizeof(majiang.current_game_score[0]));
								if(strlen(tempstr)<5)
								{
									memcpy(majiang.current_game_score[0], tempstr,strlen(tempstr) );
									ESP_LOGI(TAG,"C current_game_score =%s",tempstr);
								}
									
								else
								{
									majiang.current_game_score[0][0]='\\';
									ESP_LOGI(TAG,"current_game_score too long");
								}
									
							}
							
							if(strstr(pjsonprint, "\"details\"") > 0)
							{
								tempobj1 = cJSON_GetObjectItem(tempobj, "details");
								//if(tempstr == NULL) 
								//{
								//	ESP_LOGI(TAG,"json data format err");
								//	return;
								//}
								
								arrnum = cJSON_GetArraySize(tempobj1);
								ESP_LOGI(TAG,"arrnum=%d",arrnum);
								if(arrnum<20)
								{
									majiang.inform_nums = arrnum ;
									char templiu[20][MAXCHARNUM];
									memset(templiu,0,sizeof(templiu));
									//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
									//ESP_LOGI(TAG,"C details =%s",tempstr);
									for(i = 0; i < arrnum; i++)
									{
										tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
										//utf16_to_utf8(tempstr,strlen(tempstr) );
										memcpy(templiu[i], tempstr,strlen(tempstr) );
										ESP_LOGI(TAG,"%s",tempstr);
										
									}
									drawscore(templiu);
								}
									
								else
								{
									ESP_LOGI(TAG,"C details too long");
								}
									
							}
						}
						break ;
						case DIR_SOUTH :
						if(strstr(pjsonprint, "\"B\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "B");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(pjsonprint, "\"Score\"") > 0)
							{
								tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
								if(tempstr == NULL) 
								{
									ESP_LOGI(TAG,"json data format err");
									return;
								}
								
								//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
								memset(majiang.current_game_score[1],0,sizeof(majiang.current_game_score[1]));
								if(strlen(tempstr)<5)
								{
									memcpy(majiang.current_game_score[1], tempstr,strlen(tempstr) );
									ESP_LOGI(TAG,"C current_game_score =%s",tempstr);
								}
									
								else
								{
									majiang.current_game_score[1][0]='\\';
									ESP_LOGI(TAG,"current_game_score too long");
								}
									
							}
							
							if(strstr(pjsonprint, "\"details\"") > 0)
							{
								tempobj1 = cJSON_GetObjectItem(tempobj, "details");
								//if(tempstr == NULL) 
								//{
								//	ESP_LOGI(TAG,"json data format err");
								//	return;
								//}
								
								arrnum = cJSON_GetArraySize(tempobj1);
								ESP_LOGI(TAG,"arrnum=%d",arrnum);
								if(arrnum<20)
								{
									majiang.inform_nums = arrnum ;
									char templiu[20][MAXCHARNUM];
									memset(templiu,0,sizeof(templiu));
									//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
									//ESP_LOGI(TAG,"C details =%s",tempstr);
									for(i = 0; i < arrnum; i++)
									{
										tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
										//utf16_to_utf8(tempstr,strlen(tempstr) );
										memcpy(templiu[i], tempstr,strlen(tempstr) );
										ESP_LOGI(TAG,"%s",tempstr);
										
									}
									drawscore(templiu);
								}
									
								else
								{
									ESP_LOGI(TAG,"C details too long");
								}
									
							}
						}
						break ;
						case DIR_WEST :
						if(strstr(pjsonprint, "\"C\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "C");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(pjsonprint, "\"Score\"") > 0)
							{
								tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
								if(tempstr == NULL) 
								{
									ESP_LOGI(TAG,"json data format err");
									return;
								}
								
								//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
								memset(majiang.current_game_score[2],0,sizeof(majiang.current_game_score[2]));
								if(strlen(tempstr)<5)
								{
									memcpy(majiang.current_game_score[2], tempstr,strlen(tempstr) );
									ESP_LOGI(TAG,"C current_game_score =%s",tempstr);
								}
									
								else
								{
									majiang.current_game_score[2][0]='\\';
									ESP_LOGI(TAG,"current_game_score too long");
								}
									
							}
							
							if(strstr(pjsonprint, "\"details\"") > 0)
							{
								tempobj1 = cJSON_GetObjectItem(tempobj, "details");
								//if(tempstr == NULL) 
								//{
								//	ESP_LOGI(TAG,"json data format err");
								//	return;
								//}
								
								arrnum = cJSON_GetArraySize(tempobj1);
								ESP_LOGI(TAG,"arrnum=%d",arrnum);
								if(arrnum<20)
								{
									majiang.inform_nums = arrnum ;
									char templiu[20][MAXCHARNUM];
									memset(templiu,0,sizeof(templiu));
									//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
									//ESP_LOGI(TAG,"C details =%s",tempstr);
									for(i = 0; i < arrnum; i++)
									{
										tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
										//utf16_to_utf8(tempstr,strlen(tempstr) );
										memcpy(templiu[i], tempstr,strlen(tempstr) );
										ESP_LOGI(TAG,"%s",tempstr);
										
									}
									drawscore(templiu);
								}
									
								else
								{
									ESP_LOGI(TAG,"C details too long");
								}
									
							}
						}
						break ;
						case DIR_NORTH :
						if(strstr(pjsonprint, "\"D\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "D");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(pjsonprint, "\"Score\"") > 0)
							{
								tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
								if(tempstr == NULL) 
								{
									ESP_LOGI(TAG,"json data format err");
									return;
								}
								
								//ESP_LOGI(TAG,"A Score len = %d  ",strlen(tempstr));
								memset(majiang.current_game_score[3],0,sizeof(majiang.current_game_score[3]));
								if(strlen(tempstr)<5)
								{
									memcpy(majiang.current_game_score[3], tempstr,strlen(tempstr) );
									ESP_LOGI(TAG,"C current_game_score =%s",tempstr);
								}
									
								else
								{
									majiang.current_game_score[3][0]='\\';
									ESP_LOGI(TAG,"current_game_score too long");
								}
									
							}
							
							if(strstr(pjsonprint, "\"details\"") > 0)
							{
								tempobj1 = cJSON_GetObjectItem(tempobj, "details");
								//if(tempstr == NULL) 
								//{
								//	ESP_LOGI(TAG,"json data format err");
								//	return;
								//}
								
								arrnum = cJSON_GetArraySize(tempobj1);
								ESP_LOGI(TAG,"arrnum=%d",arrnum);
								if(arrnum<20)
								{
									majiang.inform_nums = arrnum ;
									char templiu[20][MAXCHARNUM];
									memset(templiu,0,sizeof(templiu));
									//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
									//ESP_LOGI(TAG,"C details =%s",tempstr);
									for(i = 0; i < arrnum; i++)
									{
										tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
										//utf16_to_utf8(tempstr,strlen(tempstr) );
										memcpy(templiu[i], tempstr,strlen(tempstr) );
										ESP_LOGI(TAG,"%s",tempstr);
										
									}
									drawscore(templiu);
								}
									
								else
								{
									ESP_LOGI(TAG,"C details too long");
								}
									
							}
						}
						break ;
						
						default :
						
						break;
						
					}
					
				}
				
				else if(strcmp(psub->valuestring, "A010") == 0) //按键 确认改分等
				{	
					if(strstr(pjsonprint, "\"keyv\"") > 0)
					{
						if(strstr(pjsonprint, "\"pos\"") > 0)
						{
							
							if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "changed") == 0) 
							{
								if(strstr(pjsonprint, "\"Score\"") > 0)
								{
									tempobj1 = cJSON_GetObjectItem(root, "Score");
									arrnum = cJSON_GetArraySize(tempobj1);
									ESP_LOGI(TAG,"Scorenum=%d",arrnum);
									if(arrnum==4)
									{
										for(i = 0; i < arrnum; i++)
										{
											majiang.current_keyV[i] = KEYV_U;
											memset(majiang.current_game_score[i],0,sizeof(majiang.current_game_score[i]));
											tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
											memcpy(majiang.current_game_score[i], tempstr,strlen(tempstr) );
											ESP_LOGI(TAG,"%s",majiang.current_game_score[i]);
											//分数已修改
											drawmodify ();
										}
														
									}
									else
									{
										ESP_LOGI(TAG,"Scorenum err");
									
									}
									ESP_LOGI(TAG,"A KEY changed");
								}
							
							}
							
							if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "Y") == 0) 
							{
								if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "A") == 0) 
								{
									majiang.current_keyV[0] = KEYV_Y;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "B") == 0) 
								{
									majiang.current_keyV[1] = KEYV_Y;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "C") == 0) 
								{
									majiang.current_keyV[2] = KEYV_Y;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "D") == 0) 
								{
									majiang.current_keyV[3] = KEYV_Y;
								}
							
							}
							if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "N") == 0) 
							{
								if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "A") == 0) 
								{
									majiang.current_keyV[0] = KEYV_N;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "B") == 0) 
								{
									majiang.current_keyV[1] = KEYV_N;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "C") == 0) 
								{
									majiang.current_keyV[2] = KEYV_N;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "D") == 0) 
								{
									majiang.current_keyV[3] = KEYV_N;
								}
							
							}
							if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "cancel") == 0) 
							{
								if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "A") == 0) 
								{
									majiang.current_keyV[0] = KEYV_U;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "B") == 0) 
								{
									majiang.current_keyV[1] = KEYV_U;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "C") == 0) 
								{
									majiang.current_keyV[2] = KEYV_U;
								}
								else if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "D") == 0) 
								{
									majiang.current_keyV[3] = KEYV_U;
								}
							
							}
							
							
							
						
							switch(majiang.position )
							{
								case DIR_EAST :
								if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "A") == 0) 
								{
									
										if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "N") == 0) 
										{
											//跳转呼叫裁判界面
											ESP_LOGI(TAG,"A KEY N");
											drawmodify1 ();
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "cancel") == 0) 
										{
											//跳转取消呼叫界面
											ESP_LOGI(TAG,"A KEY cancel");
											if(strstr(pjsonprint, "\"details\"") > 0)
											{
												tempobj1 = cJSON_GetObjectItem(root, "details");
												
												arrnum = cJSON_GetArraySize(tempobj1);
												ESP_LOGI(TAG,"arrnum=%d",arrnum);
												if(arrnum<20)
												{
													majiang.inform_nums = arrnum ;
													char templiu[20][MAXCHARNUM];
													memset(templiu,0,sizeof(templiu));
													//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
													//ESP_LOGI(TAG,"C details =%s",tempstr);
													for(i = 0; i < arrnum; i++)
													{
														tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
														//utf16_to_utf8(tempstr,strlen(tempstr) );
														memcpy(templiu[i], tempstr,strlen(tempstr) );
														ESP_LOGI(TAG,"%s",tempstr);
														
													}
													drawscore(templiu);
												}
													
												else
												{
													ESP_LOGI(TAG,"A details too long");
												}
													
											}
											//drawmodify1 ();
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "Y") == 0) 
										{
											//确认 跳转等待页面
											ESP_LOGI(TAG,"A KEY Y");
											drawwaiting();
										}
								}
								break ;
								case DIR_SOUTH :
								if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "B") == 0) 
								{
									
										if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "N") == 0) 
										{
											//跳转呼叫裁判界面
											ESP_LOGI(TAG,"B KEY N");
											drawmodify1 ();
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "cancel") == 0) 
										{
											//跳转取消呼叫界面
											ESP_LOGI(TAG,"B KEY cancel");
											if(strstr(pjsonprint, "\"details\"") > 0)
											{
												tempobj1 = cJSON_GetObjectItem(root, "details");
												
												arrnum = cJSON_GetArraySize(tempobj1);
												ESP_LOGI(TAG,"arrnum=%d",arrnum);
												if(arrnum<20)
												{
													majiang.inform_nums = arrnum ;
													char templiu[20][MAXCHARNUM];
													memset(templiu,0,sizeof(templiu));
													//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
													//ESP_LOGI(TAG,"C details =%s",tempstr);
													for(i = 0; i < arrnum; i++)
													{
														tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
														//utf16_to_utf8(tempstr,strlen(tempstr) );
														memcpy(templiu[i], tempstr,strlen(tempstr) );
														ESP_LOGI(TAG,"%s",tempstr);
														
													}
													drawscore(templiu);
												}
													
												else
												{
													ESP_LOGI(TAG,"B details too long");
												}
													
											}
											//drawmodify1 ();
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "Y") == 0) 
										{
											//确认 跳转等待页面
											ESP_LOGI(TAG,"B KEY Y");
											drawwaiting();
										}
								}
								break ;
								case DIR_WEST :
								
								if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "C") == 0) 
								{
									
										if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "N") == 0) 
										{
											//跳转呼叫裁判界面
											ESP_LOGI(TAG,"C KEY N");
											drawmodify1 ();
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "cancel") == 0) 
										{
											//跳转取消呼叫界面
											ESP_LOGI(TAG,"C KEY cancel");
											if(strstr(pjsonprint, "\"details\"") > 0)
											{
												tempobj1 = cJSON_GetObjectItem(root, "details");
												
												arrnum = cJSON_GetArraySize(tempobj1);
												ESP_LOGI(TAG,"arrnum=%d",arrnum);
												if(arrnum<20)
												{
													majiang.inform_nums = arrnum ;
													char templiu[20][MAXCHARNUM];
													memset(templiu,0,sizeof(templiu));
													//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
													//ESP_LOGI(TAG,"C details =%s",tempstr);
													for(i = 0; i < arrnum; i++)
													{
														tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
														//utf16_to_utf8(tempstr,strlen(tempstr) );
														memcpy(templiu[i], tempstr,strlen(tempstr) );
														ESP_LOGI(TAG,"%s",tempstr);
														
													}
													drawscore(templiu);
												}
													
												else
												{
													ESP_LOGI(TAG,"D details too long");
												}
													
											}
											
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "Y") == 0) 
										{
											//确认 跳转等待页面
											ESP_LOGI(TAG,"C KEY Y");
											drawwaiting();
										}
								}
								break ;
								case DIR_NORTH :
								if(strcmp((cJSON_GetObjectItem(root, "pos")->valuestring), "D") == 0) 
								{
									
										if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "N") == 0) 
										{
											//跳转呼叫裁判界面
											ESP_LOGI(TAG,"D KEY N");
											drawmodify1 ();
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "cancel") == 0) 
										{
											//跳转取消呼叫界面
											ESP_LOGI(TAG,"D KEY cancel");
											if(strstr(pjsonprint, "\"details\"") > 0)
											{
												tempobj1 = cJSON_GetObjectItem(root, "details");
												
												arrnum = cJSON_GetArraySize(tempobj1);
												ESP_LOGI(TAG,"arrnum=%d",arrnum);
												if(arrnum<20)
												{
													majiang.inform_nums = arrnum ;
													char templiu[20][MAXCHARNUM];
													memset(templiu,0,sizeof(templiu));
													//memcpy(majiang.standard_score[0], tempstr,strlen(tempstr) );
													//ESP_LOGI(TAG,"C details =%s",tempstr);
													for(i = 0; i < arrnum; i++)
													{
														tempstr = cJSON_GetArrayItem(tempobj1, i)->valuestring;
														//utf16_to_utf8(tempstr,strlen(tempstr) );
														memcpy(templiu[i], tempstr,strlen(tempstr) );
														ESP_LOGI(TAG,"%s",tempstr);
														
													}
													drawscore(templiu);
												}
													
												else
												{
													ESP_LOGI(TAG,"D details too long");
												}
													
											}
											//drawmodify1 ();
										}
										else if(strcmp((cJSON_GetObjectItem(root, "keyv")->valuestring), "Y") == 0) 
										{
											//确认 跳转等待页面
											ESP_LOGI(TAG,"D KEY Y");
											drawwaiting();
										}
								}
								break ;
								
								default :
									ESP_LOGI(TAG,"err  position %d " ,majiang.position);
								break;
							
							}
							
							if(majiang.current_page == PAGE_CWAITING)
							{
								//刷新等待页面
								drawwaiting();
								
							}
							
						}
						else
						{
							ESP_LOGI(TAG,"NOT  pos OBJ ");
						}
					}
				
				//cJSON_Delete(tempobj);
				//cJSON_Delete(tempobj1);
				//消息队列剩余大小
				uint8_t tcttx_size=uxQueueSpacesAvailable(Queue_Tcpsenddata);//得到队列剩余大小
				if(tcttx_size>0)
					tcpdataretrun(psub->valuestring);	
				//cJSON_Delete(psub);
			}
			
				else if(strcmp(psub->valuestring, "A011") == 0) //音频
				{
					if(strstr(pjsonprint, "\"audio\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "audio");
						tempstr = tempobj->valuestring;
						if(majiang.current_audio == AUDIO_NULL) 
						{
							majiang.current_audio = tempstr[0]-'0';
							//play_audio_demo(majiang.current_audio) ;
						}
						printf("audio = %d \n", majiang.current_audio);
					}
				}
			
			//cJSON_Delete(psub );
			}
		}

		free(pjsonprint);
		
	}
    //ESP_LOGI(TAG,"%s 222",__func__);

	
	
    cJSON_Delete(root);
	
}

void dataHandle_task(void *parameter) {
	
	char Tcpdata_rxbuffer[512];
	
	while(1) {//TEST
	
		//printf("dataHandle is run\r\n");
		
		memset(Tcpdata_rxbuffer,0,sizeof(Tcpdata_rxbuffer));
		if(xQueueReceive(Queue_Tcprecvdata, Tcpdata_rxbuffer, 10) == pdPASS) {
		
			cjson_to_struct_info(Tcpdata_rxbuffer);
		}
		vTaskDelay(200 / portTICK_RATE_MS);
		//ESP_LOGI("== ==", "[APP] Free memory: %d bytes", esp_get_free_heap_size());
		
		
	}
}
