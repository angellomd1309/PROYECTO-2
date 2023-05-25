#include <Keypad.h>

#define SALIDA0 0
#define SALIDA1 1

const int filas = 4; // Número de filas en el teclado matricial
const int columnas = 4; // Número de columnas en el teclado matricial

// Definición de los pines utilizados para el teclado matricial
byte filaPIN[filas] = { 9, 8, 7 };
byte columnaPin[columnas] = { 5, 4, 3 };

// Definición de los códigos de tecla
char keys[filas][columnas] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}

};

Keypad MATRICIAL = Keypad(makeKeymap(keys), filaPIN, columnaPin, filas, columnas);

const int cantidadRELE = 5; // Número de LEDs a controlar
const int relePINS[cantidadRELE] = { 10, 11, 12, 13, A5 }; // Pines de los LEDs

const int cantidadBOTON = 5; // Número de botones
const int botonPINS[cantidadBOTON] = { A0, A1, A2, A3, A4 }; // Pines de los botones

bool estadoRELE[cantidadRELE] = { true }; // Estado de los LEDs

void setup() {
  ReleConfig();
  BotonConfig();

}

void loop() {
  char TECLA = MATRICIAL.getKey();

  // Control de los LEDs a través del teclado matricial
  if (TECLA) {
    if (TECLA >= '1' && TECLA <= '5') {
      int ledIndex = TECLA - '1'; // Mapea el valor de tecla a un índice de LED
      estadoRELE[ledIndex] = !estadoRELE[ledIndex]; // Invierte el estado del LED
      digitalWrite(relePINS[ledIndex], estadoRELE[ledIndex]);
    }
  }

  // Control de los LEDs a través de los botones adicionales
  for (int i = 0; i < cantidadBOTON; i++) {
    if (digitalRead(botonPINS[i]) == LOW) {
      estadoRELE[i] = !estadoRELE[i]; // Invierte el estado del LED
      digitalWrite(relePINS[i], estadoRELE[i]);
      delay(100); // Retardo para evitar lecturas múltiples debido a rebotes
    }
  }
}

int ReleConfig()
{
    for (int i = 0; i < cantidadRELE; i++) {
    pinMode(relePINS[i], OUTPUT);
    pinMode(SALIDA0, OUTPUT);
    pinMode(SALIDA1, OUTPUT);
    digitalWrite(relePINS[i], HIGH); // Apaga los LEDs al inicio
  }
      return (cantidadRELE);
}

int BotonConfig()
{
    for (int i = 0; i < cantidadBOTON; i++) {
    pinMode(botonPINS[i], INPUT_PULLUP);
  }
  return (cantidadBOTON);
}
