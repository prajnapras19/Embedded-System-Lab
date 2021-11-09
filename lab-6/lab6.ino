void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.write("AAAAAAAA\n");
  delay(1000);
  Serial.write("BBBBBBBB\n");
  delay(1000);
  Serial.write("CCCCCCCC\n");
  delay(1000);
}
