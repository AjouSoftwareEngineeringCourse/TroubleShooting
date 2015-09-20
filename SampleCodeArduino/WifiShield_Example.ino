#include <SoftwareSerial.h>

#include <TimerOne.h>


int flag;
boolean dataModeFlag = false;

/* 
After power on, WizFi210 would try to connect to a specific AP. That is the default setting.
In short, WizFi210 boots within 100ms and tries to connect to a specific AP for 3~5 seconds.
If that is failed, Wizfi210 returns an "[ERROR]" string. 
*/
#define PIN_RX 2
#define PIN_TX 3
SoftwareSerial mySerial(PIN_TX, PIN_RX);
void Callback()
{
  switch(flag)
  {
    // Exit DataMode
    case 0: //Serial.print("+++");    // Monitoring
            //Serial1.print("+++");
            mySerial.print("+++");
            flag++;
            break;
    // Delay 1 sec
    case 1: 
            flag++;
            break;
    case 2: Serial.print("+++");    // Monitoring
            //Serial1.print("+++");
            mySerial.print("+++");
            flag++;
            break;    
    // Disassociate from the current network.
    case 3: //Serial.print("AT+WD\r\n");    // Monitoring
            //Serial1.print("AT+WD\r");
            //mySerial.print("AT+WD\r");
            mySerial.print("AT\r");
            break;
    // This command sets WPA Passphrase as “cndi313182246”
    case 4: Serial.print("ATB=9600,8,n,1\r\n");    // Monitoring
            mySerial.print("ATB=9600,8,n,1\r");
            //Serial.print("AT+WWPA=cndi313182246\r\n");  
            //Serial1.print("AT+WWPA=cndi313182246\r");
            //mySerial.print("AT+WWPA=cndi313182246\r");
            break; 
    // This command sets WizFi210 as using fixed IP address (not DHCP). When the IP address is acquired by DHCP..                      
    case 5: Serial.print("AT+NDHCP=0\r\n");          
            //Serial1.print("AT+NDHCP=0\r");
            mySerial.print("AT+NDHCP=0\r");
            break;
    //This command sets the module with IP address (192.168.10.118), Subnet Mask (255.255.0.0), and Gateway Address (192.168.10.1).                     
    case 6: Serial.print("AT+NSET=192.168.0.100, 255.255.255.0, 192.168.0.1\r\n");
            //Serial1.print("AT+NSET=192.168.10.118, 255.255.0.0, 192.168.10.1\r");
            mySerial.print("AT+NSET=192.168.0.100, 255.255.255.0, 192.168.0.1\r");
            break; 
    // This command sets WizFi210 with Server(1), TCP(1) and Port number (8011).
    case 7: Serial.print("AT+NAUTO=1,1,,8011\r\n");
            //Serial1.print("AT+NAUTO=1,1,,8011\r");
            mySerial.print("AT+NAUTO=1,1,,8011\r");
            break;    
    // This command sets the AP’s Mode (0:Infrastructure), SSID(“CNDI_TKIP”), BSSID(Default)                  
    case 8: Serial.print("AT+WAUTO=0,se_ajou\r\n");
            //Serial1.print("AT+WAUTO=0,se_ajou\r");
            mySerial.print("AT+WAUTO=0,se_ajou\r");
            break; 
    // This command initializes auto connection.                     
    case 9: Serial.print("ATA\r\n");
            //Serial1.print("ATA\r");
            mySerial.print("ATA\r");
            flag++;
            dataModeFlag = true;
            break;
    default: break;
  }
}

void setup()
{
   flag = 0;                                         // init flag

   //Serial1.begin(115200);                           // setting baud to 115200
   mySerial.begin(9600);
   Serial.begin(9600);
   Serial.println("Wifi Start ...");
   Timer1.initialize(1000000);                       // setting callback is 1s
   Timer1.attachInterrupt(Callback); 
}
 
void loop()
{
  int i = 0;
  char someChar[128] = {0};
  // when characters arrive over the serial port...
  if(Serial.available()) {
    do{
      someChar[i++] = Serial.read();
      delay(3);                  
    }while (Serial.available() > 0);  
    Serial.println(someChar);
    //Serial1.println(someChar);
    mySerial.println(someChar);
  }
 
  char someChar2[128] = {0};
  //while(Serial1.available()) {
  
  while(mySerial.available()) {
    do{
      someChar2[i++] = mySerial.read();//Serial1.read();
      //delay(10);                  
    }while (mySerial.available() > 0);//Serial1.available() > 0);
    
    if( strstr(someChar2, "OK"))
    {
      Serial.print("OK\r\n");
      flag++;
    }
    else if( dataModeFlag){
      Serial.print(someChar2);
      mySerial.write("HELLO\n");
    }else{
      Serial.print(someChar2);
    }
  }
}
