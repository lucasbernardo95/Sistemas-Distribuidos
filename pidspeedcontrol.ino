/******************************************************************
 * PID Simple Example (Augmented with Processing.org Communication)
 * Version 0.3
 * by Brett Beauregard
 * License: Creative-Commons Attribution Share-Alike
 * April 2011
 ******************************************************************/
 /* Alterado por Josenalde Oliveira em 18.04.2018 e 01/05/2018 */
 
/* PID control RPM Fan */
#include <PID_v1.h>

// signal pins
int inputPin = 2; // Hall sensor speed signal Digital Pin 2
int outputPin = 3; //D3~ for pwm control signal In1 Pin L298N driver, In2 = GND for clockwise direction

unsigned long duration;
unsigned long serialTime; //this will help us know when to talk with processing
double aux = 0; // To calculate frequency
double rpmSetpoint, Setpoint, rpmInput, Input, Output;

const int rpmMax = 5000; // set the maximum speed for each fan

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 10,1,0, DIRECT);

void setup()
{
  Serial.begin(9600); // 115200 for rpm readings !!!
  pinMode(inputPin, INPUT);
  //initialize the variables we're linked to
  Input = 0;
  rpmSetpoint = 2800; // RPM
  Setpoint = map(rpmSetpoint, 0, rpmMax, 0, 1023);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}
void loop()
{
  // read speed signal and convert it to RPM (bipolar hall effect sensor)
  duration = pulseIn(inputPin, HIGH);
  aux = duration; //Atribui a aux o tempo que o sinal passou em nível alto (em microssegundos).
  aux = 500000/aux; /* Como o pulsein mede apenas o tempo em nível alto e o valor está em microssegundos, 
                       dividir 500000 pelo retorno do pulsein nos dará a frequência.
                       freq (Hz) = 0.5 x 10^-6 / (aux * 10^-6) = 500000 / aux */
  rpmInput = (aux/2)*60;
  Input = map(rpmInput, 0, rpmMax, 0, 1023);

  myPID.Compute();
  analogWrite(outputPin, Output);

  if(millis() > serialTime)
  {
    SerialReceive();
    SerialSend();
    serialTime+=100; // communicate at each 100 ms = 0.1s
  }
}

/********************************************
 * Serial Communication functions / helpers
 ********************************************/


union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array



// getting float values from processing into the arduino
// was no small task.  the way this program does it is
// as follows:
//  * a float takes up 4 bytes.  in processing, convert
//    the array of floats we want to send, into an array
//    of bytes.
//  * send the bytes to the arduino
//  * use a data structure known as a union to convert
//    the array of bytes back into an array of floats

//  the bytes coming from the arduino follow the following
//  format:
//  0: 0=Manual, 1=Auto, else = ? error ?
//  1: 0=Direct, 1=Reverse, else = ? error ?
//  2-5: float setpoint
//  6-9: float input
//  10-13: float output  
//  14-17: float P_Param
//  18-21: float I_Param
//  22-245: float D_Param
void SerialReceive()
{

  // read the bytes sent from Processing
  int index=0;
  byte Auto_Man = -1;
  byte Direct_Reverse = -1;
  while(Serial.available()&&index<26)
  {
    if(index==0) Auto_Man = Serial.read();
    else if(index==1) Direct_Reverse = Serial.read();
    else foo.asBytes[index-2] = Serial.read();
    index++;
  } 
  
  // if the information we got was in the correct format, 
  // read it into the system
  if(index==26  && (Auto_Man==0 || Auto_Man==1)&& (Direct_Reverse==0 || Direct_Reverse==1))
  {
    Setpoint=double(foo.asFloat[0]);
    //Input=double(foo.asFloat[1]);       // * the user has the ability to send the 
                                          //   value of "Input"  in most cases (as 
                                          //   in this one) this is not needed.
    if(Auto_Man==0)                       // * only change the output if we are in 
    {                                     //   manual mode.  otherwise we'll get an
      Output=double(foo.asFloat[2]);      //   output blip, then the controller will 
    }                                     //   overwrite.
    
    double p, i, d;                       // * read in and set the controller tunings
    p = double(foo.asFloat[3]);           //
    i = double(foo.asFloat[4]);           //
    d = double(foo.asFloat[5]);           //
    myPID.SetTunings(p, i, d);            //
    
    if(Auto_Man==0) myPID.SetMode(MANUAL);// * set the controller mode
    else myPID.SetMode(AUTOMATIC);             //
    
    if(Direct_Reverse==0) myPID.SetControllerDirection(DIRECT);// * set the controller Direction
    else myPID.SetControllerDirection(REVERSE);          //
  }
  Serial.flush();                         // * clear any random data from the serial buffer
}

// unlike our tiny microprocessor, the processing ap
// has no problem converting strings into floats, so
// we can just send strings.  much easier than getting
// floats from processing to here no?
void SerialSend()
{
  Serial.print("PID ");
  Serial.print(Setpoint);   
  Serial.print(" ");
  Serial.print(Input);   
  Serial.print(" ");  
  Serial.print(Output);   
  Serial.print(" ");
  
  //Serial.print(rpmInput);   
  //Serial.print(" ");
  Serial.print(myPID.GetKp());   
  Serial.print(" ");
  Serial.print(myPID.GetKi());   
  Serial.print(" ");
  Serial.print(myPID.GetKd());   
  Serial.print(" ");
  if(myPID.GetMode()==AUTOMATIC) Serial.print("Automatic");
  else Serial.print("Manual");  
  Serial.print(" ");
  if(myPID.GetDirection()==DIRECT) Serial.println("Direct");
  else Serial.println("Reverse");
}

