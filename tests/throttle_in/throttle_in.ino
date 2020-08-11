#define ADC_THROTTLE_IN     33
#define POT_OFFSET          850
#define throttle_ave_n      10
int potValue = 0;
int throttle_ave = 0;
int throttle_i = 0;
int throttle_sum = 0;
int throttle_filter[throttle_ave_n] = {0};

unsigned long StartTime = micros();
unsigned long CurrentTime = micros();
unsigned long ElapsedTime = CurrentTime - StartTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  StartTime = micros();
  
  throttle_i += 1;
  if ( throttle_i >= (throttle_ave_n) ) { throttle_i = 0; }
  potValue = (analogRead(ADC_THROTTLE_IN) - POT_OFFSET) * 100;
  throttle_sum = throttle_sum - throttle_filter[throttle_i] + potValue;
  throttle_ave = throttle_sum / throttle_ave_n / 222;
  throttle_filter[throttle_i] = potValue;
  if ( throttle_ave < 5 ) { throttle_ave = 0; }

  CurrentTime = micros();
  ElapsedTime = CurrentTime - StartTime;
  Serial.print(ElapsedTime);
  Serial.print("  -  ");
  Serial.print(throttle_ave);
  Serial.print("  -  ");
  Serial.println(potValue);
//  delay(100);

}
