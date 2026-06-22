#include <Arduino.h>

/*
  CONTROL MPPT P&O PARA CONVERTIDOR BOOST CON ESP32

  PWM:
  GPIO25 -> INPUT del TC4422

  Medida de tension de entrada:
  Vin_12V -> R5 = 33 kOhm -> GPIO34 -> R6 = 8.2 kOhm -> GND

  Medida de corriente de entrada:
  Pin 2 del conector Vin -> R_shunt -> GND de la placa
  GPIO35 conectado al lado izquierdo de R_shunt

  Comandos por monitor serie:
  start  -> iniciar MPPT
  stop   -> detener convertidor
  status -> mostrar medidas
  cal    -> recalibrar cero de corriente
*/

// PINES
const int PWM_PIN = 25;
const int VIN_ADC_PIN = 34;
const int SHUNT_ADC_PIN = 35;

// PWM
const int PWM_FREQUENCY_HZ = 50000;
const int PWM_RESOLUTION_BITS = 10;
const int PWM_MAX_VALUE = 1023;

// DIVISOR DE TENSION DE ENTRADA
const float VIN_R1_OHM = 33000.0;
const float VIN_R2_OHM = 8200.0;

const float VIN_DIVIDER_FACTOR =
  (VIN_R1_OHM + VIN_R2_OHM) / VIN_R2_OHM;


// RESISTENCIA SHUNT
// Cambiar este valor /////////////
const float SHUNT_RESISTANCE_OHM = 0.47;

// PARAMETROS DEL MPPT
const float MPPT_STEP = 0.001;
const float DUTY_INITIAL = 0.10;
const float DUTY_MIN = 0.05;
const float DUTY_MAX = 0.60;

const unsigned long MPPT_PERIOD_MS = 200;

// PROTECCIONES
const float MIN_INPUT_VOLTAGE_V = 1.0;
const float MAX_INPUT_VOLTAGE_V = 15.0;

// Con shunt de 0.47 Ohm
const float MAX_INPUT_CURRENT_A = 1.20;

// FILTRADO DE MEDIDAS
const int ADC_SAMPLES = 50;


// VARIABLES GLOBALES
float duty = 0.0;

float vin = 0.0;
float iin = 0.0;
float pin = 0.0;

float vinPrevious = 0.0;
float pinPrevious = 0.0;

float shuntOffsetVoltage = 0.0;

bool mpptRunning = false;
bool firstMeasurement = true;

unsigned long lastMpptTime = 0;
unsigned long lastPrintTime = 0;

// FUNCION PARA APLICAR DUTY
void setDuty(float newDuty)
{
  if (newDuty < 0.0)
  {
    newDuty = 0.0;
  }

  if (newDuty > DUTY_MAX)
  {
    newDuty = DUTY_MAX;
  }

  duty = newDuty;

  int pwmValue = (int)(duty * PWM_MAX_VALUE);

  ledcWrite(PWM_PIN, pwmValue);
}
// LECTURA FILTRADA ADC EN VOLTIOS
float readAdcVoltage(int pin)
{
  unsigned long sumMillivolts = 0;

  for (int i = 0; i < ADC_SAMPLES; i++)
  {
    sumMillivolts += analogReadMilliVolts(pin);
    delayMicroseconds(100);
  }

  float averageMillivolts =
    (float)sumMillivolts / ADC_SAMPLES;

  return averageMillivolts / 1000.0;
}
// CALIBRAR CERO DE CORRIENTE
void calibrateCurrentOffset()
{
  setDuty(0.0);

  delay(500);

  float sumVoltage = 0.0;

  for (int i = 0; i < 100; i++)
  {
    sumVoltage += readAdcVoltage(SHUNT_ADC_PIN);
    delay(5);
  }

  shuntOffsetVoltage = sumVoltage / 100.0;

  Serial.print("Offset shunt = ");
  Serial.print(shuntOffsetVoltage, 6);
  Serial.println(" V");
}

// MEDIR TENSION, CORRIENTE Y POTENCIA
void readMeasurements()
{
  float vinAdcVoltage = readAdcVoltage(VIN_ADC_PIN);

  vin = vinAdcVoltage * VIN_DIVIDER_FACTOR;

  float shuntAdcVoltage = readAdcVoltage(SHUNT_ADC_PIN);

  float shuntVoltage =
    shuntAdcVoltage - shuntOffsetVoltage;

  if (shuntVoltage < 0.0)
  {
    shuntVoltage = 0.0;
  }

  iin = shuntVoltage / SHUNT_RESISTANCE_OHM;

  pin = vin * iin;
}

