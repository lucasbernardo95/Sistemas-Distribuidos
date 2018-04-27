volatile uint32_t rpm;//Váriavel que armazena as rotações.
 
 
void setup()
{
   Serial.begin(115200);//Inicia a Serial.
   attachInterrupt(0, ctt, RISING);//Habilita a interrupção 0 no pino 2.
}
 
void loop()
{
   delay(1000);//Aguarda um segundo.
   Serial.print(rpm * 30);//Mostra o RPM.
   Serial.println(" RPM");//Mostra o RPM.
   
   rpm = 0;//Reseta a váriavel para proxima leitura.
 
   //Como estamos lendo a cada UM segundo, usariamos RPM*60 para determinar as rotações por minuto,
   //porém, como foi dito na explicação, é preciso dividir o valor por 2, por isto 30!
}
 
void ctt()
{
   rpm++;//Incrementa a váriavel.
}
