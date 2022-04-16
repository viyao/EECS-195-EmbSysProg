int bump_switch_0 = A15;  // pin definitions for bumper switches; currently everything is following the schematic
int bump_switch_1 = A14;  // bump_switch_0 is for counting up; bump_switch_1 is for counting down; and bump_switch_2 is for the 'enter', also known as the enter key on your keyboard;
int bump_switch_2 = A13;

int led1 = P2_0;    // the red led in the tri-led
int led2 = P2_1;    // the green led in the tri-led
int led3 = P2_2;    // the blue led in the tri-led
int led4 = P1_0;    // the individual red led

int photo_in_0 = A11;
int photo_in_1 = A9;
int photo_in_2 = A8;
int photo_in_3 = A6;
int photo_in_4 = A1;
int photo_in_5 = A0;
int photo_in_6 = A12;
int photo_in_7 = A10;
int photo_in_8 = A7;
int photo_in_9 = A4;

int bump_switch_0_value = 0;  // intializations for bump_switch values;
int bump_switch_1_value = 0;
int bump_switch_2_value = 0;

int photo_value_0 = 0;        // initializaitons for photoresistor values;
int photo_value_1 = 0;
int photo_value_2 = 0;
int photo_value_3 = 0;
int photo_value_4 = 0;
int photo_value_5 = 0;
int photo_value_6 = 0;
int photo_value_7 = 0;
int photo_value_8 = 0;
int photo_value_9 = 0;

int menu_option = 1; // initialization with menu_option being the first option;
char *user_str = NULL;  // defining a pointer that takes user input;

int photo_ref_0 = 0;        // initializations for photoresistor reference values;
int photo_ref_1 = 0;
int photo_ref_2 = 0;
int photo_ref_3 = 0;
int photo_ref_4 = 0;
int photo_ref_5 = 0;
int photo_ref_6 = 0;
int photo_ref_7 = 0;
int photo_ref_8 = 0;
int photo_ref_9 = 0;

int offset = 150;         // the photoresistor offset value

int current_input_pointer = 0;  // for recording of the current pointer location
char enter = '\n';        // the enter key on your keyboard;
int default_length = 1024;  // maximum input string length is 1KB

int test_int = 0;         // for testing converting the user input string into an integer and add with an integer to see if output is correct;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(bump_switch_0, INPUT);  // pinmode definitions for all pins used
  pinMode(bump_switch_1, INPUT);
  pinMode(bump_switch_2, INPUT);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
  pinMode(photo_in_0, INPUT);
  pinMode(photo_in_1, INPUT);
  pinMode(photo_in_2, INPUT);
  pinMode(photo_in_3, INPUT);
  pinMode(photo_in_4, INPUT);
  pinMode(photo_in_5, INPUT);
  pinMode(photo_in_6, INPUT);
  pinMode(photo_in_7, INPUT);
  pinMode(photo_in_8, INPUT);
  pinMode(photo_in_9, INPUT);
  
  user_str = (char *) malloc(default_length * sizeof(char));  // allocating memory spaces for the user_str pointer
  
  photo_ref_0 = analogRead(photo_in_0) + offset;  // these are necessary, each photoresistor's threshold value must be determined individually;
  delay(10);
  photo_ref_1 = analogRead(photo_in_1) + offset;
  delay(10);
  photo_ref_2 = analogRead(photo_in_2) + offset;
  delay(10);
  photo_ref_3 = analogRead(photo_in_3) + offset;
  delay(10);
  photo_ref_4 = analogRead(photo_in_4) + offset;
  delay(10);
  photo_ref_5 = analogRead(photo_in_5) + offset;
  delay(10);
  photo_ref_6 = analogRead(photo_in_6) + offset;
  delay(10);
  photo_ref_7 = analogRead(photo_in_7) + offset;
  delay(10);
  photo_ref_8 = analogRead(photo_in_8) + offset;
  delay(10);
  photo_ref_9 = analogRead(photo_in_9) + offset;
  delay(10);
  
  Serial.begin(9600); 
}

void loop() {
  Serial.println("The program starts...");
  while (true) {
    new_scan();   // function call
//    Serial.print("Current user_str is ");     // these were for previous testing purposes;
//    Serial.println(user_str);
//    test_int = atoi(user_str) + 10;           // for testing converting the user_str into integer and see if the conversion was successful;
//    Serial.print("Current int verification is ");
//    Serial.println(test_int);
    delay(1000);
    if ((menu_option == 4) & (*(user_str + current_input_pointer - 1) == enter)) {break;} // this is for exiting the program;
  }
  Serial.println("Exiting...");
  free(user_str);
}

//void square_wave(int pin_num, int frequency, float duty_ratio) {  //  This was created for fun, ignore this part
//  digitalWrite(pin_num, HIGH);
//  delay(1/frequency * duty_ratio * 1000);
//  digitalWrite(pin_num, LOW);
//  delay(1/frequency * duty_ratio * 1000);
//}

