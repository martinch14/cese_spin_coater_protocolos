Trabajo Practico para la materia Protocolos
Carrera de Especialización en Sistemas Embebidos UBA

En el siguiente proyecto incorporo las bibliotecas necesarias del fabricante de circuitos integrados
TRINAMIC especificas para la utilización del Driver de Motor BLDC  TMC4671
Se desarrolla sobre la plataforma EDU-CIAA.

El proyecto desarrollado permite la configuracion a través de la interfaz SPI de la placa EDU CIAA
de los Modos de operación del Driver TMC4671.

Modo básico sin realimentación
1)  MODO_OPENLOOP

Modo TORQUE-FLUJO 	FOC(Field Oriented Control )
2)  MODO_TORQUE_FLUJO_HALL
    Incorporando para la realimentación para conocer la posición del rotor un Digital Hall Sensor,
    provee una baja resolución absoluta de la posición,recomendado para uso si se trabaja con bajo torque,


3)  MODO_TORQUE_FLUJO_ABN_ENCODER
    Incorporando para la realimentación un encoder incremental óptico de alta resolución,
    recomendado para trabajar a altas velocidades.


A través de la consola se pueden elegir cada una de las configuraciones para luego,
a través de las Teclas de la placa EDUCIAA poder ir probando cada una de ella.
    TEC1-carga la configuración seleccionada
    TEC2-Setea una velocidad inicial nula
    TEC3-Setea una velocidad particular para cada modo
    TEC4-Borra la configuración y permite probar las restantes

En main.c se realizan la carga y configuracion de las bibliotecas para el driver TMC4671
en motor_control.c se ejecutan los modos de operación según el modo seleccionado y la tecla apretada.
