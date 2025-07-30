#include <Adafruit_ST7735.h>

#define TFT_SCLK 2
#define TFT_RST 4
#define TFT_CS 5
#define TFT_MOSI 13
#define TFT_DC 16

#define RIGHTBUTTON 12
#define MIDDLEBUTTON 0
#define LEFTBUTTON 14

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

int gameBoard[6][7];
int playerTurn = 1;
int numTurns = 0;
int lastSlotX = 0;
int lastSlotY = 0;
int chainLength = 0;
int winner = 0;
int currentSlot = 0;
int lastSlot = -1;
boolean startUI = true;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() 
{
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("tft set");

  pinMode(RIGHTBUTTON, INPUT_PULLUP);
  pinMode(MIDDLEBUTTON, INPUT_PULLUP);
  pinMode(LEFTBUTTON, INPUT_PULLUP);

  tft.fillRect(25,35,105,1,ST77XX_WHITE);
  tft.fillRect(25,35,1,90,ST77XX_WHITE);
  tft.fillRect(25,50,105,1,ST77XX_WHITE);
  tft.fillRect(40,35,1,90,ST77XX_WHITE);
  tft.fillRect(25,65,105,1,ST77XX_WHITE);
  tft.fillRect(55,35,1,90,ST77XX_WHITE);
  tft.fillRect(25,80,105,1,ST77XX_WHITE);
  tft.fillRect(70,35,1,90,ST77XX_WHITE);
  tft.fillRect(25,95,105,1,ST77XX_WHITE);
  tft.fillRect(85,35,1,90,ST77XX_WHITE);
  tft.fillRect(25,110,105,1,ST77XX_WHITE);
  tft.fillRect(100,35,1,90,ST77XX_WHITE);
  tft.fillRect(25,125,105,1,ST77XX_WHITE);
  tft.fillRect(115,35,1,90,ST77XX_WHITE);
  tft.fillRect(130,35,1,91,ST77XX_WHITE);
  createBoard();
}

void loop()
{
  
  while(true)
  {
    while(playerTurn != 0)
    {
      if(playerTurn == 1)
      {
        if(startUI)
        {
          tft.fillRect(23,17,120,18,ST77XX_BLACK);
          tft.fillRect(50,3,50,9,ST77XX_BLACK);
          drawBorder(playerTurn);
          tft.setCursor(50,3);
          tft.write("Player 1! ");
          tft.setCursor(15,10);
          tft.write("Please select a slot:");
          startUI = false;
          currentSlot = 0;
          drawArrow();
        }
        if(digitalRead(RIGHTBUTTON) == LOW)
        {
          if(currentSlot < 6)
          {
            currentSlot++;
            drawArrow();
          }
        }
        
        if(digitalRead(LEFTBUTTON) == LOW)
        {
          if(currentSlot > 0)
          {
            currentSlot--;
            drawArrow();
          }
        }

        if(digitalRead(MIDDLEBUTTON) == LOW)
        {
          Serial.println("drop req");
          dropPiece();
        }
        delay(100);
      }
      else if(playerTurn == 2)
      {
        if(startUI)
        {
          tft.fillRect(23,17,120,18,ST77XX_BLACK);
          tft.fillRect(50,3,50,9,ST77XX_BLACK);
          drawBorder(playerTurn);
          tft.setCursor(50,3);
          tft.write("Player 2!");
          tft.setCursor(15,10);
          tft.write("Please select a slot: ");
          startUI = false;
          currentSlot = 0;
          drawArrow();
        }
        if(digitalRead(RIGHTBUTTON) == LOW)
        {
          if(currentSlot < 6)
          {
            currentSlot++;
            drawArrow();
          }
        }

        if(digitalRead(LEFTBUTTON) == LOW)
        {
          if(currentSlot > 0)
          {
            currentSlot--;
            drawArrow();
          }
        }

        if(digitalRead(MIDDLEBUTTON) == LOW)
        {
          dropPiece();
        }
        delay(100);
      }
    }
  }
}

void createBoard()
{
  for(int i = 0; i < 6; i++)
  {
    for(int j = 0; j < 7; j++)
    {
        gameBoard[i][j] = 0;
    }
  }
  Serial.println("Board Created!");
}

void drawBorder(int player)
{
  if(player == 1)
  {
    tft.drawRect(0,0,160,128,ST77XX_RED);
    tft.drawRect(1,1,158,126,ST77XX_RED);
  }
  else if(player == 2)
  {
    tft.drawRect(0,0,160,128,ST77XX_YELLOW);
    tft.drawRect(1,1,158,126,ST77XX_YELLOW);
  }
}

void drawArrow()
{
  int arrowX = 23 + (currentSlot * 15);
  
  int lastArrowX = 23 + (lastSlot * 15);
  if(lastSlot == -1)
  {
    lastArrowX += 15;
  }
  if(currentSlot != lastSlot)
  {
    tft.fillRect(lastArrowX, 17 , 18, 18, ST77XX_BLACK);
    tft.drawBitmap(arrowX, 17, arrow, 18, 18,ST77XX_WHITE);
    lastSlot = currentSlot;
  }
}

void dropPiece()
{
  Serial.println("In Method");
  for(int i = 5; i >= 0; i--)
  {
    if(gameBoard[i][currentSlot] == 0)
    {
      int xCoord = 26 + (currentSlot * 15);
      int yCoord = 35 + (i * 15);
      Serial.print("X coord ");
      Serial.println(xCoord);
      Serial.print("Y coord ");
      Serial.println(yCoord);
      if(playerTurn == 1)
      {
        tft.drawBitmap(xCoord,yCoord,circle,15,15,ST77XX_RED);
        gameBoard[i][currentSlot] = 1;
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
        playerTurn = 1;
        startUI = true;
        lastSlot = -1;
        currentSlot = 1;
        break;
      }
    }
    else if(gameBoard[0][currentSlot] == 1 || gameBoard[0][currentSlot] == 2)
    {
      
    }
  }
}





