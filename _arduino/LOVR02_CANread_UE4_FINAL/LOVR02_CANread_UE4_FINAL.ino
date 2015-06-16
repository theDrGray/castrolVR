//standrad libraries
#include <Time.h>
#include <defaults.h>
#include <global.h>
#include <Wire.h>
#include <DS1307RTC.h>

//canbus libraries
#include <Canbus.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(115200); // For debug use
  
  pinMode(10, OUTPUT);
  SD.begin(chipSelect);
  
  Canbus.init(CANSPEED_500);
  delay(100);
}

void loop(){

  tCAN message;
  char buffer[512];
  
  String bit_00;
  String bit_01;
  String bit_05;
  
  String temp = "";
  String temp2 = "";  

  String bit_02;
  String bit_03;
  
  int count;
  String cnt;
  int conv1;
  int conv2;  
  float steering;
  float brake;
    
  if (mcp2515_get_message(&message)) 
  {
    
    if (message.id == XXXX || message.id == XXXX || message.id == XXXX || message.id == XXXX)
    {

      //STEERING ANGLE
      if (message.id == XXXX)
      {
        bit_00 = String(message.data[0], HEX);
        bit_01 = String(message.data[1], HEX);
        cnt = bit_00.substring(0,1);
        
        if (bit_00.length() < 2)
        {
          bit_00 = "0"+bit_00;
          }
        
        if (bit_01.length() < 2)
        {
          bit_01 = "0"+bit_01;
          }
        
        temp = bit_00+bit_01;
        
        int low = 3200;
        int high = 3333.3;
        
        conv1 = (int)strtol( &temp[1], NULL, 16);
        count = atol(cnt.c_str());
        
        conv2 = abs(abs(count*4096)+conv1);
        steering = ((conv2*0.100f)-3200)/-1;

        steering = (steering)/(942.000f)-0.46f;
        Serial.print("ST,");
        Serial.print(steering);
        Serial.print("\n");  
      }
        
      //BRAKE PRESSURE
      if (message.id == XXXX)           
      {
        bit_02 = String(message.data[0], HEX);
        bit_03 = String(message.data[1], HEX);

        if (bit_02.length() < 2)
        {
          bit_02 = "0"+bit_02;
         }
        if (bit_03.length() < 2)
        {
          bit_03 = "0"+bit_03;
         }                  

        temp = bit_02+bit_03;
        
        conv1 = (int)strtol( &temp[1], NULL, 16);
        brake = conv1/4;
    
        brake = brake/560; 
        Serial.print("BR,");
        Serial.print(brake);
        Serial.print("\n");
      }

      //WHEEL SPIN
      if (message.id == XXXX)           
      {
      XXXXXXXXXXXXXXXXXXXXXXX
      }

      //OIL TEMP
      if (message.id == XXXX)           
      {
      XXXXXXXXXXXXXXXXXXXXXXX
      }


      //STANDARD PID AS REQUESTS. THESE NEEDS TO BE REQUESTED IN YOUR LOOP OF 
      //PULLING RAW CAN OR YOU WLL GET DELAYS WHILE THE REQUEST IS HAPPENING. 
      //THROTTLE
      if(Canbus.ecu_req(THROTTLE, buffer) == 1)
      {
       int num = atol( buffer );
       Serial.print("TH,");
       Serial.print(num);
       Serial.print("\n");
      }

      //RPM
      if(Canbus.ecu_req(ENGINE_RPM, buffer) == 1)
      {
       int num = atol( buffer );
       Serial.print("RP,");
       Serial.print(num);
       Serial.print("\n");
      }          
       
      //Vehicle Speed
      if(Canbus.ecu_req(VEHICLE_SPEED, buffer) == 1)
      {
       int num = atol( buffer );
       Serial.print("SP,");
       Serial.print(num);
       Serial.print("\n");
      } 

      //Engine Coolant Temp
      if(Canbus.ecu_req(ENGINE_COOLANT_TEMP, buffer) == 1)
      {
       int num = atol( buffer );
       Serial.print("ET,");
       Serial.print(num);
       Serial.print("\n");
      }                     
       
      //MAF Sensor
      if(Canbus.ecu_req(MAF_SENSOR, buffer) == 1)
      {
       int num = atol( buffer );
       Serial.print("MS,");
       Serial.print(num);
       Serial.print("\n");
      } 
    }
  }
}

void showTime(time_t t){
  // display the given time 
  Serial.print(hour(t));
  printDigits(minute(t));
  printDigits(second(t));
  Serial.print(" ");
  Serial.print(day(t));
  Serial.print(" ");
  Serial.print(month(t));
  Serial.print(" ");
  Serial.print(year(t)); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
