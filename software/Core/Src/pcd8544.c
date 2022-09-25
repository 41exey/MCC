/*
 * Copyright 2015 Gauss
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "pcd8544.h"
#include "font6x8.h"
#include <string.h>


// Managing LCD_CS
#define LCD_CS1 LCD_PORT->BSRR = SCE
#define LCD_CS0 LCD_PORT->BRR = SCE
// Managing LCD_RST
#define LCD_RST1        LCD_PORT->BSRR = RST
#define LCD_RST0        LCD_PORT->BRR = RST
// Managing LCD_DC
#define LCD_DC1 LCD_PORT->BSRR = DC
#define LCD_DC0 LCD_PORT->BRR = DC
// Managing LCD_SCK
#define LCD_SCK1        LCD_PORT->BSRR = SCLK
#define LCD_SCK0        LCD_PORT->BRR = SCLK
// Managing LCD_MOSI
#define LCD_MOSI1       LCD_PORT->BSRR = MOSI
#define LCD_MOSI0       LCD_PORT->BRR = MOSI

/// Display buffer
unsigned char lcd8544_buff[84 * 6]; 

/**
 * Program delay in milliseconds
 * @param ms delay in milliseconds
 */
void Delay_ms(uint32_t ms) {
  for(int i = 0; i < ms; i ++)
  {//48000000
    for(int j = 0; j < 48000; j ++) // 48000
    {
      __NOP();
    }
  }
}

/**
 * Clear the display buffer
 */
void lcd8544_clear(void) {

	for(int i=0; i < 504; i ++)
		lcd8544_buff[i] = 0;

}

/**
 * Send data\\command to display
 */
void lcd8544_senddata(unsigned char data) {
	unsigned char i;
	for(i = 0; i < 8; i++) {
		if (data & 0x80)
			LCD_MOSI1;
		else 
			LCD_MOSI0;
		data = data<<1;
		LCD_SCK0;
		LCD_SCK1;
	}
}

/**
 * Refresh display from buffer
 */
void lcd8544_refresh(void) {
    Delay_ms(1);
	LCD_CS0;            // ÑS=0 - seance start
    Delay_ms(1);
	LCD_DC0;            // transmit
    Delay_ms(1);
	lcd8544_senddata(0x40); // set position Y=0; X=0
    Delay_ms(1);
	lcd8544_senddata(0x80);
    Delay_ms(1);
	LCD_DC1;            // transfer

	unsigned char y, x;

	for(y = 0; y < 6; y++)
		for (x = 0; x < 84; x++)
			lcd8544_senddata(lcd8544_buff[y * 84 + x]);
}


/**
 * Initialization
 */
void lcd8544_init(void) {
          
    Delay_ms(1);
	LCD_SCK0;
    Delay_ms(1);
	// reset
	LCD_CS0;            // CS=0  - seance start
    Delay_ms(1);
	LCD_RST0;           // RST=0 - reset
    Delay_ms(1);
	LCD_RST1;           // RST=1
    Delay_ms(1);
	// init
	LCD_DC0;            // transfer command
    Delay_ms(1);
	lcd8544_senddata(0x21);      // switch to advance
    // 0x21 = b10101
    Delay_ms(1);
          
	lcd8544_senddata(0xC1);
    // 0xC1 = b11000001
    Delay_ms(1);
	lcd8544_senddata(0x06);		// temperature: from 4 to 7
    Delay_ms(1);
	lcd8544_senddata(0x13);		// Bias 0b0001 0xxx - contrast
    Delay_ms(1);
	lcd8544_senddata(0x20); 	// switch to normal
    Delay_ms(1);
	lcd8544_senddata(0x000C);	// 0b1100 - normal mode
    Delay_ms(1);
			                    // 0b1101 - invert mode
			                    // 0b1001 - light
			                    // 0b1000 - clean
          
        // my added
        // 0b1100 = 0x000C
        // 0b1101 = 0x000D
        // 0b1001 = 0x0009
        // 0b1000 = 0x0008

	LCD_DC1;            // transfer command
    Delay_ms(1);
	lcd8544_refresh();
}

