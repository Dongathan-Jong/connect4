#include <Adafruit_ST7735.h>

#define TFT_CS 5
#define TFT_RST 4
#define TFT_MOSI 13
#define TFT_SCLK 2
#define TFT_DC 16


int gameBoard[6][7];
int playerTurn = 1;
int numTurns = 0;
int lastSlotX = 0;
int lastSlotY = 0;
int chainLength = 0;
int winner = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(5, 16, 13, 2, 4);

void setup() 
{
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("tft set");

  tft.fillRect(25,30,105,1,ST77XX_WHITE);
  tft.fillRect(25,30,1,90,ST77XX_WHITE);
  tft.fillRect(25,45,105,1,ST77XX_WHITE);
  tft.fillRect(40,30,1,90,ST77XX_WHITE);
  tft.fillRect(25,60,105,1,ST77XX_WHITE);
  tft.fillRect(55,30,1,90,ST77XX_WHITE);
  tft.fillRect(25,75,105,1,ST77XX_WHITE);
  tft.fillRect(70,30,1,90,ST77XX_WHITE);
  tft.fillRect(25,90,105,1,ST77XX_WHITE);
  tft.fillRect(85,30,1,90,ST77XX_WHITE);
  tft.fillRect(25,105,105,1,ST77XX_WHITE);
  tft.fillRect(100,30,1,90,ST77XX_WHITE);
  tft.fillRect(25,120,105,1,ST77XX_WHITE);
  tft.fillRect(115,30,1,90,ST77XX_WHITE);
  tft.fillRect(130,30,1,90,ST77XX_WHITE);
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
        tft.setCursor(50,3);
        tft.write("Player 1! ");
        tft.setCursor(15,10);
        tft.write("Please select a slot:");
      }
      else if(playerTurn == 2)
      {
        
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
}

void drawBorder(int player)
{
  
}
