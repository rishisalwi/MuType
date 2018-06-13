const int thumb = A0;
const int index = A1;
const int middle = A2;
const int ring = A3;
const int pinky = A4;
int fingers[] = {thumb, index, middle, ring, pinky};
bool pressed[] = {false,false,false,false,false};
int calibrateLight[5];
int calibrateDark[5];
int total;
int poww;
bool doneCalibration = false;

int check;
String totalWord;
void raspiPrint(String x){
  Serial.println(x);
}
int analogReadFinger(int x){
  return analogRead(fingers[x]);
}
void calibrateL(){
  delay(1000);
  for (int i = 0; i < 5; i++){
    calibrateLight[i] = analogReadFinger(i);
  }
}
void calibrateD(){
  while (doneCalibration == false){
    doneCalibration = true;
    for (int i = 0; i < 5; i++){
      if (analogReadFinger(i) < calibrateLight[i]-50){
        calibrateDark[i] = analogReadFinger(i);
      }
      else{
        doneCalibration=false; 
      }
    }
    delay(1000);
    for (int i = 0; i < 5; i++){
      calibrateDark[i] = analogReadFinger(i);
    }
  }
}
bool readFinger(int x){
  int reading = analogReadFinger(x);
  if (reading - calibrateDark[x] < 80){
    return true;
  }
  else{
    return false;
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(thumb,INPUT);
  pinMode(index,INPUT);
  pinMode(middle,INPUT);
  pinMode(ring,INPUT);
  pinMode(pinky,INPUT);
  Serial.begin(9600);
  raspiPrint("Please lift your fingers");
  delay(1000);
  calibrateL();
  raspiPrint("Please drop your fingers");
  calibrateD();
  raspiPrint("OK. Time to Start!");
  delay(1000);
  
}

void loop() {
  check = false;
  for (int i = 0; i<5; i++){
    if (readFinger(i)){
      check = true;
      if (!pressed[i]){
        poww=1;
        for (int j = 0; j<i;j++){
          poww*=2;
        }
        total+=poww;
        pressed[i] = true;
      }
    }
  }
  /*Serial.print(readFinger(0));
  Serial.print("\t");
  Serial.print(readFinger(1));
  Serial.print("\t");
  Serial.print(readFinger(2));
  Serial.print("\t");
  Serial.print(readFinger(3));
  Serial.print("\t");
  Serial.print(readFinger(4));
  Serial.print("\t");
  raspiPrint(total);*/
  if (total>0 && !check){
    for (int i=0;i<5;i++){
      pressed[i]=false;
    }
    if (total==27){
      totalWord = totalWord + ".";
    }
    else if (total==28){
      totalWord = totalWord + "?";
    }
    else if (total==29){
      totalWord = totalWord + "!";
    }
    else if (total==30){
      totalWord = totalWord + " ";
    }
    else if (total < 27){
      totalWord = totalWord + char(total+96);
      //Serial.println(totalWord);
    }
    else{
      raspiPrint(totalWord);
      delay(1000);
      totalWord="";
    }
    
    total=0;
    
  }
}
