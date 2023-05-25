/*
FUNDACION KINAL
CENTRO EDUCATIVO TECNICO LARELEORAL KINAL
PERITO EN ELECTRONICA
QUINTO PERITO
SECCION: A
CURSO: TALLER DE ELECTRONICA DIGITAL Y REPARACION DE COMPUTADORAS
ALUMNOS: MIGUEL ANGEL ALVARADO GONZALEZ, JORGE DIEGO SEBASTIAN QUEME ANDRINO, ANGELLO GABRIEL MANSILLA DIEGUEZ
CARNETS: 2022486, 2021430, 2022380
PRACTICA 4 DISEÑO DE FIRMWARE
*/




#include <Keypad.h>

#define SALIDA0 2
#define SALIDA1 3

const int filas = 4; // numero de filas en el teclado matricial
const int columnas = 4; // numero de columnas en el teclado matricial

// definir de los pines utilizados para el teclado matricial
byte filaPIN[filas] = { 9, 8, 7 };
byte columnaPin[columnas] = { 6, 5, 4 };

// definir de los códigos de tecla
char keys[filas][columnas] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}

};

Keypad MATRICIAL = Keypad(makeKeymap(keys), filaPIN, columnaPin, filas, columnas);

const int cantidadRELE = 5; // numero de rele a controlar
const int relePINS[cantidadRELE] = { 10, 11, 12, 13, A5 }; // pins de los LEDs

const int cantidadBOTON = 5; // numero de botones
const int botonPINS[cantidadBOTON] = { A0, A1, A2, A3, A4 }; // pins de los botones

bool estadoRELE[cantidadRELE] = { true }; // estado de los rele

void setup() {
  ReleConfig();
  BotonConfig();

}

void loop() {
  char TECLA = MATRICIAL.getKey();

 
  if (TECLA) {  // Control de los LEDs a través del teclado matricial
    if (TECLA >= '1' && TECLA <= '5') {
      int ledIndex = TECLA - '1'; 
      estadoRELE[ledIndex] = !estadoRELE[ledIndex]; // Invierte el estado del rele
      digitalWrite(relePINS[ledIndex], estadoRELE[ledIndex]);
    }
  }

  // Control de los rele a través de los botones adicionales
  for (int i = 0; i < cantidadBOTON; i++) {
    if (digitalRead(botonPINS[i]) == LOW) {
      estadoRELE[i] = !estadoRELE[i]; // Invierte el estado del rele
      digitalWrite(relePINS[i], estadoRELE[i]);
      delay(100); //antirebote
    }
  }
}

int ReleConfig()
{
    for (int i = 0; i < cantidadRELE; i++) {
    pinMode(relePINS[i], OUTPUT);
    pinMode(SALIDA0, OUTPUT);
    pinMode(SALIDA1, OUTPUT);
    digitalWrite(relePINS[i], HIGH); // Apaga los rele al inicio
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
