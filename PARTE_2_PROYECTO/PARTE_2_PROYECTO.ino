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
const int tecladoFilas = 4;       // numero de fila
const int tecladoColumnas = 4;    // numero de columnas
char teclas[tecladoFilas][tecladoColumnas] = { 
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinFilas[tecladoFilas] = {2};              // pines de filas
byte pinColumnas[tecladoColumnas] = {3};  // pines de columnas
const int pinBoton = 8;      
const int pinServo = 9;     
const int pinVentilador = 10; 

const int pinSensorTemperatura = 6; 
OneWire oneWire(pinSensorTemperatura);               // creamos variable one wire
DallasTemperature sensors(&oneWire);                 // creamos variable dalas temperature.

const int pinTrigger = 11;    
const int pinEcho = 12;       

LiquidCrystal_I2C lcd(0x27, 16, 2);  

boolean estadoServo = false;  
boolean estadoBoton = false;  
boolean estadoVentilador = false; 

void setup() {  // Configuración del teclado
  for (byte fila = 0; fila < tecladoFilas; fila++) {
    pinMode(pinFilas[fila], OUTPUT);
    digitalWrite(pinFilas[fila], HIGH);
  }
  for (byte columna = 0; columna < tecladoColumnas; columna++) {
    pinMode(pinColumnas[columna], INPUT_PULLUP);
  }
  pinMode(pinBoton, INPUT_PULLUP);

  miServo.attach(pinServo);
  miServo.write(0);

  pinMode(pinVentilador, OUTPUT);
  digitalWrite(pinVentilador, LOW);


  sensors.begin();

  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temp: --- C");
  lcd.setCursor(0, 1);
  lcd.print("Dist: --- cm");
}

void loop() {
  
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


  sensors.requestTemperatures();
  float temperatura = sensors.getTempCByIndex(0);

 
  if (temperatura > 25) {
    digitalWrite(pinVentilador, HIGH);
    estadoVentilador = true;
  } else {
    digitalWrite(pinVentilador, LOW);
    estadoVentilador = false;
  }

  
  float distancia = leerSensorUltrasonico();


  lcd.setCursor(6, 0);
  lcd.print("   ");  
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
        delay(20);  // Anti rebote
        while (digitalRead(pinColumnas[columna]) == LOW); 
        digitalWrite(pinFilas[fila], HIGH);
        return teclas[fila][columna];
      }
    }
    digitalWrite(pinFilas[fila], HIGH);
  }
  return '\0';  // No se ha pulsado ninguna tecla, en el matricial.
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
    return 0; 
  } else {
    return distancia;
  }
}
