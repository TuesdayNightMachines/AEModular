// 2019-08-04 The Tuesday Night Machines
// thundersound's Generative CV Sequencer AE Modular Synth Module
// more info: http://forum.aemodular.com/thread/362/generative-ambient-cv-sequencer
// License: CC-BY-NC-SA

int globalOffset = 0;

int l1Value = 0;
bool l1LastInput = 0; // LOW Trigger input
int l1Output = 0;

int l2Value = 0;
bool l2LastInput = 0; // LOW Gate input
int l2Output = 0;

int l3Value = 0;
int l3CurrentCV = 0;
bool l3LastCVabove = false;
int l3Threshold = 0;
bool l3RiseAbove = true;
int l3Output = 0;

bool resetLastInput = false;

int invertVals = 1; // either 1 or -1

int sumValue = 0;
int sumOutput = 0;

bool valueWrap = false; // false will constrain values to 0 - 1023, true will wrap values around if above 1023 or below 0

void setup() {

  // set up digital pins
  pinMode(0, INPUT); // D0 L2 Gate intput
  pinMode(1, INPUT); // D1 L1 Trig intput
  pinMode(2, INPUT); // D2 valueWrap switch 
  pinMode(3, OUTPUT); // D3 L1 CV output
  pinMode(5, OUTPUT); // D5 L2 CV output
  pinMode(7, INPUT); // D7 Reset 1+2 Gate input
  pinMode(10, OUTPUT); // D10 Sum CV output
  pinMode(9, OUTPUT); // D9 L3 CV output
  pinMode(14, INPUT); // D14 L3 above/below switch intput
  pinMode(15, INPUT); // D15 valueWrap gate input
  pinMode(16, INPUT); // D16 invertVals 2+3 gate input


  // set all outputs to 0
  analogWrite(3, 0);
  analogWrite(5, 0);
  analogWrite(9, 0);
  analogWrite(10, 0);

}

void loop() {

  // reset 1+2 input, sets the output values of L1 and L2 back to 0 when rising edge is detected
  if(digitalRead(7) == HIGH && resetLastInput == false){ 
    l1Value = 0;
    l2Value = 0;
    resetLastInput = true;
  }else if (digitalRead(7) == LOW){
    resetLastInput = false;
  }

  // invertVals input, inverts the pot values of L2 and L3
  if(digitalRead(16) == HIGH){
    invertVals = -1;
  }else{
    invertVals = 1;
  }
  
  // read l3 CV input and threshold Pot
  l3CurrentCV = analogRead(A1);  
  l3Threshold = analogRead(A3);

  // check L3 rise above/fall below switch, if HIGH L3 will change as the CV falls below the threshold, if LOW L3 changes when CV rises above threshold
  if (digitalRead(14) == HIGH) {
    l3RiseAbove = false;
  } else{
    l3RiseAbove = true;
  }

  // check valueWrap switch and gate input
  if (digitalRead(2) == HIGH && digitalRead(15) == LOW){
    valueWrap = true;
  } else if (digitalRead(2) == HIGH && digitalRead(15) == HIGH){
    valueWrap = false;
  } else if (digitalRead(2) == LOW && digitalRead(15) == LOW){
    valueWrap = false;
  } else if (digitalRead(2) == LOW && digitalRead(15) == HIGH){
    valueWrap = true;
  }

  // add global offset from CV input and Pot
  globalOffset = analogRead(A0) + analogRead(A2);

  // check for L1 condition, on rising edge change L2 CV output
  if (digitalRead(1) == HIGH && l1LastInput == 0) {
    l1Value = l1Value + analogRead(A6) - 512;
    if (l1Value >= 1023 && valueWrap == false){
      	l1Value = 1023;
      } else if (l1Value <= 0 && valueWrap == false){
      	l1Value = 0;
      }
    l1LastInput = 1;

  } else if (digitalRead(1) == LOW) {
    l1LastInput = 0;
  }


  // check for L2 condition, on falling edge change L2 CV output
  if (digitalRead(0) == LOW && l2LastInput == 1) {
    l2Value = l2Value + ((analogRead(A7) - 512)*invertVals); // invertVals inverts the value pot
    if (l2Value >= 1023 && valueWrap == false){
      	l2Value = 1023;
      } else if (l2Value <= 0 && valueWrap == false){
      	l2Value = 0;
      }
    l2LastInput = 0;

  } else if (digitalRead(0) == HIGH) {
    l2LastInput = 1;
  }

  // check for L3 condition, when CV rises above or falls below (according to switch) threshold, L3 cv output changes
  if (l3RiseAbove == true) {
    if (l3CurrentCV > l3Threshold && l3LastCVabove == false) {
      l3Value = l3Value + ((analogRead(A8) - 512)*invertVals); // invertVals inverts the value pot
      if (l3Value >= 1023 && valueWrap == false){
      	l3Value = 1023;
      } else if (l3Value <= 0 && valueWrap == false){
      	l3Value = 0;
      }
      l3LastCVabove = true;
    }else if (l3CurrentCV < l3Threshold && l3LastCVabove == true){
      l3LastCVabove = false;
    }

  } else {
    if (l3CurrentCV < l3Threshold && l3LastCVabove == true) {
      l3Value = l3Value + ((analogRead(A8) - 512)*invertVals); // invertVals inverts the value pot
      if (l3Value >= 1023 && valueWrap == false){
        l3Value = 1023;
      } else if (l3Value <= 0 && valueWrap == false){
        l3Value = 0;
      }
      l3LastCVabove = false;
    }else if (l3CurrentCV > l3Threshold && l3LastCVabove == false){
      l3LastCVabove = true;
    }
  }


  sumValue = l1Value + l2Value + l3Value; //add all output values together for SUM CV Output

  // output values
  if (valueWrap == false) {
    l1Output = constrain(l1Value + globalOffset, 0, 1023);  
    l2Output = constrain(l2Value + globalOffset, 0, 1023); 
    l3Output = constrain(l3Value + globalOffset, 0, 1023);
    sumOutput = constrain(sumValue + globalOffset, 0, 1023);    

  } else {
	l1Output = (l1Value + globalOffset)%1024;	
    l2Output = (l2Value + globalOffset)%1024;    
    l3Output = (l3Value + globalOffset)%1024;    
    sumOutput = (sumValue + globalOffset)%1024;
    
    // prevent values from increasing or decreasing indefinitely
    l1Value = l1Value%1024;  
    l2Value = l2Value%1024;
    l3Value = l3Value%1024;

  }

// write to CV outputs
analogWrite(3, map(l1Output,0,1023,0,255));
analogWrite(5, map(l2Output,0,1023,0,255));
analogWrite(9, map(l3Output,0,1023,0,255));
analogWrite(10, map(sumOutput,0,1023,0,255));


}
