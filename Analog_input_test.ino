int bump_switch_0 = A15;
int bump_switch_1 = A14;
int bump_switch_2 = A13;

int bump_switch_0_value = 0;
int bump_switch_1_value = 0;
int bump_switch_2_value = 0;

int menu_option = -1; // initialization with invalid input
char *user_str = NULL;  // defining a pointer that takes user input;

int current_input_pointer = 0;
char enter = '\n';
int default_length = 1024;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(bump_switch_0, INPUT);
  pinMode(bump_switch_1, INPUT);
  pinMode(bump_switch_2, INPUT);
  user_str = (char *) malloc(default_length * sizeof(char));
  Serial.begin(9600); 
}

void loop() {
  bump_switch_0_value = analogRead(bump_switch_0);
  delay(10);
  bump_switch_0_value = analogRead(bump_switch_0);
  delay(10);
  bump_switch_1_value = analogRead(bump_switch_1);
  delay(10);
  bump_switch_1_value = analogRead(bump_switch_1);
  delay(10);
  bump_switch_2_value = analogRead(bump_switch_2);
  delay(10);
  bump_switch_2_value = analogRead(bump_switch_2);
  delay(10);
  Serial.print("This is bumper 0 value:");
  Serial.println(bump_switch_0_value);
  Serial.print("This is bumper 1 value:");
  Serial.println(bump_switch_1_value);
  Serial.print("This is bumper 2 value:");
  Serial.println(bump_switch_2_value);
  delay(1000);
}

void square_wave(int pin_num, int frequency, float duty_ratio) {
  digitalWrite(pin_num, HIGH);
  delay(1/frequency * duty_ratio * 1000);
  digitalWrite(pin_num, LOW);
  delay(1/frequency * duty_ratio * 1000);
}

int up_down_enter(int switch_value, int switch_number) {
  if ((switch_value == 1023) & (switch_number == A15)){
    menu_option = (menu_option >= 4) ? menu_option == 4: menu_option++;
  }
  if ((switch_value == 1023) & (switch_number == A14)){
    menu_option = (menu_option <= 0) ? menu_option == 0: menu_option--;
  }
  if ((switch_value == 1023) & (switch_number == A13)){
    *(user_str + current_input_pointer) = enter;
    current_input_pointer++;
  }
}
