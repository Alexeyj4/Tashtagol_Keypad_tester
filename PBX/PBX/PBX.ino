#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

const int line_pin=2; //D2

const int SCREEN_WIDTH=128; // OLED display width, in pixels
const int SCREEN_HEIGHT=64; // OLED display height, in pixels
const int first_string=12;  //first string on LCD
const int second_string=28;  //second string on LCD
const int third_string=44;  //third string on LCD
const int fourth_string=62;  //fourth string on LCD

int v=0; //line voltage //0-phone on (taked handset)//1-phone off
int v_prev=0; //previous state
int count=0; //pulse dial count //-1 not count
unsigned long t_prev=millis(); //standby timout
int reset_flag=0; //ready to reset 1=yes


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

void setup() {
  // put your setup code here, to run once:
ADCSRA |= (1 << ADPS2);                     //Биту ADPS2 присваиваем единицу - коэффициент деления 16 //ADC speed up accuracy down
ADCSRA &= ~ ((1 << ADPS1) | (1 << ADPS0));  //Битам ADPS1 и ADPS0 присваиваем нули  //ADC speed up accuracy down

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println("SSD1306 allocation failed");
  for(;;);
}
display.setFont(&FreeSerif9pt7b);
display.setTextSize(1);             
display.setTextColor(WHITE); 

display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
display.setCursor(0,first_string);
display.println("tash tester v1");
display.println("pbx os loading...");     
display.display();  
delay(1500);
display.clearDisplay();
display.display();  
  
pinMode(line_pin, INPUT);  
analogReference(DEFAULT); //5v
analogRead(line_pin); //init
delay(50);
}

void loop() {
 
  v=digitalRead(line_pin);  
    
  if((v!=v_prev)or(count==0)){    
    display.fillRect(0, 0, SCREEN_WIDTH, second_string, BLACK);
    display.setCursor(0,first_string);
    display.setTextSize(1); 
    if(v==0){
      display.print("O---]");
      if(v_prev==1 and reset_flag==0){
        count++;
      }
      reset_flag=0;      
      }
    if(v==1)display.print("O]");          

    display.display();  
    v_prev=v; 
    t_prev=millis();    
  }
  if((millis()-t_prev)>300){  //dial timeout //end of number
    reset_flag=1;
    
    display.fillRect(0, second_string, SCREEN_WIDTH, fourth_string, BLACK);
    display.setCursor(0,fourth_string); 
    display.setTextSize(2);
    display.print(count); 

    t_prev=millis();
    count=0;
    
    
    
  }
  

}
