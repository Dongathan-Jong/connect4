#include <Adafruit_ST7735.h>

// define pins for screen
#define TFT_SCLK 2
#define TFT_RST 4
#define TFT_CS 5
#define TFT_MOSI 13
#define TFT_DC 16

// define pins for buttons
#define RIGHTBUTTON 12
#define MIDDLEBUTTON 0
#define LEFTBUTTON 14

//define the arrow bitmap
const unsigned char arrow[] PROGMEM = {
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x01, 0xe0, 0x00,
  0x01, 0xe0, 0x00,
  0x01, 0xe0, 0x00,
  0x01, 0xe0, 0x00,
  0x01, 0xe0, 0x00,
  0x01, 0xe0, 0x00,
  0x01, 0xe0, 0x00,
  0x1f, 0xfe, 0x00,
  0x0f, 0xfc, 0x00,
  0x07, 0xf8, 0x00,
  0x03, 0xf0, 0x00,
  0x01, 0xe0, 0x00,
  0x00, 0xc0, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00
};

//define the circle bitmap
const unsigned char circle[] PROGMEM = {
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x07, 0x80,
  0x1f, 0xe0,
  0x1f, 0xe0,
  0x3f, 0xf0,
  0x3f, 0xf0,
  0x3f, 0xf0,
  0x3f, 0xf0,
  0x1f, 0xe0,
  0x1f, 0xe0,
  0x07, 0x80,
  0x00, 0x00,
  0x00, 0x00
};

//create and initialize variables + array
int gameBoard[6][7];
int playerTurn = 1;
int lastSlotX = 0;
int lastSlotY = 0;
int chainLength = 1;
int winner = 0;
int currentSlot = 0;
int lastSlot = -1;
boolean startUI = true;
boolean drawWinScreen = true;

// create the screen object
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  // start serial monitor, tft, and initalize tft
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("tft set");

  //set pin modes for buttons to be inputs
  pinMode(RIGHTBUTTON, INPUT_PULLUP);
  pinMode(MIDDLEBUTTON, INPUT_PULLUP);
  pinMode(LEFTBUTTON, INPUT_PULLUP);

  // create and initialize the board on the tft
  drawBoard();
  createBoard();
}

