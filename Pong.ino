// Name: Br&&on Hindes
// Date: 30-10-12
// Purpose: Allow any one with TV remotes to play pong on their TV
//Library Declaration ============================
#include <IRremote.h>
#include <TVout.h>
//Varriables and Placeholders ====================
//Pins and Libraries =============================
TVout TV;                                       //Create and Set Varriable TVout to TV
int POWLED=13;                                  //Set the Power LED to pin 13
int IRIN=1;                                     //Set the IR Input to pin 1
int button=0;                                   //Placeholder for reset button
//IR =============================================
IRrecv irrecv(IRIN);                            //Fairly sure setting IR varriable to pin 1
int p1[4]={0,0,0,0};                            //Placeholder for Player 1 controls
int ir=0;                                       //Placeholder for IR
decode_results results;
//Settings =======================================
boolean drawB=true;                             //Weather or not to draw the boarder
int ballSpeed=2;                                //Ball Speed on the Screen
boolean initConf=false;                         //Placeholder for Initial Config run
//Display ========================================
String conf[14]={
"       CONFIG       ",
"      PLAYER 1      ",
"   Please hit the   ",
"button on the remote",
"        doWN        ",
"         UP         ",
"        MENU        ",
"       SELECT       ",
"Select the ballspeed",
"  Draw the Boarder  ",
"       PAUSED       ",
"       CONFIG       ",
"    RESTART GAME    ",
"     SLEEP MODE     "};
int confL[14]={                                 //What line the text is located
0,1,2,4,3,3,3,3,2,2,0,2,3,4};
String YN[6]={"YES", "NO", "1", "2", "3", "4"}; //Stored text for use in menus
int YNX[6]={30,94,50,58,66,74};                 //Yes No X,Y location
int size=0;                                     //Placeholder for array text display
int sel[6]={0,0,0,0,0,0};                       //Placeholder for menu selections(on/off,X,Y,W,H,#)
int T[14];
//Gameplay =======================================
int pscore[2]={0,0};                            //Player 1 and 2 Score
int pPaddle[2]={40,40};                         //Player 1 and 2 Paddle Positon
int ball[2]={64,48};                            //Ball X,Y location
int bd[2]={0,1};                                //Ball Direction (X 0 left, 1 right),(Y 0 up, 1 straight, 2 down)
int dir=0;                                      //Player 2 AI Paddle Movement Calcualtion Varriable
//Initial Setup ==================================
void setup() {
  TV.begin(_NTSC);                              //Set output format to NTSC or PAL
  TV.clear_screen();                            //Clear the screen initially
  //TV.select_font(font4x6);                      //Set font size to 4x6 pixels
  pinMode(POWLED,OUTPUT);                       //Set Power LED pin to output mode
  pinMode(IRIN,INPUT);                          //Set IR Input pin to input mode
  digitalWrite(POWLED,HIGH);                    //Turn the Power LED on
  irrecv.enableIRIn();                          //Start the receiver
  Serial.begin(9600);                           //Start the connection with USB port for debug
  Serial.println("Setup Complete");             //Send message to debug
}
//Main Loop ======================================
void loop() {
  button=analogRead(4);                         //Read analog input for button
  ir=irRead();                                  // irRead
  if(initConf==false){                          //Run Configuration at least once
    config();                                   // Config
  }
  else if(button>=900){                         //if button is pressed
    reset();                                    // reset
  }
  else if(ir==p1[2]){                           //if menu button is pressed
    menu();                                     // menu
  }
  else if(ir==p1[1]){                           //if Up is pressed
    if(pPaddle[0]<=1){                          //if Player 1 Paddle is less than or = 1
      pPaddle[0]=pPaddle[0]-2;                  //Move Player 1 Paddle up
  }} 
  else if(ir==p1[0]){                           //if down is pressed
    if(pPaddle[0]>=95){                         //if Player 1 Paddle is greater than or = 95
      pPaddle[0]=pPaddle[0]+2;                  //Move Player 1 Paddle down
  }}
  AI();                                         // AI to move player 2 Paddle
  draw(false,0);                                // draw to update screen
  delay(50);                                    //delay 1/20 second
}
//IR Read ========================================
int irRead(){
  if(irrecv.decode(&results)){                  //if IR Input decoding sucseeds
    Serial.println(results.value);              //Print current value to debug monitor
    irrecv.resume();                            //Receive the next value
    return(results.value);                      //return the the IR value
  }
  else{
    return(0);                                  //return defualt
  }
  return(0);                                    //return to main loop
}

