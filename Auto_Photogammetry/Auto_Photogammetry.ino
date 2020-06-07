#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Stepper.h>
#include <NikonRemote.h>

NikonRemote remote( 2 );

const unsigned int steprev = 2048;
Stepper stepper = Stepper(steprev, 3, 5, 4, 6);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
char keys[4][4] = {
    {'D','#','0','*'},
    {'C','9','8','7'},
    {'B','6','5','4'},
    {'A','3','2','1'}
  };
byte rowPins[4] = {A3, A2, A1, A0};
byte colPins[4] = {8, 9, 10, 11};
char key;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

unsigned int x, temp, layers, rounds, pics_per_round, image_step, pics = 0;
unsigned int i, j, k;

void clearLCDlower()
{
   lcd.setCursor(0,1);
   lcd.print("                ");
   lcd.setCursor(0,1);
}

int keypadInput(int over)
{
  x = 0;
  temp = 0;
  while(true)
  {
    key = keypad.getKey();
    if(key)
    {
      if(key == '*')
      {
        return x;
      }
      else if(key == '#')
      {
        x = 0;
        clearLCDlower();
      }
      else
      {
        temp = key - '0';
        x = x * 10;
        x = x + temp;
        //Serial.println(x);
        clearLCDlower();
        lcd.print(x);
      }
    }
    if(x > over)
    {
       clearLCDlower();
       lcd.print("Overflow");
       delay(500);
       clearLCDlower();
       x = 0;
       temp = 0;
    }
  }
}

void click1()
{
  remote.click();
  delay(5000);
}

void updateLCD()
{
  lcd.setCursor(0,0);
  lcd.print("Layer:");
  lcd.print(i + 1);
  lcd.print(" Round:");
  lcd.print(j + 1);
  lcd.setCursor(0,1);
  lcd.print("Pics Taken: ");
  lcd.print(pics);
}

void setup() 
{
  stepper.setSpeed(1);
  digitalWrite(7, HIGH);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Layers(max 10):");
  clearLCDlower();
  layers = keypadInput(10);
  Serial.println(layers);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pics(1~100):");
  clearLCDlower();
  pics_per_round = keypadInput(100);
  Serial.println(pics_per_round);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rounds(1~10):");
  clearLCDlower();
  rounds = keypadInput(10);
  Serial.println(pics_per_round);
  image_step = round(steprev / pics_per_round);
  Serial.println(image_step);
}

void loop() 
{
  lcd.clear();
  for(i = 0; i < layers; i++)
  {
    for(j = 0; j < rounds; j++)
    {
      for(k = 0; k < pics_per_round; k++)
      {
        stepper.step(image_step);
        delay(100);
        click1();
        pics++;
        updateLCD();
        key = keypad.getKey();
        if(key == '#')
        {
          lcd.setCursor(0,0);
          lcd.clear();
          lcd.print("Confirm stop: ");
          lcd.setCursor(0, 1);
          while(1)
          {
             key = keypad.getKey();
             if(key)
             {
               if(key == '#')
                 digitalWrite(7, LOW);
               if(key == '*')
                 break;
             }
          }
        } 
      }
    }
    if(i < layers - 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pics: ");
      lcd.print(pics);
      lcd.setCursor(0, 1);
      lcd.print("Change Layer");
      while(1)
      {
        key = keypad.getKey();
        if(key)
        {
          if(key == '#')
            digitalWrite(7, LOW);
          if(key == '*')
            break;
        }
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("All pictures");
  lcd.setCursor(0, 1);
  lcd.print("taken: ");
  lcd.print(pics);
  while(1)
  {
     key = keypad.getKey();
     if(key)
     {
       digitalWrite(7,LOW);
     }
  }
}
