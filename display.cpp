/* Adapted from Sermus' project : https://github.com/Sermus/ESP8266_Adafruit_ILI9341*/

#include <Adafruit_GFX_AS.h>
#include <Adafruit_ILI9341_fast_as.h>
#include <time.h>

extern "C" {
#include "mini-printf.h"
}

#define TARGETTEMPSCREEN

#define VGA_BLACK		0x0000
#define VGA_WHITE		0xFFFF
#define VGA_RED			0xF800
#define VGA_GREEN		0x0400
#define VGA_BLUE		0x001F
#define VGA_SILVER		0xC618
#define VGA_GRAY		0x8410
#define VGA_MAROON		0x8000
#define VGA_YELLOW		0xFFE0
#define VGA_OLIVE		0x8400
#define VGA_LIME		0x07E0
#define VGA_AQUA		0x07FF
#define VGA_TEAL		0x0410
#define VGA_NAVY		0x0010
#define VGA_FUCHSIA		0xF81F
#define VGA_PURPLE		0x8010

extern Adafruit_ILI9341 tft;

extern float target_room_temperature;
extern float RW_temperature;
extern float target_RW_temperature;
extern float room1_temperature;
extern float room2_temperature;
extern float outside_temperature;
extern float min_target_temp;
extern float max_target_temp;
extern bool heater_enabled;
extern time_t room1_updated;
extern time_t room2_updated;
extern time_t outside_updated;
extern time_t heater_state_changed_time;
extern time_t total_on_time;
extern time_t total_off_time;
extern time_t last24h_on_time;
extern time_t last24h_off_time;

time_t start_time;

#define HEADERTEXT 2
#define LINETEXT 2

int color(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r&248)|g>>5) << 8 | ((g&28)<<3|b>>3);
}

time_t now()
{
return 1;
	//return system_get_rtc_time() / 100000 - start_time;
}


void setupUI()
{

	tft.setRotation(2);
	tft.fillScreen(ILI9341_WHITE);
	tft.setRotation(0);
	tft.setTextColor(VGA_GREEN, VGA_BLACK);
	tft.fillScreen(ILI9341_WHITE);

}
