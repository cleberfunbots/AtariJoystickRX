/*
      Programa para Receptor de Atari - RF Nano Joystick FunBots

      Baseado nos vídeos:
        RF Nano: https://youtu.be/MDSvQfHoJ_g
        Leitura 18650: https://youtu.be/GJ3ypx0nxBY e https://youtu.be/o_71fRD2R08
        Joystick: https://youtu.be/w1wScVYknD0

      Componentes:
        - Arduino RF Nano
        - 5x Transistor 2N3904
        - 5x Resistor 56k

      Versão 1.0 - Versão inicial - 02/Ago/2023

 *    * Criado por Cleber Borges - FunBots - @cleber.funbots  *     *

      Instagram: https://www.instagram.com/cleber.funbots/
      Facebook: https://www.facebook.com/cleber.funbots
      YouTube: https://www.youtube.com/channel/UCKs2l5weIqgJQxiLj0A6Atw
      Telegram: https://t.me/cleberfunbots

*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

uint64_t address[6] = {0x7878787878LL,
                       0xB3B4B5B6F1LL,
                       0xB3B4B5B6CDLL,
                       0xB3B4B5B6A3LL,
                       0xB3B4B5B60FLL,
                       0xB3B4B5B605LL
                      };

RF24 radio(10, 9);

struct joystick {
  int potLX;
  int potLY;
  int potRX;
  int potRY;
  byte buttons;
  char message[7];
};

joystick joystickData;
joystick payload;

bool SW1 = false;
bool SW2 = false;
bool SW3 = false;
bool SW4 = false;
bool SW5 = false;
bool SW6 = false;
bool SW7 = false;


int ledPin = 3;
int upPin = 8;
int downPin = 7;
int rightPin = 5;
int leftPin = 6;
int firePin = 4;

void setup() {

  pinMode (ledPin, OUTPUT);
  pinMode (upPin, OUTPUT);
  pinMode (downPin, OUTPUT);
  pinMode (leftPin, OUTPUT);
  pinMode (rightPin, OUTPUT);
  pinMode (firePin, OUTPUT);

  //Serial.begin(115200);
  //Serial.println("");
  //Serial.println("Iniciando RX");
  
  delay(1000);

  while (!radio.begin()) {
    //Serial.println(F("Radio não responde!!"));
    digitalWrite(ledPin, HIGH);
    delay (1000); // preso em loop
    radio.begin();
    digitalWrite(ledPin, LOW);
  }

  memcpy(payload.message, "ATARI", 6);

  radio.enableDynamicPayloads();
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, address[3]);
  radio.startListening();

  digitalWrite(ledPin, HIGH);

}

void loop() {

  if (radio.available()) {
    radio.read(&joystickData, sizeof(joystick));
    radio.writeAckPayload(1, &payload, sizeof(payload));
  } 

  SW1 = bitRead(joystickData.buttons, 1);
  SW2 = bitRead(joystickData.buttons, 2);
  SW3 = bitRead(joystickData.buttons, 3);
  SW4 = bitRead(joystickData.buttons, 4);
  SW5 = bitRead(joystickData.buttons, 5);
  SW6 = bitRead(joystickData.buttons, 6);
  SW7 = bitRead(joystickData.buttons, 7);

  digitalWrite(firePin, SW7);

  if (joystickData.potLX > 700) {
    digitalWrite(leftPin, HIGH);
  } else {
    digitalWrite(leftPin, LOW);
  }

  if (joystickData.potLX < 300) {
    digitalWrite(rightPin, HIGH);
  } else {
    digitalWrite(rightPin, LOW);
  }

  if (joystickData.potLY > 700) {
    digitalWrite(upPin, HIGH);
  } else {
    digitalWrite(upPin, LOW);
  }

  if (joystickData.potLY < 300) {
    digitalWrite(downPin, HIGH);
  } else {
    digitalWrite(downPin, LOW);
  }

}
