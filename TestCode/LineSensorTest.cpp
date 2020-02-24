
int tempData[5];
const int IRSensor[] = {A2, A1, A0, A7, A6};//Pins reflect current wiring (2/18/2020)


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < 5; i++){
    tempData[i] = analogRead(IRSensor[i]);
    Serial.print(tempData[i]);
    Serial.print(" ");

  }
  Serial.println();

}
