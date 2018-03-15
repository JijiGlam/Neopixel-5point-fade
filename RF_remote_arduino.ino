//arduino send communication to teensy via TX

byte x;
byte x0;

void setup() {
  Serial.begin(9600);

}

void loop() {
  byte a = analogRead(A3); // forward right
  byte b = analogRead(A2); // forward left
  byte c = analogRead(A1); // reverse right
  byte d = analogRead(A0);
  delay(30);
  if (a > 150){
    x = 1;
  }
  else if (b > 150){
    x = 2;
  }
  else if (c > 150){
    x = 3;
  }
  else if (d > 150){
    x = 4;
  }
  if (x != x0){
  Serial.write(x);
  delay(60);
  Serial.write(x);
  x0 = x;
  delay(30);
  }
  delay (10);
  
}
