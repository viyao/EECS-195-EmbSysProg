;/* BinMastermiind.c: Binary mastermind guessing game
 * author: Vanessa Yao
 * contributors: Andy Fok, Sherlock Qin
 *
 * Modifications
 * 4/16/2022  VIY, SQ, AF
 *  - Updated with hardware inputs
 * 4/9/2022   VIY
 *  - ported to Energia
 * 3/31/2022  VIY
 *  - reset default guesses and digits after
 *  each game
 * 3/30/2022  VIY
 *  - initial version
 */
//
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <time.h>

/****************HARDWARE***************/
#define PR_NUM 2 // number of photoresistors
#define BMP_NUM 3 // number of bumper switches
#define LED_NUM 4 // number of LEDs
/***************************************/

#define NEWGAME 'n'
#define SETTINGS 's'
#define QUITGAME 'q'
#define DEFAULT_GUESSES 10
#define DEFAULT_DIGITS 5
#define MAX 50

/****************PIN DEFINITIONS***************/
//int bump_switch_0 = A15;  // pin definitions for bumper switches; currently everything is following the schematic
//int bump_switch_1 = A14;  // bump_switch_0 is for counting up; bump_switch_1 is for counting down; and bump_switch_2 is for the 'enter', also known as the enter key on your keyboard;
//int bump_switch_2 = A13;

const int bump_switches[] = {A15, A14, A13};

//int led1 = P2_0;    // the red led in the tri-led
//int led2 = P2_1;    // the green led in the tri-led
//int led3 = P2_2;    // the blue led in the tri-led
//int led4 = P1_0;    // the individual red led

const int leds[] = {P2_0, P2_1, P2_2, P1_0};

int photo_in_0 = A11;
int photo_in_1 = A9;
////int photo_in_2 = A8;
//int photo_in_3 = A6;
//int photo_in_4 = A1;
//int photo_in_5 = A0;
//int photo_in_6 = A12;
//int photo_in_7 = A10;
//int photo_in_8 = A7;
//int photo_in_9 = A4;

const int pr_in[PR_NUM] = {A11, A9}; // testing 2 photoresistor pins
// const int pr_in[PR_NUM] = {A11, A9, A8, A6, A1, A0, A12, A10, A7, A4}; // 10 photoresistors


/*******************SETTINGS*******************/
int bump_switch_values[BMP_NUM];
// intializations for bump_switch values;

//int bump_switch_0_value = 0;  // intializations for bump_switch values;
//int bump_switch_1_value = 0;
//int bump_switch_2_value = 0;

int pr_values[PR_NUM];
// initialize photoresistor values

int photo_value_0 = 0;        // initializaitons for photoresistor values;
int photo_value_1 = 0;
//int photo_value_2 = 0;
//int photo_value_3 = 0;
//int photo_value_4 = 0;
//int photo_value_5 = 0;
//int photo_value_6 = 0;
//int photo_value_7 = 0;
//int photo_value_8 = 0;
//int photo_value_9 = 0;

int menu_option = 1; // initialization with menu_option being the first option;

char *user_guess = NULL;  // defining a pointer that takes user input;


int pr_ref[PR_NUM]; // photoresistor reference values

//int photo_ref_0 = 0;        // initializations for photoresistor reference values;
//int photo_ref_1 = 0;
//int photo_ref_2 = 0;
//int photo_ref_3 = 0;
//int photo_ref_4 = 0;
//int photo_ref_5 = 0;
//int photo_ref_6 = 0;
//int photo_ref_7 = 0;
//int photo_ref_8 = 0;
//int photo_ref_9 = 0;

int offset = 150;         // the photoresistor offset value

int current_input_pointer = 0;  // for recording of the current pointer location
char enter = '\n';    // the enter key on your keyboard;
// int default_length = 1024;  // maximum input string length is 1KB (also = to MAX macro)
int num_digits = DEFAULT_DIGITS;
int num_guesses = DEFAULT_GUESSES;

int test_int = 0;         // for testing converting the user input string into an integer and add with an integer to see if output is correct;

/*********************************************/

void menu(void);
int startGame(void);
int genRandBinNum(int digits);
int compare(char answer[], char guess[]);
int changeSettings(void);

