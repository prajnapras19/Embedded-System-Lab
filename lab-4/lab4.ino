// references:
// - https://www.circuitbasics.com/how-to-set-up-uart-communication-for-arduino/
// - https://www.tinkercad.com/things/eqQfhbhZPNa-arduino-uno-tutorial-3-servo-motor-project

#include <Servo.h>

Servo servo;
int position = 0;

void setup() {
  // put your setup code here, to run once:

  servo.attach(10);
  Serial.begin(9600);       // initialize UART with baud rate of 9600 bps
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    String recv = Serial.readString();
    //Serial.print(recv + '\n');

    //char data_rcvd = Serial.read(); // read one byte from serial buffer and save to data_rcvd
    if (recv.equals("kiri\n")) {
      // putar ke kiri (berlawanan arah jarum jam)
      while (position < 180) {
        position++;
        servo.write(position); 
        delay(5);
      }
      Serial.write("kiri\n");
    }
    if (recv.equals("kanan\n")) {
      // putar ke kanan (searah jarum jam)
      while (position > 0) {
        position--;
        servo.write(position); 
        delay(5);
      }
      Serial.write("kanan\n");
    }
  }
}
