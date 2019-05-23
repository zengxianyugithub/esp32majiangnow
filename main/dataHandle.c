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
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "clientId", cJSON_CreateString(majiang.Table_number));
    cJSON_AddItemToObject(root, "actionCode", cJSON_CreateString(str));
    char *buf = cJSON_PrintUnformatted(root);

    xQueueSend(Queue_Tcpsenddata, (void*)buf, (TickType_t) 10);
   
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
    //cJSON *arrayItem;
	int arrnum =0,i=0;
	ESP_LOGI(TAG,"===%s===",text);
    //截取有效json

    //char *index=strchr(text,'{');
    //strcpy(text,index);

    root = cJSON_Parse(text);
    
    if(root!=NULL)
    {
		printJson(root);
		
		if(strstr(cJSON_Print(root), "\"actionCode\"") > 0)
		{
			psub = cJSON_GetObjectItem(root, "actionCode");//动作
			if(psub!=NULL)
			{
				cJSON *tempobj ,*tempobj1 ;
				char *tempstr ;
				ESP_LOGI(TAG,"===%s===",psub->valuestring );
				if(strcmp(psub->valuestring, "A001") == 0)     //返回二维码
				{
					//画登录二维码界面
					switch(majiang.position )
					{
						case DIR_EAST :
						//arrayItem = cJSON_GetObjectItem(root,"version");
						if(strstr(cJSON_Print(root), "\"A\"") > 0)
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
						if(strstr(cJSON_Print(root), "\"B\"") > 0)
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
						if(strstr(cJSON_Print(root), "\"C\"") > 0)
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
						if(strstr(cJSON_Print(root), "\"D\"") > 0)
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
					
					
					if(strstr(cJSON_Print(root), "\"pos\"") > 0)
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
					if(strstr(cJSON_Print(root), "\"current\"") > 0)
					{
						if(cJSON_GetObjectItem(root, "current")->valueint<100)
							majiang.current_game =  cJSON_GetObjectItem(root, "current")->valueint;
					}
					if(strstr(cJSON_Print(root), "\"total\"") > 0)
					{
						if(cJSON_GetObjectItem(root, "total")->valueint<100)
							majiang.games =  cJSON_GetObjectItem(root, "total")->valueint;
					}
					
					if(strstr(cJSON_Print(root), "\"A\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "A");
						//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
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
								ESP_LOGI(TAG,"Score too long");
							}
								
						}
						if(strstr(cJSON_Print(tempobj), "\"rank\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "rank")->valuestring;
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
					
					if(strstr(cJSON_Print(root), "\"B\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "B");
						
						//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"C Score len = %d  ",strlen(tempstr));
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
						if(strstr(cJSON_Print(tempobj), "\"rank\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "rank")->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"C rank len = %d  ",strlen(tempstr));
							memset(majiang.standard_score[1],0,sizeof(majiang.standard_score[1]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.standard_score[1], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"B rank =%s",tempstr);
							}
								
							else
							{
								majiang.standard_score[1][0]='\\';
								ESP_LOGI(TAG,"B rank too long");
							}
								
						}
						
					}
					
					if(strstr(cJSON_Print(root), "\"C\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "C");
						
						//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"C Score len = %d  ",strlen(tempstr));
							memset(majiang.match_score[2],0,sizeof(majiang.match_score[2]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.match_score[2], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"C Score =%s",tempstr);
							}
								
							else
							{
								majiang.match_score[2][0]='\\';
								ESP_LOGI(TAG,"C Score too long");
							}
						}
						if(strstr(cJSON_Print(tempobj), "\"rank\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "rank")->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"C rank len = %d  ",strlen(tempstr));
							memset(majiang.standard_score[2],0,sizeof(majiang.standard_score[2]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.standard_score[2], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"C rank =%s",tempstr);
							}
								
							else
							{
								majiang.standard_score[2][0]='\\';
								ESP_LOGI(TAG,"C rank too long");
							}
								
						}
						
					}
					
					if(strstr(cJSON_Print(root), "\"D\"") > 0)
					{
						tempobj = cJSON_GetObjectItem(root, "D");
						
						ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
						if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "Score")->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							ESP_LOGI(TAG,"D Score len = %d  ",strlen(tempstr));
							memset(majiang.match_score[3],0,sizeof(majiang.match_score[3]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.match_score[3], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG," D Score =%s",tempstr);
							}
								
							else
							{
								majiang.match_score[3][0]='\\';
								ESP_LOGI(TAG,"D Score too long");
							}
						}
						if(strstr(cJSON_Print(tempobj), "\"rank\"") > 0)
						{
							tempstr = cJSON_GetObjectItem(tempobj, "rank")->valuestring;
							if(tempstr == NULL) 
							{
								ESP_LOGI(TAG,"json data format err");
								return;
							}
							//ESP_LOGI(TAG,"D rank len = %d  ",strlen(tempstr));
							memset(majiang.standard_score[3],0,sizeof(majiang.standard_score[3]));
							if(strlen(tempstr)<5)
							{
								memcpy(majiang.standard_score[3], tempstr,strlen(tempstr) );
								ESP_LOGI(TAG,"D rank =%s",tempstr);
							}
								
							else
							{
								majiang.standard_score[3][0]='\\';
								ESP_LOGI(TAG,"D rank too long");
							}
								
						}
						
					}
					
					drawranking ();
					//drawptotal ();
					
					//ESP_LOGI(TAG,"current =    %d",cJSON_GetObjectItem(root, "current")->valueint);
					//ESP_LOGI(TAG,"current =    %d",cJSON_GetObjectItem(root, "total")->valueint);
					
				}
				
				else if(strcmp(psub->valuestring, "A005") == 0) //结算页面
				{
					if(strstr(cJSON_Print(root), "\"current\"") > 0)
					{
						if(cJSON_GetObjectItem(root, "current")->valueint<100)
							majiang.current_game =  cJSON_GetObjectItem(root, "current")->valueint;
					}
					switch(majiang.position )
					{
						case DIR_EAST :
						if(strstr(cJSON_Print(root), "\"A\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "A");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
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
							
							if(strstr(cJSON_Print(tempobj), "\"details\"") > 0)
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
						if(strstr(cJSON_Print(root), "\"B\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "B");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
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
							
							if(strstr(cJSON_Print(tempobj), "\"details\"") > 0)
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
						if(strstr(cJSON_Print(root), "\"C\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "C");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
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
							
							if(strstr(cJSON_Print(tempobj), "\"details\"") > 0)
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
						if(strstr(cJSON_Print(root), "\"D\"") > 0)
						{
							tempobj = cJSON_GetObjectItem(root, "D");
							//ESP_LOGI(TAG,"cJSON_Print(tempobj)=%s",cJSON_Print(tempobj));
							if(strstr(cJSON_Print(tempobj), "\"Score\"") > 0)
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
							
							if(strstr(cJSON_Print(tempobj), "\"details\"") > 0)
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
				
				tcpdataretrun(psub->valuestring);	
			}
		}
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
		vTaskDelay(10 / portTICK_RATE_MS);

		
		
	}
}
