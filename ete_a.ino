#include <MicroLCD.h>    // biblioteca que define parametros do display oled
#include <Wire.h>        // biblioteca que permite comunicaçao com dispositivos I2C (display oled)

// Bibliotecas
//#include <TimerOne.h>   // biblioteca de timer, utilizado para fazer a interrupçao 

// Variaveis de controle do skech
#define nivelMin  13        // distancia entre sensor e nivel do tanque. Quanto maior for essa distancia, mais vazio estara o tanque   
#define nivelMax  10         // distancia entre sensor e nivel do tanque. Quanto menor for essa distancia, mais cheio estara o tanque
//bool alarmeAtivado = false; // variavel para controlar alarme da interrupçao 
float senTempo;             // tempo de ida e volta do sinal ultrassonico do sensor
float nivelTanqueCm;        // armazena nivel do tanque em cm
int ph;                     // armazena o valor de pH detectado 
LCD_SSD1306 lcd;            // criando objeto lcd de acordo com o modelo do display oled CI SSD1306

// Atribuindo pinos do Arduino para variaveis
#define botEmergencia     2   // botao de emergencia para interrupçao (INT0)
#define relBombaPrincipal 3   // modulo rele bomba principal (ligada entre tanque principal e tanque auxiliar)
#define relBombaPh        5   // modulo rele bomba pH (ligada entre tanque principal e tanque estabilizador de ph)
#define senTrig           6   // trigger = envia sinal ultrasonico
#define senEcho           7   // echo = le sinal ultrasonico de retorno
#define buzEmergencia     8   // buzzer da funcao emergencia
// define Ethernet        4,10,11,12,13
#define inAnalog          A0  // porta para sorteio das listas na funçao random
#define ledRedOff         A1  // led indicativo de bombas OFF
#define ledGreOn          A2  // led indicativo de relBombaPrincipal ON 
#define ledYelOn          A3  // led indicativo de relBombaPh ON
// define oled            A4,A5    // portas de comunicaçao I2C

//****************************** Funçao SETUP ******************************************************
void setup() {
  lcd.begin();                            // inicializando display oled
  pinMode(botEmergencia, INPUT_PULLUP);   // setando pino do botao de emergencia
  pinMode(relBombaPrincipal, OUTPUT);     // setando pino do modulo rele bomba principal para saida
  pinMode(relBombaPh, OUTPUT);            // setando pino do modulo rele bomba pH para saida
  pinMode(senTrig, OUTPUT);               // setando pino do trig do sensor para saida
  pinMode(senEcho, INPUT);                // setando pino do echo do sensor para entrada
  pinMode(buzEmergencia, OUTPUT);         // setando pino do buzzer saida
  pinMode(ledRedOff, OUTPUT);             // setando pino do led para saida
  pinMode(ledGreOn, OUTPUT);              // setando pino do led para saida
  pinMode(ledYelOn, OUTPUT);              // setando pino do led para saida
  
//  digitalWrite(relBombaPrincipal, HIGH);  // inicializamos o relBombaPrincipal em high
//  digitalWrite(relBombaPh, HIGH);         // inicializamos o relBombaPh em high
  digitalWrite(senTrig, LOW);             // inicializamos o trig em low
  delayMicroseconds(10);

  randomSeed(analogRead(inAnalog));             // sorteando listas da função random para melhorar precisão no sorteio
//  attachInterrupt(0, FunEmergencia, FALLING); // interrupçao para executar a funçao emergencia, sem que o bot
}

//****************************** Funçao LOOP ******************************************************
void loop() {
  digitalWrite(senTrig, HIGH);          // disparando pulso ultrassonico
  delayMicroseconds(10);                
  digitalWrite(senTrig, LOW);           
  senTempo = pulseIn(senEcho, HIGH);    // calculo de tempo de ida e volta do pulso
  nivelTanqueCm = senTempo / 29.4 / 2;  // convertendo tempo de ida e volta para cm

  lcd.setCursor(40,0);
  lcd.setFontSize(FONT_SIZE_MEDIUM);
  lcd.println("ETE-A"); 
  
  if(nivelTanqueCm != 0){
/*    Serial.print("Nivel do Tanque Principal: ");
    Serial.print(nivelTanqueCm);
    Serial.println(" cm");
*/
    lcd.print("Nivel: ");
    lcd.print(nivelTanqueCm);
    lcd.println("cm");

    if(nivelTanqueCm < nivelMax){             // verifica nivel tanque esta menor que o maximo
      digitalWrite(ledRedOff, HIGH);          // ledRedOff = indica bomba principal OFF
      digitalWrite(ledGreOn, LOW);
      digitalWrite(ledYelOn, LOW);          
      delay(100);
      digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
      digitalWrite(relBombaPh, HIGH);          // pino rele em HIGH, Bomba pH OFF
    }
    
    if(nivelTanqueCm > (nivelMax - 0.3) && nivelTanqueCm < (nivelMax + 0.3)){             // verifica nivel tanque esta no maximo
      digitalWrite(ledRedOff, LOW);          
      digitalWrite(ledGreOn, LOW);
      digitalWrite(ledYelOn, HIGH);            // ledYelOn = indica bomba pH ON
      delay(100);
      digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF

      ph = random(7,15);  // pH = 7 neutro // quanto mais alto que 7 for o pH mais alcalino é o efluente
      if(ph == 7){
// implementar milis aqui para nao acionar pH qndo estiver descartando

//        Serial.print("pH NEUTRO detectado: ");
//        Serial.println(ph);
        lcd.print("pH neutro: ");
        lcd.println(ph);         
        delay(1000);
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
        digitalWrite(relBombaPh, HIGH);          // pino rele em HIGH, Bomba pH OFF       
      }else{
        FunPh(ph);
      }
    }
  
    if(nivelTanqueCm > nivelMin){              // verifica nivel tanque esta no minimo
      digitalWrite(ledRedOff, LOW);          
      digitalWrite(ledGreOn, HIGH);            // ledGreOn = indica bomba principal ON
      digitalWrite(ledYelOn, LOW);
      delay(100);    
      digitalWrite(relBombaPrincipal, LOW);    // pino rele em LOW, Bomba Principal ON
      digitalWrite(relBombaPh, HIGH);          // pino rele em HIGH, Bomba pH OFF
    }
    
    delay(1000);    // delay para o sensor fazer leitura
  }
}

