#include <BTS7960.h>
#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bonezegei_DHT11.h>


#define L_PWM1 12                           
#define R_PWM1 13  
#define L_PWM2 10                           
#define R_PWM2 11  

Bonezegei_DHT11 dht(2);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BTS7960 motor1(-1, -1,R_PWM1, L_PWM1);  
BTS7960 motor2(-1, -1,R_PWM2, L_PWM2);  

void act1(int mode, int time) {
  if (mode > 0) {
      motor1.front();   
  } else if (mode < 0) {
      motor1.back();
  }

  delay(time*1000);
  motor1.stop();
}

void act2(int mode, int time) {
  if (mode > 0) {
      motor2.front();   
  } else if (mode < 0) {
      motor2.back();
  }

  delay(time*1000);
  motor2.stop();
}


void setup() {
  Serial.begin(9600);                      
  motor1.begin();                          
  motor2.begin();
  motor1.stop();                          
  motor2.stop();

  dht.begin();

  Serial.println("\nstarting");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Solar Panel");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Starting up...");

  int i = 255;
  motor1.pwm = i; 
  motor2.pwm = i;
  
 
  act1(-1,10);
  act2(-1,10);



}

void loop() {
  int sensorValueTL = analogRead(A0); 
  int sensorValueTR = analogRead(A1);
  int sensorValueBL = analogRead(A2);
  int sensorValueBR = analogRead(A3);   


  float VoltageTL = (sensorValueTL * 5.0)/1023.0;
  float VoltageTR = (sensorValueTR * 5.0)/1023.0;
  float VoltageBL= (sensorValueBL * 5.0)/1023.0;
  float VoltageBR = (sensorValueBR * 5.0)/1023.0;


  int TopRow = sensorValueTL + sensorValueTR;
  int BottomRow = sensorValueBL + sensorValueBR;

  int LeftCol = sensorValueTL + sensorValueBL;
  int RightCol = sensorValueTR + sensorValueBR;
 

  dht.getData();                        
  int tempFar = dht.getTemperature(true); 
  int hum = dht.getHumidity();               


  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print(VoltageTL,2);
  lcd.print("V ");
  lcd.print(VoltageTR,2);
  lcd.print("V ");
  lcd.print(tempFar);
  lcd.print("F");

  lcd.setCursor(0, 1);
  lcd.print(VoltageBL,2);
  lcd.print("V ");
  lcd.print(VoltageBR,2);
  lcd.print("V ");
  lcd.print(hum);
  lcd.print("%");

  Serial.print(sensorValueTL);
  Serial.print("    ");
  Serial.println(sensorValueTR);
  Serial.print(sensorValueBL);
  Serial.print("    ");
  Serial.println(sensorValueBR);
  Serial.println();

  if ((fabs(TopRow-BottomRow)>50)) {
    if (TopRow>BottomRow) {
      act1(-1,1);
    } else {
      act1(1,1);
    }
  }
  

  if ((fabs(LeftCol-RightCol)>50)) {
    if (LeftCol>RightCol) {
      act2(-1,1);
    } else {
      act2(1,1);
    }
  }

  delay(5000);


 
}
