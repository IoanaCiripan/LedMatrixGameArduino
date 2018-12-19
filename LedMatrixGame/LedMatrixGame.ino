#include "LedControl.h" 
#include <LiquidCrystal.h>
///////////////////// <3 si omuletul https://www.youtube.com/watch?v=iHDQdBhdFxI
#define SW_PIN  13     // digital pin connected to switch output
#define joyStickVRx A0 // analog pin connected to X output
#define joyStickVRy A1 // analog pin connected to Y output
#define V0_PIN 9       // PWN 
#define randomPin 0    // an unconected pin for fairly random input

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // RS,E,D4,D5,D6,D7
LedControl lc = LedControl(12, 11, 10, 1); // DIN, CLK, LOAD, No. DRIVER

int lifes = 2;
int randomPoint;
int monsterPosition;
int playerPosition = 3; 
int previsiousPosition = playerPosition;
int i = 1;
unsigned long int lastUpdate = 0;
unsigned long int lastUpdate2 = 0;
unsigned long int lastMove = 0;
boolean moved = false;
boolean monsterPushed = false;
boolean shot = false;
boolean start = false;
boolean dead = false; // check if it is dead
boolean firstTime = true;
long score;
boolean isScoreIncreased = 0;

 bool ledMatrix[8][8] = 
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
 bool gameOverMatrix[8][8] = 
{
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0}, 
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1}
};
 bool arrowUp[8][8] = 
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}, 
  {0, 1, 0, 1, 1, 0, 1, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}
};


void pushMatrix() 
{
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, i, j, ledMatrix[i][j]);
    }
  }
}


void clearMatrix() 
{
  for (int i = 0; i < 7; i++)
    for (int j = 0; j < 8; j++)
        ledMatrix[i][j] = 0;
}

void pushRandomMonster() 
{
  if (monsterPushed == false)
  {
    i = 1;
    randomSeed(analogRead(randomPin));
    randomPoint = random(0, analogRead(joyStickVRx)) % 6;
    monsterPosition = randomPoint;
    Serial.println(randomPoint);
    clearMatrix();
    if (randomPoint == 0)
    {
      ledMatrix[0][0] = 1;
      ledMatrix[0][1] = 1;
      ledMatrix[0][2] = 1;
      ledMatrix[0][3] = 0;
      ledMatrix[0][4] = 0;
      ledMatrix[0][5] = 0;
      ledMatrix[0][6] = 0;
      ledMatrix[0][7] = 0;

    }

    if (randomPoint == 1)
    {
      ledMatrix[0][0] = 0;
      ledMatrix[0][1] = 1;
      ledMatrix[0][2] = 1;
      ledMatrix[0][3] = 1;
      ledMatrix[0][4] = 0;
      ledMatrix[0][5] = 0;
      ledMatrix[0][6] = 0;
      ledMatrix[0][7] = 0;
    }

    if (randomPoint == 2)
    {
      ledMatrix[0][0] = 0;
      ledMatrix[0][1] = 0;
      ledMatrix[0][2] = 1;
      ledMatrix[0][3] = 1;
      ledMatrix[0][4] = 1;
      ledMatrix[0][5] = 0;
      ledMatrix[0][6] = 0;
      ledMatrix[0][7] = 0;
    }

    if (randomPoint == 3)
    {
      ledMatrix[0][0] = 0;
      ledMatrix[0][1] = 0;
      ledMatrix[0][2] = 0;
      ledMatrix[0][3] = 1;
      ledMatrix[0][4] = 1;
      ledMatrix[0][5] = 1;
      ledMatrix[0][6] = 0;
      ledMatrix[0][7] = 0;
    }

    if (randomPoint == 4)
    {
      ledMatrix[0][0] = 0;
      ledMatrix[0][1] = 0;
      ledMatrix[0][2] = 0;
      ledMatrix[0][3] = 0;
      ledMatrix[0][4] = 1;
      ledMatrix[0][5] = 1;
      ledMatrix[0][6] = 1;
      ledMatrix[0][7] = 0;
    }

    if (randomPoint == 5)
    {
      ledMatrix[0][0] = 0;
      ledMatrix[0][1] = 0;
      ledMatrix[0][2] = 0;
      ledMatrix[0][3] = 0;
      ledMatrix[0][4] = 0;
      ledMatrix[0][5] = 1;
      ledMatrix[0][6] = 1;
      ledMatrix[0][7] = 1;
    }

    monsterPushed = true;
  }
  
  pushMatrix();
  
  int j;
  for (j = 0; j < 8; j++) // shift the monster
  {
    ledMatrix[i][j] = ledMatrix[i - 1][j];
    ledMatrix[i - 1][j] = 0;
  }
  
  i++;
  
  if (i > 8)
    monsterPushed = false;

  pushMatrix();

}


