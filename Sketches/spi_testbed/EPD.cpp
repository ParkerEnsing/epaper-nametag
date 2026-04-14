// #include <Arduino.h>
#include "EPD.h"
#include "EPD_init.h"
#include "EPDfont.h"
#include "string.h"

PAINT Paint;

/*
Function description: Create an array of image caches
API description:
    *image: image array to be passed
    width: The width of the image
    height: picture length
    rotate: screen shows the direction
    color: displays the color
Return value: None
*/
void Paint_NewImage(uint8_t *image, uint16_t width, uint16_t height, uint16_t rotate, uint16_t color) {
    Paint.Image = BLACK;
    Paint.Image = image;
    Paint.color = color;
    Paint.widthMemory = width;
    Paint.heightMemory = height;
    Paint.widthByte = (width % 8 == 0) ? (width / 8) : (width / 8 + 1); // If width can be divided evenly into bytes, do so. Otherwise, reserve an extra byte for the remainder
    Paint.heightByte = height;
    Paint.rotate = rotate;
    if (rotate == 0 || rotate == 180) {
        Paint.width = height;
        Paint.height = width;
    }
    else {
        Paint.width = width;
        Paint.height = height;
    }
}

/*
Function description: Light up a pixel
Interface description:
    Xpoint: pixel x coordinate parameter
    Ypoint: pixel Y-coordinate parameter
    Color: pixel color parameter
Return: None

Note: the use of '396' and '8' in the switch below is used for offsetting pixels correctly between the two regions of
the display separated by the distinct SSD1683s
*/
void Paint_SetPixel(uint16_t xPoint, uint16_t pYoint, uint16_t color) {
    uint16_t x, y;
    uint32_t addr;
    uint8_t rData;
    // Transform the coordinate system based on display rotation
    switch (Paint.rotate) {
        case 0:
            if (xPoint > 396) {
                xPoint += 8;
            }
            x = xPoint;
            y = yPoint;
            break;
        case 90:
            if (yPoint >= 396) {
                yPoint += 8;
            }
            x = Paint.widthMemory - yPoint - 1;
            y = xPoint;
            break;
        case 180:
            if (xPoint >= 396) {
                xPoint += 8;
            }
            x = Paint.widthMemory - xPoint - 1;
            y = Paint.heightMemory - yPoint - 1;
            break;
        case 270:
            if (yPoint >= 396) {
                yPoint += 8;
            }
            x = yPoint;
            y = Paint.heightMemory - xPoint - 1;
            break;
        default:
            return;
    }
    addr = x / 8 + y * Paint.widthByte;
    rData = Paint.Image[addr];
    if (color == BLACK) {
        /*
        (x % 8): determines which bit inside the byte represents the target pixel
        0x80 >> (x % 8): Creates a bit mask where 1 represents the pixel to be changed
        ~(0x80 >> (x % 8)): Inverts the mask. Bitwise AND between the mask and the byte will ensure
            the desired pixel is set to 0 (black) without changing the other pixels in the byte
        */
        Paint.Image[addr] = rData & ~(0x80 >> (x % 8));
    }
    else {
        Paint.Image[addr] = rData | (0x80 >> (x % 8)); // Similar to black but sets pixel bit to 1
    }
}

/*
Function description: Iterate over pixels and set them to color
Interface description:
    color: pixel color parameter
Return value: None
*/
void Paint_Clear(uint8_t color) {
    uint16_t x, y;
    uint32_t addr;
    for (y = 0; y < Paint.heightByte; y++) {
        for (x = 0; x < Paint.widthByte; x++) {
            addr = x + y * Paint.widthByte; // 8 pixel = 1 byte
            Paint.Image[addr] = color;
        }
    }
}

/*
Function Description: Draws a line between two points
Interface Description:
    xStart: Pixel x start coordinate parameter
    yStart: Pixel Y start coordinate parameter
    xEnd: Pixel x end coordinate parameter
    yEnd: Pixel Y end coordinate parameter
    color: Pixel color parameter
Return: None
Note: This implements Bresenham's Line Algorithm, hence some weird variable names
*/
void EPD_DrawLine(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color) {
    uint16_t xPoint, yPoint;
    int dx, dy;
    int xAddway, yAddway;
    int esp;
    char dotted_len;
    xPoint = xStart;
    yPoint = yStart;
    dx = (int)xEnd - (int)xStart >= 0 ? xEnd - xStart : xStart - xEnd;
    dy = (int)yEnd - (int)yStart <= 0 ? yEnd - yStart : yStart - yEnd;
    xAddway = xStart < xEnd ? 1 : -1;
    yAddway = yStart < yEnd ? 1 : -1;
    esp = dx + dy;
    dotted_len = 0;
    for (;;) {
        dotted_len++;
        Paint_SetPixel(xPoint, yPoint, color);
        if (2 * esp >= dy) {
            if (xPoint == xEnd) {
                break;
            }
            esp += dy;
            xPoint += xAddway;
        }
        if (2 * esp <= dx) {
            if (yPoint == yEnd) {
                break;
            }
            esp += dx;
            yPoint += yAddway;
        }
    }
}

