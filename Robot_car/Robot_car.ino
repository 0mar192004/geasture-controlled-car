int IN1 = 2 ;
int IN2 = 4 ;
int IN3 = 7 ;
int IN4 = 8 ;
char rec ;

void setup() {
  
Serial.begin(9600);
Serial.println("AT MODE: READY");
pinMode(IN1 , OUTPUT);
pinMode(IN2 , OUTPUT);
pinMode(IN3 , OUTPUT);
pinMode(IN4 , OUTPUT);

}

void loop() {
  
if (Serial.available() >0) {
  rec = Serial.read();
  if (rec == 'F') {
    digitalWrite(IN1 , HIGH);
    digitalWrite(IN2 , LOW);
    digitalWrite(IN3 , HIGH);
    digitalWrite(IN4 , LOW);
    }
    
  else if (rec == 'B') {
    digitalWrite(IN1 , LOW);
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN3 , LOW);
    digitalWrite(IN4 , HIGH);
    }
    
  else if (rec == 'L') {
    digitalWrite(IN1 , HIGH);
    digitalWrite(IN2 , LOW);
    digitalWrite(IN3 , LOW);
    digitalWrite(IN4 , HIGH);
    }
    
  else if (rec == 'R') {
    digitalWrite(IN1 , LOW);
    digitalWrite(IN2 , HIGH);
    digitalWrite(IN3 , HIGH);
    digitalWrite(IN4 , LOW);
    }
  
    else if (rec == 'S') {
    digitalWrite(IN1 , LOW);
    digitalWrite(IN2 , LOW);
    digitalWrite(IN3 , LOW);
    digitalWrite(IN4 , LOW);
    }
    
  }
}
