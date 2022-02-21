#define BARRIER 19
static unsigned long prev_time = 0;
bool read_Barrier_Sens()
{
   Serial.println("Dropped");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(19,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BARRIER),read_Barrier_Sens,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  //read_Barrier_Sens();
}