void loop() {
  // always run
  while (true) {
    // only run when in game, not win screen
    while (playerTurn != 0) {
      // player 1 turn
      if (playerTurn == 1) {
        // first turn start
        if (startUI) {
          // fill border with respective colour and prompt user
          tft.fillRect(23, 17, 120, 18, ST77XX_BLACK);
          tft.fillRect(20, 3, 130, 8, ST77XX_BLACK);
          drawBorder(playerTurn);
          tft.setCursor(50, 3);
          tft.write("Player 1! ");
          tft.setCursor(15, 10);
          tft.write("Please select a slot:");
          startUI = false;
          currentSlot = 0;
          drawArrow();
          drawWinScreen = true;
        }
        // check if the right button was pressed
        if (digitalRead(RIGHTBUTTON) == LOW) {
          // check if current slot is outside of bounds
          if (currentSlot < 6) {
            // change slot index and draw the new arrow
            currentSlot++;
            drawArrow();
          }
        }

        // check if the left button was pressed
        if (digitalRead(LEFTBUTTON) == LOW) {
          // check if current slot is outside of bounds
          if (currentSlot > 0) {
            // change slot index and draw the new arrow
            currentSlot--;
            drawArrow();
          }
        }

        // check if middle button was pressed
        if (digitalRead(MIDDLEBUTTON) == LOW) {
          // drop the piece in the slot
          dropPiece();
        }

        // check if there is a winner
        if (winner != 0) {
          break;
        }
        // delay so that the esp doesnt go crazy
        delay(100);
      }

      //second player turn, same comments
      else if (playerTurn == 2) {
        if (startUI) {
          tft.fillRect(23, 17, 120, 18, ST77XX_BLACK);
          tft.fillRect(20, 3, 130, 8, ST77XX_BLACK);
          drawBorder(playerTurn);
          tft.setCursor(50, 3);
          tft.write("Player 2!");
          tft.setCursor(15, 10);
          tft.write("Please select a slot: ");
          startUI = false;
          currentSlot = 0;
          drawArrow();
          drawWinScreen = true;
        }
        if (digitalRead(RIGHTBUTTON) == LOW) {
          if (currentSlot < 6) {
            currentSlot++;
            drawArrow();
          }
        }

        if (digitalRead(LEFTBUTTON) == LOW) {
          if (currentSlot > 0) {
            currentSlot--;
            drawArrow();
          }
        }

        if (digitalRead(MIDDLEBUTTON) == LOW) {
          dropPiece();
        }

        if (winner != 0) {
          break;
        }
        delay(100);
      }
    }

    // check if a player has won
    if (drawWinScreen) {
      // if iuts player 1
      if (winner == 1) {
        // draw respective win screen
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextSize(2);
        tft.setCursor(35, 30);
        tft.write("PLAYER 1 ");
        tft.setCursor(55, 50);
        tft.write("WINS!");
        tft.drawRect(0, 0, 160, 128, ST77XX_RED);
        tft.drawRect(1, 1, 158, 126, ST77XX_RED);
        tft.setCursor(50, 70);
        tft.setTextSize(1);
        tft.write("Play again? ");
        tft.setCursor(25, 80);
        tft.write("Press center button");
        drawWinScreen = false;
        // add delay
      }
      // if its player 2
      else if (winner == 2) {
        // draw respective win screen
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextSize(2);
        tft.setCursor(35, 30);
        tft.write("PLAYER 2 ");
        tft.setCursor(55, 50);
        tft.write("WINS!");
        tft.drawRect(0, 0, 160, 128, ST77XX_YELLOW);
        tft.drawRect(1, 1, 158, 126, ST77XX_YELLOW);
        tft.setCursor(50, 70);
        tft.setTextSize(1);
        tft.write("Play again? ");
        tft.setCursor(25, 80);
        tft.write("Press center button");
        drawWinScreen = false;
        // add delay
      }
    }
    // check if middle button was pressed
    if (digitalRead(MIDDLEBUTTON) == LOW) {
      //reset variables and start next game
      startUI = true;
      playerTurn = 1;
      winner = 0;
      createBoard();
      lastSlot = -1;
      tft.fillScreen(ST77XX_BLACK);
      drawBoard();
      chainLength = 1;
    }
    //delay so that esp doesnt explode lol
    delay(200);
    // create play again screen
  }
}

// creates the board and makes everything 0 (empty)
void createBoard() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      gameBoard[i][j] = 0;
    }
  }
  Serial.println("Board Created!");
}

// draws the border
void drawBorder(int player) {
  if (player == 1) {
    tft.drawRect(0, 0, 160, 128, ST77XX_RED);
    tft.drawRect(1, 1, 158, 126, ST77XX_RED);
  } else if (player == 2) {
    tft.drawRect(0, 0, 160, 128, ST77XX_YELLOW);
    tft.drawRect(1, 1, 158, 126, ST77XX_YELLOW);
  }
}

//draws the arrow
void drawArrow() {
  int arrowX = 23 + (currentSlot * 15);

  int lastArrowX = 23 + (lastSlot * 15);
  if (lastSlot == -1) {
    lastArrowX += 15;
  }
  if (currentSlot != lastSlot) {
    tft.fillRect(lastArrowX, 17, 18, 18, ST77XX_BLACK);
    tft.drawBitmap(arrowX, 17, arrow, 18, 18, ST77XX_WHITE);
    lastSlot = currentSlot;
  }
}

