

void setup() {
  // put your setup code here, to run once:
Serial1.begin(115200);
Serial.begin(115200);

pinMode(14, OUTPUT);
pinMode(16, OUTPUT);
pinMode(15, OUTPUT);

digitalWrite(14, LOW);
digitalWrite(16, LOW);

digitalWrite(15, HIGH);
delay(500);
digitalWrite(15, LOW);
delay(500);
digitalWrite(15, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial1.available()){
  Serial.write(Serial1.read());
}
if(Serial.available()){
  Serial1.write(Serial.read());
}
}
