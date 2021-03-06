;/* BinMastermiind.c: Binary mastermind guessing game
 * author: Vanessa Yao
 * contributors: Andy Fok, Sherlock Qin
 *
 * Modifications
 * 4/17/2022  VIY
 *  - Cleaned up comments and dead functions 
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
const int PR_NUM = 10; // number of photoresistors
const int BMP_NUM = 3; // number of bumper switches
const int LED_NUM = 4; // number of LEDs
const int ENTER_BMP = 2; // index of 'enter' bumper switch 
/***************************************/

#define NEWGAME 'n'
#define SETTINGS 's'
#define QUITGAME 'q'
#define DEFAULT_GUESSES 10
#define DEFAULT_DIGITS 5
#define MAX 50

/****************PIN DEFINITIONS***************/
// pin definitions for bumper switches; currently everything is following the schematic
// bump_switches[0]: count down
// bump_switches[1]: count up
// bump_switches[2]: Enter
const int bump_switches[BMP_NUM] = {A15, A12, A10};

// used for previous setup to test
//const int bump_switches[BMP_NUM] = {A15, A14, A13};

// bump_switch menu control {count down, count up}
const int bmp_menu_control[2] = {-1, 1};

// leds[0]: the red led in the tri-led
// leds[1]: the green led in the tri-led
// leds[2]: the blue led in the tri-led
// leds[3]: the individual red led
const int leds[LED_NUM] = {P2_0, P2_1, P2_2, P1_0};

//int photo_in_0 = A9;
//int photo_in_1 = A8;
////int photo_in_2 = A6;
//int photo_in_3 = A1;
//int photo_in_4 = A0;
//int photo_in_5 = A19;
//int photo_in_6 = A18;
//int photo_in_7 = A16;
//int photo_in_8 = 22;
//int photo_in_9 = A2;

//const int pr_in[PR_NUM] = {A11, A9}; // testing 2 photoresistor pins

/**** Uncomment to implement 10 Photoresistors, change PR_NUM to 10, and comment the line above *********/
const int pr_in[PR_NUM] = {A9, A8, A6, A1, A0, A19, A18 , A16, A7, A2}; // 10 photoresistors


/*******************SETTINGS*******************/
int bump_switch_values[BMP_NUM];

int pr_values[PR_NUM];

// menu option 1: Start game
//             2: Settings
//             3: Auit game
int menu_option = 1; // initialization with menu_option being the first option;

char *user_guess = NULL;  // defining a pointer that takes user input;

int pr_ref[PR_NUM]; // photoresistor reference values

int offset = 150;         // the photoresistor offset value

int current_input_pointer = 0;  // for recording of the current pointer location
char enter = '\n';    // the enter key on your keyboard;
// int default_length = 1024;  // maximum input string length is 1KB (also = to MAX macro)
int num_digits = DEFAULT_DIGITS;
int num_guesses = DEFAULT_GUESSES;

int test_int = 0;         // for testing converting the user input string into an integer and add with an integer to see if output is correct;

int in_mainmenu = 1;  // inidicates if user is in main menu(disable photoresistor input since it is only used in-game and in settings)
/*********************************************/

void menu(void);
int startGame(void);
int genRandBinNum(int digits);
int compare(char answer[], char guess[]);
void changeSettings(void);
int getInput(int c_fl);
char* getGuess();
void donothing();
void photo_value(int photo_in_num, int photo_value_num, int current_pr);
void up_down_enter(int bump_switch_num, int bump_switch_value, int bmp_num);
void show_menu();
void turn_off_led();

void setup() {  
  Serial.begin(9600);
  //Serial.println("Beginning initializationn");
  
  for (int i=0;i<BMP_NUM;i++) {bump_switch_values[i] = 0;} // intializations for bump_switch values;
  for (int i=0;i<PR_NUM;i++) {pr_ref[i] = 0;} // initializations for photoresistor reference values;
  for (int i=0; i<PR_NUM; i++) {pr_values[i] = 0;} // initialize photoresistor values
  
  for (int i=0; i<BMP_NUM;i++) { pinMode(bump_switches[i], INPUT);}
  
  //Serial.println("Initializing LEDs as output");
  for (int i=0;i<LED_NUM;i++) {pinMode(leds[i], OUTPUT);}

  //Serial.println("Initializationn photoresistors as input");
  for (int i=0;i<PR_NUM;i++) {pinMode(pr_in[i], INPUT);}

  // initialize user guess with pointer to string of size MAX
//  Serial.println("Allocating memory for user guess");
  user_guess = (char*) malloc(MAX * sizeof(char));

  //Serial.println("Reading base photoresistor values...");
  for (int i=0;i<PR_NUM;i++) {
    pr_ref[i] = analogRead(pr_in[i]) + offset;
//    Serial.print(i);
//    Serial.print(": ");
//    Serial.println(pr_ref[i]);
    delay(10);
  }

  Serial.println("Beginning game...");
}

