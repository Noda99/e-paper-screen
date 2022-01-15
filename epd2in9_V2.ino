
#include <SPI.h>
#include "epd2in9_V2.h"
#include "epdpaint.h"
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

#define HALL_PIN 6
#define GREEN_PIN 5
#define RED_PIN 4
#define BLUE_PIN 3
#define PWBUTTON_PIN 2
#define MBUTTON_PIN 1

byte lastPWButtonState = LOW;
byte lastMButtonState = LOW;
byte GledState = LOW;
byte RledState = LOW;
byte BledState = LOW;

unsigned long debounceDuration = 50; // millis
unsigned long lastTimeButtonStateChanged = 0;


unsigned long CurrentTime = 0;
unsigned long RegisterTime = 3000;


/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;

//unsigned long time_start_ms;
//unsigned long time_now_s;

char ContaGiri1[]={'M','a','i','l',':','0','0','0','\0'};
char ContaGiri2[]={'m','i','n','/','V','i','d','e','o',':','0','0','0','\0'};
char ContaGiri3[]={'m','i','n','/','W','e','b',':','0','0','0','\0'};


int Status=0;


int giri=0;
int RPM=0;
float energia=0;
float connection_time=0;
bool print_count= false;
bool connection_ended= false;
int gmail=0;
int gmailold=0;
int gvid=0;
int gvidold=0;
int gweb=0;
int gwebold=0;

int connection=0;
int oldconnection=0;

