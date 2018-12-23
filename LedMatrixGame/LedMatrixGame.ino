#include "LedControl.h" 
#include <LiquidCrystal.h>
#include <EEPROM.h>

#define SW_PIN  13      // digital pin connected to switch output
#define joyStickVRx A0 // analog pin connected to X output
#define joyStickVRy A1 // analog pin connected to Y output
#define V0_PIN 9       // PWN 
#define randomPin A5   // an unconected pin for fairly random input

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // RS,E,D4,D5,D6,D7
LedControl lc = LedControl(12, 11, 10, 1); // DIN, CLK, LOAD, No. DRIVER

const int LIMIT_LINE = 7;
const int LIMIT_COLUMN = 7;
const int MAX_LOST_PICES = 30;
int counter;
int lives = 2;
int minMonsterSpeed = 400; // the minimum speed
int maxMonsterSpeed = 150; // full speed
int monsterSpeed = minMonsterSpeed; 
int acceleration = 20;
int monsterPosition;
int playerPosition = 3; 
int previousPosition = playerPosition;
unsigned long int lastUpdate = 0;
boolean moved = false;
boolean monsterPushed = false;
boolean shot = false;
boolean start = false; 
boolean isModified = 0;
boolean isScoreIncreased = false;
boolean isShoted = false;
int lostPices = 0; // represents how many "pices" of monsters you have missed
int score = 0;
int score2 = 0;
int highScore;


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
  {0, 0, 0, 0, 0, 0, 0, 0}
};


void pushMatrix()
{
    for (int i = 0; i <= LIMIT_LINE; i++)
    {
        for (int j = 0; j <= LIMIT_COLUMN; j++)
        {
            lc.setLed(0, i, j, ledMatrix[i][j]);
        }
    }
}


void clearMatrix()
{
    for (int i = 0; i <= LIMIT_LINE; i++)
    {
        for (int j = 0; j <= LIMIT_COLUMN; j++)
        {
            ledMatrix[i][j] = 0;
        }
    }
}


void pushRandomMonster()
{
   
    if (monsterPushed == false)
    {
        isModified = 0;
        counter= 1;
        monsterPosition = random(0, 6); // generates a random position for the next monster
        clearMatrix();
        
        if (monsterPosition == 0)
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

        if (monsterPosition == 1)
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

        if (monsterPosition == 2)
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

        if (monsterPosition == 3)
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

        if (monsterPosition == 4)
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

        if (monsterPosition == 5)
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

    for (int j = 0;j <= LIMIT_COLUMN; j++) // shift the monster next line
    {
        ledMatrix[counter][j] = ledMatrix[counter - 1][j];
        ledMatrix[counter - 1][j] = 0;
    }

    counter++;

    if (counter > LIMIT_LINE)
    {
        monsterPushed = false;
    }
    
    for (int j = 0;j <= LIMIT_COLUMN; j++)
    {
        if (j != playerPosition)
        {
            if (ledMatrix[LIMIT_LINE][j] == true) // if a pice of a monster touch the limit line, it is lost
            {
                lostPices++;
                if (lostPices >= MAX_LOST_PICES)  // if the maximum number of lost pices is exceeded you are dead
                {
                    lives = -1;
                }
            }

            ledMatrix[LIMIT_LINE][j] = false;
        }
    }

    pushMatrix();

}


void setDot(int &playerPosition) // sets the dot in the matrix 8 x 8 range
{
  if (playerPosition > LIMIT_COLUMN)
  {
    playerPosition = LIMIT_COLUMN;
  }
  
  if (playerPosition < 0)
  {
    playerPosition = 0;
  }
  
}



void fire(int line, int playerPosition)
{
    isShoted = false;
    isScoreIncreased = false;
    int i = 1;

    for (i = 0; i < LIMIT_LINE - 1; i++)
    {
        if (ledMatrix[i][playerPosition] == 1)
        {
            isShoted = true;
            break;
        }
    }
    
    if (isScoreIncreased == false && isShoted == true)
    {
        isScoreIncreased = true;
        score++;
        score2++;
        if(score2 == 10)
        {
            monsterSpeed -= acceleration;
            score2 = 0;
            
            if (monsterSpeed < maxMonsterSpeed) // the speed of the monster can not exceed the maxMonsterSpeed
            {
                monsterSpeed = maxMonsterSpeed;
            }
        }

        if (score > highScore)
        {
            highScore = score;
            EEPROM.write(0, score); // write the score in EEPROM if it is bigger than the highscore
        }
    }

    i = 1;
    while (i < 9)
    {
        ledMatrix[line-i][playerPosition] = true;

        if (i > 1)
        {
            ledMatrix[line - i + 1][playerPosition] = false;
        }

        i++;
        pushMatrix();
    }
    shot = true;
}


void moveDot()
{
    int valX = analogRead(joyStickVRx);

    if (valX > 600)
    {
        if (moved == false)
        {
            previousPosition = playerPosition;
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
                previousPosition = playerPosition;
                playerPosition--;
                moved = true;
            }
        }
        else
            moved = false;
    }

    setDot(playerPosition);

    if (previousPosition != playerPosition)
        ledMatrix[LIMIT_LINE][previousPosition] = false;

    ledMatrix[LIMIT_LINE][playerPosition] = true;

    pushMatrix();

    if (ledMatrix[LIMIT_LINE - 1][playerPosition] == 1) // check if the monster hits me
    {
        if (lives >= 0)
        {
            if (isModified == 0)
            {
                lives = lives - 1;
                isModified = 1;
            }
        }
    }

    else
    {
        start = true;
    }

}


