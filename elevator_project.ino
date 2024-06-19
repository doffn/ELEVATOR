              /***********************
               * Elevator Simulation *
               ***********************
 * Functionality:     						   *                         *
 * - Controls a simulated elevator system.     *
 * - Monitors button presses, floor sensors,   *
 *	and controls movement and door operations. *
 ***********************************************/

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CALL_BUTTONS A0
#define CABIN_BUTTONS A2
#define FLOOR_SENSORS A1
#define N 4 // Number of floors
#define STOP_BUTTON 2  // USED TO STOP THE CALIBRATION
#define POWER_PIN 13
#define GROUND_PIN 12

// Specifying the type of Electronics we use
LiquidCrystal_I2C lcd(32, 16, 2);
Servo servo;

// Defining the Led value
short ledFloor[N] = {3, 4, 5, 6};
// Defining the calls in each floor
short buttonEvent[N] = {0, 0, 0, 0}; // 0: None, 1: Up, -1: Down
// Defining the variables
short registeredFloor = 4; // define the level of the lift

// Defines if the lift is going up or down 1 UP and -1 Down
short movement = 1;

void setup() {
  Serial.begin(9600);
  pinMode(CALL_BUTTONS, INPUT);
  pinMode(FLOOR_SENSORS, INPUT);
  pinMode(STOP_BUTTON, INPUT);
  pinMode(POWER_PIN, OUTPUT);
  pinMode(GROUND_PIN, OUTPUT);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  servo.attach(11);

  for (byte i = 0; i < N; i++)
    pinMode(ledFloor[i], OUTPUT);

  calibration();
  ledOn();
}

void loop() {
  // Detecting the button event and determining movement direction
  for (int ind = 0; ind < sizeof(buttonEvent) / sizeof(buttonEvent[0]); ind++) {
    floorReg();
    if (buttonEvent[ind] == 1) {
      movement = 1;
      break;
    } else if (buttonEvent[ind] == -1) {
      movement = -1;
      break;
    } else
      movement = 0;
  }
  registerFloor();
  Serial.println(registeredFloor);
  printFloor();
  mainLift();
  floorReg();
}

// Function to print the state of buttonEvent array
void printButtonEvents() {
  for (int j = sizeof(buttonEvent) / sizeof(buttonEvent[0]) - 1; j >= 0; j--) {
    Serial.print("Element ");
    Serial.print(j);
    Serial.print(": ");
    Serial.println(buttonEvent[j]);
  }
}

// Function to register floor based on button presses
void floorReg() {
  int value = analogRead(CALL_BUTTONS);
  int cabin = analogRead(CABIN_BUTTONS);
  int level = 0;

  if (value <= 100 || cabin <= 150) {
    level = 1;
  } else if ((300 <= value && value <= 550) || (450 < cabin && cabin < 600)) {
    level = 2;
  } else if ((550 <= value && value <= 700) || (650 < cabin && cabin < 750)) {
    level = 3;
  } else if ((720 <= value && value <= 800) || (780 < cabin && cabin < 900)) {
    level = 4;
  } else {
    level = 0;
  }
  //Serial.print("pressed : ");
  //Serial.print(level);
  //Serial.print(level);
  //Serial.println("");   
  if (level != 0) {
    if (registeredFloor >= level) {
      buttonEvent[level - 1] = -1;
    } else if (registeredFloor < level) {
      buttonEvent[level - 1] = 1;
    }
  }
}

// Function to register the current floor
short registerFloor() {
  unsigned short value = analogRead(FLOOR_SENSORS);
  short bufRegisteredFloor = 0;
  if (value <= 350) bufRegisteredFloor = 1;
  else if (value >= 500 && value <= 700) bufRegisteredFloor = 2;
  else if (value >= 700 && value <= 750) bufRegisteredFloor = 3;
  else if (value >= 750 && value <= 900) bufRegisteredFloor = 4;
  Serial.println(bufRegisteredFloor);
    return bufRegisteredFloor;
}

