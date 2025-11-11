#include <Servo.h>

Servo servoMotor1;
Servo servoMotor2;

int pinoEixoX = 0; // A0
int pinoEixoY = 1; // A1

// --- AJUSTES DE SENSIBILIDADE ---

const int TOLERANCIA_ZONA_MORTA = 100; 
const int CENTRO_JOYSTICK = 512;

const int LIMITE_MINIMO = CENTRO_JOYSTICK - TOLERANCIA_ZONA_MORTA; 
const int LIMITE_MAXIMO = CENTRO_JOYSTICK + TOLERANCIA_ZONA_MORTA; 

// --- FILTRO SUAVIZADOR ---
const float FATOR_SUAVIZACAO = 0.3; 
float valorSuaveX = 512.0;
float valorSuaveY = 512.0;


unsigned long tempoAnterior = 0;
const long intervalo = 500;

void setup() {
  Serial.begin(9600);
  servoMotor1.attach(4);
  servoMotor2.attach(5);
  
  // Inicia os servos no centro
  servoMotor1.write(90);
  servoMotor2.write(90);
}

void loop() {
  
  // --- EIXO X (Servo 1) ---
  
  // 1. Lê o valor bruto do joystick
  int valorBrutoX = analogRead(pinoEixoX);
  
  // 2. Aplica o filtro suavizador
  valorSuaveX = (FATOR_SUAVIZACAO * valorBrutoX) + ((1.0 - FATOR_SUAVIZACAO) * valorSuaveX);
  
  // 3. Usa o valor suavizado (convertido para int) para a lógica
  int valorEixoX = (int)valorSuaveX;

  // 4. Verifica a zona morta
  if (valorEixoX < LIMITE_MINIMO || valorEixoX > LIMITE_MAXIMO) {
    int posicaoServo1 = map(valorEixoX, 0, 1023, 0, 180);
    servoMotor1.write(posicaoServo1);
  } else {
    servoMotor1.write(90); 
  }

  
  // --- EIXO Y (Servo 2) ---
  
  int valorBrutoY = analogRead(pinoEixoY);

  valorSuaveY = (FATOR_SUAVIZACAO * valorBrutoY) + ((1.0 - FATOR_SUAVIZACAO) * valorSuaveY);
  int valorEixoY = (int)valorSuaveY;
  
  if (valorEixoY < LIMITE_MINIMO || valorEixoY > LIMITE_MAXIMO) {
    int posicaoServo2 = map(valorEixoY, 0, 1023, 0, 180);
    servoMotor2.write(posicaoServo2);
  } else {
    servoMotor2.write(90);
  }

  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;
  }

  delay(20);
}
