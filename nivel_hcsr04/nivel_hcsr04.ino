// AquaGuard - Medicion de nivel con HC-SR04 (Arduino)
// Salida serial (9600 baudios): distanciaCruda,distanciaFiltrada [cm]

const int TRIG = 9;
const int ECHO = 10;

const float alpha = 0.2;

float distanciaFiltrada = 0;
bool primeraLectura = true;
// Medir una distancia con el HC-SR04
float medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  unsigned long duracion = pulseIn(ECHO, HIGH, 30000UL);

  if (duracion == 0) {
    return -1;
  }

  float distancia = duracion * 0.0343 / 2.0;

  if (distancia < 2 || distancia > 400) {
    return -1;
  }

  return distancia;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(TRIG, LOW);
}
void loop() {
  float muestras[5];
  int validas = 0;
  float distanciaCruda = -1;
  // Tomar cinco mediciones
  for (int i = 0; i < 5; i++) {

    float lectura = medirDistancia();

    if (lectura > 0) {
      muestras[validas] = lectura;
      validas++;
      distanciaCruda = lectura;
    }
    // Evitar interferencia entre pulsos ultrasonicos
    delay(60);
  }
  // Solo calcular si hay suficientes lecturas validas
  if (validas >= 3) {
    // Ordenar las mediciones
    for (int i = 0; i < validas - 1; i++) {
      for (int j = i + 1; j < validas; j++) {
        if (muestras[j] < muestras[i]) {
          float temporal = muestras[i];
          muestras[i] = muestras[j];
          muestras[j] = temporal;
        }
      }
    }
    // Calcular mediana
    float mediana;
    if (validas % 2 == 1) {
      mediana = muestras[validas / 2];
    } else {
      mediana = (muestras[validas / 2 - 1] +
                 muestras[validas / 2]) / 2.0;
    }
    // Filtro exponencial EMA
    if (primeraLectura) {
      distanciaFiltrada = mediana;
      primeraLectura = false;
    } else {
      distanciaFiltrada =
        alpha * mediana +
        (1.0 - alpha) * distanciaFiltrada;
    }
    // Formato compatible con AquaGuard
    Serial.print(distanciaCruda, 2);
    Serial.print(",");
    Serial.println(distanciaFiltrada, 2);
  }
  delay(200);
}