void setDot (int &playerPosition)
{
  if (playerPosition > 7)
    playerPosition = 7;
  if (playerPosition < 0)
    playerPosition = 0;
}


void fire(int line, int &playerPosition)
{
  //isScoreIncreased = 0;
  int i = 1;
  //Serial.print("random: ");
  //Serial.println(monsterPosition);
  //Serial.print("player: ");
  //Serial.println(playerPosition);
 /* if(playerPosition == randomPoint || playerPosition == randomPoint+1 || playerPosition == randomPoint+2)
  {
    if (isScoreIncreased == 0)
    {
      score ++;
      isScoreIncreased = 1;
      
    }
  }*/

    while (i < 9)
    { 
      
       /* if(ledMatrix[line - i][playerPosition] == true) // e impuscat
        {
          ledMatrix[line][playerPosition] = false;
          ledMatrix[line - i][playerPosition] = false;
          //Serial.print(ledMatrix[line][playerPosition]);
         // isScoreIncreased = 1;
          if (isScoreIncreased == 0)
          {
          score = score + 1;
          Serial.println(score);
          isScoreIncreased =1;}
          
          //delay(10);
          
          break;
        } */
        ledMatrix[line-i][playerPosition] = true;

        if (i > 1)
             ledMatrix[line - i + 1][playerPosition] = false;
        i++;
        pushMatrix();    
    }
    shot = true;


    

}

 void moveDot() 
 {
  int valX = analogRead(joyStickVRx);

  if (valX > 600) {
    if (moved == false) {
      previsiousPosition = playerPosition;
      playerPosition++;
      moved = true;
    }
  }
  else
  {
    if (valX < 400)
    {
      if (moved == false)
      {
        previsiousPosition = playerPosition;
        playerPosition--;
        moved = true;
      }
    } 
    else moved = false;
  }

  setDot(playerPosition);

  if (previsiousPosition != playerPosition)
    ledMatrix[7][previsiousPosition] = false;

  ledMatrix[7][playerPosition] = true;

  pushMatrix();
  
 if (ledMatrix[6][playerPosition] == 1) 
  {
    if (lifes >= 0)
     { 
      lifes = lifes - 1;
      delay(1000);
     }    
  }
   
  else
  {
    dead = false;
    start = true;
  }

}

void gameOver()
{
    dead = true;
    start = false;

    lcd.clear();
    lcd.setCursor(0, 0); 
//    lcd.print(score);
  //  lcd.setCursor(0,1);
    lcd.print("HIGHSCORE: ");
    delay(5000);

     for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
        ledMatrix[i][j] = arrowUp[i][j];
    pushMatrix();    
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PRESS ARROW UP");
    lcd.setCursor(1,1);
    lcd.print("TO START AGAIN");   
}

void startGame()
{
  if(millis() - lastUpdate > 300)
  {
    lastUpdate = millis();
    pushRandomMonster();  
    
  }   
    moveDot();

 
  shot = digitalRead(SW_PIN);
  isScoreIncreased = 0;
  if(shot == false)
  {
   fire(7,playerPosition);
   shot = true;
   score++;
   Serial.print("score: ");
   Serial.println(score);
   
  } 
}



void setup() 
{
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT);
  pinMode(V0_PIN, OUTPUT);
  
  lc.shutdown(0, false); 
  lc.setIntensity(0, 1); 
  lc.clearDisplay(0);

  digitalWrite(SW_PIN, HIGH); 
  analogWrite(V0_PIN, 90); 

  lc.clearDisplay(0);
  lcd.begin(16,2);
  lcd.setCursor(1, 0); 
  lcd.print("PRESS ARROW UP");
  lcd.setCursor(4,1);
  lcd.print("TO START");
  
  }
  

  
void loop() 
{

  
  int  valY = analogRead(joyStickVRy);

   if(start == false)
    {
      if(valY < 400)
      {
        start = true;
        lifes = 2;
        score = 0;
      }
    }



  if (start == true) 
  {
     if (lifes >=  0)
     { startGame();
      lcd.clear();
      lcd.setCursor(0,0);
      if (lifes == 2)
      lcd.print("You have 2 lifes");
      if (lifes == 1)
      { lcd.clear();
      lcd.print("You have 1 life");
      }
      }
     else
     {
       for (int i = 0; i < 8; i++)
       for (int j = 0; j < 8; j++)
        ledMatrix[i][j] = gameOverMatrix[i][j];
       pushMatrix(); 
       gameOver();
     }
  }
 
 
  
}
