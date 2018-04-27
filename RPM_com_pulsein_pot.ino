#define pot A0
#define pwm 11
#define en 7
#define en2 6
int pin = 2; //Entrada do sinal
unsigned long duration;
double aux = 0; //Usada para fazer o cálculo da frequência.

int val_pwm = 0;

void setup()

{
  Serial.begin(115200);
  pinMode(pin, INPUT);
  pinMode(pwm, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(en2, OUTPUT);

  digitalWrite(en, LOW);
}

void loop()
{
  //duration = pulseIn(pin, HIGH);
  aux = duration; //Atribui a aux o tempo que o sinal passou em nível alto (em microssegundos).
  aux = 500000/aux; // Como o pulsein mede apenas o tempo em nível alto e o valor está em microssegundos, dividor 500000 por pelo returno do pulsein nos dará a frequência.

  val_pwm = map(analogRead(pot), 0, 1023, 0, 255);
  analogWrite(pwm, val_pwm);
  
  if(millis() > 5000)
    digitalWrite(en2, HIGH);
  else
    digitalWrite(en2, LOW);
  
  //Serial.print(duration); //Imprime o tempo do sinal em nível alto.
  Serial.print(val_pwm);
  Serial.print("PWM | ");
  Serial.print(aux/2); //Imprime a frequência em Hz. Um pulsos.
  //delay(50);
  Serial.print("Hz | ");
  Serial.print((aux/2)*60);
  Serial.println(" RPM");
  delay(25);
  
}
