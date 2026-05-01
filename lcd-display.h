#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

// void setup() {
//   Serial.begin(9600);
 
//   ledcSetup(0, 2000, 8);
//   ledcAttachPin(buzzer, 0);

//   lcd.init();
//   lcd.backlight();
// }

// void loop() {
//   gameSystem();
// }

// void gameSystem(){
//     while (gameSelector == 0){
//         int potValue = analogRead(potentiometer);
//         int startButtonstate = digitalRead(startButton);
        
//         if (potValue < 1365) {
//             lcd.setCursor(0, 0);
//             lcd.print("Select Gamemode           ");
            
//             lcd.setCursor(0, 1); 
//             lcd.print("Classic                              ");  
            
//         } else if (potValue < 2730) {
//             lcd.setCursor(0, 0);
//             lcd.print("Select Gamemode           ");
            
//             lcd.setCursor(0, 1); 
//             lcd.print("PVP            ");  
            
//         } else  {
//             lcd.setCursor(0, 0);
//             lcd.print("Select Gamemode               ");
            
//             lcd.setCursor(0, 1); 
//             lcd.print("TEAM                    "); 
//         }

//         if(potValue<1365 && startButtonstate == LOW){
//             gameSelector = 1;
//             gamemode = 1;
//             finished = false;
//         } else if(potValue<2730 && startButtonstate == LOW){
//             gameSelector = 2;
//             gamemode = 2;
//             finished = false;
//         } else if(startButtonstate == LOW){
//             gameSelector = 3;
//             gamemode = 3;
//             finished = false;
//         }

class LCDDisplay {
  public:
    float moisture;

    void setup() {
      Wire.begin(4, 5);   // SDA, SCL
      lcd.init();
      lcd.backlight();
    }
    
    void loop() {
    }
    
    void printMoisture (int moisture, float foodPercent) {
      moisture = moisture;
      foodPercent = foodPercent;
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Moisture: ");
      lcd.print(moisture);
      
      lcd.setCursor(0, 1);
      lcd.print("Percent:  ");
      lcd.print(foodPercent);
    }
};

LCDDisplay lcdDisplay;