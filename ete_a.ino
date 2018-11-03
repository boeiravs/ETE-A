// Bibliotecas
#include <NewPing.h>      // biblioteca que define parametros do sensor ultrasonico

// Variaveis de controle
#define distMax   200    // distancia maxima captada pelo sensor ultra em cm
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
  pinMode(relBombaPrincipal, OUTPUT);     // setando pino do modulo rele bomba principal para saida
  pinMode(relBombaPh, OUTPUT);            // setando pino do modulo rele bomba pH para saida

  randomSeed(analogRead(A0));             // sorteando listas da função random para melhorar precisão no sorteio
}

// Funçao LOOP
void loop() {
  int nivelTanqueCm = sonar.ping_cm();     // funçao para obter distância em cm
  int ph, cont=1;

  if(nivelTanqueCm != 0){
    Serial.print("Nivel do Tanque Principal: ");
    Serial.print(nivelTanqueCm);
    Serial.println(" cm");
  
    if(nivelTanqueCm < nivelMax){             // verifica nivel tanque esta menor que o maximo
      digitalWrite(ledGreOn, LOW); 
      digitalWrite(ledRedOff, HIGH);           // ledRedOff = indica bomba principal OFF
      delay(100);
      digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
      digitalWrite(relBombaPh, HIGH);          // pino rele em HIGH, Bomba pH OFF
    }
    
    if(nivelTanqueCm == nivelMax){             // verifica nivel tanque esta no maximo
      digitalWrite(ledGreOn, LOW); 
      digitalWrite(ledRedOff, HIGH);           // ledRedOff = indica bomba principal OFF
      delay(100);
      digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
      
      ph = random(7,15);  // pH = 7 neutro // quanto mais alto que 7 for o pH mais alcalino é o efluente
      if(ph == 7){
        nivelMax == 4;
        Serial.print("pH NEUTRO detectado: ");
        Serial.println(ph);
        delay(1000);        
      }else{
        FunPh(ph);
      }
    }
  
    if(nivelTanqueCm > nivelMin){              // verifica nivel tanque esta no minimo
      digitalWrite(ledGreOn, HIGH);            // ledGreOn = indica bomba principal ON
      digitalWrite(ledRedOff, LOW);  
      delay(100);    
      digitalWrite(relBombaPrincipal, LOW);    // pino rele em LOW, Bomba Principal ON
      digitalWrite(relBombaPh, HIGH);          // pino rele em HIGH, Bomba pH OFF
    }
    
    delay(1000);    // delay para o sensor fazer leitura
  }
}

// Funçao EMERGENCIA
void FunEmergencia(){
  /*
  Ativar buzzer para alertar q esta em estado de emergencia
  avaliar colocar delay ou colocar interrupçao para tratar troca do estado de LOW para HIGH, inicializar em HIGH se for usado troca de estado no LOOP   
  colocar botao escondido em outra porta para desativar a interrupçao  
   
   */
}

void FunPh(int ph){
    switch(ph){
      case 8:
        digitalWrite(relBombaPh, LOW);   // pH = 8 --> bomba acionada por 1s
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        Serial.print("pH detectado: ");
        Serial.println(ph);
        delay(3000);
      break;
      case 9:
        digitalWrite(relBombaPh, LOW);   // pH = 9 --> bomba acionada por 2s
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        Serial.print("pH detectado: ");
        Serial.println(ph);
        delay(4000);
      break;      
      case 10:
        digitalWrite(relBombaPh, LOW);   // pH = 10 --> bomba acionada por 3s
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        Serial.print("pH detectado: ");
        Serial.println(ph);
        delay(5000);
      break;
      case 11:
        digitalWrite(relBombaPh, LOW);   // pH = 11 --> bomba acionada por 4s
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        Serial.print("pH detectado: ");
        Serial.println(ph);
        delay(6000);
      break;      
      case 12:
        digitalWrite(relBombaPh, LOW);   // pH = 12 --> bomba acionada por 5s
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        Serial.print("pH detectado: ");
        Serial.println(ph);
        delay(7000);
      break;
      case 13:
        digitalWrite(relBombaPh, LOW);   // pH = 13 --> bomba acionada por 6s
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        Serial.print("pH detectado: ");
        Serial.println(ph);
        delay(8000);
      break;
      case 14:
        digitalWrite(relBombaPh, LOW);   // pH = 14 --> bomba acionada por 7s
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        Serial.print("pH detectado: ");
        Serial.println(ph);
        delay(9000);
      break;
    }
}
