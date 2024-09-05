String a;

void setup() {
Serial1.setRX(1);

  Serial1.begin(115200); 
  Serial.begin(115200); 
}

void loop() {

  while(Serial1.available()) {

  a= Serial1.readStringUntil('\n');// read the incoming data as string
  a.trim();
  if (a.startsWith("$GNGLL")){
  Serial.println(a.substring(7, 17));
  Serial.println(a.substring(20, 31));
  }


  }

}