void loop() {  
  delay(300);
  Serial.print("\n*************Binary Mastermind*************\n");
  Serial.print("Welcome to binary mastermind!\n");
  menu();

  scan_input();

  if (menu_option == 1) {
    Serial.println(NEWGAME);
    /* start new game */
    in_mainmenu = 0;
    startGame();
  }
  else if (menu_option == 2) {
    in_mainmenu = 0;
    Serial.println(SETTINGS);
    /* go to settings */
    changeSettings();
  }
  else if (menu_option == 3) {
    Serial.println(QUITGAME);
    /* quit game */
    Serial.print("\nQuitting game...");
    free(user_guess);
  }
  in_mainmenu = 1;
}

// print main menu
void menu(void)
{
    /* print main menu */
    /* return option chosen */
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
    Serial.print("\nBMP0: navigate up");
    Serial.print("\nBMP1: navigate down");
    Serial.print("\nBMP2: enter");
    Serial.print("\nEnter your choice: ");
}

// start game
int startGame(void)
{
  char answer[num_digits];  // answer to game
  int guesses_left = num_guesses; // keep track of guesses left
  int result = 1; // result is 0 if win, 1 if lose/quit
  
  /* generate random binary number based on number of digits given */
  Serial.print("Enter a random number to generate the answer (less than ");
  Serial.print(MAX);
  Serial.print(" digits): ");
  scan_input();
  int seed = atoi(user_guess); // get seed from user
  srand(seed);

  // generate random binary number(as a string)
  for (int i=0; i < num_digits; i+=1)
  {
    //num = rand() % 2;
    answer[i] = rand() % 2 + '0'; // answer[i] = 0 if random # is even, 1 if odd
  }

  // check the answer is random 
  // Serial.print("Answer: ");
  // Serial.println(answer);
  Serial.println("\nGame start!");
  while (guesses_left > 0)
  {
    Serial.print("Enter your guess, no more than ");
    Serial.print(num_digits);
    Serial.print(" digits (");
    Serial.print(guesses_left);
    Serial.print(" guesses left): "); 
    scan_input(); // get user guess in user_guess
        
    guesses_left -= 1;
    if (compare(answer, user_guess) == 1)
    {
      result = 0;
      break;
    }
    else {continue;}
  }
  if (result == 0) {Serial.print("Congratulations, you guessed right!\n");}
  else {
    Serial.print("Game over! You've used all your guesses. Better luck next time!\n");
    Serial.print("The correct answer is ");
    Serial.print(answer);
    }
  
// Uncomment below two lines to reset to default guesses and digits
//  num_guesses = DEFAULT_GUESSES;
//  num_digits = DEFAULT_DIGITS;
  
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
void changeSettings()
{
  Serial.print("Please enter the length of the bit string: ");
  scan_input();
  if (atoi(user_guess) > MAX) {num_digits = MAX;} 
  else {num_digits = atoi(user_guess);}

  Serial.print("Please enter the max number of guesses: ");
  scan_input();
  num_guesses = atoi(user_guess);
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

    // check for photoresistor input if not in main menu
    if (!(in_mainmenu)) {
      for (int i=0;i<PR_NUM;i++) {photo_value(pr_in[i], pr_values[i], i);}
    }

    // check for bumper switch input
    for (int i=0;i<BMP_NUM;i++) {up_down_enter(bump_switches[i], bump_switch_values[i], i);}
  
    show_menu();
    // check if user tries to enter more than MAX number of characters
    if (current_input_pointer == MAX+1) {break;}
  }
  Serial.println();
}

void photo_value(int photo_in_num, int photo_value_num, int current_pr) {   // this is for reading photoresistor value and convert them into string of number characters
  photo_value_num = analogRead(photo_in_num);
  delay(100);
  if ((photo_in_num == pr_in[current_pr]) & (photo_value_num > pr_ref[current_pr])) {
    //Serial.print(current_pr + '0');
    *(user_guess + current_input_pointer) = current_pr + '0'; // add '0' to convert from int to char
    Serial.print(current_pr); // print user input
    current_input_pointer++;
  }
}


void up_down_enter(int bump_switch_num, int bump_switch_value, int bmp_num) {    // this is for menu_selection using the bumper_switches
  
  bump_switch_value = analogRead(bump_switch_num);      // read bump_switch_value;
  if ((bmp_num != ENTER_BMP) & (bump_switch_value == 1023)){    // this is for counting up/counting down;  
      // checks if counting down and 1 < menu option <= 3, or counting up and 3 > menu option >= 1
      if (((menu_option <= 3) & (menu_option > 1) & (bmp_num==0) ) | ((menu_option >=1) & (menu_option < 3) & (bmp_num==1))) {
        menu_option += bmp_menu_control[bmp_num];
      }
    }
    else if ((bmp_num == ENTER_BMP) & (bump_switch_value == 1023)){   // this is for the enter key on your keyboard;
      *(user_guess + current_input_pointer) = enter;
      current_input_pointer++;
    }  
  
}

void show_menu() {    // this show menu shows the menu on the board by lighting differnt LED.
  turn_off_led();
  //Serial.println(menu_option);
  digitalWrite(leds[menu_option-1], HIGH); //menu options are 1,2,3,4, lists are 0-indexed so subtract 1
}

void turn_off_led() {     // as it looks like, this function turns off all LEDs
  for (int i=0;i<LED_NUM;i++) {digitalWrite(leds[i], LOW);}
}

void donothing() {
  while(1) {}
}