// Not using Serial monitor inputs
//int getInput(int c_fl);
//char* getGuess();

void donothing();
void photo_value(int photo_in_num, int photo_value_num, int i);
void up_down_enter(int bump_switch_num, int bump_switch_value);
void show_menu();
void turn_off_led();

void setup() {  
  for (int i=0;i<BMP_NUM;i++) {bump_switch_values[i] = 0;}
  for (int i=0;i<PR_NUM;i++) {pr_ref[i] = 0;}
  for (int i=0; i<(sizeof(pr_in)/sizeof(pr_in[0])); i++) {pr_values[i] = 0;}
  user_guess = (char *) malloc(MAX * sizeof(char));  // allocating memory spaces for the user_str pointer

  for (int i=0; i<BMP_NUM;i++) { pinMode(bump_switches[i], INPUT);}
  //pinMode(bump_switch_0, INPUT);  // pinmode definitions for all pins used
  //pinMode(bump_switch_1, INPUT);
  //pinMode(bump_switch_2, INPUT);

  for (int i=0;i<LED_NUM;i++) {pinMode(leds[i], OUTPUT);}
//  pinMode(led1, OUTPUT);
//  pinMode(led2, OUTPUT);
//  pinMode(led3, OUTPUT);
//  pinMode(led4, OUTPUT);

  for (int i=0;i<PR_NUM;i++) {pinMode(pr_in[i], INPUT);} 
  
//  pinMode(photo_in_0, INPUT);
//  pinMode(photo_in_1, INPUT);
//  pinMode(photo_in_2, INPUT);
//  pinMode(photo_in_3, INPUT);
//  pinMode(photo_in_4, INPUT);
//  pinMode(photo_in_5, INPUT);
//  pinMode(photo_in_6, INPUT);
//  pinMode(photo_in_7, INPUT);
//  pinMode(photo_in_8, INPUT);
//  pinMode(photo_in_9, INPUT);

  user_guess = (char*) malloc(MAX * sizeof(char));

  for (int i=0;PR_NUM;i++) {
    pr_ref[i] = analogRead(pr_in[i]) + offset;
    delay(10);
  }
//  
//  photo_ref_0 = analogRead(photo_in_0) + offset;  // these are necessary, each photoresistor's threshold value must be determined individually;
//  delay(10);
//  photo_ref_1 = analogRead(photo_in_1) + offset;
//  delay(10);
//  photo_ref_2 = analogRead(photo_in_2) + offset;
//  delay(10);
//  photo_ref_3 = analogRead(photo_in_3) + offset;
//  delay(10);
//  photo_ref_4 = analogRead(photo_in_4) + offset;
//  delay(10);
//  photo_ref_5 = analogRead(photo_in_5) + offset;
//  delay(10);
//  photo_ref_6 = analogRead(photo_in_6) + offset;
//  delay(10);
//  photo_ref_7 = analogRead(photo_in_7) + offset;
//  delay(10);
//  photo_ref_8 = analogRead(photo_in_8) + offset;
//  delay(10);
//  photo_ref_9 = analogRead(photo_in_9) + offset;
//  delay(10);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {  
  delay(200);
  char answer;
  menu();

  scan_input();

  if (menu_option == 1) {
    Serial.println(NEWGAME);
    /* start new game */
    startGame();
  }
  else if (menu_option == 2) {
    Serial.println(SETTINGS);
    /* go to settings */
    changeSettings();
  }
  else if (menu_option == 3) {
    Serial.println(QUITGAME);
    /* quit game */
    Serial.print("\nQuitting game...");
    free(user_guess);
    donothing();
  }
  else
  {
    Serial.print("Not a valid option.\n");
  }
}

