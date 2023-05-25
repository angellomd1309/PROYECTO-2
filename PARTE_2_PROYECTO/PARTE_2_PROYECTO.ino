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

#include <Wire.h>              
#include <LiquidCrystal_I2C.h> 
#include <Servo.h>                           
#include <OneWire.h>           
#include <DallasTemperature.h> 
Servo miServo;   
const int tecladoFilas = 4;       // Número de filas del teclado
const int tecladoColumnas = 4;    // Número de columnas del teclado
char teclas[tecladoFilas][tecladoColumnas] = {     // Definición de las teclas del teclado
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinFilas[tecladoFilas] = {2};              // Pines conectados a las filas
byte pinColumnas[tecladoColumnas] = {3};  // Pines conectados a las columnas

const int pinBoton = 8;       // Pin conectado al botón
const int pinServo = 9;       // Pin conectado al servo
const int pinVentilador = 10; // Pin conectado al ventilador

const int pinSensorTemperatura = 6; // Pin conectado al sensor de temperatura
OneWire oneWire(pinSensorTemperatura);               // Crea un objeto OneWire
DallasTemperature sensors(&oneWire);                 // Crea un objeto DallasTemperature

const int pinTrigger = 11;    // Pin conectado al Trigger del sensor ultrasónico
const int pinEcho = 12;       // Pin conectado al Echo del sensor ultrasónico

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C del LCD y número de columnas y filas

boolean estadoServo = false;   // Estado actual del servo
boolean estadoBoton = false;  // Estado actual del botón
boolean estadoVentilador = false; // Estado actual del ventilador

void setup() {
  // Configuración del teclado
  for (byte fila = 0; fila < tecladoFilas; fila++) {
    pinMode(pinFilas[fila], OUTPUT);
    digitalWrite(pinFilas[fila], HIGH);
  }
  for (byte columna = 0; columna < tecladoColumnas; columna++) {
    pinMode(pinColumnas[columna], INPUT_PULLUP);
  }

  // Configuración del botón
  pinMode(pinBoton, INPUT_PULLUP);

  // Configuración del servo
  miServo.attach(pinServo);
  miServo.write(0);

  // Configuración del ventilador
  pinMode(pinVentilador, OUTPUT);
  digitalWrite(pinVentilador, LOW);

  // Configuración del sensor de temperatura
  sensors.begin();

  // Configuración del sensor ultrasónico
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);

  // Configuración del LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temp: --- C");
  lcd.setCursor(0, 1);
  lcd.print("Dist: --- cm");
}

void loop() {
  // Lectura del teclado
  char teclaPulsada = leerTeclado();
  if (teclaPulsada == 'A') {
    if (estadoServo) {
      miServo.write(0);
      estadoServo = false;
    } else {
      miServo.write(180);
      estadoServo = true;
    }
  }

  // Lectura del botón
  if (digitalRead(pinBoton) == LOW) {
    if (!estadoBoton) {
      if (estadoServo) {
        miServo.write(0);
        estadoServo = false;
      } else {
        miServo.write(180);
        estadoServo = true;
      }
      estadoBoton = true;
      delay(100);
    }
  } else {
    estadoBoton = false;
  }

  // Lectura de la temperatura
  sensors.requestTemperatures();
  float temperatura = sensors.getTempCByIndex(0);

  // Control del ventilador
  if (temperatura > 25) {
    digitalWrite(pinVentilador, HIGH);
    estadoVentilador = true;
  } else {
    digitalWrite(pinVentilador, LOW);
    estadoVentilador = false;
  }

  // Lectura del sensor ultrasónico
  float distancia = leerSensorUltrasonico();

  // Actualización del LCD
  lcd.setCursor(6, 0);
  lcd.print("   ");  // Borra el valor anterior
  lcd.setCursor(6, 0);
  lcd.print(temperatura);
  lcd.setCursor(6, 1);
  lcd.print("   ");  // Borra el valor anterior
  lcd.setCursor(6, 1);
  lcd.print(distancia);

  delay(100);
}

char leerTeclado() {
  for (byte fila = 0; fila < tecladoFilas; fila++) {
    digitalWrite(pinFilas[fila], LOW);
    for (byte columna = 0; columna < tecladoColumnas; columna++) {
      if (digitalRead(pinColumnas[columna]) == LOW) {
        delay(20);  // Debounce
        while (digitalRead(pinColumnas[columna]) == LOW);  // Espera a que se suelte la tecla
        digitalWrite(pinFilas[fila], HIGH);
        return teclas[fila][columna];
      }
    }
    digitalWrite(pinFilas[fila], HIGH);
  }
  return '\0';  // No se ha pulsado ninguna tecla
}

float leerSensorUltrasonico() {
  digitalWrite(pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);
  long duracion = pulseIn(pinEcho, HIGH);
  float distancia = duracion * 0.034 / 2;
  if (distancia < 2 || distancia > 400) {
    return 0;  // Fuera de rango
  } else {
    return distancia;
  }
}