/**
 * Enter the display to save power state
 */
void lcd8544_power_down(void) {
	LCD_CS0;            // ÑS=0 - start seance
	Delay_ms(1);
	LCD_DC0;            // transfer command
	Delay_ms(1);
	lcd8544_senddata(0x24);
}

/**
 * Return the display from save power state
 */
void lcd8544_power_up(void) {
	LCD_CS0;            // ÑS=0 - start seance
	Delay_ms(1);
	LCD_DC0;            // transfer command
	Delay_ms(1);
	lcd8544_senddata(0x20);
}

/**
 * Draw pixel
 */
void lcd8544_putpix(unsigned char x, unsigned char y, unsigned char mode) {
	
	if ((x > 84) || (y > 47))
		return;

	unsigned int adr = (y >> 3) * 84 + x;
	unsigned char data = (1 << (y & 0x07));

	if (mode)
		lcd8544_buff[adr] |= data;
	else
		lcd8544_buff[adr] &= ~data;
}

/**
 * Draw line
 */
void lcd8544_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode) {
	signed char   dx, dy, sx, sy;
	unsigned char  x,  y, mdx, mdy, l;


	dx = x2 - x1;
	dy = y2 - y1;

	if (dx >= 0) { 
		mdx = dx; 
		sx = 1; 
	} else { 
		mdx = x1 - x2;
		sx = -1;
	}
	
	if (dy >= 0) {
		mdy = dy;
		sy = 1;
	} else { 
		mdy = y1 - y2;
		sy = -1;
	}

	x = x1;
	y = y1;

	if (mdx >= mdy) {
		l = mdx;
		while(l > 0) {
			if (dy > 0) { 
				y = y1 + mdy * (x - x1) / mdx;
			} else {
				y = y1 - mdy * (x - x1) / mdx;
			}
			lcd8544_putpix(x, y, mode);
			x = x + sx;
			l--;
		}
	} else {
		l = mdy;
		while (l > 0) {
			if (dy > 0) { 
				x = x1 +((mdx * (y - y1)) / mdy);
			} else {
				x = x1 + ((mdx * (y1 - y)) / mdy);
			}
			lcd8544_putpix(x, y, mode);
			y = y + sy;
			l--;
		}
	}
	lcd8544_putpix(x2, y2, mode);

}

/**
 * Draw reactangle
 */
void lcd8544_rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode) {
	lcd8544_line(x1, y1, x2, y1, mode);
	lcd8544_line(x1, y2, x2, y2, mode);
	lcd8544_line(x1, y1, x1, y2, mode);
	lcd8544_line(x2, y1, x2, y2, mode);
}

/**
 * Put char in buffer
 */
void lcd8544_putchar(unsigned char px, unsigned char py, unsigned char ch, unsigned char mode) {
	const unsigned char *fontpointer;

	if (ch < 127) {	// latin
		fontpointer = NewFontLAT;
		ch = ch - 32;
	} else {	// russian
		fontpointer = NewFontRUS;
		ch = ch - 192;
	}

    unsigned char lcd_YP = 7 - (py & 0x07);    // bit position char in byte
    unsigned char lcd_YC = (py & 0xF8) >> 3; 	// bit position char in display
    unsigned char x;
	for(x = 0; x < 6; x++) {

		unsigned char temp=*(fontpointer+ch*6+x);

		if (mode != 0) {
			temp = ~temp;
			if (py > 0)
				lcd8544_putpix(px, py - 1, 1);	// if inverse
		}

		temp &= 0x7F;

		lcd8544_buff[lcd_YC * 84 + px] = lcd8544_buff[lcd_YC * 84 + px] | (temp << (7 - lcd_YP)); // print head of char

	    if (lcd_YP < 7)
			lcd8544_buff[(lcd_YC + 1) * 84 + px] = lcd8544_buff[(lcd_YC + 1) * 84 + px] | (temp>>(lcd_YP + 1)); // print foot of char

		px++;
		if (px > 83)
			return;
	}
}

