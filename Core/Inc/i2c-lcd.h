#include "stm32f1xx_hal.h"

#define LCD_ROWS 4
#define LCD_COLS 20


void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_clear (void);  

void lcd_cursor_pos(char x, char y);
void lcd_send_char (char chr);
void lcd_create_char(uint8_t *chr, uint8_t addr);
