//Name: Brandon Hindes 200187478
//Date: Nov 27 12
//Purpose: Pong with TV remotes

#include <TVout.h>
#include <fontALL.h>

TVout TV;
//int RECV_PIN = 2;
int POWLED=4;

//String up=0;
//String down=0;
//String pause=0;
//String check=0;
int button[4]={2,3,4,5};
int buttonC[4]={0,0,0,0};
int cont=0;

int pScore[2]={0,0};
int pPaddle[2]={40,40};
int lPaddle[2]={40,40};
int ball[4]={62,46,0,1};
int lBall[2]={62,46};
int ballSpeed=2;

void setup() {
  TV.begin(NTSC,128,96);
  TV.select_font(font4x6);
  pinMode(POWLED,OUTPUT);
  digitalWrite(POWLED,HIGH);
  TV.clear_screen();
}

void loop() {
  //check = getIR();
  getButton();
  
  if(buttonC[1]>=900){
    if(pPaddle[0]>2){
      pPaddle[0]=pPaddle[0]-2;
    }
  }
  else if(buttonC[2]>=900){
    if(pPaddle[0]<77){
      pPaddle[0]=pPaddle[0]+2;
    }
  }
  else if(buttonC[3]>=900){
    paused();
  }
  //else if(buttonC[0]>=900){
  //  menu();
  //}
  
  ballMove();
  AI();
  draw();
}

void getButton(){
  buttonC[0]=analogRead(button[0]);             //enter
  buttonC[1]=analogRead(button[1]);             //up
  buttonC[2]=analogRead(button[2]);             //down
  buttonC[3]=analogRead(button[3]);             //pause
}

//String getIR() {
//  return(0);
//}

void paused(){
  TV.clear_screen();
  TV.println(24,4,"       PAUSED       ");
  TV.println(24,28," PLEASE PUSH PAUSE ");
  TV.println(24,36,"  AGAIN TO RESUME  ");
  delay(500);
  do{
    getButton();
  }while(buttonC[3]<900);
  TV.clear_screen();
  delay(500);
  return;
}

/*void menu(){
  int menuSel=0;
  int menuSelC=1;
  int numSel[2]={0,0};
  int menuNum=0;
  int enterB=0;
  int reDraw=1;
  TV.clear_screen();
  do{
    //Button Read
    getButton();
    
    if(buttonC[1]>=900){                        //up
      menuSel=menuSel-1;
      delay(500);
    }
    else if(buttonC[2]>=900){                   //down
      menuSel=menuSel+1;
      delay(500);
    }
    else if(buttonC[3]>=900){                   //back
      if(menuNum>0){
        menuNum=0;
        reDraw=1;
        delay(500);
      }
      else if(menuNum==0){
        TV.clear_screen();
        delay(500);
        return;
      }
    }
    if(menuSel<numSel[0]){ menuSel=numSel[1]; }
    else if(menuSel>numSel[1]){ menuSel=numSel[0]; }
    
    else if(buttonC[0]>=900){
      switch (menuNum){
        case 0:
          switch (menuSel){
            case 0:
              menuNum=1;
              delay(500);
              break;
            case 1:
              TV.clear_screen();
              delay(500);
              return;
          }
          break;
        case 1:
          
          break;
      }
    }
    
    //Display
    if(reDraw==1){
      reDraw=0;
      if(menuNum==0){
        numSel[0]=0;
        numSel[1]=1;
        menuSel=0;
        menuSelC=1;
        TV.println(24,5, "        MENU        ");
        TV.println(24,13,"      SETTINGS      ");
        TV.println(24,21,"     START OVER     ");
        TV.println(24,29,"       RESUME       ");
        TV.println(24,37,"                    ");
      }
      else if(menuNum==1){
        numSel[0]=0;
        numSel[1]=1;
        menuSel=0;
        menuSelC=1;
        TV.println(24,5, "      SETTINGS      ");
        TV.println(24,13,"     BALL SPEED     ");
        TV.println(24,21,"   PLAYER 1 SPEED   ");
        TV.println(24,29,"   PLAYER 2 SPEED   ");
        TV.println(24,37,"        BACK        ");
      }
    }
    
    //Selector
    if(menuSelC!=menuSel){
      //Clear
      TV.draw_rect(23,11,82,8,0);
      TV.draw_rect(23,19,82,8,0);
      TV.draw_rect(23,27,82,8,0);
      TV.draw_rect(23,35,82,8,0);
      TV.draw_rect(29,44,6,8,0);
      TV.draw_rect(94,44,6,8,0);
      //Draw
      if(menuSel==0){                             // L1
        TV.draw_rect(23,11,82,8,1);
      }
      else if(menuSel==1){                        // L2
        TV.draw_rect(23,19,82,8,1);
      }
      else if(menuSel==2){                        // L3
        TV.draw_rect(23,27,82,8,1);
      }
      else if(menuSel==3){                        // L4
        TV.draw_rect(23,35,82,8,1);
      }
      else if(menuSel==4){                        // Y
        TV.draw_rect(29,44,6,8,1);
      }
      else if(menuSel==5){                        // N
        TV.draw_rect(94,44,6,8,1);
      }
      else if(menuSel==6){                        // 1
        TV.draw_rect(49,44,6,8,1);
      }
      else if(menuSel==7){                        // 2
        TV.draw_rect(57,44,6,8,1);
      }
      else if(menuSel==8){                        // 3
        TV.draw_rect(65,44,6,8,1);
      }
      else if(menuSel==9){                        // 4
        TV.draw_rect(73,44,6,8,1);
      }
      menuSelC=menuSel;
    }
    
  }while(true);
  return;
}*/

