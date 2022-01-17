#include <SharpIR.h>
#define Porta A1
#define modelo 1080
SharpIR detect(Porta, modelo);

void setup() {
Serial.begin(9600);
Serial.println("Capteur infrarouge Sharp 10-80 cm");
}

void loop() {
 delay(100);
 float distance_now=detect.distance();
Serial.print("distance  [cm]= ");
Serial.println(distance_now);
 
}
