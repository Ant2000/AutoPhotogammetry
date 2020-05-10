#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Stepper.h>

const int steprev = 4096;
Stepper stepper = Stepper(steprev, 3,5,4,6);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
  };
byte rowPins[4] = {A3,A2,A1,A0};
byte colPins[4] = {8,9,10,11};
char key;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

unsigned int x, temp, layers, rounds, pics_per_round;

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

void click(int d)
{
  digitalWrite(2, 1);
  delay(d);
  digitalWrite(2, 0);
  x=0;
}

void setup() 
{
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
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
  pics_per_round = keypadInput(10);
  Serial.println(pics_per_round);
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
        click(500);
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
  }
}
