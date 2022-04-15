/* BinMastermiind.c: Binary mastermind guessing game
 * author: Vanessa Yao
 *
 * Modifications
 * 4/9/2022   VIY
 *  - ported to Energia
 * 3/31/2022  VIY
 *  - reset default guesses and digits after
 *  each game
 * 3/30/2022  VIY
 *  - initial version
 */

//  #include <stdio.h>
//  #include <string.h>
//  #include <stdlib.h>
//  #include <time.h>

//  #define NEWGAME 'n'
//  #dine SETTINGS 's'
//  #dine QUITGAME 'q' 
#define DEFAULT_GUESSES 10
#define DEFAULT_DIGITS 5
#define MAX 50


/*******************SETTINGS*******************/
int num_digits = DEFAULT_DIGITS;
int num_guesses = DEFAULT_GUESSES;
char user_guess[MAX];  

/*********************************************/

char menu(void);
int startGame(void);
int genRandBinNum(int digits);
int compare(char answer[], char guess[]);
int changeSettings(void);
int getInput(int c_fl);
char* getGuess();
void donothing();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  delay(200);
  char answer;
  menu();
  do_nothing();  // do nothing if no input
  
  if (Serial.available() > 0) {
    answer = char(getInput(1)); // get character input from user
    Serial.println(answer);
    
    if (answer == QUITGAME)
      {
        /* quit game */
        Serial.print("\nQuitting game...");
        donothing();
      }
      else if (answer == SETTINGS)
      {
       /* go to settings */
        changeSettings();
      }
      else if (answer == NEWGAME)
      {
        /* start new game */
        startGame();
      }
      else
      {
        Serial.print("Not a valid option.\n");
      }
  }
}

// print main menu
char menu(void)
{
    /* print main menu */
    /* return option chosen */
    Serial.print("\n*************Binary Mastermind*************\n");
    Serial.print("Welcome to binary mastermind!\n");
    Serial.print("*************Main Menu*************\n");
    Serial.print("\tNew Game: ");
    Serial.print(NEWGAME);
    Serial.print("\n\tSettings: ");
    Serial.print(SETTINGS);
    Serial.print("\n\tQuit Game: ");
    Serial.print(QUITGAME);
    Serial.print("\nEnter your choice: ");
}

// start game
int startGame(void)
{
  Serial.read(); // read the enter line 
  char answer[num_digits];  // answer to game
  int guesses_left = num_guesses; // keep track of guesses left
  int result = 1; // result is 0 if win, 1 if lose/quit
  char* guess; // user's guess
  
  /* generate random binary number based on number of digits given */
  Serial.print("Enter a random number(less than ");
  Serial.print(MAX);
  Serial.print(" digits): ");
  while (!(Serial.available()>0)) {delay(200);}  // do nothing if no input
  int seed = getInput(0); // get seed from user
  Serial.println(seed);
  srand(seed);
  
  for (int i=0; i < num_digits; i+=1)
  {
    //num = rand() % 2;
    answer[i] = rand() % 2 + '0'; // char[i] = 0 if random # is even, 1 if odd
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
    Serial.print(" guesses left) or 'q' to quit: ");
    while (!(Serial.available()>0)) {delay(200);}  // do nothing if no input
    
    getGuess();   // guess in user_guess
    Serial.println(user_guess);
    if (user_guess[0] == QUITGAME) {
      Serial.print("Quitting game...\n");
      return 1;
    }
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
int compare(char answer[], char guess[])
{
  int num_right = 0;
  /* compare answer and guess, return comparison */
  /* and print result */
  Serial.print("Total number of digits correct in correct place: ");
  for (int i=0; i < num_digits; i+=1)
  {
    if (guess[i]==answer[i])
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
    num_entered = getInput(0);  // get number
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

      guesses = getInput(0);  // get # of guesses
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

      digits = getInput(0); // get # of digits
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
int getInput(int c_fl) {
  // add a bit of delay
  char answer[MAX];
  int i = 0;  // keep track of num characters entered
  int byteReceived; 
  while (Serial.available() > 0) {
    byteReceived = Serial.read();
    answer[i] = char(byteReceived);
    if (c_fl) {return byteReceived;}
    i++;
    if ((i > MAX) | (i == MAX)) {
      while(Serial.available() > 0) {Serial.read();}
      return atoi(answer);
    }
    if (byteReceived == 10) {
      return atoi(answer);  // return integer value input
    }
  }
}   
// !! must read serial input after every call to eliminate input from enter key !!
// get string input from user (for guessing game)
char* getGuess() {
  int i = 0;  // keep track of num characters entered
  int byteReceived;

  while (Serial.available() > 0) {
    byteReceived = Serial.read();
    // check if user entered more than the number of digits
    if ((i > num_digits) | (i == num_digits)) {
      char *rtn_answer = user_guess;  // answer to return
      while(Serial.available()>0) {Serial.read();};  // flush any character in the buffer
      return rtn_answer;
    }
    user_guess[i] = char(byteReceived);
    i++;

    // check if user entered end of line
    if (byteReceived == 10) {
      if (i < num_digits) {
        for (int j=i-1;j<num_digits; j++) {
          user_guess[j] = NULL;
          // fill the rest of user_guess with NULL if user enters less than number of digits required
          // start at i-1 to remove newline character
        }
      }
      char *rtn_answer = user_guess;
      return rtn_answer;
    }
  }
}
  
//
void do_nothing() {
  while(!Serial.available()){delay(10);
}

// Add switch functions 
