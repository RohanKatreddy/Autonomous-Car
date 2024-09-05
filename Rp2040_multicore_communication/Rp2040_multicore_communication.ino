void setup() {
  // put your setup code here, to run once:

}

void loop() {
  rp2040.fifo.push(69);
  delay(1000);

}


void setup1() {
  Serial.begin(115200);

}

void loop1() {
  if (rp2040.fifo.available()){
    Serial.println(rp2040.fifo.pop());
  }

}