void gameOver()
{
    start = false;

    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Score: ");
    lcd.setCursor(7,0);
    lcd.print(score);
    lcd.setCursor(0,1);
    lcd.print("HIGHSCORE: ");
    lcd.setCursor(12,1);
    lcd.print(highScore);
    delay(5000);

     for (int i = 0; i <= LIMIT_LINE; i++)
      for (int j = 0; j <= LIMIT_COLUMN; j++)
        ledMatrix[i][j] = arrowUp[i][j];
        
    pushMatrix();    
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DO YOU WANT");
    lcd.setCursor(1,1);
    lcd.print("TO START AGAIN?");   
}

void startGame()
{

    if(millis() - lastUpdate > monsterSpeed)
    {
        lastUpdate = millis();
        pushRandomMonster();
    }

    moveDot();

    shot = digitalRead(SW_PIN);
    if(shot == 0)
    {
        fire(LIMIT_LINE, playerPosition);
    }
}


void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(randomPin));
  pinMode(SW_PIN, INPUT);
  pinMode(V0_PIN, OUTPUT);
  
  lc.shutdown(0, false); 
  lc.setIntensity(0, 0); 
  lc.clearDisplay(0);

   for (int i = 0; i <= LIMIT_LINE; i++)
      for (int j = 0; j <= LIMIT_COLUMN; j++)
        ledMatrix[i][j] = arrowUp[i][j];
        
  pushMatrix();  
    
  lcd.begin(16,2);
  lcd.setCursor(1, 0); 
  lcd.print("PRESS ARROW UP");
  lcd.setCursor(4,1);
  lcd.print("TO START"); 

  digitalWrite(SW_PIN, HIGH); 
  analogWrite(V0_PIN, 90);
  int val = 0;
  highScore = EEPROM.read(0); 
}
  
  
void loop()
{
    int  valY = analogRead(joyStickVRy);

    if(start == false)
    {
        if(valY < 400)
        {
            start = true;
            lives = 2;
            score = 0;
            lostPices = 0;
            monsterSpeed = minMonsterSpeed;
        }
    }

    if (start == true)
    {
        if (lives >=  0)
        {
            startGame();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Score: ");
            lcd.setCursor(8,0);
            lcd.print(score);
            lcd.setCursor(0,1);
            if (lives == 2)
            {
                lcd.setCursor(0,1);
                lcd.print("2 lives");  
            }
            if (lives == 1)
            {
                lcd.setCursor(0,1);
                lcd.print("1 life");
            }
            if(lives == 0)
            {
                lcd.setCursor(0,1);
                lcd.print("0 lives");
            }
            
            lcd.setCursor(10,1);
            lcd.print(MAX_LOST_PICES - lostPices);

        }
        else
        {
            delay(1000);
            for (int i = 0; i <= LIMIT_LINE; i++)
                for (int j = 0; j <= LIMIT_COLUMN; j++)
                    ledMatrix[i][j] = gameOverMatrix[i][j];
                    
            pushMatrix();
            gameOver();
        }
    }
}
