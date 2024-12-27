//////////////////////////////////////////////////////////////////////////////////////////
/* Arduino pins where the LED is attached*/     
#define LED_1 2
#define LED_2 3
#define LED_3 11
#define LED_4 12
#define LED_5 13

#define sensorPin A0 // Analog input pin that the Sensor is attached to
//////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////Variables for Ultrasonic
int distanceThreshold = 0;
int cm = 0;
int inches = 0;
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}
////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;        
//the time when the sensor outputs a low impulse
long unsigned int lowIn;         
//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 30;  
boolean lockLow = true;
boolean takeLowTime;  
int pirPin = 7;    //the digital pin connected to the PIR sensor's output
int pinblue = 8;   //the digital pin connected to the LED output
int ledPin = 5;    //the digital pin connected to the LED output
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/* boolean variables to hold the status of the pins*/
bool ledPin1Status;
bool ledPin2Status;
bool ledPin3Status;
bool ledPin4Status;
bool ledPin5Status;
/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pinblue, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);

     pinMode(LED_1, OUTPUT);
     pinMode(LED_2, OUTPUT);
     pinMode(LED_3, OUTPUT);
     pinMode(LED_4, OUTPUT);
     pinMode(LED_5, OUTPUT);
     pinMode(sensorPin, INPUT);
    
    ///////////
     pinMode(4, OUTPUT);
     pinMode(6, OUTPUT);
  }


////////////////////////////
//LOOP
void loop(){
  PIR();
  Microphone();
  Ultrasonic();
  }

void PIR(){
   if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       digitalWrite(pinblue, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/5000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
       digitalWrite(pinblue, LOW);  //the led visualizes the sensors output pin state
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/500);
           Serial.println(" sec");
           delay(50);
           }
       }
}

void Microphone(){
   int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  if (sensorValue > 555  )
    ledPin1Status = 1;
   if (sensorValue > 558  )
    ledPin2Status = 1;
   if (sensorValue > 560  )
    ledPin3Status = 1;
   if (sensorValue > 562  )
    ledPin4Status = 1;
   if (sensorValue > 564 )
    ledPin5Status = 1;
  if (ledPin1Status == 1 || ledPin2Status == 1 || ledPin3Status == 1 || ledPin4Status == 1 || ledPin5Status == 1)
  {
    if (sensorValue > 555 || sensorValue < 537  )
      digitalWrite(LED_1, HIGH);
     if (sensorValue > 558 || sensorValue < 534  )
      digitalWrite(LED_2, HIGH);
     if (sensorValue > 560 || sensorValue < 534  )
      digitalWrite(LED_3, HIGH);
     if (sensorValue > 562 || sensorValue < 531 )
      digitalWrite(LED_3, HIGH);
     if (sensorValue > 564 || sensorValue < 528)
      digitalWrite(LED_4, HIGH);
     if (sensorValue > 568 || sensorValue < 525)
      digitalWrite(LED_5, HIGH);
    delay(200);
    ledPin5Status = 0;
    ledPin4Status = 0;
    ledPin3Status = 0;
    ledPin2Status = 0;
    ledPin1Status = 0;
  }
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_4, LOW);
  digitalWrite(LED_5, LOW);
}
//////////////////////////////////////////////////////////////////////////////
void Ultrasonic(){
    // set threshold distance to activate LEDs
    distanceThreshold = 300;
    // calculate the distance to the target in cm
    cm = 0.09923 * readUltrasonicDistance(10, 9); //normal cm 0.01739
    // convert to inches by dividing by 2.54
    inches = (cm / 2.54);
    Serial.print(cm);
    Serial.print("cm, ");
    Serial.print(inches);
    Serial.println("in");
    if (cm > distanceThreshold) {
      digitalWrite(4, LOW);
      digitalWrite(6, LOW);
    }
    if (cm <= distanceThreshold && cm > distanceThreshold - 100) {
      digitalWrite(4, HIGH);
      digitalWrite(6, HIGH);
    }
    if (cm <= distanceThreshold - 100 && cm > distanceThreshold - 200) {
      digitalWrite(4, HIGH);
      digitalWrite(6, HIGH);
    }
    if (cm <= distanceThreshold - 200 && cm > distanceThreshold - 300) {
      digitalWrite(4, HIGH);
      digitalWrite(6, HIGH);

    }
    if (cm <= distanceThreshold - 300) {
      digitalWrite(4, HIGH);
      digitalWrite(6, HIGH);
    }
    delay(200); // Wait for 100 millisecond(s)
}