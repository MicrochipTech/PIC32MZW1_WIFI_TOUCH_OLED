#ifndef _OLED_W_H
#define _OLED_W_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// Specific to OLED Display
#define OLED_SCREEN_WIDTH                           96
#define OLED_SCREEN_HEIGHT                          39 + 1  //+1
#define OLED_SCREEN_FIRST_COLUMN                    32
#define OLED_SCREEN_LAST_COLUMN                     128
#define OLED_SCREEN_FIRST_PAGE                      0
#define OLED_SCREEN_LAST_PAGE                       4
#define OLED_SCREEN_X_MAX                           OLED_SCREEN_WIDTH - 1
#define OLED_SCREEN_Y_MAX                           OLED_SCREEN_LAST_PAGE
    
// Common to SSD1306 Controller
#define SSD1306_RAM_FIRST_COLUMN                    0
#define SSD1306_RAM_LAST_COLUMN                     127
#define SSD1306_RAM_FIRST_PAGE                      0
#define SSD1306_RAM_LAST_PAGE                       4
#define SSD1306_CMD_EXTERNALVCC                     0x01
#define SSD1306_CMD_SWITCHCAPVCC                    0x02
#define SSD1306_CMD_COL_SET_LSB                     0x00
#define SSD1306_CMD_COL_SET_MSB                     0x10
#define SSD1306_CMD_COL_ADD_SET_LSB(column)         (0x00 | (column))
#define SSD1306_CMD_COL_ADD_SET_MSB(column)         (0x10 | (column))
#define SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE      0x20
#define SSD1306_CMD_SET_COLUMN_ADDRESS              0x21
#define SSD1306_CMD_SET_PAGE_ADDRESS                0x22
#define SSD1306_CMD_SET_DISPLAY_START_LINE(line)    (0x40 | (line))
#define SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0  0x81
#define SSD1306_CMD_SET_CHARGE_PUMP_SETTING         0x8D
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0    0xA0
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0  0xA1
#define SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON    0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON               0xA5
#define SSD1306_CMD_SET_NORMAL_DISPLAY              0xA6
#define SSD1306_CMD_SET_INVERSE_DISPLAY             0xA7
#define SSD1306_CMD_SET_MULTIPLEX_RATIO             0xA8
#define SSD1306_CMD_SET_DISPLAY_ON                  0xAF
#define SSD1306_CMD_SET_DISPLAY_OFF                 0xAE
#define SSD1306_CMD_SET_PAGE_START_ADDRESS(page)    (0xB0 | (page))
#define SSD1306_CMD_START_PAGE_ADDRESS              0xB0
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_UP          0xC0
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN        0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET              0xD3
#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO  0xD5
#define SSD1306_CMD_SET_PRE_CHARGE_PERIOD           0xD9
#define SSD1306_CMD_SET_COM_PINS                    0xDA
#define SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL        0xDB
#define SSD1306_CMD_NOP                             0xE3
    
#define SSD1306_CMD_SCROLL_H_RIGHT                  0x26
#define SSD1306_CMD_SCROLL_H_LEFT                   0x27
#define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_RIGHT 0x29
#define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_LEFT  0x2A
#define SSD1306_CMD_DEACTIVATE_SCROLL               0x2E
#define SSD1306_CMD_ACTIVATE_SCROLL                 0x2F
#define SSD1306_CMD_SET_VERTICAL_SCROLL_AREA        0xA3

#define RECTANGLE_POS_X     27                  // static position of the rectangle
#define CURSOR_WIDTH        4                   // in pixel
#define CURSOR_STEPS        (255/CURSOR_WIDTH)+1// 0-63 (64 elements)
#define CURSOR_MIN_POS      28                  // min
#define CURSOR_MAX_POS      96-CURSOR_WIDTH-1   // -1 for the rectangle width    
    
void OLED_Delay(void) ;
void OLED_Reset(void) ;
void OLED_Init(void) ;
void OLED_SendCmd(uint8_t cmd) ;
void OLED_SendData(uint8_t data) ;
void OLED_SetPage(uint8_t addr) ;
void OLED_SetColumn(uint8_t addr) ;
void OLED_ClearScreen(void) ;
void OLED_ClearRAM(void) ;
void OLED_SetContrast(uint8_t value) ;
void OLED_InvertDisplay(void) ;
void OLED_NormalDisplay(void) ;
void OLED_PutPicture(const uint8_t *pic) ;
void OLED_Putchar(char c) ;
void OLED_Puts(uint8_t x, uint8_t y, char *s) ;
void OLED_PutInt(uint8_t x, uint8_t y, uint16_t value) ;
void OLED_PutFloat(uint8_t x, uint8_t y, float value) ;
void OLED_GotoXY(uint8_t x, uint8_t y) ;
void OLED_ScrollRight(uint8_t startPageAddr, uint8_t endPageAddr) ;
void OLED_ScrollLeft(uint8_t startPageAddr, uint8_t endPageAddr) ;
void OLED_ScrollDiagRight(uint8_t startPageAddr, uint8_t endPageAddr) ;
void OLED_ScrollDiagLeft(uint8_t startPageAddr, uint8_t endPageAddr) ;
void OLED_StopScroll(void) ;
void OLED_DrawCircle(uint8_t x, uint8_t y) ;
void OLED_DrawFilledCircle(uint8_t x, uint8_t y) ;
void OLED_DrawRectangle(uint8_t x, uint8_t y) ;
void OLED_DrawCursor(uint8_t x, uint8_t y) ;
void OLED_SplashScreen(void) ;
void OLED_MainScreen(void) ;


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif

/*******************************************************************************
 End of File
 */

