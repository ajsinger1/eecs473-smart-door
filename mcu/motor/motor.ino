#include <Servo.h>
#include <wifi101.h>


Servo myservo;

void setup() {
  //myservo.attach(2);

}

void loop() {
  myservo.attach(2);
  delay(1000);
  myservo.write(0);
  delay(100);
  myservo.detach();
  delay(1000);
  myservo.attach(2);
  delay(1000);
  myservo.write(180);
  delay(100);
  myservo.detach();
  delay(1000);

  /*myservo.write(0);
  delay(5000);
  myservo.write(180);
  delay(5000);*/
}
