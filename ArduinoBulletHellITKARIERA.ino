
#include "LedControl.h"
#include "Math.h"
LedControl lc=LedControl(12,11,10,1); //12 - DataInput; 11 - CLK; 10 - LOAD

int score;
int stotni;
String scoreStr;
String scoreStr1; //izpolzva se za po-lesen draw
String scoreArr[] = {"" ,"" ,"" }; //individualnite chisla za izpisvane na rezultat
volatile unsigned long buttonPressed;
volatile bool gameOver = false;
int tick;
int speedUp = 1;
unsigned long now;
int player;
int columns[] = {0,0,0,0,0,0,0,0}; 
int pinLeft = 2; //buton 1
int pinRight = 3; //buton 2
int randomInt;
int numdr;
int scorecount;
volatile bool buttonmode = false;
volatile bool modeset = false;
volatile bool potentmode = false;

byte startanim[] = {
B11110000,
B10000000,
B10110000,
B10011111,
B11111001,
B00001001,
B00001001,
B00001111
};

byte endanim1[] = {
B01000010,
B00100100,
B01011010,
B01100110,
B00000000,
B00111100,
B00100100,
B00000000
};

byte endanim2[] = {
B10000001,
B01000010,
B00111100,
B01011010,
B00111100,
B11111111,
B10100101,
B10111101
};

byte endanim3[] = {
B00000000,
B01100110,
B01100110,
B00011000,
B00111100,
B00111100,
B00100100,
B00000000
};

byte endanim4[] = {
B00000000,
B00101000,
B01111100,
B00100010,
B00111100,
B00100010,
B01111100,
B00101000
};

byte endanim5[] = {
B11000011,
B10100101,
B01011010,
B01000010,
B01000010,
B00100100,
B10011001,
B11000011
};


void nachAnim(byte *sprite){
  for(int i = 0; i<8; i++)
  {
    columns[i] = 0;
  }
  lc.clearDisplay(0);
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
    for(int r = 0; r < 8; r++){
       for(int c = 0; c < 8; c++){
          lc.setLed(0, r, c, HIGH);
              delay(35);
       }
    }
    for(int r = 0; r < 8; r++){
        lc.setRow(0, r, sprite[r]);
    }
    delay(1500);
    lc.clearDisplay(0);
    pinMode(pinLeft, INPUT);
    pinMode(pinRight, INPUT);
    pinMode(A5, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinLeft), left, FALLING);
    attachInterrupt(digitalPinToInterrupt(pinRight), right, FALLING);
    loop();
}

void setplayer() {
  if(potentmode == true)
  {
    player = round(map(analogRead(A5), 0, 1020, 8 - 1, 0));
    modeset=true;
    buttonmode=false;
    potentmode=true;
  }
}

void kraiAnim(byte *sprite, int endmusic){
  for(int r = 0; r < 8; r++){
     for(int c = 0; c < 8; c++){
        lc.setLed(0, r, c, HIGH);
          delay(35);
     }
  }
  for(int r = 0; r < 8; r++){
      lc.setRow(0, r, sprite[r]);
  }
}

void setup() {
  lc.clearDisplay(0);
  gameOver = false;
  stotni = 0;
  scoreArr[0] = "";
  scoreArr[1] = "";
  scoreArr[2] = "";
  score = 0;
  tick = 300;
  speedUp = 1;
  player = 3;
  now = millis();
  buttonPressed = millis();
  randomSeed(analogRead(A4)); // po-dobri random chisla na neizpolzvan pin
  nachAnim(startanim);
}

void left()
{
  if(millis() - buttonPressed > 150)
  {
        if(player != 0)
        player--;
      else
        player = 7;
    
      lc.clearDisplay(0);
      buttonPressed = millis();
  }

  if(modeset == false)
  {
    buttonmode=true;
    modeset=true;
    potentmode=false;
  }
  if(gameOver == true){
  gameOver = false;
  lc.clearDisplay(0);
  setup();
  }
}

void right()
{
    if(millis() - buttonPressed > 150) 
    {
      if(player != 7)
        player++;
      else
        player = 0;
      
      lc.clearDisplay(0);
       buttonPressed = millis();
    }
  if(modeset == false)
  {
    buttonmode=true;
    modeset=true;
    potentmode=false;
  }
  if(gameOver == true){
  gameOver = false;
  lc.clearDisplay(0);
  setup();
  }   
}

