// AquaGuard - Caudalimetro YF-S201 (ESP32)
// Salida serial (115200 baudios): FLOW,caudal[L/min],volumen[L],pulsos

const int FLOW_PIN = 27;
// Calibracion inicial del YF-S201
const float PULSOS_POR_LITRO = 450.0;
volatile unsigned long pulsos = 0;
unsigned long tiempoAnterior = 0;

float caudal = 0;
float volumenTotal = 0;

void IRAM_ATTR contarPulsos() {
  pulsos++;
}

void setup() {
  Serial.begin(115200);

  pinMode(FLOW_PIN, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(FLOW_PIN),
    contarPulsos,
    FALLING
  );
  Serial.println("AQUACARD - CAUDALIMETRO INICIADO");
  tiempoAnterior = millis();
}

void loop() {

  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= 3000) {
    noInterrupts();
    unsigned long pulsosActuales = pulsos;
    pulsos = 0;
    interrupts();
    unsigned long intervalo =
      tiempoActual - tiempoAnterior;
    caudal = (pulsosActuales * 60000.0) /
             (PULSOS_POR_LITRO * intervalo);
    float litros = pulsosActuales /
                   PULSOS_POR_LITRO;
    volumenTotal += litros;
    Serial.print("FLOW,");
    Serial.print(caudal, 3);
    Serial.print(",");
    Serial.print(volumenTotal, 4);
    Serial.print(",");
    Serial.println(pulsosActuales);
    tiempoAnterior = tiempoActual;
  }
}
