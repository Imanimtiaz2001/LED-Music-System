int LED=2;
void setup() {
  // put your setup code here, to run once:
pinMode(LED ,OUTPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("LED is on.");
digitalWrite(LED ,HIGH);
delay(2000);
Serial.print("LED is off.");
digitalWrite(LED ,LOW);
delay(1000);
}