// drops the piece
void dropPiece() {
  for (int i = 5; i >= 0; i--) {
    if (gameBoard[i][currentSlot] == 0) {
      int xCoord = 26 + (currentSlot * 15);
      int yCoord = 35 + (i * 15);
      if (playerTurn == 1) {
        tft.drawBitmap(xCoord, yCoord, circle, 15, 15, ST77XX_RED);
        gameBoard[i][currentSlot] = 1;

        lastSlotX = currentSlot;
        lastSlotY = i;
        checkWin();
        if (playerTurn != 0) {
          playerTurn = 2;
        }
        startUI = true;
        lastSlot = -1;
        currentSlot = 1;
        break;
      } else if (playerTurn == 2) {
        tft.drawBitmap(xCoord, yCoord, circle, 15, 15, ST77XX_YELLOW);
        gameBoard[i][currentSlot] = 2;

        lastSlotX = currentSlot;
        lastSlotY = i;
        checkWin();

        if (playerTurn != 0) {
          playerTurn = 1;
        }
        startUI = true;
        lastSlot = -1;
        currentSlot = 1;
        break;
      }
    } else if (gameBoard[0][currentSlot] == 1 || gameBoard[0][currentSlot] == 2) {
      tft.fillRect(15, 3, 125, 15, ST77XX_BLACK);
      tft.setCursor(25, 3);
      tft.write("Pick another slot!");
      break;
    }
  }
}

// checks the piece
void checkPieces() {
  /*
  for(int i = 5; i >= 0; i--)
  {
    if(gameBoard[i][currentSlot] == 0)
    {
      int xCoord = 26 + (currentSlot * 15);
      int yCoord = 35 + (i * 15);
      if(playerTurn == 1)
      {
        tft.drawBitmap(xCoord,yCoord,circle,15,15,ST77XX_RED);
        gameBoard[i][currentSlot] = 1;

        lastSlotX = currentSlot;
        lastSlotY = i;
        checkWin();

        playerTurn = 2;
        startUI = true;
        lastSlot = -1;
        currentSlot = 1;
        break;
      }
      else if(playerTurn == 2)
      {
        tft.drawBitmap(xCoord,yCoord,circle,15,15,ST77XX_YELLOW);
        gameBoard[i][currentSlot] = 2;

        lastSlotX = currentSlot;
        lastSlotY = i;
        checkWin();

        playerTurn = 1;
        startUI = true;
        lastSlot = -1;
        currentSlot = 1;
        break;
      }
    }
    else if(gameBoard[0][currentSlot] == 1 || gameBoard[0][currentSlot] == 2)
    {
      tft.fillRect(15,3,125,15,ST77XX_BLACK);
      tft.setCursor(25,3);
      tft.write("Pick another slot!");
      break;
    }

    Serial.println("---------------------------------");
    //check up
    // debug these 
    for(int i = lastSlotY-1; i >= 0; i--)
    {
      if(gameBoard[i][lastSlotX] == playerTurn)
      {
        chainLength++;
      }
    }
    Serial.print("Up chain length: ");
    Serial.println(chainLength);
    if(chainLength >= 4)
    {
      winner = playerTurn;
      Serial.println("Win detected!");
    }

    chainLength = 1;
    //check down
  }*/
}

