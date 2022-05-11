//led on esp32
int LED=2;
void setup() {
  // put your setup code here, to run once:
//set the led on esp32 as output
pinMode(LED ,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("LED is on.");
digitalWrite(LED ,HIGH);//turn the led on (voltage high)
delay(2000);//keep the led on for 2 secs
Serial.print("LED is off.");
digitalWrite(LED ,LOW);//turn the led off (voltage low)
delay(1000);//keep the led off for 1 sec