void setup() {
  pinMode(HALL_PIN, INPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(PWBUTTON_PIN, INPUT);
  pinMode(MBUTTON_PIN, INPUT);
  // put your setup code here, to run once:
  
  
 Serial.begin(115200);
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }
  
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

 
  
  

  
  

  
  //if (epd.Init() != 0) {
    //Serial.print("e-Paper init failed ");
    //return;
 // }
  
  
  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
  
  //epd.SetFrameMemory_Base(IMAGE_DATA1);
  //epd.DisplayFrame();
  CurrentTime = millis();
  connection_time= millis();
  //time_start_ms = millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int Digital;
  int refresh=0;

  

  Digital = digitalRead (HALL_PIN);

  if (millis() - lastTimeButtonStateChanged > debounceDuration) {
    byte PWbuttonState = digitalRead(PWBUTTON_PIN);
    if (PWbuttonState != lastPWButtonState) {
      lastTimeButtonStateChanged = millis();
      lastPWButtonState = PWbuttonState;
      if (PWbuttonState == LOW) {
        if(GledState == HIGH){
          GledState = LOW;
          epd.Init();
          epd.SetFrameMemory_Base(IMAGE_DATA3);
          epd.DisplayFrame();
          connection=0;
         
          
          }else{
           GledState = HIGH; 
           
          }
        digitalWrite(GREEN_PIN, GledState);
      }
    }
  }

  if (millis() - lastTimeButtonStateChanged > debounceDuration) {
      byte MbuttonState = digitalRead(MBUTTON_PIN);
      if(GledState== HIGH){
        if(MbuttonState != lastMButtonState){
          lastTimeButtonStateChanged = millis();
          lastMButtonState = MbuttonState;
          if (MbuttonState == LOW){
            if(RledState == HIGH){
              RledState = LOW;
              BledState = HIGH;
              digitalWrite(RED_PIN, RledState);
              digitalWrite(BLUE_PIN, BledState);
              energia=energia*1000;
              connection_time= millis();
              if (millis()- connection_time > energia){
                  epd.Init();
                  epd.SetFrameMemory_Base(IMAGE_DATA5);
                  epd.DisplayFrame();
                  connection=0;
                 
                }else{
                  epd.Init();
                  epd.SetFrameMemory_Base(IMAGE_DATA4);
                  epd.DisplayFrame();
                  print_count=false;
                  connection=1;
                 
                }
              
            }
            else{
              RledState = HIGH;
              BledState = LOW;
              digitalWrite(RED_PIN, RledState);
              digitalWrite(BLUE_PIN, BledState);
              epd.Init();
              epd.SetFrameMemory_Base(IMAGE_DATA2);
              epd.DisplayFrame();
              energia= energia-(millis()-connection_time);
              energia=energia/1000;
              connection=0;
               
            }
            digitalWrite(RED_PIN, RledState);
            digitalWrite(BLUE_PIN, BledState);
          }
          if(RledState==HIGH){
            
            }
        }
      }else{
        RledState = LOW;
        BledState = LOW;
        digitalWrite(RED_PIN, RledState);
        digitalWrite(BLUE_PIN, BledState);
        connection=0;
         
        }
  }
  
  
  
  
  //read current value
  if(RledState ==HIGH){
    
      if(connection_ended==true){
          ContaGiri1[5]= '0';
          ContaGiri1[6]= '0';
          ContaGiri1[7]= '0';
          
          ContaGiri2[10]= '0';
          ContaGiri2[11]= '0';
          ContaGiri2[12]= '0';

          ContaGiri3[8]= '0';
          ContaGiri3[9]= '0';
          ContaGiri3[10]= '0';
         

          connection_ended=false;

          }
      if(Digital==0 && Status==1){       
        refresh=1;
        giri++;
        gmail=giri/8;
        gvid=giri/15;
        gweb=giri/5;

        
          
        if(ContaGiri1[7] < '9' && gmail!= gmailold ){
          ContaGiri1[7] = ContaGiri1[7]+1;
          gmailold=gmail;
        }else if(gmail!= gmailold){
              ContaGiri1[7] = '0';
              if(ContaGiri1[6]<'9' && gmail!= gmailold){
                ContaGiri1[6] = ContaGiri1[6]+1;
                gmailold=gmail;
         
              }else if(gmail!= gmailold){
                ContaGiri1[6] = '0';
                ContaGiri1[5] = ContaGiri1[5]+1;
                gmailold=gmail;
              }      
        }

        if(ContaGiri2[12] < '9' && gvid!= gvidold ){
          ContaGiri2[12] = ContaGiri2[12]+1;
          gvidold=gvid;
        }else if(gvid!= gvidold){
              ContaGiri2[12] = '0';
              if(ContaGiri2[11]<'9' && gvid!= gvidold){
                ContaGiri2[11] = ContaGiri2[11]+1;
                gvidold=gvid;
              }else if(gvid!= gvidold){
                ContaGiri2[11] = '0';
                ContaGiri2[10] = ContaGiri2[10]+1;
                gvidold=gvid;
              }      
        }

        if(ContaGiri3[10] < '9' && gweb!= gwebold ){
          ContaGiri3[10] = ContaGiri3[10]+1;
          gwebold=gweb;
        }else if(gweb!= gwebold){
              ContaGiri3[10] = '0';
              if(ContaGiri3[9]<'9' && gweb!= gwebold){
                ContaGiri3[9] = ContaGiri3[9]+1;
                gwebold=gweb;
         
              }else if(gweb!= gwebold){
                ContaGiri3[9] = '0';
                ContaGiri3[8] = ContaGiri3[8]+1;
                gwebold=gweb;
              }      
        }
        Status=0;
        
        
      }
       else if(Digital==1){
         Status=1;
         refresh=0;
       }
    
      if(millis()-CurrentTime > RegisterTime){
        
        RPM= giri*8;
        
        
        paint.SetRotate(ROTATE_0);
        paint.SetWidth(90);
        paint.SetHeight(12);
        paint.Clear(UNCOLORED);
        paint.DrawStringAt(0, 0, ContaGiri1, &Font12, COLORED);
        epd.SetFrameMemory_Partial(paint.GetImage(), 0, 80, paint.GetWidth(), paint.GetHeight()); 

        
        paint.Clear(UNCOLORED);
        paint.DrawStringAt(0, 0, ContaGiri2, &Font12, COLORED);
        epd.SetFrameMemory_Partial(paint.GetImage(), 0, 90, paint.GetWidth(), paint.GetHeight()); 

        
        paint.Clear(UNCOLORED);
        paint.DrawStringAt(0, 0, ContaGiri3, &Font12, COLORED);
        epd.SetFrameMemory_Partial(paint.GetImage(), 0, 100, paint.GetWidth(), paint.GetHeight()); 
        
        
        paint.SetRotate(ROTATE_180);
        paint.SetWidth(60);
        paint.SetHeight(125);
        
        paint.Clear(UNCOLORED);
        paint.DrawFilledRectangle(0, 0, 60, RPM , COLORED);
        epd.SetFrameMemory_Partial(paint.GetImage(), 0, 172, paint.GetWidth(), paint.GetHeight());
        
        epd.DisplayFrame_Partial();
        
        CurrentTime=millis();
        RPM=0;
        energia=energia+giri;
        giri=0;
        
        
        /*
        RPM= giri*20;


        
        paint.SetRotate(ROTATE_0);
        paint.SetWidth(128);
        paint.SetHeight(20);

        if(RPM <=30){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
          }
        else if(RPM>30 && RPM <=60){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }
        else if(RPM>60 && RPM <=90){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }
        else if(RPM>90 && RPM <=120){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }
        else if(RPM>120 && RPM <=150){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }
        else if(RPM>150 && RPM <=180){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }
        else if(RPM>180 && RPM <=210){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }
        else if(RPM>210 && RPM <=240){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(COLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , UNCOLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }
        else if(RPM>240){
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 276, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 254, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 232, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 210, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 188, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 166, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 144, paint.GetWidth(), paint.GetHeight());
          paint.Clear(UNCOLORED);
          paint.DrawFilledRectangle(0, 0, 128, 20 , COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 0, 122, paint.GetWidth(), paint.GetHeight());
        }

        epd.DisplayFrame_Partial();
        CurrentTime=millis();
        RPM=0;
        giri=0;
        */
        
      }
  }else if (BledState==HIGH){
      // traduzione giri in tempo
     
     
     if(print_count== false){
      if(millis()-connection_time > energia){
        epd.Init();
        epd.SetFrameMemory_Base(IMAGE_DATA5);
        epd.DisplayFrame();
        print_count=true;
        energia=0;
        connection_ended=true;
        }
      }
     
      
      /*
      energia=energia*5;
      for(long int j=energia; j>0; j--){
        
          
          char timer_string[] ={'0', '0',':','0','0','.','0','0','\0'};
          timer_string[0] = j/3600/10+'0';
          timer_string[1] = j/3600%10+'0';
          timer_string[3] = j%3600/60/10+'0';
          timer_string[4] = j%3600/60%10+'0';
          timer_string[6] = j%3600%60/10+'0';
          timer_string[7] = j%3600%60%10+'0';
    
          paint.SetWidth(32);
          paint.SetHeight(180);
          paint.SetRotate(ROTATE_270);
        
          paint.Clear(UNCOLORED);
          paint.DrawStringAt(0, 4, timer_string, &Font24, COLORED);
          epd.SetFrameMemory_Partial(paint.GetImage(), 80, 100, paint.GetWidth(), paint.GetHeight());
          epd.DisplayFrame_Partial();
          

          if(BledState==LOW){
            energia=j/5;
            break;
            }
        }
     */
        /*time_now_s = (millis() - time_start_ms) / 1000;
      char time_string[] = {'0', '0', ':', '0', '0', '\0'};
      time_string[0] = time_now_s / 60 / 10 + '0';
      time_string[1] = time_now_s / 60 % 10 + '0';
      time_string[3] = time_now_s % 60 / 10 + '0';
      time_string[4] = time_now_s % 60 % 10 + '0';
    
      paint.SetWidth(32);
      paint.SetHeight(96);
      paint.SetRotate(ROTATE_270);
    
      paint.Clear(UNCOLORED);
      paint.DrawStringAt(0, 4, time_string, &Font24, COLORED);
      epd.SetFrameMemory_Partial(paint.GetImage(), 80, 72, paint.GetWidth(), paint.GetHeight());
      epd.DisplayFrame_Partial();
      */
    
    }
    
  
              
          
          
}
          
       
    