//****************************** Funçao EMERGENCIA ***************************************************
/*
void FunEmergencia(){
  if(alarmeAtivado){                           // variavel inicializada em false
    digitalWrite(buzEmergencia, LOW);          // buzzer Off
    alarmeAtivado = false;                     // alarme desativado, condiçao normal
    Timer1.detachInterrupt();                  // interrupçao nao ativada
  }else{
//    digitalWrite(buzEmergencia, HIGH);         // buzzer On
    digitalWrite(relBombaPrincipal, HIGH);     // pino rele em HIGH, Bomba Principal OFF
    digitalWrite(relBombaPh, HIGH);            // pino rele em HIGH, Bomba pH OFF
    alarmeAtivado = true;                      // alarme ativado, quando botao emergencia for apertado
    Timer1.initialize(1000000);                // 1 segundo
    Timer1.attachInterrupt(FunAlarme);         // interrupçao ativada e chamando funçao para tocar buzzer
  }
}
*/
//****************************** Funçao ALARME ******************************************************
void FunAlarme(){
  digitalWrite(buzEmergencia, !digitalRead(buzEmergencia)); // trocando status do buzzer de 1 em 1s
}

//****************************** Funçao PH **********************************************************
void FunPh(int ph){
    switch(ph){
      case 8:
        digitalWrite(relBombaPh, LOW);   // pH = 8 --> bomba acionada de acordo com o dalay
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
//        Serial.print("pH detectado: ");
//        Serial.println(ph);
        lcd.print("pH: ");
        lcd.println(ph);       
        delay(3000);
        digitalWrite(relBombaPh, HIGH);
      break;
      case 9:
        digitalWrite(relBombaPh, LOW);   // pH = 9 --> bomba acionada de acordo com o dalay
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
//        Serial.print("pH detectado: ");
//        Serial.println(ph);
        lcd.print("pH: ");
        lcd.println(ph);       
        delay(4000);
        digitalWrite(relBombaPh, HIGH);
      break;      
      case 10:
        digitalWrite(relBombaPh, LOW);   // pH = 10 --> bomba acionada de acordo com o dalay
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
//        Serial.print("pH detectado: ");
//        Serial.println(ph);
        lcd.print("pH: ");
        lcd.println(ph);       
        delay(5000);
        digitalWrite(relBombaPh, HIGH);
      break;
      case 11:
        digitalWrite(relBombaPh, LOW);   // pH = 11 --> bomba acionada de acordo com o dalay
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
//        Serial.print("pH detectado: ");
//        Serial.println(ph);
        lcd.print("pH: ");
        lcd.println(ph);       
        delay(6000);
        digitalWrite(relBombaPh, HIGH);
      break;      
      case 12:
        digitalWrite(relBombaPh, LOW);   // pH = 12 --> bomba acionada de acordo com o dalay
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
//        Serial.print("pH detectado: ");
//        Serial.println(ph);
        lcd.print("pH: ");
        lcd.println(ph);       
        delay(7000);
        digitalWrite(relBombaPh, HIGH);
      break;
      case 13:
        digitalWrite(relBombaPh, LOW);   // pH = 13 --> bomba acionada de acordo com o dalay
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
//        Serial.print("pH detectado: ");
//        Serial.println(ph);
        lcd.print("pH: ");
        lcd.println(ph);       
        delay(8000);
        digitalWrite(relBombaPh, HIGH);
      break;
      case 14:
        digitalWrite(relBombaPh, LOW);   // pH = 14 --> bomba acionada de acordo com o dalay
        digitalWrite(relBombaPrincipal, HIGH);   // pino rele em HIGH, Bomba Principal OFF
//        Serial.print("pH detectado: ");
//        Serial.println(ph);
        lcd.print("pH: ");
        lcd.println(ph);       
        delay(9000);
        digitalWrite(relBombaPh, HIGH);
      break;
    }
}