//Initial Configuration ==========================
void config() {
  int I=4;                                      //Varriable used for loops
  //Step 1 =======================================
  do{
    T[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]=0,1,2,3,4,15,15,15,15,15,15,15,15,15;
    T[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]=0,1,2,3,15,5,15,15,15,15,15,15,15,15;
    T[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]=0,1,2,3,15,15,6,15,15,15,15,15,15,15;
    T[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]=0,1,2,3,15,15,15,7,15,15,15,15,15,15;
    draw(true,0);
    ir=irRead();                                // irRead
    if(ir!=0){                                  //if ir does not = 0
      p1[I-4]=ir;                               //Player 1 Control
      I++;                                      //Increase I by 1
    }
  }while(p1[0]!=0 && p1[1]!=0 && p1[2]!=0 && p1[3]!=0);
  //Step 2 =======================================
  sel[5]=0;                                     //Reset Selection
  do{
    ir=irRead();                                // irRead
    for(I=2;I<6;I++){                           //Loop through sel[5]
      if(sel[5]==I){                            //if sel[5] = I
        sel[0,1,2,3,4,5]=1,(YNX[I]-1),45,(YNX[I]+6),53,I;
        ballSpeed=I-2;                          //Ball speed = displayed selection
      }
    }
    if(ir==p1[0]){                              //if Player 1 hits doWN button
      sel[5]=sel[5]+1;                          //Increase sel[5] by 1
      if(sel[5]>=6){                            //if sel[5] = 6
        sel[5]=0;                               //Set sel[5] = 0
      }
      delay(500);                               //delay 1/2 second
    }
    if(ir==p1[1]){                              //if Player 1 hits UP button
      sel[5]=sel[5]-1;                          //Decrease sel[5] by 1
       if(sel[5]<=-1){                          //if sel[5] = -1
        sel[5]=5;                               //Set sel[5] = 5
       }
      delay(500);                               //delay 1/2 second
    }
    T[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]=0,1,15,15,15,15,15,15,8,15,15,15,15,15;
    draw(true,2);                               // draw
  }while(ir==p1[3]);                            //Loop while Player 1 hits Select Button
  //Step 3 =======================================
  sel[5]=0;                                     //Reset Selection
  do{
    ir=irRead();                                //irRead
    switch (sel[5]){                            //Sel Selection #
      case 0:                                   //Turn on sel && boarder option 1
        sel[0,1,2,3,4,5]=1,(YNX[0]-1),45,(YNX[0]+14),53,0;
        break;                                  //Stop switch Statement
      case 1:                                   //Turn on sel && boarder option 2
        sel[0,1,2,3,4,5]=1,(YNX[1]-1),45,(YNX[1]+10),53,1;
        break;                                  //Stop switch Statement
    }
    if(ir==p1[0] && ir==p1[1]){                 //if Player 1 hits doWN button
      switch (sel[5]){                          //Rotate through selections
        case 0:
          sel[5]=1;
          delay(500);                           //delay 1/2 second
          break;                                //Stop switch Statement
        case 1:
          sel[5]=0;
          delay(500);                           //delay 1/2 second
          break;                                //Stop switch Statement
    }}
    T[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]=0,1,15,15,15,15,15,15,15,9,15,15,15,15;
    draw(true,2);                               // draw
  }while(ir==p1[3]);                            //Loop while Player 1 hits Select button
  return;                                       //return to main loop
}
//Pause Screen/Menu ==============================
void menu() {
  int I=0;                                      //Varriable used for loops
  sel[5]=0;                                     //Reset Selection
  do{
    ir=irRead();                                //irRead
    if(ir==p1[2]){                              //if Player 1 hits Start button
      return;                                   //return to main loop
    }
    else if(ir==p1[0]){                         //if Player 1 hits doWN button
      sel[5]=sel[5]+1;                          //Increase sel[5] by 1
      if(sel[5]>=3){                            //if sel[5] = 6
        sel[5]=0;                               //Set sel[5] = 0
      }
      delay(500);                               //delay 1/2 second
    }
    else if(ir==p1[1]){                         //if Player 1 hits UP button
      sel[5]=sel[5]-1;                          //Decrease sel[5] by 1
      if(sel[5]<=-1){                           //if sel[5] = -1
        sel[5]=2;                               //Set sel[5] = 5
      }
      delay(500);                               //delay 1/2 second
    }
    for(I=0;I<3;I++){                           //Loop through sel[5]
      if(sel[5]==I){                            //if sel[5] = I
        sel[0,1,2,3,4,5]=(1,23,(3+(confL[I+11]*6)+(2*confL[I+11])),105,(4+(confL[I+11]*6)+(2*confL[I+11])+7),I);
    }}
    T[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]=15,15,15,15,15,15,15,15,15,15,11,12,13,14;
    draw(true,0);
  }while(ir==p1[3]);                            //Loop while Player 1 hits Select Button
  //Selection=====================================
  switch (sel[5]){
    case 0: 
      config();                                 //Config
      break;                                    //Stop switch Statement
    case 1: 
      reset();                                  //Reset
      break;                                    //Stop switch Statement
    case 2: 
      sleep();                                  //Sleep
      break;                                    //Stop switch Statement
  }
  return;                                       //return to main loop
}
//Reset score and settings =======================
void reset() {
  digitalWrite(POWLED,LOW);                     //Turn the Power LED off
  TV.clear_screen();                            //Clear the TV screen
  pscore[0,1]=(0,0);                            //Player 1 && 2 score = 0
  ballSpeed=2;                                  //Ball Speed = 2
  ball[0,1]=(64,48);                            //Ball Position = 64,48
  bd[0,1]=(0,1);                                //Ball Direction = 0,1
  drawB=true;                                   //Draw Boarder = true
  initConf=false;                               //Run the Initial Config true
  pPaddle[0,1]=(40,40);                            //Player 1 && 2 Paddle position = 40
  p1[0,1,2,3]=(0,0,0,0);                        //Reset Controls
  sel[0,1,2,3,4,5]=(0,0,0,0,0,0);               //Reset Selection
  button=0;                                     //Reset readout on button
  delay(1000);                                  //delay 1 second
  return;                                       //return to main loop
}
//Put the device to sleep, still draws power =====
void sleep() {
  boolean slp=false;                            //Varriable used for wakeup
  reset();                                      // Reset
  do{
    delay(1000);                                //delay 1 second
    button=analogRead(4);                       //Read analog input for button
      if(button>=900){                          //if button is pressed
        slp=true;                               //Set slp to true to end loop
      }
  }while(slp=true);                             //Loop whilel button is pressed
  return;                                       //return to main loop
}
//Draw Ball and move it ==========================
void drawBall() {
  int I=0;                                      //Varriable used for loops
  //Movement =====================================
  switch (bd[0]){                               //Ball Direction X
    case 0:                                     //Left
      ball[0]=ball[0]-ballSpeed;
      break;                                    //Stop switch Statement
    case 1:                                     //Right
      ball[0]=ball[0]+1;
      break;                                    //Stop switch Statement
  }
  switch (bd[1]){
    case 0:                                     //Up
      ball[1]=ball[1]-ballSpeed;
      break;                                    //Stop switch Statement
    case 1:                                     //Straight
      ball[0]=ball[0]+1;
      break;                                    //Stop switch Statement
    case 2:                                     //down
      ball[1]=ball[1]+ballSpeed;
      break;                                    //Stop switch Statement
  }
  //Collision Detection ==========================
  if(ball[0]<=1){//Check scorezones
    score(2);//score
    ball[0,1]=(64,48);//Ball Position = 64,48
    bd[1]=1;//Ball Direction Straight
  }
  else if(ball[0]>=127){ 
    score(1);//score
    ball[0,1]=(64,48);//Ball Position = 64,48
    bd[1]=1;//Ball Direction Straight
  }
  if(ball[1]<=1 && ball[1]>=95){//Check top and bottom
    switch (bd[1]){
      case 0://Moving Up
        bd[1]=2;//Change direction to down
        break;//Stop switch Statement
      case 2://Moving down
        bd[1]=0;//Change Direction to Up
        break;//Stop switch Statement
  }}
  //Player 1 Paddle ============================
  if(ball[1]>=(pPaddle[0]-2) && ball[1]<=(pPaddle[0]+17) && ball[0]<=9 && ball[0]>=3){
    bd[0]=1;//Ball Direction = Right
    if(ball[1]<=(pPaddle[0]+5)){//if ball hits top side of paddle
      switch (bd[1]){
        case 0://Moving Up
          break;//Stop switch Statement
        case 1://Moving Straight
          bd[1]=0;//Change Direction to Up
          break;//Stop switch Statement
        case 2://Moving down
          bd[1]=1;//Change Direction to Straight
          break;//Stop switch Statement
    }}
    else if(ball[1]>=(pPaddle[0]+11)){//if ball hits bottom side of paddle
      switch (bd[1]){
        case 2://Moving down
          break;//Stop switch Statement
        case 1://Moving Straight
          bd[1]=2;//Change Direction to down
          break;//Stop switch Statement
        case 0://Moving Up
          bd[1]=1;//Change Direction to Straight
          break;//Stop switch Statement
  }}}
  //Player 2 Paddle ============================
  if(ball[1]>=(pPaddle[1]-2) && ball[1]<=(pPaddle[1]+17) && ball[0]>=119 && ball[0]<=124){
    bd[0]=0;//Ball Direction = Left
    if(ball[1]<=(pPaddle[1]+5)){//if ball hits top side of paddle
      switch (bd[1]){
        case 0://Moving Up
          break;//Stop switch Statement
        case 1://Moving Straight
          bd[1]=0;//Change Direction to Up
          break;//Stop switch Statement
        case 2://Moving down
          bd[1]=1;//Change Direction to Straight
        break;//Stop switch Statement
    }}
    else if(ball[1]>=(pPaddle[1]+11)){//if ball hits bottom side of paddle
      switch(bd[1]){
        case 2://Moving down
          break;//Stop switch Statement
        case 1://Moving Straight
          bd[1]=2;//Change Direction to down
          break;//Stop switch Statement
        case 0://Moving Up
          bd[1]=1;//Change Direction to Straight
          break;//Stop switch Statement
  }}}
  //Draw =======================================
  TV.draw_rect(ball[0],ball[1],(ball[0]+1),(ball[1]+1),1,1);
  return;  //return to draw
}
//Player 2 AI Movement ===========================
void AI() {
  if(ball[0]>=64){//if Ball is greater than or = to half way
    if(pPaddle[1]>=1 && pPaddle[1]<=95){ 
      dir=ball[1]-pPaddle[1];//Get direction paddle needs to move
      dir=constrain(dir,-1,1);//force dir range from -1 to 1
      switch (dir){
        case -1://Ball is higher
          pPaddle[1]=pPaddle[1]-2;
          break;//Stop switch Statement
        case 0://Aligned with ball
          dir=random(-1,2);//Generate a random Number between -1 && 1
          pPaddle[1]=pPaddle[1]+(dir*2);
          break;//Stop switch Statement
        case 1://Ball is lower
          pPaddle[1]=pPaddle[1]+1;
          break;//Stop switch Statement
  }}}
  return;  //return to main loop
}
//Displays and modifies score ====================
void score(int F) {
  if(F==0){ 
    TV.set_cursor(8,8);//Set cursor location to 8 pixels in, 8 pixels down from top left
    TV.print(pscore[0]);//Draw Player 1 Score
    if(pscore[1]<=9){//Adjust for double digit
      TV.set_cursor(120,8);//Set cursor location to 8 pixels in from, 8 pixels down top right
    }
    else{
      TV.set_cursor(112,8);//Set cursor location to 16 pixels in from, 8 pixels down top right
    }
    TV.print(pscore[1]);//Draw Player 2 Score
  }
  else if(F==1){//if = 1
    pscore[0]=pscore[0]+1;//Increase Player 1 Score
    return;  //return to drawBall
  }
  else if(F==2){//if = 2
    pscore[1]=pscore[1]+1;//Increase Player 2 Score
    return;  //return to drawBall
  }
  return;  //return to draw
}
//Draw the boarder ===============================
void boarder() {
  TV.draw_rect(64,0,65,96,1,1);//Draw a line down the middle of the screen
  return;  //return to draw
}
//Draw the paddles ===============================
void paddle() {
  TV.draw_rect(4,pPaddle[0],8,(pPaddle[0]+16),1,1);
  TV.draw_rect(124,pPaddle[1],120,(pPaddle[1]+16),1,1);
  return;  //return to draw
}
//Auto call and draw nessesary things for the screen
void draw(boolean F, int slt) {
  if(F==false){//if F = false display game
    if(drawB==true){//if draw boarder is true
      boarder();// boarder
    }
    score(0);// score
    paddle();// paddle
    drawBall();// drawBall
  }
  else if(F==true){//if F = true display menus
    int I=0;//Placeholder for loops
    size=0;
    for(I=0;I<14;I++){
      if(T[I]<15){
        size=size+1;//Find and store the size of array T
    }}
    for(I=0;I<size;I++){//for every line in array T
      TV.set_cursor(24,(4+(confL[T[I]]*6)+(2*confL[T[I]])));
      TV.print(conf[T[I]]);//Display text line
    }
    if(slt==1){//if yes no display = 1 display yes no options
      for(I=0;I<2;I++){//Loop through array
        TV.set_cursor(YNX[I],46);//Set cursor to location
        TV.print(YN[I]);//display text
    }}
    else if(slt==2){//if yes no display = 2 display 1 2 3 4 options
      for(I=2;I<6;I++){//Loop through array
        TV.set_cursor(YNX[I],46);//Stored text for use in menus
        TV.print(YN[I]);//display text
    }}
    if(sel[0]==1){//if selection = on
      TV.draw_rect(sel[1],sel[2],sel[3],sel[4],1);
      sel[0]=0;//Turn Selection off
  }}
  return;  //return to main loop
}
