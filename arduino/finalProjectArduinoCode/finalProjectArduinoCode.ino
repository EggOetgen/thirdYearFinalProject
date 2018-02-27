int potPin = 0;


void setup() {
  // put your setup code here, to run once:
 //pinMode(potPin, INPUT);

  Serial.begin(9600);   
}

void loop() {

 int val = analogRead(potPin);
 int realVal = analogRead(potPin);
int mappedVal =  map(realVal, 0, 1023, 0, 16);
 Serial.println(mappedVal);
  // put your main code here, to run repeatedly:

}