void up_down_enter(int bump_switch_num, int bump_switch_value) {    // this is for menu_selection using the bumper_switches
  
  bump_switch_value = analogRead(bump_switch_num);      // read bump_switch_value;
  
  if ((bump_switch_num == bump_switch_0) & (bump_switch_value == 1023)){    // this is for count_up;
    (menu_option >= 4) ? menu_option = 4: menu_option++;
  }
  else if ((bump_switch_num == bump_switch_1) & (bump_switch_value == 1023)){   // this is for count_down;
    (menu_option <= 1) ? menu_option = 1: menu_option--;
  }
  else if ((bump_switch_num == bump_switch_2) & (bump_switch_value == 1023)){   // this is for the enter key on your keyboard;
    *(user_str + current_input_pointer) = enter;
    current_input_pointer++;
  }
}

void photo_value(int photo_in_num, int photo_value_num) {   // this is for reading photoresistor value and convert them into string of number characters
  photo_value_num = analogRead(photo_in_num);
  delay(10);
  if ((photo_in_num == photo_in_0) & (photo_value_num > photo_ref_0)) {
    *(user_str + current_input_pointer) = '0';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_1) & (photo_value_num > photo_ref_1)) {
    *(user_str + current_input_pointer) = '1';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_2) & (photo_value_num > photo_ref_2)) {
    *(user_str + current_input_pointer) = '2';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_3) & (photo_value_num > photo_ref_3)) {
    *(user_str + current_input_pointer) = '3';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_4) & (photo_value_num > photo_ref_4)) {
    *(user_str + current_input_pointer) = '4';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_5) & (photo_value_num > photo_ref_5)) {
    *(user_str + current_input_pointer) = '5';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_6) & (photo_value_num > photo_ref_6)) {
    *(user_str + current_input_pointer) = '6';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_7) & (photo_value_num > photo_ref_7)) {
    *(user_str + current_input_pointer) = '7';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_8) & (photo_value_num > photo_ref_8)) {
    *(user_str + current_input_pointer) = '8';
    current_input_pointer++;
  }
  else if ((photo_in_num == photo_in_9) & (photo_value_num > photo_ref_9)) {
    *(user_str + current_input_pointer) = '9';
    current_input_pointer++;
  }
}

void new_scan() {   // the new scan function
  Serial.println("The new_scan() function being called");
  for (int i = 0; i < current_input_pointer; i++) {   // initializing the pointer everytime this function is called; 
    *(user_str + i) = ' ';
  }
  current_input_pointer = 0;    // initializing the pointer location offset
  
  while (*(user_str + current_input_pointer - 1) != enter) {    // this loop exits when the last pointer is 'enter', also known as the enter key on your keyboard;
    
    Serial.println("I'm in loop");      // these are for testing purposes only
    Serial.println(user_str);
    Serial.print("The menu_option is ");
    Serial.println(menu_option);
    delay(1000);
    
    photo_value(photo_in_0, photo_value_0);
    photo_value(photo_in_1, photo_value_1);
    photo_value(photo_in_2, photo_value_2);
    photo_value(photo_in_3, photo_value_3);
    photo_value(photo_in_4, photo_value_4);
    photo_value(photo_in_5, photo_value_5);
    photo_value(photo_in_6, photo_value_6);
    photo_value(photo_in_7, photo_value_7);
    photo_value(photo_in_8, photo_value_8);
    photo_value(photo_in_9, photo_value_9);
    
    up_down_enter(bump_switch_0, bump_switch_0_value);
    up_down_enter(bump_switch_1, bump_switch_1_value);
    up_down_enter(bump_switch_2, bump_switch_2_value);
    
    show_menu();
  }
}

void show_menu() {    // this show menu shows the menu on the board by lighting differnt LED.
  if (menu_option == 1) {
//      digitalWrite(led2, LOW);    // these were just some garbage codes that works fine too, unnecessary though;
//      digitalWrite(led3, LOW);
//      digitalWrite(led4, LOW);
//      delay(10);
      turn_off_led();
      digitalWrite(led1, HIGH);
    }
    else if (menu_option == 2) {
//      digitalWrite(led1, LOW);
//      digitalWrite(led3, LOW);
//      digitalWrite(led4, LOW);
//      delay(10);
      turn_off_led();
      digitalWrite(led2, HIGH);
    }
    else if (menu_option == 3) {
//      digitalWrite(led2, LOW);
//      digitalWrite(led1, LOW);
//      digitalWrite(led4, LOW);
//      delay(10);
      turn_off_led();
      digitalWrite(led3, HIGH);
    }
    else if (menu_option == 4) {
//      digitalWrite(led2, LOW);
//      digitalWrite(led3, LOW);
//      digitalWrite(led1, LOW);
//      delay(10);
      turn_off_led();
      digitalWrite(led4, HIGH);
    }  
}

void turn_off_led() {     // as it looks like, this function turns off all LEDs
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
}
