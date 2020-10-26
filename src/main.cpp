#include <Arduino.h>
#include <CayenneMQTTESP32.h>
#include "password.h" // ssid e senha do wifi

#define CAYENNE_PRINT Serial
const int LED = 2;
const int LDR = 35;
const int LIMIT = 3100;
int LED_STATE = 0;

// Autenticação do Cayenne. Essas informaçações são obtidas do Dashboard Cayenne.
char username_cayenne[] = "a69900f0-0979-11eb-a2e4-b32ea624e442";
char password_cayenne[] = "a89f9c828b41b745de5532850906652b4ad56717";
char clientID_cayenne[] = "fe1f7640-17e0-11eb-b767-3f1a8f1211ba";

void setup()
{
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  Cayenne.begin(username_cayenne, password_cayenne, clientID_cayenne, ssid, wifi_password);
  delay(1000);
}

void loop()
{
  Serial.println(analogRead(LDR));
  Cayenne.loop();
  delay(500);
}

/*Função que envia os dados do sensor em um intervalo de 15s para o Cayenne. 
* Objetivo: Enviar o valor atual do LDR
* Parâmetros : Canal de envio
* Retorno : Nenhum
*/
CAYENNE_OUT(0)
{
  Cayenne.luxWrite(0, analogRead(LDR));
  //Cayenne.virtualWrite(0, analogRead(LDR));
  //Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

/*Função que recebe os dados do Cayenne. 
* Objetivo: Recebe o estado do LED
* Parâmetros : Canal de envio
* Retorno : Nenhum
*/
CAYENNE_IN(1)
{
  LED_STATE = getValue.asInt();
  digitalWrite(LED, LED_STATE); // Muda estado do LED
  //Cayenne.virtualWrite(1, LED_STATE,"digital_actuator", "d");
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString()); // Exibe no serial
}

// Encaminha o estado atual do LED para o canl 1 ( canal do LED no dashboard cayenne)
CAYENNE_OUT(1)
{
  Cayenne.digitalSensorWrite(1,LED_STATE);
  //Cayenne.virtualWrite(1, LED_STATE);
}
