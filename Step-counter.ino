#include <M5Core2.h>
float accX = 0.0F; 
float accY = 0.0F;
float accZ = 0.0F;
float moving_time = 0.000F;
int steps = 0;
int gender = 0; 
int age_group = 0;
int steps_required = 0;
int steps_left = 0;
float acceleration_modulus = 0.0; 
int program_state = 1; 
bool steps_achieved = false;

void setup() {
  Serial.begin(57600, SERIAL_8N1, 13, 14);
  M5.begin(); 
  M5.IMU.Init();  
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK); 
}


void show_buttons() {
  M5.Lcd.setTextSize(2);
  show_button_A();
  show_button_B();
  show_button_C();
}

void clear_button_A() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(50, 220);
  M5.Lcd.println(" ");
}

void clear_button_B() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(160, 220);
  M5.Lcd.println(" ");
}

void clear_button_C() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(270, 220);
  M5.Lcd.println(" ");
}

void show_button_A() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(50, 220);
  M5.Lcd.println("A");
}

void show_button_B() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(160, 220);
  M5.Lcd.println("B");
}

void show_button_C() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(270, 220);
  M5.Lcd.println("C");
}
void write(String message) {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(100, 45); 
  M5.Lcd.println(message); 
}

void loop() {
  M5.update(); 


  switch (program_state) {
    case 1:
      steps = 0;
      steps_left = 0;
      moving_time = 0.000F;
      steps_achieved = false;
      write("Welcome          \n    Let's count your steps\n    Press B to continue.                                                                                                        ");
      clear_button_A();
      show_button_B();
      clear_button_C();

      if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
        program_state = 2;
      }
      break;
    case 2:
      write("Choose your gender                          \n    A - Male.                                       \n    C - Female                               ");
      show_button_A();
      clear_button_B();
      show_button_C();
      if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
        gender = 0;
        program_state = 3;
      }
      if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
        gender = 1;
        program_state = 3;
      }
      break;
    case 3:
      write("Choose your age      \n       Press for long                             \n    A - 6 to 11 years                                  \n    B - 12 to 19 years     \n    C - 20 to 65 years     ");
      show_button_A();
      show_button_B();
      show_button_C();
      
      if (M5.BtnA.wasReleasefor(500) || M5.BtnA.pressedFor(1000, 200)) {
        age_group = 0;
        program_state = 4;
      }
      if (M5.BtnB.wasReleasefor(500) || M5.BtnB.pressedFor(1000, 200)) {
        age_group = 1;
        program_state = 4;
      }
      if (M5.BtnC.wasReleasefor(500) || M5.BtnC.pressedFor(1000, 200)) {
        age_group = 2;
        program_state = 4;
      }
      break;
    case 4:
      if(steps_achieved){
        show_button_B();
      }else{
        clear_button_B(); 
      }
      clear_button_A();
      show_button_C(); 
      
      M5.IMU.getAccelData(&accX, &accY, &accZ);
      acceleration_modulus = abs(sqrt((accX * accX * 100) + (accY * accY * 100) + (accZ * accZ * 100)) - 10);
      
      
      if (acceleration_modulus > 0.3) { 
        moving_time += 0.223;
        Serial.print(moving_time);
      }
      steps = (1.4611 * moving_time) - 0.4559;
      Serial.println(moving_time);
      M5.Lcd.setCursor(100, 45);
      
      if(age_group == 0 && gender == 0){
        steps_required = 14000;
      }else if(age_group == 0 && gender == 1){
        steps_required = 11500;
      }else if(age_group == 1){
        steps_required = 10850;
      }else if(age_group == 2){
        steps_required = 8500;
      }

      steps_left = steps_required - steps;
      if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
          steps = 0;
          steps_left = steps_required;
          moving_time = 0.000F;
      }

      if(steps_left >= 0){
        M5.Lcd.printf("%d steps complete                 \n     %d steps remaining  \n                                       \n        C to reset.                                                                                                                                                                                                             ", steps,steps_left);
      }else{
          steps_achieved = true;
          M5.Lcd.printf("Congrats \n    You have done your exercise for now.                                \n     Press B to restart                                                                                                                                                                                                                              ");
          if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
          program_state = 1;
        }
      }      
      break;
    default:
      write("");
      break;
  }
}
