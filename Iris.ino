//Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>
#define INTERVALO_LEITURA 100 //(ms)

//conexão dos pinos para o sensor ultrasonico
//#define PIN_TRIGGER 4
//#define PIN_ECHO 5 
#define PIN 4
const int PIN_TRIGGER=4;
const int PIN_ECHO=5; 
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);  
// Module connection pins (Digital Pins)
#define CLK 14
#define DIO 13

//definição dos PINOS que utilizaremos para os LEDS e o BUZZER
#define PIN_WHITE_LED  16
#define PIN_GREEN_LED   17
#define PIN_YELLOW_LED  18
#define PIN_RED_LED   19
#define PIN_MOTOR   21

//variável responsável
unsigned int distancia = 0;
unsigned int distanciaAnterior = 0;
unsigned int contVibra = 0;
unsigned int buzzer = 1;


void setup()
{
  Serial.begin(9600);
  configurarPinos();
  Serial.println("Setup Iris!");
} 
void loop()
{
  verificarDistancia();
  delay(INTERVALO_LEITURA);
  Serial.println("Loop Iris!");
}
/*
configura o modos de cada um dos pinos que utilizaremos como saida
*/
void configurarPinos()
{
  pinMode(PIN_WHITE_LED,   OUTPUT);
  pinMode(PIN_GREEN_LED,  OUTPUT);
  pinMode(PIN_YELLOW_LED, OUTPUT);
  pinMode(PIN_RED_LED,  OUTPUT);
  pinMode(PIN_MOTOR,  OUTPUT);

}
/*
faz a leitura da distancia atual calculada pelo sensor
*/
int getDistance()
{
  //faz a leitura das informacoes do sensor (em cm)
  int distanciaCM;
  //long microsec = ultrasonic.timing();
  //distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);
  distanciaCM=ultrasonic.Ranging(CM);
  return distanciaCM;
}

/*
//verifica a distancia atual que o sensor ultrasonico esta lendo e em seguida, imprime o valor no display, e acende o led correspondente
*/
void verificarDistancia()
{
 
  distanciaAnterior = distancia;
  //recupera a distância atual lida pelo sensor
  distancia = getDistance();
    
  //imprime no display o valor lido
  //display.showNumberDec(distancia);
 
  Serial.print("Distancia: ");
  Serial.println(distancia);
  Serial.print("DistanciaAnterior: ");
  Serial.println(distanciaAnterior);
  Serial.print("contVibra: ");
  Serial.println(contVibra);
 
  //desliga o MOTOR
  digitalWrite(PIN_MOTOR, LOW);

  if ((distancia == 0 && distanciaAnterior > 55) ||
   (distanciaAnterior == 0 && distancia > 55)||
   (distancia == 0 && distanciaAnterior == 0 )) {
      //nao faz nada
  }
  //caso a distancia lida seja menor ou igual a 5, tomaremos como uma distância de perigo
   //então acenderemos o LED VERMELHO e ligaremos o BUZZER
  else if( distancia <= 55 )
  {
      apagaLedsExcetoVermelho();
      digitalWrite(PIN_RED_LED, HIGH);
      //if (contVibra <= 3){
      digitalWrite(PIN_MOTOR, HIGH);
      //contVibra ++;
  }

  //caso a distancia seja maior que 5 e menor ou igual a 20,
  //tomaremos como uma distância de atenção, e ligaremos o LED AMARELO
  else if(distancia <=75)
  {
      apagaLeds();
      digitalWrite(PIN_YELLOW_LED, HIGH);
      contVibra = 0;
  }
  //caso a distancia seja maior que 20 e menor ou igual a 55,
  //tomaremos como uma distância segura, e ligaremos o LED VERDE
  else if(distancia <= 95)
  {
      apagaLeds();
      digitalWrite(PIN_GREEN_LED, HIGH);
      contVibra = 0;
  }
  //para distâncias maiores que 55, tomaremos como uma distância sem perigo
  //acenderemos o LED BRANCO para indicar
  else
  {
    apagaLeds();
    digitalWrite(PIN_WHITE_LED, HIGH);
    contVibra = 0;
  }
}
void apagaLeds(){
  //esse FOR tem como objetivo apagar todos os LEDS que estejam acesos.
  for(int i=PIN_WHITE_LED; i<=PIN_RED_LED; i++)
  {
    digitalWrite(i, LOW);    
  }

}
void apagaLedsExcetoVermelho(){
  //esse FOR tem como objetivo apagar todos os LEDS que estejam acesos.
  for(int i=PIN_WHITE_LED; i<=PIN_YELLOW_LED; i++)
  {
    digitalWrite(i, LOW);    
  }
}
void alternaBuzzer(){
  if(buzzer){
    digitalWrite(PIN_MOTOR, HIGH);
    buzzer=0;
  } else {
     digitalWrite(PIN_MOTOR, LOW);
     buzzer=1;
  }
}
