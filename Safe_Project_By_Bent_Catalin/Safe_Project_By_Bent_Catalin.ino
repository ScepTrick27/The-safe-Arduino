/**
 * Name: Safe Assignment
 * Author: Bent Cătălin
 */
#include "Display.h"

const int PIN_KEY1 = 8;
const int PIN_KEY2 = 9;
const int PIN_LDR = 16;
const int PIN_BUZZER = 3;
const int RED_LED = 4;
const int GREEN_LED = 5;
const int YELLOW_LED = 7;

int buttonState1 =  0;
int lastButtonState1 = 0;

int buttonState2 = 0;
int lastButtonState2 = 0;

//The values for the hysterisis
const int TRESHOLD_HIGH = 600;
const int TRESHOLD_LOW = 400;

//The values for the state of the safe
String door;
String safe = "locked";
int lastSafeState = 0;

//The password for opening the safe
const char *password = "1234";

//With these intergers we control the numbers on the display
//By changig them with the press of the buttons
int count;
int buttonCount = 0;
int pos = 0;
int number;

//The strings for saving the numbers of the password
String num1;
String num2;
String num3;
String num4;
String inputPassword;

//The i helps with sounding the alarm for only 5 seconds
int i = 0;

//The attempts made to put in the right password
int attempts = 0;

void setup() {
  // put your setup code here, to run once:
  Display.on();
  Display.clear();

  pinMode(PIN_KEY1, INPUT_PULLUP);
  pinMode(PIN_KEY2, INPUT_PULLUP);

  pinMode (PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  pinMode(PIN_LDR, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  Serial.begin(9600);

}

//What this void does is that it changes the numbers on the display,
//Saves the values that we chose and verifies if we put in the right password
//It was also done to make it easier to read the void loop 
void passwordInput()
{
  buttonState1 = digitalRead(PIN_KEY1);
  buttonState2 = digitalRead(PIN_KEY2);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      delay(80);
      buttonState1 = digitalRead(PIN_KEY1);

      if (buttonState1 == LOW) {
        tone (PIN_BUZZER, 400, 30);
        count++;
        pos++;
        buttonCount = 0;
      }
    }
  }
  lastButtonState1 = buttonState1;

  if ( count == 0)
  {
    num1 = String(number);
  }
  else if ( count == 1)
  {
    num2 = String(number);
  }
  else if (count == 2)
  {
    num3 = String(number);
  }
  else if (count == 3)
  {
    num4 = String(number);
  }
  else if ( count == 4)
  {
    inputPassword = num1 + num2 + num3 + num4;
    if (inputPassword == password)
    {
      tone(PIN_BUZZER, 800, 200);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      safe = "unlocked";
      Display.clear();
      digitalWrite(YELLOW_LED, HIGH);
      delay(300);
      digitalWrite(YELLOW_LED, LOW);

    }
    else
    {
      attempts++;
      safe = "locked";
      Display.clear();
      pos = 0;
      count = 0;
      buttonCount = 0;
    }
  }


  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      delay(80);
      buttonState2 = digitalRead(PIN_KEY2);
      if (buttonState2 == LOW) {
        buttonCount++;
        tone(PIN_BUZZER, 600, 50);
      }

    }
  }
  lastButtonState2 = buttonState2;

  if (buttonCount % 7 == 1)
  {
    number = 1;
  }
  else if (buttonCount % 7 == 2)
  {
    number = 2;
  }
  else if (buttonCount % 7 == 3)
  {
    number = 3;
  }
  else if (buttonCount % 7 == 4)
  {
    number = 4;
  }
  else if (buttonCount % 7 == 5)
  {
    number = 5;
  }
  else if (buttonCount % 7 == 6)
  {
    buttonCount = 1;
  }
  if (buttonCount % 7 == 0)
  {
    Display.showCharAt(pos, '-');
  }
  else
  {
    Display.showCharAt(pos, '0' + number);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  int val = analogRead(PIN_LDR);

//Here we do the hysterisis for the LDR
  if (val < TRESHOLD_LOW)
  {
    door = "closed";
    i = 0;
  }
  if (val > TRESHOLD_HIGH)
  {
    door = "open";
  }

//If the state is locked and the door is closed,
//We call the method and start writing the password
  if (safe == "locked" && door == "closed")
  {
    passwordInput();
  }

//This if statement enables us to check if the safe is broken in
//But it also enables us to lock the safe after unlocking it
  if (door == "open")
  {
    if (safe == "locked")
    {
      buttonCount = 0;
      pos = 0;
      count = 0;
      Display.clear();
      while ( i <= 4)
      {
        digitalWrite(RED_LED, HIGH);
        delay(400);
        digitalWrite(RED_LED, LOW);
        delay(400);
        tone(PIN_BUZZER, 440, 1000);
        delay(999);
        i++;
      }
    }
    else
    {
      lastSafeState = 1;
    }
  }
  else
  {
    if (lastSafeState == 1)
    {
      tone(PIN_BUZZER, 800, 200);
      buttonCount = 0;
      i = 0;
      pos = 0;
      safe = "locked";
      lastSafeState = 0;
      count = 0;
      Display.clear();
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      delay(300);
      digitalWrite(YELLOW_LED, LOW);
    }
  }

  //This if statement checks the number of incorrect attempts made at 
  //opening the safe, and if they are equal to three, we sound the alarm
  if (attempts == 3)
  {
     while ( i <= 4)
      {
        Serial.println("ALARM SAFE #");
        Display.clear();
        digitalWrite(RED_LED, HIGH);
        delay(400);
        digitalWrite(RED_LED, LOW);
        delay(400);
        tone(PIN_BUZZER, 440, 1000);
        delay(999);
        i++;
      }
      delay(5000);
      attempts = 0;
  }

}
