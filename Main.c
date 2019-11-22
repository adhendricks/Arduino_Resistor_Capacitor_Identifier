// Initialize Pins
int analogPin = 0;
int chargePin = 3;
int dischargePin = 11; //speeds up discharging process, not necessary though

// Initialize Resistor
float resistorValue = 10000.0;
float dischargeresistor = 510.0; // Enter correct value
float parallelResistance = (resistorValue*dischargeresistor)/(resistorValue+dischargeresistor);
float unknownResistor = 0;

// Initialize Timer
unsigned long startTime;
unsigned long elapsedTime;

// Initialize Capacitance Variables
float microFarads;                
float nanoFarads;

void setup()
{
  pinMode(chargePin, OUTPUT);  
  pinMode(dischargePin, OUTPUT);      
  digitalWrite(chargePin, HIGH);
  digitalWrite(dischargePin, LOW);
 
  Serial.begin(9600); // Necessary to print data to serial monitor over USB
}

float VoltageValue = 0;
double val=0;
int state=0; 
double initVal=0;
int counter=0; 
void loop()
{
  startTime=millis();
  state=1;
while((val=analogRead(analogPin)) < 648){
  if(counter < 100000){
    counter++;
  }
  if(counter == 5){
    initVal= val;
    state=0;
  }
  else if((counter == 100)&& (initVal==val)){
    state=1; //Resistor
    break;
  }
}
//Reset counter

if (state == 1){
  pinMode(dischargePin, OUTPUT);
  digitalWrite(chargePin, HIGH);
  digitalWrite(dischargePin, HIGH);
  //check resistor
  VoltageValue = ((float)analogRead(analogPin)/1024.0) * 5.0;

unknownResistor = ((float)VoltageValue * (float)parallelResistance) / (5.0 - VoltageValue);
Serial.print(unknownResistor);
Serial.print("ohm");
}
else{
  //check capacitor
   
  elapsedTime= millis() - startTime; // Determines how much time it took to charge capacitor
  microFarads = ((float)elapsedTime / resistorValue) * 1000;
  Serial.print(microFarads);      
  Serial.print(" uF    "); 
}
digitalWrite(chargePin, LOW); // Stops charging capacitor
  pinMode(dischargePin, OUTPUT); 
  digitalWrite(dischargePin, LOW); // Allows capacitor to discharge    
  while(analogRead(analogPin) > 0)
  {
    // Do nothing until capacitor is discharged      
  }

  pinMode(dischargePin, INPUT); // Prevents capacitor from discharging  

//Pin Reset
  pinMode(chargePin, OUTPUT);  
  pinMode(dischargePin, INPUT);      
  digitalWrite(chargePin, HIGH);
  digitalWrite(dischargePin, LOW);

counter=0;
state=0;
}