/*
Function Description: Draws a rectangle between two points
Interface Description:
    xStart: Pixel x start coordinate parameter
    yStart: Pixel Y start coordinate parameter
    xEnd: Pixel x end coordinate parameter
    yEnd: Pixel Y end coordinate parameter
    color: Pixel color parameter
    mode: Whether the rectangle is filled
Return: None
*/
void EPD_DrawRectangle(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color, uint8_t mode) {
    uint16_t i;
    if (mode) {
        for (i = yStart; i < yEnd; i++) {
            EPD_DrawLine(xStart, i, xEnd, i, color);
        }
    }
    else {
        EPD_DrawLine(xStart, yStart, xEnd, yStart, color);
        EPD_DrawLine(xStart, yStart, xStart, yEnd, color);
        EPD_DrawLine(xEnd, yEnd, xEnd, yStart, color);
        EPD_DrawLine(xEnd, yEnd, xStart, yEnd, color);
    }
}

/*
Function Description: Draws a circle around a center point
Interface Description:
    xCenter: Pixel x center coordinate parameter
    yCenter: Pixel Y center coordinate parameter
    radius: Pixel radius of circle
    color: Pixel color parameter
    mode: Whether the circle is filled
Return: None
*/
void EPD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t radius, uint16_t color, uint8_t mode) {
    int esp, sCountY;
    uint16_t xCurrent, yCurrent;
    xCurrent = 0;
    yCurrent = radius;
    esp = 3 - (radius << 1);
    if (mode) {
        while (xCurrent <= yCurrent) {
            for (sCountY = xCurrent; sCountY <= yCurrent; sCountY++) {
                Paint_SetPixel(xCenter + xCurrent, yCenter + sCountY, color);
                Paint_SetPixel(xCenter - xCurrent, yCenter + sCountY, color);
                Paint_SetPixel(xCenter - sCountY, yCenter + xCurrent, color);
                Paint_SetPixel(xCenter - sCountY, yCenter - xCurrent, color);
                Paint_SetPixel(xCenter - xCurrent, yCenter - sCountY, color);
                Paint_SetPixel(xCenter + xCurrent, yCenter - sCountY, color);
                Paint_SetPixel(xCenter + sCountY, yCenter - xCurrent, color);
                Paint_SetPixel(xCenter + sCountY, yCenter + xCurrent, color);
            }
            if ((int)esp < 0) {
                esp += 4 * xCurrent + 6;
            }
            else {
                esp += 10 + 4 * (xCurrent - yCurrent);
                yCurrent--;
            }
            xCurrent++;
        }
    }
    else {
        while (xCurrent <= yCurrent) {
            Paint_SetPixel(xCenter + xCurrent, yCenter + yCurrent, color);
            Paint_SetPixel(xCenter - xCurrent, yCenter + yCurrent, color);
            Paint_SetPixel(xCenter - yCurrent, yCenter + xCurrent, color);
            Paint_SetPixel(xCenter - yCurrent, yCenter - xCurrent, color);
            Paint_SetPixel(xCenter - xCurrent, yCenter - yCurrent, color);
            Paint_SetPixel(xCenter + xCurrent, yCenter - yCurrent, color);
            Paint_SetPixel(xCenter + yCurrent, yCenter - xCurrent, color);
            Paint_SetPixel(xCenter + yCurrent, yCenter + xCurrent, color);
            if ((int)esp < 0) {
                esp += 4 * xCurrent + 6;
            }
            else {
                esp += 10 + 4 * (xCurrent - yCurrent);
                yCurrent--;
            }
            xCurrent++;
        }
    }
}