void ballMove(){
  int I=0;                                      //Varriable used for loops
  //Movement =====================================
  switch (ball[2]){                             //Ball Direction X
    case 0:                                     //Left
      ball[0]=ball[0]-ballSpeed;
      break;                                    //Stop switch Statement
    case 1:                                     //Right
      ball[0]=ball[0]+ballSpeed;
      break;                                    //Stop switch Statement
  }
  switch (ball[3]){
    case 0:                                     //Up
      ball[1]=ball[1]-ballSpeed;
      break;                                    //Stop switch Statement
    case 1:                                     //Straight
      if(ball[2]==0){
        ball[0]=ball[0]-ballSpeed;
      }
      else{
        ball[0]=ball[0]+ballSpeed;
      }
      break;                                    //Stop switch Statement
    case 2:                                     //down
      ball[1]=ball[1]+ballSpeed;
      break;                                    //Stop switch Statement
  }
  //Collision Detection ==========================
  if(ball[0]<=2){//Check scorezones
    pScore[1]=pScore[1]+1;
    ball[0]=62;
    ball[1]=46;
    ball[3]=1;//Ball Direction Straight
    pPaddle[0]=40;
    pPaddle[1]=40;
  }
  else if(ball[0]>=122){ 
    pScore[0]=pScore[0]+1;
    ball[0]=62;
    ball[1]=46;
    ball[3]=1;//Ball Direction Straight
    pPaddle[0]=40;
    pPaddle[1]=40;
  }
  if(ball[1]<=2 || ball[1]>=90){//Check top and bottom
    TV.tone(500,100);
    switch (ball[3]){
      case 0://Moving Up
        ball[3]=2;//Change direction to down
        break;//Stop switch Statement
      case 2://Moving down
        ball[3]=0;//Change Direction to Up
        break;//Stop switch Statement
  }}
  //Player 1 Paddle ============================
  if(ball[1]>=(pPaddle[0]-4) && ball[1]<=(pPaddle[0]+17) && ball[0]<=10 && ball[0]>=3){
    ball[2]=1;//Ball Direction = Right
    TV.tone(500,100);
    if(ball[1]<=(pPaddle[0]+5)){//if ball hits top side of paddle
      switch (ball[3]){
        case 0://Moving Up
          break;//Stop switch Statement
        case 1://Moving Straight
          ball[3]=0;//Change Direction to Up
          break;//Stop switch Statement
        case 2://Moving down
          ball[3]=1;//Change Direction to Straight
          break;//Stop switch Statement
    }}
    else if(ball[1]>=(pPaddle[0]+11)){//if ball hits bottom side of paddle
      switch (ball[3]){
        case 2://Moving down
          break;//Stop switch Statement
        case 1://Moving Straight
          ball[3]=2;//Change Direction to down
          break;//Stop switch Statement
        case 0://Moving Up
          ball[3]=1;//Change Direction to Straight
          break;//Stop switch Statement
  }}}
  //Player 2 Paddle ============================
  if(ball[1]>=(pPaddle[1]-4) && ball[1]<=(pPaddle[1]+17) && ball[0]>=114 && ball[0]<=121){
    ball[2]=0;//Ball Direction = Left
    TV.tone(500,100);
    if(ball[1]<=(pPaddle[1]+5)){//if ball hits top side of paddle
      switch (ball[3]){
        case 0://Moving Up
          break;//Stop switch Statement
        case 1://Moving Straight
          ball[3]=0;//Change Direction to Up
          break;//Stop switch Statement
        case 2://Moving down
          ball[3]=1;//Change Direction to Straight
        break;//Stop switch Statement
    }}
    else if(ball[1]>=(pPaddle[1]+11)){//if ball hits bottom side of paddle
      switch(ball[3]){
        case 2://Moving down
          break;//Stop switch Statement
        case 1://Moving Straight
          ball[3]=2;//Change Direction to down
          break;//Stop switch Statement
        case 0://Moving Up
          ball[3]=1;//Change Direction to Straight
          break;//Stop switch Statement
  }}}
  return;
}

void AI(){
  if(ball[0]>=63){//if Ball is greater than or = to half way
    if(ball[1]>(pPaddle[1]+6) && pPaddle[1]<77){
      pPaddle[1]=pPaddle[1]+2;
    }
    else if(ball[1]<(pPaddle[1]+6) && pPaddle[1]>2){
      pPaddle[1]=pPaddle[1]-2;
    }
    else{
      
    }
  }
  return;
}

void draw(){
  TV.print(16,4,pScore[0]);               //P1 Score
  TV.print(104,4,pScore[1]);              //P2 Score
  
  if(lPaddle[0]!=pPaddle[0]){             //If Player 1 Paddle has moved
    TV.draw_rect(4,0,4,96,BLACK,0);
    lPaddle[0]=pPaddle[0];
  }
  
  if(lPaddle[1]!=pPaddle[1]){             //If Player 2 Paddle has moved
    TV.draw_rect(120,0,4,96,BLACK,0);
    lPaddle[1]=pPaddle[1];
  }
  
  TV.draw_rect(lBall[0],lBall[1],4,4,0,0);//Clear old Ball
  lBall[0]=ball[0];                       //Store Ball location for clear next time
  lBall[1]=ball[1];
  TV.draw_rect(ball[0],ball[1],4,4,1,1);  //Ball
  
  TV.draw_rect(63,0,2,96,1,1);            //Boarder
  TV.draw_rect(4,pPaddle[0],4,16,1,1);    //P1 paddle
  TV.draw_rect(120,pPaddle[1],4,16,1,1);  //P2 paddle
  delay(100);
  //Serial.println("Draw");
  return;
}