// print main menu
void menu(void)
{
    /* print main menu */
    /* return option chosen */
    Serial.print("\n*************Binary Mastermind*************\n");
    Serial.print("Welcome to binary mastermind!\n");
    Serial.print("*************Main Menu*************\n");
    Serial.print("\tNew Game: ");
    Serial.print(NEWGAME);
    Serial.print(" (RGB LED, Red)");
    Serial.print("\n\tSettings: ");
    Serial.print(SETTINGS);
    Serial.print(" (RGB LED, Green)");
    Serial.print("\n\tQuit Game: ");
    Serial.print(QUITGAME);
    Serial.print(" (RGB LED, Blue)");
    Serial.print("\nUse BMP0 to navigate down, BMP1 to navigate up, and BMP2 to press enter");
    Serial.print("\nEnter your choice: ");
}

// start game
int startGame(void)
{
  Serial.read(); // read the enter line 
  char answer[num_digits];  // answer to game
  int guesses_left = num_guesses; // keep track of guesses left
  int result = 1; // result is 0 if win, 1 if lose/quit
  // char* guess;  user's guess
  
  /* generate random binary number based on number of digits given */
  Serial.print("Enter a random number(less than ");
  Serial.print(MAX);
  Serial.print(" digits): ");
  scan_input();
  int seed = atoi(user_guess); // get seed from user
  Serial.println(seed);
  srand(seed);
  
  for (int i=0; i < num_digits; i+=1)
  {
    //num = rand() % 2;
    answer[i] = rand() % 2 + '0'; // answer[i] = 0 if random # is even, 1 if odd
  }

  // check the answer is random 
  // Serial.print("Answer: ");
  // Serial.println(answer);

  while (guesses_left > 0)
  {
    Serial.print("Enter your guess, no more than ");
    Serial.print(num_digits);
    Serial.print(" digits (");
    Serial.print(guesses_left);
    Serial.print(" guesses left)"); // 'q' to quit: "); user can only enter numbers 0-9
    scan_input(); // get user guess in user_guess
  
    //Serial.println(user_guess);
    //if (user_guess[0] == QUITGAME) {
    //  Serial.print("Quitting game...\n");
    //  return 1;
    //}
    guesses_left -= 1;
    if (compare(answer, user_guess) == 1)
    {
      result = 0;
      break;
    }
    else {continue;}
  }
  if (result == 0) {Serial.print("Congratulations, you guessed right!\n");}
  else {Serial.print("Game over! You've used all your guesses. Better luck next time!\n");}
  // reset to default guesses and digits
  num_guesses = DEFAULT_GUESSES;
  num_digits = DEFAULT_DIGITS;
  
  return 0;
} 

// compare answer to guess and generate result
// returns 1 if user guesses every digit, otherwise 0
int compare(char answer[], char* guess)
{
  int num_right = 0;
  /* compare answer and guess, return comparison */
  /* and print result */
  Serial.print("Total number of digits correct in correct place: ");
  for (int i=0; i < num_digits; i++)
  {
    if (*(guess+i)==answer[i])
    {
      num_right +=1;
    }
  }
  Serial.println(num_right);
  if (num_right == num_digits) {return 1;}
  else {return 0;}
}

