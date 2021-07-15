#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"
#include "images/house.h"
#include "images/gbachar.h"
#include "images/welcomescreen.h"
#include "images/endscreen.h"

/* TODO: */
// Add any additional states you need for your app. You are not required to use
// these specific provided states.
enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

int main(void) {
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  //int deltas[] = { -3, -2, -1, 1, 2, 3 };
  //int ndeltas = sizeof(deltas) / sizeof(deltas[0]);
  struct house gbahouse;
    gbahouse.row = HEIGHT/2;
    gbahouse.col = 10;
    gbahouse.width = 44;
    gbahouse.height = 41;
  struct dog characdog, previousdog;
          characdog.row = 0;
          characdog.col = 0;
          previousdog = characdog;

  int stop = 0;
  int onlybackspace = 0;
  int changetext = 0;

  // start screen
  drawFullScreenImageDMA(welcomescreen);
  while (1) {

    if ((previousdog.row > gbahouse.row - 10) && (previousdog.row < gbahouse.row + gbahouse.height-10)
        && (previousdog.col > gbahouse.col) && (previousdog.col < gbahouse.col + gbahouse.width) ) {
            waitForVBlank();
            drawFullScreenImageDMA(endscreen);
            stop = 0;
        }


     if (KEY_DOWN(BUTTON_SELECT, BUTTONS)) {
        waitForVBlank();
        drawFullScreenImageDMA(welcomescreen);
        characdog.row = 0;
        characdog.col = 0;
        previousdog = characdog;
        onlybackspace = 0;


     }
    if (onlybackspace == 0) {
    // start STATE
    if (KEY_DOWN(BUTTON_START, BUTTONS)) {
        stop = 1;
        waitForVBlank();
        fillScreenDMA(BLACK);
        characdog.row = HEIGHT/2;
        characdog.col = 215;
        previousdog.row = characdog.row;
        previousdog.col = characdog.col;
        drawImageDMA(characdog.row, characdog.col, 21, 21, gbachar);
        drawImageDMA(gbahouse.row, gbahouse.col, gbahouse.width, gbahouse.height, house);
        onlybackspace = 1;
    }
    }
    if (stop == 1) {
    if (KEY_DOWN(BUTTON_LEFT, BUTTONS)) {
        waitForVBlank();
        changetext = 1;
        previousdog = characdog;
        if ((characdog.row >= HEIGHT/2 && characdog.row <= (HEIGHT/2 + 2) )&& characdog.col > 54) {

            drawRectDMA(previousdog.row, previousdog.col, 21, 21, BLACK);
            characdog.col = characdog.col - 1;
            previousdog.col = characdog.col;
            drawImageDMA(characdog.row, characdog.col, 21, 21, gbachar);
        } else {
            if (characdog.col > 5) {
            drawRectDMA(previousdog.row, previousdog.col, 21, 21, BLACK);
            characdog.col = characdog.col - 1;
            previousdog.col = characdog.col;
            drawImageDMA(characdog.row, characdog.col, 21, 21, gbachar);
            }


        }

                if (changetext) {
                    changetext = 0;

                    char buffer[51];
                    int distance = (characdog.col - gbahouse.col) - 40;


                    sprintf(buffer, "YOU ARE %d STEPS closer!", distance);
                    waitForVBlank();
                    drawRectDMA(150, 5, 200, 10, BLACK);

                    drawString(150, 5, buffer, MAGENTA);

                }


    }
    if (KEY_DOWN(BUTTON_RIGHT, BUTTONS)) {
        waitForVBlank();

        previousdog = characdog;
        if (characdog.col < 215) {

            drawRectDMA(previousdog.row, previousdog.col, 21, 21, BLACK);
            characdog.col = characdog.col + 1;
            previousdog.col = characdog.col;
            drawImageDMA(characdog.row, characdog.col, 21, 21, gbachar);
        }



    }

    if (KEY_DOWN(BUTTON_UP, BUTTONS)) {
        waitForVBlank();
        previousdog = characdog;
        if (characdog.row > 5) {

            drawRectDMA(previousdog.row, previousdog.col, 21, 21, BLACK);
            characdog.row = characdog.row - 1;
            previousdog.row = characdog.row;
            drawImageDMA(characdog.row, characdog.col, 21, 21, gbachar);
        }

    }

    if (KEY_DOWN(BUTTON_DOWN, BUTTONS)) {
        waitForVBlank();
        previousdog = characdog;
        if (characdog.row < 135) {

            drawRectDMA(previousdog.row, previousdog.col, 21, 21, BLACK);
            characdog.row = characdog.row + 1;
            previousdog.row = characdog.row;
            drawImageDMA(characdog.row, characdog.col, 21, 21, gbachar);
        }

    }
    }



  }




}

/*void changeString(void) {
        char buffer[51];
        const char *a[3];
        a[0] = "Keep going!";
        a[1] = "You got this";
        a[2] = "GO GO GO!";
		sprintf(buffer, "%s", a[rand() % 3]);
		drawRectDMA(150, 5, 200, 10, BLACK);
		drawString(150, 5, buffer, YELLOW);
}
*/