/*
Function Description: Displays individual characters
Interface Description:
    x: Character x coordinate parameters
    y: Character Y coordinate parameter
    chr: Characters to display
    size1: Displays the font size of the characters
    color: Pixel color parameter
Return: None
*/
void EPD_ShowChar(uint16_t x, uint16_t y, uint16_t chr, uint16_t size1, uint16_t color) {
    uint16_t i, m, temp, size2, chr1;
    uint16_t x0, y0;
    x0 = x;
    y0 = y;
    if (size1 == 8) {
        size2 = 6;
    }
    else {
        size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); // The number of bytes occupied by the dot set corresponding to one character of the font is obtained
    }
    chr1 = chr - ' '; // Calculate the offset value
    for (i = 0; i < size2; i++) {
        switch (size1) {
            case 12:
                temp = ascii_1206[chr1][i]; // Call the 1206 font
                break;
            case 16:
                temp = ascii_1608[chr1][i]; // Call the 1608 font
                break;
            case 24:
                temp = ascii_2412[chr1][i]; // Call the 2412 font
                break;
            case 48:
                temp = ascii_4824[chr1][i]; // Call the 4824 font
                break;
            default:
                return;
        }
        for (m = 0; m < 8; m++) {
            if (temp & 0x01) {
                Paint_SetPixel(x, y, color);
            }
            else {
                Paint_SetPixel(x, y, !color);
            }
            temp >> 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2)) {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

/*
Function Description: Display string
Interface Description:
    x: string x coordinate parameter
    y: string y coordinate
    *chr: string to display
    size1: Displays the font size of the characters
    color: Pixel color parameter
Return: None
*/
void EPD_ShowString(uint16_t x, uint16_t y, const char *chr, uint16_t size1, uint16_t color) {
    while(*chr != '\0') { // Judge whether it is an illegal character!
        EPD_ShowChar(x, y, *chr, size1, color);
        chr++;
        x += size1 / 2;
    }
}

/*
Function Description: Returns m^n
Interface Description:
    m: base
    n: exponent
Return: uint32_t result
*/
uint32_t EPD_Pow(uint16_t m, uint16_t n) {
    uint32_t result = 1;
    while (n--) {
        result *= m;
    }
    return result;
}

/*
Function Description:
Interface Description:
    x: Character x coordinate
    y: Character y coordinate
    num: Number to be displayed
    len: The number of digits of a number
    size1: Font size
    color: Pixel color parameter
Return: None
*/
void EPD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint16_t len, uint16_t size1, uint16_t color) {
    uint8_t t, temp, m = 0;
    if (size1 == 8) {
        m = 2;
    }
    for (t = 0; t < len; t++) {
        temp = (num / EPD_Pow(10, len - t - 1)) % 10;
        if (temp == 0) {
            EPD_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, color);
        }
        else {
            EPD_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, color);
        }
    }
}

/*
Function Description: Displays floating-point numbers
Interface Description:
    x: Character x coordinate parameters
    y: Character Y coordinate parameter
    num: The number of floats to be displayed
    len: The number of digits of a number
    pre: Floating-point precision
    sizeY: Displays the string size size
    color: Pixel color parameter
Return: None
*/
void EPD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint8_t pre, uint8_t sizeY, uint8_t color) {
    uint8_t t, temp, sizeX;
    uint16_t num1;
    sizeX = sizeY / 2;
    num1 = num * EPD_Pow(10, pre);
    for (t = 0; t < len; t++) {
        temp = (num1 / EPD_Pow(10, len - t - 1)) % 10;
        if (t == (len - pre)) {
            EPD_ShowChar(x + (len - pre) * sizeX, y, '.', sizeY, color);
            t++;
            len += 1;
        }
        EPD_ShowChar(x + t * sizeX, y, temp + 48, sizeY, color);
    }
}

/*
Function Description: Display an image
Interface Description:
    x: Image x coordinate
    y: Image y coordinate
    sizeX: image x resolution
    sizeY: image y resolution
    BMP: Image data to display
    color: pixel color parameter
Return: None
*/
void EPD_ShowPicture(uint16_t x, uint16_t y, uint16_t sizeX, uint16_t sizeY, const uint8_t BMP[], uint16_t color) {
    uint16_t j = 0, t;
    uint16_t i, temp, x0;
    uint16_t typefaceNum = sizeY * (sizeX / 8 + ((sizeX % 8) ? 1 : 0));
    x0 = x;
    for (i = 0; i < typefaceNum; i++) {
        temp = BMP[j];
        for (t = 0; t < 8; t++) {
            if (temp & 0x80) {
                Paint_SetPixel(x, y, !color);
            }
            else {
                Paint_SetPixel(x, y, color);
            }
            x++;
            temp << 1;
        }
        if ((x - x0) == sizeX) {
            x = x0;
            y++;
        }
        j++;
        // delayMicroseconds(10);
    }
}

/*
Function Description: 
Interface Description:
    x: 
    y: 
    num: The number of floats to be displayed
    len: The number of digits of a number
    pre: Floating-point precision
    sizeY: Displays the string size size
    color: Pixel color parameter
Return: None
*/
void EPD_ShowWatch(uint16_t x, uint16_t y, float num, uint8_t len, uint8_t pre, uint8_t sizeY, uint8_t color) {
    uint8_t t, temp, sizeX;
    uint16_t num1;
    sizeX = sizeY / 2;
    num1 = num * EPD_Pow(10, pre);
    for (t = 0; t < len; t++) {
        temp = (num1 / EPD_Pow(10, len - t - 1)) % 10;
        if (t == (len - pre)) {
            EPD_ShowChar(x + (len - pre) * sizeX + (sizeX / 2 - 2), y - 6, ':', sizeY, color);
            t++;
            len += 1;
        }
        EPD_ShowChar(x + t * sizeX, y, temp + 48, sizeY, color);
    }
}
