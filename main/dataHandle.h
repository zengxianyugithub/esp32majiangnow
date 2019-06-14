#ifndef _dataHandle_H_
#define _dataHandle_H_
#include "lvgl/lvgl.h"

extern QueueHandle_t Queue_Tcprecvdata;
extern QueueHandle_t Queue_Tcpsenddata;

esp_err_t TcprecvdataQueueCreate() ;
esp_err_t TcpsenddataQueueCreate() ;

void dataHandle_task(void *parameter);
void cjson_to_struct_info(char *text);
void tcpdataretrun(char *str);
void tcpdataretrunA012();
void tcpdataretrunA011_keyv(char *str);


#endif