// change settings
int changeSettings(void)
{
  Serial.read(); // read the enter line 
  int guesses;  // # of guesses user chooses
  int digits; // # of digits user chooses
  char num_entered;  // option chosen by user
  
    // settings menu
    Serial.print("*************Settings*************\n");
    Serial.print("Here you can choose to change the number of digits\n");
    Serial.print("and the number of guesses\n");
    Serial.print("\tNumber of digits: 1\n");
    Serial.print("\tNumber of guesses: 2\n");
    Serial.print("\tExit to main menu: 3\n");
    Serial.print("Enter your choice: ");
    while (!(Serial.available()>0)) {delay(200);}  // do nothing if no input
//    num_entered = getInput(0);  // get number
    Serial.println(num_entered);
    
    if (num_entered == 3) { return 0;}
    else if (num_entered == 2)
    {
      Serial.print("\nEnter the number of guesses greater than 0 less than ");
      Serial.print(MAX);
      Serial.print("(default ");
      Serial.print(DEFAULT_GUESSES);
      Serial.print("): ");
      Serial.read(); // read the enter line 
      while (!(Serial.available()>0)) {delay(200);}  // do nothing if no input

//      guesses = getInput(0);  // get # of guesses
      Serial.println(guesses);
      if ((guesses > 0) & (guesses < MAX)) {num_guesses = guesses;}
    }
    else if (num_entered == 1)
    {
      Serial.print("\nEnter the number of digits greater than 0 less than ");
      Serial.print(MAX);
      Serial.print("(default ");
      Serial.print(DEFAULT_DIGITS);
      Serial.print("): ");
      Serial.read(); // read the enter line 
     while (!(Serial.available()>0)) {delay(200);}  // do nothing if no input

//      digits = getInput(0); // get # of digits
      Serial.print(digits);
      if ((digits > 0) & (digits < MAX)) {num_digits = digits;}
    }
    else
    {
      Serial.print("Enter a valid option\n");
    }
}

    
// get a single character/integer input from user
// if getting character, c_fl == 1
// !! must read serial input after every call to eliminate input from enter key !!
// max input is 50 char
//int getInput(int c_fl) {
//  // add a bit of delay
//  char answer[MAX];
//  int i = 0;  // keep track of num characters entered
//  int byteReceived; 
//  while (Serial.available() > 0) {
//    byteReceived = Serial.read();
//    answer[i] = char(byteReceived);
//    if (c_fl) {return byteReceived;}
//    i++;
//    if ((i > MAX) | (i == MAX)) {
//      while(Serial.available() > 0) {Serial.read();}
//      return atoi(answer);
//    }
//    if (byteReceived == 10) {
//      return atoi(answer);  // return integer value input
//    }
//  }
//}   
// !! must read serial input after every call to eliminate input from enter key !!
// get string input from user (for guessing game)
//char* getGuess() {
//  int i = 0;  // keep track of num characters entered
//  int byteReceived;
//
//  while (Serial.available() > 0) {
//    byteReceived = Serial.read();
//    // check if user entered more than the number of digits
//    if ((i > num_digits) | (i == num_digits)) {
//      char *rtn_answer = user_guess;  // answer to return
//      while(Serial.available()>0) {Serial.read();};  // flush any character in the buffer
//      return rtn_answer;
//    }
//    user_guess[i] = char(byteReceived);
//    i++;
//
//    // check if user entered end of line
//    if (byteReceived == 10) {
//      if (i < num_digits) {
//        for (int j=i-1;j<num_digits; j++) {
//          user_guess[j] = NULL;
//          // fill the rest of user_guess with NULL if user enters less than number of digits required
//          // start at i-1 to remove newline character
//        }
//      }
//      char *rtn_answer = user_guess;
//      return rtn_answer;
//    }
//  }
//}
  

//
void donothing() {
  while(1) {}
}

void scan_input() {
  for (int i = 0; i < current_input_pointer; i++) {   // initializing the pointer everytime this function is called; 
    *(user_guess + i) = ' ';
  }
  current_input_pointer = 0;    // initializing the pointer location offset

  while (*(user_guess + current_input_pointer - 1) != enter) {    // this loop exits when the last pointer is 'enter', also known as the enter key on your keyboard;
//    Serial.println("I'm in loop");      // these are for testing purposes only
//    Serial.println(user_guess);
//    Serial.print("The menu_option is ");
//    Serial.println(menu_option);

    delay(1000);

    for (int i=0;i<PR_NUM;i++) {photo_value(pr_in[i], pr_values[i], i);}
//    photo_value(photo_in_0, photo_value_0);
//    photo_value(photo_in_1, photo_value_1);
  //  photo_value(photo_in_2, photo_value_2);
  //  photo_value(photo_in_3, photo_value_3);
  //  photo_value(photo_in_4, photo_value_4);
  //  photo_value(photo_in_5, photo_value_5);
  //  photo_value(photo_in_6, photo_value_6);
  //  photo_value(photo_in_7, photo_value_7);
  //  photo_value(photo_in_8, photo_value_8);
  //  photo_value(photo_in_9, photo_value_9);
  
    for (int i=0;i<BMP_NUM;i++) {up_down_enter(bump_switches[i], bump_switch_values[i]);}
//    
//    up_down_enter(bump_switch_0, bump_switch_0_value);
//    up_down_enter(bump_switch_1, bump_switch_1_value);
//    up_down_enter(bump_switch_2, bump_switch_2_value);
  
    show_menu();
    Serial.print(*(user_guess + current_input_pointer-1)); // print user input 
  }
}

