#ifndef _I2S_DAC_DEMO_H_
#define _I2S_DAC_DEMO_H_

esp_err_t i2s_init();
void play_audio_demo(char audio_num) ;
void setmyVolume(char vol) ;
char getmyVolumePercentage();


#define AUDIO_NULL      0
#define AUDIO_1         1
#define AUDIO_2         2
#define AUDIO_3         3
#define AUDIO_4         4

#endif