// checks if there is a winning chain
void checkWin() {
  Serial.println("---------------------------------");
  //check up
  // debug these
  for (int i = lastSlotY - 1; i >= 0; i--) {
    if (gameBoard[i][lastSlotX] == playerTurn) {
      chainLength++;
    } else {
      break;
    }
  }
  Serial.print("Up chain length: ");
  Serial.println(chainLength);
  if (chainLength >= 4) {
    winner = playerTurn;
    Serial.println("Win detected!");
    playerTurn = 0;
    return;
  }

  chainLength = 1;
  //check down

  for (int i = lastSlotY + 1; i < 6; i++) {
    if (gameBoard[i][lastSlotX] == playerTurn) {
      chainLength++;
    } else {
      break;
    }
  }
  Serial.print("Down chain length: ");
  Serial.println(chainLength);
  if (chainLength >= 4) {
    winner = playerTurn;
    Serial.println("Win detected!");
    playerTurn = 0;
    return;
  }

  chainLength = 1;
  //check left
  // omg this is making me crash out WHY IS THIS THE ONLY ONE THAT DOESNT WRK WTASDFOIAHSEGFOIASJEGIO
  for (int i = lastSlotX - 1; i >= 0; i--) {
    if (gameBoard[lastSlotY][i] == playerTurn) {
      chainLength++;
    } else {
      break;
    }
  }
  Serial.print("Left chain length: ");
  Serial.println(chainLength);
  if (chainLength >= 4) {
    winner = playerTurn;
    Serial.println("Win detected!");
    playerTurn = 0;
    return;
  }

  chainLength = 1;
  //check right

  for (int i = lastSlotX + 1; i < 7; i++) {
    if (gameBoard[lastSlotY][i] == playerTurn) {
      chainLength++;
    } else {
      break;
    }
  }
  Serial.print("Right chain length: ");
  Serial.println(chainLength);
  if (chainLength >= 4) {
    winner = playerTurn;
    Serial.println("Win detected!");
    playerTurn = 0;
    return;
  }

  chainLength = 1;
  //check diag one
  //up left
  int x = lastSlotX - 1;
  int y = lastSlotY - 1;

  while (x >= 0 && y >= 0 && gameBoard[y][x] == playerTurn) {
    chainLength++;
    x--;
    y--;
  }

  // down right LAIEJDFHOLKASEJGFIOASJFGOIJSDOFGASDOFAOSIJF

  x = lastSlotX + 1;
  y = lastSlotY + 1;

  while (x < 7 && y < 6 && gameBoard[y][x] == playerTurn) {
    chainLength++;
    x++;
    y++;
  }

  Serial.print("Diagonal \ chain length: ");
  Serial.println(chainLength);

  if (chainLength >= 4) {
    winner = playerTurn;
    Serial.println("Win detected!");
    playerTurn = 0;
    return;
  }

  chainLength = 1;


  //check diag two

  x = lastSlotX - 1;
  y = lastSlotY + 1;

  while (x >= 0 && y < 6 && gameBoard[y][x] == playerTurn) {
    chainLength++;
    x--;
    y++;
  }

  x = lastSlotX + 1;
  y = lastSlotY - 1;

  while (x < 7 && y >= 0 && gameBoard[y][x] == playerTurn) {
    chainLength++;
    x++;
    y--;
  }

  Serial.print("Diagonal / chain length: ");
  Serial.println(chainLength);

  if (chainLength >= 4) {
    winner = playerTurn;
    Serial.println("Win detected!");
    playerTurn = 0;
    return;
  }
}

void drawBoard() {
  tft.fillRect(25, 35, 105, 1, ST77XX_WHITE);
  tft.fillRect(25, 35, 1, 90, ST77XX_WHITE);
  tft.fillRect(25, 50, 105, 1, ST77XX_WHITE);
  tft.fillRect(40, 35, 1, 90, ST77XX_WHITE);
  tft.fillRect(25, 65, 105, 1, ST77XX_WHITE);
  tft.fillRect(55, 35, 1, 90, ST77XX_WHITE);
  tft.fillRect(25, 80, 105, 1, ST77XX_WHITE);
  tft.fillRect(70, 35, 1, 90, ST77XX_WHITE);
  tft.fillRect(25, 95, 105, 1, ST77XX_WHITE);
  tft.fillRect(85, 35, 1, 90, ST77XX_WHITE);
  tft.fillRect(25, 110, 105, 1, ST77XX_WHITE);
  tft.fillRect(100, 35, 1, 90, ST77XX_WHITE);
  tft.fillRect(25, 125, 105, 1, ST77XX_WHITE);
  tft.fillRect(115, 35, 1, 90, ST77XX_WHITE);
  tft.fillRect(130, 35, 1, 91, ST77XX_WHITE);
}

