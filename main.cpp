#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/spi_reg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_wps.h"
#include "esp_system.h"
#include "esp_event_loop.h"
#include "driver/hspi.h"
#include "nvs_flash.h"
#include "soc/gpio_reg.h"
#include "driver/gpio.h"
#include "esp_attr.h"                                  
#include "Adafruit_ILI9341_fast_as.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"




float target_room_temperature = 23.5;
float RW_temperature = 65;
float target_RW_temperature = 70;
float room1_temperature = 23.4;
float room2_temperature = 23.3;
float outside_temperature = 2.4;
float min_target_temp = 18;
float max_target_temp = 26;
bool heater_enabled = false;
unsigned long room1_updated = -1;
unsigned long room2_updated = -1;
unsigned long outside_updated = -1;
unsigned long heater_state_changed_time = 0;
time_t total_on_time = 1;
time_t total_off_time = 1;
time_t last24h_on_time = 1;
time_t last24h_off_time = 1;




#define EXAMPLE_WIFI_SSID "Jason's Wi-Fi Network"
#define EXAMPLE_WIFI_PASS "qwerty001"

#define GPIO_INPUT_IO_0 36
#define GPIO_INPUT_IO_1 39
#define GPIO_INPUT_IO_2 35
#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "Jason's WiFi ";
static char* buff = (char*) malloc(sizeof(char) * (1025));
static char* tempGame = (char*) malloc(sizeof(char) * (50));
static char **arrayOfGames = (char**) calloc(20,sizeof(char*)*20);
static char* temp = (char*) calloc(12,sizeof(char)*12);

static int maxGames = 0;
static int gameSel = 5;

static xQueueHandle gpio_evt_queue = NULL;
static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

Adafruit_ILI9341 tft;

extern void setupUI();
static void start_wps(void);
static esp_err_t event_handler(void *ctx, system_event_t *event);
static void startTCP();
static void startButtons();
static void IRAM_ATTR gpio_isr_handler(void* arg);
static void gpio_task_example(void* arg);
static void drawGame(void);
static void listenButtons(void);
static void lcd_debug(void *pvParameters)
{
    vTaskDelay(1000 / portTICK_RATE_MS);
	tft.begin();
	//tft.fillScreen(0xFFFF);
	setupUI();
	//tft.fillRect(0,0,240,320,0xFFFF);
	//tft.setTextColor(0x0000, 0xFFFF);
	//tft.drawString("Diamondbacks", 0, 12, 4);
	//tft.fillRect(180,0,50,50,0x0000);
	tft.fillScreen(0xFFFF);
	vTaskDelete(NULL);
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}


static void drawGame(void){
	setupUI();
	vTaskDelay(1500/portTICK_RATE_MS);
	ets_printf("\nscreen cleared");
	char **temparrayOfGames = (char**) calloc(20,sizeof(char*)*30);
    char* temp = (char*) calloc(12,sizeof(char)*12);
	ets_printf("\n temparray allocated");
	for(int i=0;i<maxGames;i++){
			free(temparrayOfGames[i]);
			temparrayOfGames[i] = (char*) calloc(50,sizeof(char)*50);
		}
	ets_printf("\n temparray finishing");

	for(int i=0;i<maxGames;i++){
		strcpy(temparrayOfGames[i],arrayOfGames[i]);
		ets_printf("\nitem copied: %d",i);
	}
	ets_printf("\nDrawing game %d",gameSel);



	char* AwayTeam = (char*) calloc(12,sizeof(char)*12);
	ets_printf("\nline1");
	temp = strsep(&temparrayOfGames[gameSel],",");
	ets_printf(temp);
	strcpy(AwayTeam,temp);
	ets_printf("\nline3");
	ets_printf(AwayTeam);

	char* HomeTeam = (char*) calloc(12,sizeof(char)*12);
	temp = strsep(&temparrayOfGames[gameSel],",");
	strcpy(HomeTeam,temp);
	ets_printf(HomeTeam);

	char* inningString = (char*) calloc(2,sizeof(char) * (2));
	temp = strsep(&temparrayOfGames[gameSel],",");
	strcpy(inningString,temp);
	int inning = atoi(inningString);
	ets_printf("%d",inning);


	char* awayScore = (char*) calloc(2,sizeof(char) * (2));
	temp = strsep(&temparrayOfGames[gameSel],",");
	strcpy(awayScore,temp);
	ets_printf(awayScore);

	char* homeScore = (char*) calloc(2,sizeof(char) * (2));
	temp = strsep(&temparrayOfGames[gameSel],",");
	strcpy(homeScore,temp);
	ets_printf(homeScore);

	char* startTime = (char*) calloc(4,sizeof(char) * (4));
	temp = strsep(&temparrayOfGames[gameSel],",");
	temp = (char*) calloc(12,sizeof(char)*12);
	ets_printf(startTime);

	char* period = (char*) calloc(2,sizeof(char) * (2));
	temp = strsep(&temparrayOfGames[gameSel],",");
	strcpy(period,temp);
	ets_printf(period);

	switch(inning){
		case 0: //game has not started
			tft.setTextColor(0x0000,0xFFFF);
			tft.drawString(AwayTeam,5,12,4);
			tft.drawString("vs",5,36,4);
			tft.drawString(HomeTeam,5,60,4);
			tft.drawString("Start Time:",5,84,4);
			tft.drawString(startTime,145,84,4);
			tft.drawString(period,195,84,4);
			break;


		case -1: //game is over
			tft.setTextColor(0x0000,0xFFFF);
			tft.drawString("Final Score: ",5,12,4);
			tft.drawString(AwayTeam,5,36,4);
			tft.drawString(awayScore,5,60,4);
			tft.drawString(HomeTeam,5,84,4);
			tft.drawString(homeScore,5,108,4);
			break;
		default:
			tft.setTextColor(0x000,0xFFFF);
			tft.drawString(AwayTeam,5,12,4);
			tft.drawString(awayScore,5,36,4);
			tft.drawString(HomeTeam,5,60,4);
			tft.drawString(homeScore,5,84,4);
			tft.drawString("Inning: ",5,108,4);
			tft.drawString(inningString,5,132,4);
			tft.drawString("Start Time:",5,156,4);
			tft.drawString(startTime,145,156,4);
			tft.drawString(period,195,156,4);

	}
	delete temparrayOfGames;
}

