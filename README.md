# Diseño y estudio de un convertidor DC-DC Boost para carga de vehículos eléctricos a partir de energía fotovoltaica

Repositorio asociado al Trabajo Fin de Grado:

**Diseño y estudio de un convertidor DC-DC Boost para carga de vehículos eléctricos a partir de energía fotovoltaica**
**Autor:** Javier Varas Domínguez
**Titulación:** Grado en Ingeniería Electrónica Industrial y Automática
**Universidad:** Universidad de Cantabria
**Tutor:** Francisco Javier Díaz Rodríguez
**Curso:** 2025/2026

---

## Descripción del proyecto

Este proyecto tiene como objetivo el diseño, simulación y validación de un convertidor DC-DC tipo Boost aplicado a un sistema de carga de baterías de vehículos eléctricos alimentado mediante energía fotovoltaica.

El sistema parte de una instalación fotovoltaica residencial de aproximadamente 2,4 kW, formada por seis módulos conectados en serie. La tensión generada por el campo fotovoltaico se eleva mediante un convertidor Boost hasta un nivel adecuado para la carga de baterías de vehículos eléctricos.

Además del diseño teórico del convertidor, se ha desarrollado un modelo de simulación en MATLAB/Simulink, se ha implementado un algoritmo MPPT basado en el método de perturbación y observación, y se ha construido un prototipo experimental a escala reducida controlado mediante ESP32S.

---

## Objetivos principales

* Diseñar un convertidor DC-DC tipo Boost para una aplicación fotovoltaica orientada a la carga de vehículos eléctricos.
* Dimensionar los principales componentes de la etapa de potencia.
* Simular el comportamiento del sistema en MATLAB/Simulink.
* Implementar un algoritmo MPPT para mejorar el aprovechamiento de la energía disponible.
* Diseñar y fabricar una PCB para el prototipo experimental.
* Validar el funcionamiento del sistema mediante un prototipo a escala reducida.

---

## Estructura del repositorio

```text
TFG-Boost-Fotovoltaico-JavierVaras/
│
├── Simulink_Matlab/
│   ├── 04_Simulaciones_Convertidor/
│   └── Prototipo/
│
├── Codigos_Control_ESP32/
│
├── KiCad_Prototipo/
│
├── Documentacion_TFG/

```

### Simulink_Matlab

Contiene los modelos de simulación desarrollados en MATLAB/Simulink.
Incluye las simulaciones del convertidor Boost, el funcionamiento con ciclo de trabajo fijo, la implementación del algoritmo MPPT y la validación del prototipo experimental.

### Codigos_Control_ESP32

Incluye los códigos utilizados para el control del prototipo mediante ESP32S.
Se recogen tanto las pruebas de generación PWM como la implementación del algoritmo MPPT.

### KiCad_Prototipo

Contiene los archivos relacionados con el diseño electrónico del prototipo.
Incluye el esquemático, el diseño de la PCB y los archivos necesarios para documentar la placa desarrollada.

### Documentacion_TFG

Carpeta destinada a recoger la memoria del Trabajo Fin de Grado y documentación complementaria.

### Resultados_Experimentales

Incluye capturas, medidas de laboratorio y fotografías relacionadas con las pruebas del prototipo experimental.

---

## Herramientas utilizadas

* MATLAB/Simulink
* KiCad
* Arduino IDE
* ESP32S
* Osciloscopio y fuente de alimentación de laboratorio
* Instrumentación básica de medida

---

## Contenido técnico del proyecto

El trabajo incluye:

* Estudio del funcionamiento del convertidor Boost en modo de conducción continua.
* Cálculo del ciclo de trabajo para distintos valores de tensión de entrada.
* Dimensionado de la inductancia y condensadores.
* Selección de MOSFET, diodo, driver de puerta y componentes auxiliares.
* Simulación del convertidor en lazo abierto.
* Implementación del algoritmo MPPT mediante perturbación y observación.
* Análisis ante variaciones de irradiancia.
* Diseño de un prototipo experimental a escala reducida.
* Diseño y fabricación de la PCB.
* Validación experimental mediante medidas de laboratorio.

---

## Prototipo experimental

El prototipo se ha desarrollado a escala reducida para validar el funcionamiento básico del convertidor y del sistema de control.
La etapa de potencia se ha diseñado como un convertidor Boost alimentado a baja tensión, controlado mediante una señal PWM generada por un ESP32S.

El objetivo del prototipo no es replicar la potencia del sistema fotovoltaico completo, sino comprobar de forma práctica el comportamiento del convertidor, la generación de la señal de control y la respuesta del algoritmo MPPT.

---

## Nota

Este repositorio se ha creado como complemento a la memoria del Trabajo Fin de Grado.
Los archivos incluidos tienen finalidad académica y sirven para documentar el desarrollo del proyecto, las simulaciones realizadas, el diseño de la PCB y las pruebas experimentales llevadas a cabo.

---

## Autor

**Javier Varas Domínguez**
Grado en Ingeniería Electrónica Industrial y Automática
Universidad de Cantabria