void loop() { 
  if(modeset == false)
  {
    potentmode=true;
    buttonmode=false;
    modeset=true;
  }
  if(millis() - now > tick){  
    score++;
    now = millis();
    if(speedUp == 1){  //zaburzva igrata sled 5 ticks
       tick = tick/1.02;

      randomInt = random(0, 8); //izbira red za patron

      if(columns[randomInt] == 0){  //suzdava patron
        columns[randomInt] = 1;
      }
    }
    if(speedUp != 5) //broi za zaburzvane
      speedUp++;
     else
      speedUp = 1;
    for(int i = 0; i<8; i++){ 
      
      if(columns[i] == 10) //trie patronite ot display-a za da doidat novi
        columns[i] = 0;
        
      if(columns[i] != 0) 
        columns[i]++;
    }
    lc.clearDisplay(0);
  }

  if(potentmode==true) setplayer(); 
  
  lc.setLed(0, 7, player, true);

  for(int i = 0; i<8; i++){
    if(columns[i] > 0)
      lc.setLed(0, columns[i]-2, i, true); //patronite otivat napred
      lc.setLed(0, columns[i]-3, i, true);
  }

  if(columns[player] == 10 || columns[player] == 9){ //zasicha kogato igrachut e ocelen
    lc.clearDisplay(0);

    for(int i = 0; i<4; i++){ //EXPLOSIONS!!!
      lc.setLed(0,7,player+i,true);
      lc.setLed(0,7,player-i,true);
      lc.setLed(0,7-i,player+i,true);
      lc.setLed(0,7-i,player-i,true);
      lc.setLed(0,7-1.5*i,player,true);
      tone(A1, 250);
      delay(350);
    }

    delay(700); 
    lc.clearDisplay(0);
    noTone(A1);

    delay(500);
    
    if(score>0 && score<=99) kraiAnim(endanim1, 1);
    else if(score>=100 && score <=199) kraiAnim(endanim2, 2);
    else if(score>=200 && score <=299) kraiAnim(endanim3, 3);
    else if(score>=300 && score <=399) kraiAnim(endanim4, 4);
    else if(score>=400) kraiAnim(endanim5, 5);

    delay(1500);

    lc.clearDisplay(0);
    
    scoreStr = String(score);
    
    scoreArr[0] = scoreStr.charAt(0);
    scoreArr[1] = scoreStr.charAt(1);
    scoreArr[2] = scoreStr.charAt(2);

    if(score<=99 && score>=0) scorecount=2;
    else scorecount=3;

    if(scorecount==3)
    {
      scoreStr.substring(1,2) = scoreStr1;
      scoreArr[0] = scoreStr1.charAt(0);
      scoreArr[1] = scoreStr.charAt(1);
    }

    for(int i = 0; i<2; i++){
          if(scoreArr[i] == "0")
            draw(1+i*4, 0);
          if(scoreArr[i] == "1")
            draw(1+i*4, 1);
          if(scoreArr[i] == "2")
            draw(1+i*4, 2);
          if(scoreArr[i] == "3")
            draw(1+i*4, 3);
          if(scoreArr[i] == "4")
            draw(1+i*4, 4);
          if(scoreArr[i] == "5")
            draw(1+i*4, 5);
          if(scoreArr[i] == "6")
            draw(1+i*4, 6);
          if(scoreArr[i] == "7")
            draw(1+i*4, 7);
          if(scoreArr[i] == "8")
            draw(1+i*4, 8);
          if(scoreArr[i] == "9")
            draw(1+i*4, 9);
    }  

    scoreArr[0] = scoreStr.charAt(0);
    scoreArr[1] = scoreStr.charAt(1);
    scoreArr[2] = scoreStr.charAt(2);

    if(score>=100)
    {
       for(int i = 1; i<10; i++){
          if(scoreArr[0] == String(i))
            stotni = i;
        }
        for(int i = 1; i <= stotni; i++){
          lc.setLed(0,0,(i*2)-2,true);
          lc.setLed(0,1,(i*2)-2,true);
          delay(200);          
        }
    }
   gameOver = true;
    while(gameOver == true){
          //NOTHING
      }
  }
    
}

void draw(int place, int numdr)
{
  switch(numdr)
  {
    case 1 : {lc.setColumn(0,place,B00001000);
    lc.setColumn(0,1+place,B00011111);
    break; };
    case 2 : {lc.setColumn(0,place,B00010111);
    lc.setColumn(0,1+place,B00010101);
    lc.setColumn(0,2+place,B00011101);
    break; };
    case 3 : {
       lc.setColumn(0,place,B00010001);
    lc.setColumn(0,1+place,B00010101);
    lc.setColumn(0,2+place,B00011111);  
    break; };
    case 4 : {
      lc.setColumn(0,place,B00011100);
    lc.setColumn(0,1+place,B00000100);
    lc.setColumn(0,2+place,B00011111); 
    break; };
    case 5 : {
      lc.setColumn(0,place,B00011100);
    lc.setColumn(0,1+place,B00000100);
    lc.setColumn(0,2+place,B00011111);  
    break; };
    case 6 : {
      lc.setColumn(0,place,B00011111);
    lc.setColumn(0,1+place,B00010101);
    lc.setColumn(0,2+place,B00010111);  
    break; };
    case 7 : {
      lc.setColumn(0,place,B00010000);
    lc.setColumn(0,1+place,B00010011);
    lc.setColumn(0,2+place,B00011100);  
    break; };
    case 8 : {
      lc.setColumn(0,place,B00011111);
    lc.setColumn(0,1+place,B00010101);
    lc.setColumn(0,2+place,B00011111); 
    break; };
    case 9 : {
      lc.setColumn(0,place,B00011101);
    lc.setColumn(0,1+place,B00010101);
    lc.setColumn(0,2+place,B00011111);  
    break; };
    case 0 : {
      lc.setColumn(0,place,B00011111);
    lc.setColumn(0,1+place,B00010001);
    lc.setColumn(0,2+place,B00011111);  
    break; };
  };
}
