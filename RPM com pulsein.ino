int pin = 2; //Entrada do sinal
unsigned long duration;
double aux = 0; //Usada para fazer o cálculo da frequência.

void setup() {
  Serial.begin(115200);
  pinMode(pin, INPUT);
}

void loop() {
  duration = pulseIn(pin, HIGH);
  aux = duration; //Atribui a aux o tempo que o sinal passou em nível alto (em microssegundos).
  aux = 500000/aux; // Como o pulsein mede apenas o tempo em nível alto e o valor está em microssegundos, dividor 500000 por pelo returno do pulsein nos dará a frequência.
  Serial.print(duration); //Imprime o tempo do sinal em nível alto.
  Serial.print(" | ");
  Serial.print(aux); //Imprime a frequência em Hz. Dois pulsos.
  //delay(50);
  Serial.print(" | ");
  Serial.print((aux/2)*60);
  Serial.println(" RPM");
  delay(100);
  
}