static void listenButtons(void){
	while(1){
		drawGame();
		ets_printf("\ngame drawn");
		vTaskDelay(5000 / portTICK_RATE_MS );
	}
}

extern "C" void app_main(void* arg) 
{
	int i=0;
	for(i=0;i<20;i++){
		arrayOfGames[i] = (char*) calloc(50,sizeof(char)*50);
	}

	start_wps();
	startButtons();
	ets_printf("buttons set");

	vTaskDelay(3000 / portTICK_RATE_MS);
	ets_printf("Pause\n");	
    xTaskCreate(lcd_debug,"lcd_debug", 4096, NULL, 3, NULL);
	ets_printf("LCD_init done....\n");
	ets_printf("starting TCP\n");
	vTaskDelay(3000 / portTICK_RATE_MS);
	startTCP();
	listenButtons();
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("\nGPIO[%d] intr, val: %d\n", io_num, gpio_get_level((gpio_num_t)io_num));
            if(io_num==36){
            	ets_printf("\nYou moved left: ");
            	gameSel -= 1;
            	ets_printf("%d\n",gameSel);
            }
            else if(io_num==39){
                ets_printf("\nYou moved right: ");
                gameSel += 1;
                ets_printf("%d\n",gameSel);
            }
            else if(io_num==35){
            	ets_printf("Refreshing Data...Please wait");
            	esp_restart();

            }
            if(gameSel < 0){
            	gameSel = maxGames;
            	ets_printf("Reached Bottom Value: ");
            	ets_printf("%d\n",gameSel);
            }
            else if(gameSel > maxGames){
            	gameSel = 0;
            	ets_printf("Reached Top Value: ");
            	ets_printf("%d\n",gameSel);
            }
        }
    }
}

static void startButtons(){
	gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
	gpio_config_t gpioConfig;
	gpioConfig.pin_bit_mask = GPIO_SEL_36 | GPIO_SEL_39 | GPIO_SEL_35;
	gpioConfig.mode = GPIO_MODE_INPUT;
	gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
	gpioConfig.intr_type = GPIO_INTR_POSEDGE;
	gpio_config(&gpioConfig);
	gpio_install_isr_service(0);
	xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	gpio_isr_handler_add((gpio_num_t)GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
	gpio_isr_handler_add((gpio_num_t)GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
	gpio_isr_handler_add((gpio_num_t)GPIO_INPUT_IO_2, gpio_isr_handler, (void*) GPIO_INPUT_IO_2);

}

static void startTCP(){
	int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	int i = 0;
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	inet_pton(AF_INET, "10.0.1.3", &serverAddress.sin_addr.s_addr);
	serverAddress.sin_port = htons(5007);

	int rc = connect(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in));
	if(rc == 0){
		ets_printf("\nSocket connected: %d\n",rc);
		vTaskDelay(2000 / portTICK_RATE_MS);

		read(sock, buff, 1024);
		ets_printf(buff);
		ets_printf("\ndata was recieved hopefully\n");
		tempGame = strsep(&buff,";");
		strcpy(arrayOfGames[i],tempGame);
		i++;

		while((tempGame = strsep(&buff,";")) != NULL){


			ets_printf("\n");
			ets_printf(tempGame);
			ets_printf("\n");

			strcpy(arrayOfGames[i],tempGame);
			i++;
			maxGames = i;

		}
		maxGames = maxGames - 1; //need to subtract two so count is not off
		ets_printf("Max games will be %d" ,maxGames);
		for(i=0;i<maxGames;i++){
			if(arrayOfGames[i] != NULL){
				ets_printf(arrayOfGames[i]);
				ets_printf("\n");
			}
		}
		closesocket(sock);
		ets_printf("\nsocket closed\n");
	}
	else{
		ets_printf("Socket not connected %d",rc);
	}
}


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    	case SYSTEM_EVENT_STA_START:
        	esp_wifi_connect();
        break;
    	case SYSTEM_EVENT_STA_GOT_IP:
        	xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    	case SYSTEM_EVENT_STA_DISCONNECTED:
        	/* This is a workaround as ESP32 WiFi libs don't currently
           	   auto-reassociate. */
        	esp_wifi_connect();
        	xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    	default:
        break;
    	}
    return ESP_OK;
}

/*init wifi as sta and start wps*/
static void start_wps(void)
{
	nvs_flash_init();
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            EXAMPLE_WIFI_SSID,
            EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}