// COMPROBAR SEGURIDAD
bool measurementsAreSafe()
{
  if (vin < MIN_INPUT_VOLTAGE_V)
  {
    Serial.println("ERROR: Vin demasiado baja.");
    return false;
  }

  if (vin > MAX_INPUT_VOLTAGE_V)
  {
    Serial.println("ERROR: Vin demasiado alta.");
    return false;
  }

  if (iin > MAX_INPUT_CURRENT_A)
  {
    Serial.println("ERROR: sobrecorriente.");
    return false;
  }

  return true;
}

// DETENER CONVERTIDOR
void stopConverter()
{
  mpptRunning = false;
  firstMeasurement = true;

  setDuty(0.0);

  Serial.println("Convertidor detenido.");
}

// ALGORITMO MPPT P&O
void runMppt()
{
  readMeasurements();

  if (!measurementsAreSafe())
  {
    stopConverter();
    return;
  }

  if (firstMeasurement)
  {
    vinPrevious = vin;
    pinPrevious = pin;

    firstMeasurement = false;

    return;
  }

  float dp = pin - pinPrevious;
  float dv = vin - vinPrevious;

  float newDuty = duty;

  if (dp > 0.0)
  {
    if (dv > 0.0)
    {
      newDuty = duty - MPPT_STEP;
    }
    else
    {
      newDuty = duty + MPPT_STEP;
    }
  }
  else
  {
    if (dv > 0.0)
    {
      newDuty = duty + MPPT_STEP;
    }
    else
    {
      newDuty = duty - MPPT_STEP;
    }
  }

  if (newDuty > DUTY_MAX)
  {
    newDuty = DUTY_MAX;
  }

  if (newDuty < DUTY_MIN)
  {
    newDuty = DUTY_MIN;
  }

  setDuty(newDuty);

  vinPrevious = vin;
  pinPrevious = pin;
}

// MOSTRAR ESTADO
void printStatus()
{
  Serial.print("Vin = ");
  Serial.print(vin, 3);
  Serial.print(" V | ");

  Serial.print("Iin = ");
  Serial.print(iin, 3);
  Serial.print(" A | ");

  Serial.print("Pin = ");
  Serial.print(pin, 3);
  Serial.print(" W | ");

  Serial.print("Duty = ");
  Serial.print(duty * 100.0, 2);
  Serial.println(" %");
}

// COMANDOS POR MONITOR SERIE
void readSerialCommand()
{
  if (!Serial.available())
  {
    return;
  }

  String command = Serial.readStringUntil('\n');

  command.trim();
  command.toLowerCase();

  if (command == "start")
  {
    readMeasurements();

    if (!measurementsAreSafe())
    {
      Serial.println("No se puede iniciar el MPPT.");
      return;
    }

    firstMeasurement = true;

    setDuty(DUTY_INITIAL);

    mpptRunning = true;

    Serial.println("MPPT iniciado.");
  }
  else if (command == "stop")
  {
    stopConverter();
  }
  else if (command == "status")
  {
    readMeasurements();
    printStatus();
  }
  else if (command == "cal")
  {
    if (mpptRunning)
    {
      Serial.println("Primero deten el MPPT con stop.");
    }
    else
    {
      calibrateCurrentOffset();
    }
  }
  else
  {
    Serial.println("Comandos disponibles:");
    Serial.println("start");
    Serial.println("stop");
    Serial.println("status");
    Serial.println("cal");
  }
}

// SETUP
void setup()
{
  Serial.begin(115200);

  delay(1000);

  pinMode(VIN_ADC_PIN, INPUT);
  pinMode(SHUNT_ADC_PIN, INPUT);

  analogReadResolution(12);

  analogSetPinAttenuation(VIN_ADC_PIN, ADC_11db);
  analogSetPinAttenuation(SHUNT_ADC_PIN, ADC_11db);

  bool pwmConfigured =
    ledcAttach(
      PWM_PIN,
      PWM_FREQUENCY_HZ,
      PWM_RESOLUTION_BITS
    );

  if (!pwmConfigured)
  {
    Serial.println("ERROR al configurar el PWM.");

    while (true)
    {
      delay(1000);
    }
  }

  setDuty(0.0);

  calibrateCurrentOffset();

  Serial.println();
  Serial.println("Sistema preparado.");
  Serial.println("Comandos disponibles:");
  Serial.println("start  -> iniciar MPPT");
  Serial.println("stop   -> detener");
  Serial.println("status -> ver medidas");
  Serial.println("cal    -> calibrar cero de corriente");
}

// LOOP
void loop()
{
  readSerialCommand();

  unsigned long currentTime = millis();

  if (
    mpptRunning &&
    currentTime - lastMpptTime >= MPPT_PERIOD_MS
  )
  {
    lastMpptTime = currentTime;

    runMppt();
  }

  if (
    mpptRunning &&
    currentTime - lastPrintTime >= 1000
  )
  {
    lastPrintTime = currentTime;

    printStatus();
  }
}