// Calibration process for the lift
void calibration() {
  lcd.setCursor(0, 0);
  lcd.print("  CALIBRATION...");

  while (!digitalRead(STOP_BUTTON)) {
    if (registeredFloor == 1)
      break;
    printFloor();
    digitalWrite(ledFloor[registeredFloor - 1], HIGH);
    delay(500);
    digitalWrite(ledFloor[registeredFloor - 1], LOW);
    delay(500);
    registeredFloor = registeredFloor - 1;
  }

  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("LIFT CALIBRATED");
  delay(500);
  lcd.clear();
}

// Function to move the lift up
void movingUp() {
  unsigned long startTime = millis();
  const unsigned long movementTime = 1000; // 1 second in milliseconds

  printFloor();
  digitalWrite(ledFloor[registeredFloor - 1], HIGH);

  while (millis() - startTime < movementTime) {
    digitalWrite(POWER_PIN, HIGH);
    digitalWrite(GROUND_PIN, LOW);
    floorReg();
  }

  digitalWrite(ledFloor[registeredFloor - 1], LOW);
  registeredFloor = registeredFloor + 1;
}

// Function to move the lift down
void movingDown() {
  unsigned long startTime = millis();
  const unsigned long movementTime = 1000; // 1 second in milliseconds

  printFloor();
  digitalWrite(ledFloor[registeredFloor - 1], HIGH);

  while (millis() - startTime < movementTime) {
    floorReg();
    digitalWrite(POWER_PIN, LOW);
    digitalWrite(GROUND_PIN, HIGH);
  }

  digitalWrite(ledFloor[registeredFloor - 1], LOW);
  registeredFloor = registeredFloor - 1;
}

// Function to control the door opening and closing
void door() {
  digitalWrite(POWER_PIN, LOW);
  digitalWrite(GROUND_PIN, LOW);
  servo.write(90); 
  unsigned long startTime = millis();
  const unsigned long doorDuration = 5000; // 5 seconds in milliseconds

  lcd.setCursor(0, 0);
  lcd.print("Door Opening");

  while (millis() - startTime < doorDuration / 2) {
    floorReg();
  }

  lcd.setCursor(0, 0);
  lcd.print("Door Closing");

  while (millis() - startTime < doorDuration) {
    floorReg();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("<{}>");
  servo.write(0); 
}

// Function to turn on the LED indicating current floor
void ledOn() {
  digitalWrite(ledFloor[registeredFloor - 1], HIGH);
}

// Function to print current floor on LCD
void printFloor() {
  lcd.setCursor(0, 1);
  lcd.print("FLOOR: ");
  lcd.print(registeredFloor-1);
}

// Main function controlling lift movement
void mainLift() {
  if (movement == 1) { // Going up
    for (int index = 0; index < sizeof(buttonEvent) / sizeof(buttonEvent[0]); index++) {
      floorReg();
      printButtonEvents();
      if (buttonEvent[registeredFloor - 1] == -1) {
        door();
        buttonEvent[registeredFloor - 1] = 0;
        break;
      }
      if (buttonEvent[index] == 1) {
        while (registeredFloor <= index) {
          floorReg();
          movingUp();
          printFloor();
          ledOn();

          // Check for new events in path while going up
          for (int i = registeredFloor - 1; i >= 0; i--) {
            if (buttonEvent[i] == 1) {
              buttonEvent[i] = 0; // Clear the event
              door();             // Open the door
              break;              // Exit the inner loop
            }
          }
        }
        buttonEvent[registeredFloor - 1] = 0;
      }
    }
  } else if (movement == -1) { // Going down
    for (int j = sizeof(buttonEvent) / sizeof(buttonEvent[0]); j > 0; j--) {
      if (buttonEvent[registeredFloor - 1] == -1) {
        door();
        buttonEvent[registeredFloor - 1] = 0;
        break;
      }
      if (buttonEvent[j - 1] == -1) {
        while (registeredFloor > j) {
          movingDown();
          printFloor();
          ledOn();

          // Check for new events in path while going down
          for (int i = registeredFloor; i < sizeof(buttonEvent) / sizeof(buttonEvent[0]); i++) {
            if (buttonEvent[i] == -1) {
              buttonEvent[i] = 0; // Clear the event
              break;              // Exit the inner loop
            }
          }
        }
        buttonEvent[registeredFloor - 1] = 0;
        door();
      }
    }
  } else {
    Serial.println("Nothing to do");
  }
}
