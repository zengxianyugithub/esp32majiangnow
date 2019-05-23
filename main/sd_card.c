#include "sd_card.h"

#define SD_PIN_NUM_CD   32
// This example can use SDMMC and SPI peripherals to communicate with SD card.
// By default, SDMMC peripheral is used.
// To enable SPI mode, uncomment the following line:

#define USE_SPI_MODE

// When testing SD and SPI modes, keep in mind that once the card has been
// initialized in SPI mode, it can not be reinitialized in SD mode without
// toggling power to the card.

#ifdef USE_SPI_MODE
// Pin mapping when using SPI mode.
// With this mapping, SD card can be used both in SPI and 1-line SD mode.
// Note that a pull-up on CS line is required in SD mode.
#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 15
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   13
#endif //USE_SPI_MODE



static const char *TAG = "sdcard";
esp_err_t sdmmc_mount(sdmmc_card_t *card) {
  ESP_LOGI(TAG, "Initializing SD card");

#ifndef USE_SPI_MODE
    ESP_LOGI(TAG, "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // To use 1-line SD mode, uncomment the following line:
    // slot_config.width = 1;

    // GPIOs 15, 2, 4, 12, 13 should have external 10k pull-ups.
    // Internal pull-ups are not sufficient. However, enabling internal pull-ups
    // does make a difference some boards, so we do that here.
    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
    gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);   // D2, needed in 4-line mode only
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes

#else
    ESP_LOGI(TAG, "Using SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = PIN_NUM_MISO;
    slot_config.gpio_mosi = PIN_NUM_MOSI;
    slot_config.gpio_sck  = PIN_NUM_CLK;
    slot_config.gpio_cs   = PIN_NUM_CS;
    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
#endif //USE_SPI_MODE

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    //sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
  return ESP_OK;
}

esp_err_t sdmmc_unmount() {
	//esp_vfs_fat_sdmmc_unmount();
		ESP_LOGI(TAG, "Card unmounted");

  return esp_vfs_fat_sdmmc_unmount();
}

bool sdmmc_is_valid() {
  gpio_set_direction(SD_PIN_NUM_CD, GPIO_MODE_INPUT);
  return !gpio_get_level(SD_PIN_NUM_CD);
}



static char check_music_file1(char* filename) {
  char l[5];
  memset(l, 0, sizeof(l));
  int len = strlen(filename);
  for(int i = 0; i < 4; ++i) {
    l[i] = filename[len - (4 - i)];
  }
  if(strcmp(l,".mp3") == 0 || strcmp(l, ".MP3") == 0)       return 1;
  else if(strcmp(l, ".wav") == 0 || strcmp(l, ".WAV") == 0) return 2;

  return 0;
}

int scan_music_file2(const char *basePath){

   DIR *dir_p;
   struct dirent *dirent_p;
   char path11[256];
   char type11 ;
   char tmp_title[128], tmp_fn[128];
   FILE *file_p;

   
   dir_p = opendir(basePath);

   
   
   if(dir_p == NULL) {
	   ESP_LOGE(TAG, "Failed to opendir: %s", basePath);
	   return 1 ;
   }
   ESP_LOGE(TAG, "success to opendir: %s", basePath);
   dirent_p = readdir(dir_p);

   //ESP_LOGE(TAG, "dino file num : %d", dirent_p->d_ino);
   while(dirent_p != NULL) {
	   if(strcmp(dirent_p->d_name, ".") == 0 || strcmp(dirent_p->d_name, "..") == 0)
		 continue;
   
	   if(dirent_p->d_type == 1) {
		   //file
		   strcat(path11, dirent_p->d_name);
		   type11 = check_music_file1(path11);
		   if(type11 != 0) {
			// playlist_len++;
			 memset(tmp_title, 0, sizeof(tmp_title));
			 memset(tmp_fn, 0, sizeof(tmp_fn));
			 sprintf(tmp_fn, "%s/%s", basePath, dirent_p->d_name);
			 file_p = fopen(tmp_fn, "rb");
			 if(type11 == 1)//mp3file
			 {
				ESP_LOGE(TAG, "mp3file: %s", tmp_fn);
				//FILE *datab = fopen(path11, "r");
				if (file_p ==  NULL) /* 如果失败了 */
			    {
			        ESP_LOGE(TAG,"%s openfile err!",tmp_fn);
			        
			    }
				else
				{
					 fseek(file_p, 0, SEEK_END);
					 int lefniel = ftell(file_p);
					 ESP_LOGE(TAG," openfile:%d byte !",lefniel);
					 
				}
				
			 }
			   
			 else if(type11 == 2)
			   memcpy(tmp_title, tmp_fn, 128);
			 fclose(file_p);
			 //fwrite(tmp_fn, 1, MUSICDB_FN_LEN, db);
			 //fwrite(tmp_title, 1, MUSICDB_TITLE_LEN, db);
		   }
		   
	   }
	   if(dirent_p->d_type == 2){
		   
		   strcat(path11, "/");
		   strcat(path11, dirent_p->d_name);
		   scan_music_file2(path11);
		   
	   }
	   dirent_p = readdir(dir_p);
	   //ESP_LOGE(TAG, "dino file num : %d", dirent_p->d_ino);
	   memset(path11, 0, sizeof(path11));
	   strcpy(path11, basePath);
   }

   return 0 ;
 }



