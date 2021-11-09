void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    Serial.write("AAAAAAAA");
    delay(1000);
    Serial.write("BBBBBBBB");
    delay(1000);
    Serial.write("CCCCCCCC");
    delay(1000);
  }
}
