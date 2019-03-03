#include <Arduino.h>
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 13
SSD1306AsciiWire display;

const int PWR_PIN		= 17;
const int DTR_PIN		= 16;
const int RTS_PIN		= 15;

#define nbSerial Serial1

void powerOn();
void AT_Manual();
void readBuffer(char *buffer, uint16_t count);
bool getSignalStrength(int *signal);

void setup() {
  Serial.begin(115200);
  nbSerial.begin(115200);

  powerOn();
  delay(2000);

  Wire.begin();
  Wire.setClock(400000L);
  display.begin(&Adafruit128x64, 0x3C, OLED_RESET);

  display.setFont(Arial_bold_14);
}

void loop() {
  int signal;
  if(getSignalStrength(&signal)){
    display.setCursor(25,2);
    display.print("RSRQ : ");
    display.println(signal);
    display.setCursor(25,5);
    display.print("RSSI   : ");
    display.println(signal);
  }
  delay(2000);

}


void powerOn(){
  pinMode(PWR_PIN, OUTPUT);
  pinMode(RTS_PIN, OUTPUT);
  pinMode(DTR_PIN, OUTPUT);

  digitalWrite(DTR_PIN, LOW);
  digitalWrite(RTS_PIN, LOW);

  digitalWrite(PWR_PIN, HIGH);
  delay(500);
  digitalWrite(PWR_PIN, LOW);
  delay(500);
  digitalWrite(PWR_PIN, HIGH);
}

void AT_Manual(){
  if(Serial.available()){
    nbSerial.write(Serial.read());
  }
  if(nbSerial.available()){
    Serial.write(nbSerial.read());
  }
}

void send_AT(char* command){
  nbSerial.write(command);
}

void readBuffer(char *buffer, uint16_t count){
  uint16_t i = 0;
  while(true){
    if(nbSerial.available()){
      char c = nbSerial.read();
      buffer[i] = c;
      i++;
      if(i >= count){
        break;
      } 
    }
  }
}

bool getSignalStrength(int *signal){
  //AT+CSQ                6+CR = 7
  //+CSQ:<rssi>,<ber>     CRLF + 5 + 5 + CRLF = 14
  //OK                    CRLF + 2 + CRLF = 6  

  char rxBuf[27] = {'\0'};
  char *p;
  int quality;

  send_AT("AT+CSQ\r\n");
  readBuffer(rxBuf, 27);

  if(NULL == (p = strstr(rxBuf, "+CSQ:"))){
    Serial.println("Tidak ada CSQ");
    return false;
  }              

  if(1 != sscanf(p, "+CSQ: %d, %*d", &quality)){
    return false;
  }   
  Serial.println(rxBuf);
  *signal = quality;
  return true;
}

