#include <LiquidCrystal_I2C.h>

const int buzzerPin = 3; 
const int ledPin = 5; 
const int doorPin = 4;
const int movementPin = 2;
const int activationTime = 10000; // 10 seconds
const int activationDuration = 5000; // 5 seconds
const int lcdColumns = 16;
const int lcdRows = 2;

// set up the LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int doorState; // 0 = switch closed, 1 = switch open
int movementState = 0;
int alarmState = 0;
unsigned long switchOpenTime = 0;
unsigned long activationStartTime = 0;

void setup()
{
  pinMode(movementPin, INPUT);  // Pin 2 as INPUT
  pinMode(buzzerPin, OUTPUT); // Pin 3 as OUTPUT for Buzzer
  pinMode(ledPin, OUTPUT);    // Pin 5 as OUTPUT for LED
  pinMode(doorPin, INPUT_PULLUP)

  // start the serial communication
  Serial.begin(9600);

  // initialise the LCD
  lcd.init();
  lcd.backlight();

  // display the startup message
  lcd.setCursor(0, 0);
  lcd.print("Alarm System");
  lcd.setCursor(0, 1);
  lcd.print("Initialising");
  delay(2000);
 
  }

void loop()
{
  // Check if the sensor switch is open
  doorState = digitalRead(movementPin);

  if (doorState == HIGH)
  {
    if (switchOpenTime == 0)
    {
      switchOpenTime = millis(); // Record the time the switch is first opened
      lcd.setCursor(0, 1);
      lcd.print("Door Open: 0s    ");
    }
    if (millis() - switchOpenTime >= activationTime)
    {
      if (activationStartTime == 0)
      {
        activationStartTime = millis(); // Record the time activation starts
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(ledPin, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("System: ON  ");
      }
      if (millis() - activationStartTime >= activationDuration)
      {
        digitalWrite(buzzerPin, LOW);
        digitalWrite(ledPin, LOW);
        switchOpenTime = 0;       // Reset the switch open time
        activationStartTime = 0; // Reset the activation start time
        lcd.setCursor(0, 1);
        lcd.print("System: OFF ");
      }
    }
    else
    {
      lcd.setCursor(12, 1);
      int remainingTime = (activationTime - (millis() - switchOpenTime)) / 1000;
      lcd.print(remainingTime);
      lcd.print("s    ");
    }

    lcd.setCursor(0, 0);
    lcd.print("Door: Open       ");
  }
  else
  {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    switchOpenTime = 0;           // Reset the switch open time if it's closed
    activationStartTime = 0;     // Reset the activation start time if it's closed
    lcd.setCursor(0, 1);
    lcd.print("Door: Closed     ");
    lcd.setCursor(0, 0);
    lcd.print("                 "); // Clear the previous "Door: Open" message
  }

  // Check if the button is pressed
  if (digitalRead(movementPin) == HIGH)
  {
    digitalWrite(buzzerPin, HIGH);  // Turn the Buzzer ON
    digitalWrite(ledPin, HIGH);     // Turn the LED ON
    delay(5000);                 // Wait for 5 seconds
    digitalWrite(buzzerPin, LOW);   // Turn the Buzzer OFF
    digitalWrite(ledPin, LOW);      // Turn the LED OFF
  }

  delay(200);
}