/**
 * Put object
 */
void lcd8544_putobject(unsigned char px,unsigned char py,unsigned char width,unsigned char height,unsigned char object[]) {
	const unsigned char *fontpointer;
	
	if ((strcmp(object, "platform")) == 0)
		fontpointer=platform;
	
	if ((strcmp(object, "kiss")) == 0)
		fontpointer=kiss;
	
	if ((strcmp(object, "ball")) == 0) 
		fontpointer=ball;
	
	if ((strcmp(object, "strelka")) == 0) 
		fontpointer=strelka;


    unsigned char lcd_YP = 7 - (py & 0x07);
    unsigned char lcd_YC = (py & 0xF8) >> 3;
    unsigned ch = 0;
    unsigned char bank = 1;
    unsigned char x;

    if (height <= 8)
		bank=1;
	
    if (height <= 16 && height > 8)
		bank = 2;
	
    if (height <= 24 && height > 16)
		bank = 3;
	 
    if (height <= 32 && height > 24)
		bank = 4;
	
    if (height <= 40 && height > 32)
		bank = 5;
	
    if (height <= 48 && height > 40)
		bank = 6;
	
    for(ch = 0; ch < bank; ch++) {
    	for(x = 0; x < width; x++) {
    		unsigned char temp = *(fontpointer + ch * width + x);
    		lcd8544_buff[lcd_YC * 84 + px] = lcd8544_buff[lcd_YC * 84 + px] | (temp << (7 - lcd_YP));
            if (lcd_YP < 7)
				lcd8544_buff[(lcd_YC + 1) * 84 + px] = lcd8544_buff[(lcd_YC + 1) * 84 + px] | (temp >> (lcd_YP + 1));
            px++;
    		if (px > 83)
				return;

    	}
    	lcd_YC++;
    	px = px - width;
    }
}

/**
 * Draw line in left from char
 */
void lcd8544_leftchline(unsigned char x, unsigned char y) {
	
	if (x > 0) 
		lcd8544_line(x - 1, y - 1, x - 1, y + 6, 1);
	
}

/**
 * Draw a decimal number
 */
void lcd8544_dec(unsigned int numb, unsigned char dcount, unsigned char x, unsigned char y, unsigned char mode) {
	unsigned int divid = 10000;
	unsigned char i;

    if (mode)
		lcd8544_leftchline(x, y);
        
	for (i = 5; i != 0; i--) {

		unsigned char res = numb / divid;

		if (i <= dcount) {
			lcd8544_putchar(x, y, res + '0', mode);
			x = x + 6;
		}

		numb %= divid;
		divid /= 10;
	}
}

/**
 * Put string to buffer
 */
void lcd8544_putstr(unsigned char x, unsigned char y, const unsigned char str[], unsigned char mode) {
	
	if (mode)
		lcd8544_leftchline(x, y);
	
	while(*str!=0) {
		lcd8544_putchar(x, y, *str, mode);
		x = x + 6;
		str ++;
	}

}

/**
 * Works with float
 */
int gpow(int n, int power) {
	int res = 1;
	
	while(power--)
		res *= n;
	
	return res;
}

/**
 * Works with float
 */
unsigned char *ftoa(float f, int dec) {
	static unsigned char buf[16];
	unsigned char *p = buf + 15;
	int i = f * gpow(10, dec);
	int sign = i < 0 ? -1 : 1;
	i *= sign;
	do {
		*--p = '0' + (i % 10);
		i /= 10;
		if (--dec == 0) *--p = '.';
	}
	while (i != 0);
/*
	if (dec > 0) {
		while (dec-- > 0)
		*--p = '0';
		*--p = '.';
	}
*/	
	if (*p == '.') *--p = '0';

	if (sign < 0) *--p = '-';

	return p;
}