void photo_value(int photo_in_num, int photo_value_num, int current_pr_index) {   // this is for reading photoresistor value and convert them into string of number characters
  photo_value_num = analogRead(photo_in_num);
  delay(10);
  if ((photo_in_num == pr_in[current_pr_index]) & (photo_value_num > pr_values[current_pr_index])) {
    *(user_guess + current_input_pointer) = current_pr_index + '0'; // add '0' to convert from int to char
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_1) & (photo_value_num > pr_values[0])) {
    *(user_guess + current_input_pointer) = '1';
    current_input_pointer++;
  }
//  else if ((photo_in_num == photo_in_2) & (photo_value_num > pr_values[0])) {
//    *(user_guess + current_input_pointer) = '2';
//    current_input_pointer++;
//  }
//  else if ((photo_in_num == photo_in_3) & (photo_value_num > pr_values[0])) {
//    *(user_guess + current_input_pointer) = '3';
//    current_input_pointer++;
//  }
//  else if ((photo_in_num == photo_in_4) & (photo_value_num > pr_values[0])) {
//    *(user_guess + current_input_pointer) = '4';
//    current_input_pointer++;
//  }
//  else if ((photo_in_num == photo_in_5) & (photo_value_num > pr_values[0])) {
//    *(user_guess + current_input_pointer) = '5';
//    current_input_pointer++;
//  }
//  else if ((photo_in_num == photo_in_6) & (photo_value_num > pr_values[0])) {
//    *(user_guess + current_input_pointer) = '6';
//    current_input_pointer++;
//  }
//  else if ((photo_in_num == photo_in_7) & (photo_value_num > pr_values[0])) {
//    *(user_guess + current_input_pointer) = '7';
//    current_input_pointer++;
//  }
//  else if ((photo_in_num == photo_in_8) & (photo_value_num > pr_values[0])) {
//    *(user_guess + current_input_pointer) = '8';
//    current_input_pointer++;
//  }
//  else if ((photo_in_num == photo_in_9) & (photo_value_num > photo_ref_9)) {
//    *(user_guess + current_input_pointer) = '9';
//    current_input_pointer++;
//  }
}

void up_down_enter(int bump_switch_num, int bump_switch_value) {    // this is for menu_selection using the bumper_switches
  
  bump_switch_value = analogRead(bump_switch_num);      // read bump_switch_value;
  
  if ((bump_switch_num == bump_switches[0]) & (bump_switch_value == 1023)){    // this is for count_up;
    (menu_option >= 4) ? menu_option = 4: menu_option++;
  }
  else if ((bump_switch_num == bump_switches[1]) & (bump_switch_value == 1023)){   // this is for count_down;
    (menu_option <= 1) ? menu_option = 1: menu_option--;
  }
  else if ((bump_switch_num == bump_switches[2]) & (bump_switch_value == 1023)){   // this is for the enter key on your keyboard;
    *(user_guess + current_input_pointer) = enter;
    current_input_pointer++;
  }
  
}

void show_menu() {    // this show menu shows the menu on the board by lighting differnt LED.
  turn_off_led();
  digitalWrite(leds[menu_option-1], HIGH); //menu options are 1,2,3,4, lists are 0-indexed so subtract 1
//  if (menu_option == 1) {
//      turn_off_led();
//      digitalWrite(led1, HIGH);
//  }
//  else if (menu_option == 2) {
//    turn_off_led();
//    digitalWrite(led2, HIGH);
//  }
//  else if (menu_option == 3) {
//    turn_off_led();
//    digitalWrite(led3, HIGH);
//  }
//  else if (menu_option == 4) {
//    turn_off_led();
//    digitalWrite(led4, HIGH);
//  }  
}

void turn_off_led() {     // as it looks like, this function turns off all LEDs
  for (int i=0;i<LED_NUM;i++) {digitalWrite(leds[i], LOW);}
//  digitalWrite(led1, LOW);
//  digitalWrite(led2, LOW);
//  digitalWrite(led3, LOW);
//  digitalWrite(led4, LOW);
}

// Add switch functions 