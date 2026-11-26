# AquaGuard

Banco de pruebas de filtros para planta purificadora de agua (Arduino + ESP32).

## Firmware

| Carpeta | Placa | Sensor | Qué hace |
|---|---|---|---|
| `nivel_hcsr04/` | Arduino | HC-SR04 (ultrasonido) | Mide el nivel del tanque: 5 lecturas, mediana y filtro exponencial (EMA, α = 0.2). |
| `caudal_yfs201/` | ESP32 | YF-S201 (caudalímetro) | Cuenta pulsos por interrupción y calcula caudal y volumen acumulado cada 3 s. |

## Conexiones

- **HC-SR04 → Arduino:** TRIG en pin 9, ECHO en pin 10, VCC 5 V, GND.
- **YF-S201 → ESP32:** señal en GPIO 27 (con `INPUT_PULLUP`), VCC 5 V, GND.

## Formato de salida serial

- **Nivel** (9600 baudios): `distanciaCruda,distanciaFiltrada` en cm.
- **Caudal** (115200 baudios): `FLOW,caudal_L_min,volumen_L,pulsos`.

## Calibración

El YF-S201 usa `PULSOS_POR_LITRO = 450.0` como valor inicial. Ajústalo midiendo un volumen conocido.

## Uso

1. Abre cada `.ino` en Arduino IDE.
2. Selecciona la placa (Arduino UNO / ESP32 Dev Module) y el puerto.
3. Carga el código y abre el Monitor Serie con la velocidad indicada.
