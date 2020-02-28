#include <Timing.h>
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#define ONE_WIRE_BUS 11
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
 
// Constantes usadas para leitura dos pins
const int botaoPin = 2;    // pin do PushButton
const int rele1 = 7;       // pin do Relé
const int led1 = 13;       // Led verde
const int led2 = 4;        // Led vermelho
const int sensoragua = 5;  // Sensor de nível da água
const int led3 = 12;       // Led Branco
 
// Variaveis mutaveis:
int releState = HIGH;         // Estado atual do relé (output pin)
int botaoState;               // Leitura atual do botão (input pin)
int lastBotaoState = LOW;     // Última leitura do botão(input pin)
int aguaState;
// Estado inicial dos leds:
int led1State = HIGH;
int led2State = LOW;
int led3State = LOW;
 
// Variáveis para corrigir o bounce do PushButton:
unsigned long lastDebounceTime = 0;  // Última ativação do botão
unsigned long debounceDelay = 50;    // Tempo de debounce
 
// Timer para medir a temperatura
Timing timerTemp;
Timing timerContador;
int contador = 11;
 
void setup(void) {
  lcd.begin(16,2);
  lcd.setCursor(0,1);
  lcd.print("Temperatura:"); 
 
  pinMode(botaoPin, INPUT);
  pinMode(rele1, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(sensoragua, INPUT);
 
  // Set inicial do Relé
  digitalWrite(rele1, releState);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
  digitalWrite(sensoragua, aguaState);
  digitalWrite(led3, led3State);
 
  //Iniciar leitura serial
  Serial.println("Monitoramento de Temperatura");
  sensors.begin();
  Serial.begin(9600);
}
 
 
void loop(void) {
 lcd.setBacklight(HIGH);
  
 // Caso a bóia dobre o led brando acende
 aguaState = digitalRead(sensoragua);
 if (aguaState == 0)
 {
 digitalWrite(led3, HIGH);
  lcd.setCursor(0,0);
  lcd.print("R:Cheio");
 }
 
 else
 {
 digitalWrite(led3, LOW);
  lcd.setCursor(0,0);
  lcd.print("R:Vazio");
 }
 
  int reading = digitalRead(botaoPin);
 
  // Check para verificar se o botão foi pressionado
  // desde a última verificação:
 
  // Se mudou verifique o bounce:
  if (reading != lastBotaoState) {
    // reseta o tempo de bounce
    lastDebounceTime = millis();
  }
 
  if ((millis() - lastDebounceTime) > debounceDelay) {
 
    // Se o estado do botão muda:
    if (reading != botaoState) {
      botaoState = reading;
 
      // Só altera o relé se o estado do botão for HIGH
      if (botaoState == HIGH) {
        releState = !releState;
        led1State = !led1State;
        led2State = !led2State;
      }
    }
  }
  if (releState == HIGH){
        lcd.setCursor(9,0);
        lcd.print("Bomba:D");
      }else{
        lcd.setCursor(9,0);
        lcd.print("Bomba:L");
      }
 
  // seta o relé:
  digitalWrite(rele1, releState);
  digitalWrite(led1, led1State);
  digitalWrite(led2, led2State);
 
  // Salva a leitura para próxima verificação
  lastBotaoState = reading;
 
 // Leitura da temperatura a cada 10 segundos
 if (timerTemp.onTimeout(10000)) {            // Definição do tempo
 Serial.print(" Leitura da temperatura: ");
 lcd.setCursor(0,1);
 lcd.print("Temperatura:");
 sensors.requestTemperatures();               // Comando para medir a temperatura
 Serial.println("Feita");
 Serial.print("Temperatura é: ");
 lcd.setCursor(0,1);
 lcd.print("Temperatura:");  
 Serial.print(sensors.getTempCByIndex(0));
 lcd.setCursor(12,1);
 lcd.print(sensors.getTempCByIndex(0));  
 Serial.print(" - ");
 }
}
