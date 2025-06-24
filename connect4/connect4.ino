#include <Adafruit_ST7735.h>

#define TFT_CS 5
#define TFT_RST 4
#define TFT_MOSI 1
#define TFT_SCLK 2
#define TFT_DC 3


int gameBoard[6][7];
int playerTurn = 1;
int numTurns = 0;
int lastSlotX = 0;
int lastSloyY = 0;
int chainLength = 0;
int winner = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() 
{
  Serial.begin(9600);
  pinMode(7,INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("tft set");

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
    for(int j = 0; j < 7; j++)asdasdasd
    {
        gameBoard[i][j] = 0;
    }
  }
}
