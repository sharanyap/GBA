#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {


  // (1)
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)

        while (SCANLINECOUNTER> 160)
                ;
        while (SCANLINECOUNTER < 160)
                ;

  // (2)
  // Write a while loop that keeps going until we're in vBlank:

  // (3)
  // Finally, increment the vBlank counter:
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

void setPixel(int row, int col, u16 color) {
    videoBuffer[OFFSET(row, col, WIDTH)] = color;
}

void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
    /*for(int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            setPixel(row + r, col + c, color);

        }
    }*/
    volatile unsigned short lcolor;
    lcolor = color;
      for (int r = 0; r <= height; r++) {
            DMA[3].src = &lcolor;
            DMA[3].dst = &videoBuffer[OFFSET(row + r, col, WIDTH)];
            DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
      }
}

void drawFullScreenImageDMA(const u16 *image) {
    // TODO: IMPLEMENT
    for (int r = 0; r <= HEIGHT; r++) {
        DMA[3].src = &image[OFFSET(r, 0, WIDTH)];
        DMA[3].dst = &videoBuffer[OFFSET(r, 0, WIDTH)];
        DMA[3].cnt = WIDTH | DMA_ON | DMA_SOURCE_INCREMENT| DMA_DESTINATION_INCREMENT;
    }
}

void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
// TODO: IMPLEMENT
      for (int r = 0; r <= height; r++) {
            DMA[3].src = &image[OFFSET(r, 0, width)];
            DMA[3].dst = &videoBuffer[OFFSET(row + r, col, WIDTH)];
            DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
      }
}

int set_character(struct ascii_image *image, int row, int col, char c) {
    if (image == NULL) {
        return FAILURE;
    }
    if (row < 0 || row > image->height - 1) {
        return FAILURE;
    }
    if (col < 0 || col > image->width - 1) {
        return FAILURE;
    }

      for (int r = 0; r <= height; r++) {
            DMA[3].src = &image[OFFSET(r, 0, width)];
            DMA[3].dst = &videoBuffer[OFFSET(row + r, col, WIDTH)];
            DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
      }

     return SUCCESS;
}

struct ascii_image *create_image(int height, int width, char *name) {
    if (height < 1 || width < 1) {
        return NULL;
    }
    struct ascii_image *newimage;
    newimage = (struct ascii_image *)malloc(sizeof(struct ascii_image));
    if (newimage == NULL) {
        //free(newimage);
        return NULL;
    }
    for (int r = 0; r <= height; r++) {
        for (int c = 0; c <= width; c++) {
            videoBuffer[OFFSET(row, col, WIDTH)] = ".";
        }
    }
    if (name == NULL) {
        free(newimage);
        return NULL;
    } else {
        char *copy = malloc(strlen(name) + 1);
        strcpy(copy, name);
        newimage -> name = copy;
        newimage -> height = heigth;
        newimage -> width = width;
    }
    return newimage;

}

int add_extension(struct ascii_image *image, char *extension) {
    if (image == NULL) {
        return FAILURE;
    }

    char *temp = (char*)realloc(image -> name, strlen(image -> name) + strlen(extension) + 1);
    if (temp == NULL) {
        return FAILURE;
    }
    strcat(temp, image -> name);
    strcat(temp, extension);

    image -> name = temp;
    return SUCCESS;

}



void fillScreenDMA(volatile u16 color) {
    volatile unsigned short lcolor;
    lcolor = color;
    for (int r = -1; r <= HEIGHT; r++) {
        DMA[3].src = &lcolor;
        DMA[3].dst = &videoBuffer[OFFSET(r, WIDTH, WIDTH)];
        DMA[3].cnt = WIDTH | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
    }
}

void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawCenteredString(int row, int col, int width, int height, char *str, u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}
