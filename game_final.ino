#include <Adafruit_CircuitPlayground.h>
#include <cmath>

int lightRead=0;
int globalBright=0;

int startFlag=0;
int roundFlag=0;
int movingLight=0;
int finalScore=0;
int highScore=0;
int hardHighScore=0;
int target=0;
volatile bool hardMode=0;


void setup() {
  CircuitPlayground.begin();

  // Seed RNG
  randomSeed(analogRead(0));

  Serial.begin(9600);

  // Interrupts
  attachInterrupt(digitalPinToInterrupt(4), gameStart, RISING);
  attachInterrupt(digitalPinToInterrupt(5), endRound, RISING);
  attachInterrupt(digitalPinToInterrupt(7), setHardMode, CHANGE);
}

void loop() {

  // Set game brightness
  lightRead = CircuitPlayground.lightSensor();
  globalBright = 128;

  // Start Game
  if(startFlag==1) {

    // Play round i
    for(int i=1; ; i++) {
      if(i!=1) {
        for(int i=0; i<4; i++) {
          CircuitPlayground.playTone(400+(i*30), 50);
        }
      }

      // Set variables
      int color = 47+i*10;
      int roundSpeed = 500*pow(2.72, (-0.1*(i-1)))+100;

      if(hardMode) {
        Serial.print("HARD MODE  ");
      }
      Serial.print("Round ");
      Serial.print(i);
      Serial.print(", Target: ");
      Serial.print(target);
      Serial.print(", Speed: ");
      Serial.println(roundSpeed);

      // Comet motion
      if(i % 2 == 0) {

        // Even rounds - Going backwards
        target = (target + random(2, 7)) % 10;
        
        for(int j=movingLight, k=0; j>=0; j--, k++) {

          // Set target light(s)
          CircuitPlayground.setPixelColor(target, CircuitPlayground.colorWheel(color));

          // Animate moving light
          CircuitPlayground.setPixelColor(j, globalBright, globalBright, globalBright);
          delay(roundSpeed);
          CircuitPlayground.clearPixels();

          // End round when button pressed
          if(roundFlag==1) {
            movingLight = j;
            roundFlag = 0;
            break;
          }

          // HARD MODE
          if(hardMode && j==(target+9)%10) {
            movingLight = j;
            roundFlag = 0;
            break;
          }

          // Restart for loop if light hits last pixel
          if(j==0) {
            j=10; // Becomes 9 after j--
          }

        }

      } else {

        // Odd rounds - Going forwards
        target = (target + random(2, 7)) % 10;
        
        for(int j=movingLight, k=0; j<10; j++, k++) {

          // Set target light
          CircuitPlayground.setPixelColor(target, CircuitPlayground.colorWheel(color));

          // Animate moving light
          CircuitPlayground.setPixelColor(j, globalBright, globalBright, globalBright);
          delay(roundSpeed);
          CircuitPlayground.clearPixels();

          // End round when button pressed
          if(roundFlag==1) {
            movingLight = j;
            roundFlag = 0;
            break;
          }

          // HARD MODE
          if(hardMode && j==(target+1)%10) {
            movingLight = j;
            roundFlag = 0;
            break;
          }

          // Restart for loop if light hits last pixel
          if(j==9) {
            j=-1; // Will become 0 after j++
          }
        }
      }

      // Game over!
      if(movingLight!=target) {
        CircuitPlayground.setPixelColor(movingLight, globalBright, 0, 0);
        for(int i=0; i<4; i++) {
          CircuitPlayground.playTone(400-(i*30), 500);
        }

        startFlag = 0;
        movingLight = 0;
        finalScore = i-1;
        if(hardMode) {
          Serial.println("HARD GAME OVER");
          if(finalScore>hardHighScore) {
            newHighScore();
          }
          Serial.print("Your Score:  ");
          Serial.println(finalScore);
          Serial.print("Hard High Score:  ");
          Serial.println(hardHighScore);

        } else {
        Serial.println("GAME OVER");
        if(finalScore>highScore) {
          newHighScore();
        }
        Serial.print("Your Score:  ");
        Serial.println(finalScore);
        Serial.print("High Score:  ");
        Serial.println(highScore);

        }
        break;
      }
    }
  }

  if(finalScore>highScore) {
    highScore = finalScore;
  }
  if(finalScore>hardHighScore) {
    hardHighScore = finalScore;
  }
}

void gameStart() {
  startFlag=1;
  delay(25);
}

void endRound() {
  roundFlag = 1;
  delay(25);
}

void setHardMode() {
  hardMode=!hardMode;
  delay(25);
}

void newHighScore() {
  int textDelay = 300;

  Serial.print("N");
  delay(textDelay);
  Serial.print("E");
  delay(textDelay);
  Serial.print("W");
  delay(textDelay);
  Serial.print(" ");
  delay(textDelay);
  Serial.print("H");
  delay(textDelay);
  Serial.print("I");
  delay(textDelay);
  Serial.print("G");
  delay(textDelay);
  Serial.print("H");
  delay(textDelay);
  Serial.print(" ");
  delay(textDelay);
  Serial.print("S");
  delay(textDelay);
  Serial.print("C");
  delay(textDelay);
  Serial.print("O");
  delay(textDelay);
  Serial.print("R");
  delay(textDelay);
  Serial.println("E");
  delay(textDelay);
}
