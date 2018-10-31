// Bibliotecas
#include <NewPing.h>      // biblioteca que define parametros do sensor ultrasonico

// Variaveis de controle
#define distMax   50    // distancia maxima captada pelo sensor ultra em cm
#define nivelMin  15    // distancia entre sensor e nivel do tanque. Quanto maior for essa distancia, mais vazio estara o tanque   
#define nivelMax  5     // distancia entre sensor e nivel do tanque. Quanto menor for essa distancia, mais cheio estara o tanque

// Atribuindo pinos a variaveis
#define botEmergencia     2    // botao de emergencia para 
#define relBombaPrincipal 6    // modulo rele bomba principal (ligada entre tanque principal e tanque auxiliar)
#define relBombaPh        7    // modulo rele bomba pH (ligada entre tanque principal e tanque estabilizador de ph)
#define ledRedOff         9    // led indicativo de bomba OFF
#define ledGreOn          10   // led indicativo de bomba ON 
#define senTrig           11   // trigger = envia sinal ultrasonico
#define senEcho           12   // echo = le sinal ultrasonico de retorno

NewPing sonar(senTrig, senEcho, distMax);   // funçao da biblioteca NewPing para configuração de pinos e distância máxima do sensor ultra.

// Funçao SETUP
void setup() {
  Serial.begin(9600);
  pinMode(ledRedOff, OUTPUT);             // setando pino do led para saida
  pinMode(ledGreOn, OUTPUT);              // setando pino do led para saida
  pinMode(relBombaPrincipal, OUTPUT);     // setando pino do modulo rele para saida
}

// Funçao LOOP
void loop() {
  int nivelTanqueCm = sonar.ping_cm();     // funçao para obter distância em cm 

  Serial.print("Distancia do Sensor: ");
  Serial.print(nivelTanqueCm);
  Serial.println("cm");


  if(nivelTanqueCm <= nivelMax){             // verifica nivel tanque esta no maximo
    digitalWrite(ledGreOn, LOW); 
    digitalWrite(ledRedOff, HIGH);           // ledRedOff = indica bomba principal OFF
    delay(100);
    digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
  }

  if(nivelTanqueCm > nivelMin){              // verifica nivel tanque esta no minimo
    digitalWrite(ledGreOn, HIGH);            // ledGreOn = indica bomba principal ON
    digitalWrite(ledRedOff, LOW);  
    delay(100);    
    digitalWrite(relBombaPrincipal, LOW);    // pino rele em LOW, Bomba Principal ON
  }

  delay(500);    // delay para o sensor fazer leitura do sensor em 1/2 em 1/2 segundo
}

// Funçao EMERGENCIA
void emergencia(){
  /*
  Ativar buzzer para alertar q esta em estado de emergencia
  avaliar colocar delay ou colocar interrupçao para tratar troca do estado de LOW para HIGH, inicializar em HIGH se for usado troca de estado no LOOP   
  colocar botao escondido em outra porta para desativar a interrupçao  
   
   */

}

