Trabajo Practico para la materia Protocolos
Carrera de Especialización en Sistemas Embebidos UBA

En el siguiente proyecto incorporo las bibliotecas necesarias del fabricante de circuitos integrados
TRINAMIC especificas para la utilizacion del Driver de Motor BLDC  TMC4671
Se desarrolla sobre la plataforma EDU-CIAA.

El proyecto desarrollado permite la configuracion a travs de la intefaz SPI de la placa EDU CIAA
de los Modos de operacion del Driver TMC4671.

Modo basico sin realimentacion 
1)  MODO_OPENLOOP

Modo TORQUE-FLUJO 	FOC(Field Oriented Control )
2)  MODO_TORQUE_FLUJO_HALL	
    Incoporando para la realimentacion para conocer la posicion del rotor un Digital Hall Sensor, 
    provee una baja resolucion absoluta de la posicion,recomendado para uso si se trabaja con bajo torque, 


3)  MODO_TORQUE_FLUJO_ABN_ENCODER
    Incorporando para la realimentacion un encoder incremental optico de alta resolucion, 
    recomendado para trabajar a altas velocidades.


A traves de la consola se pueden elegir cada una de las configuraciones para luego, 
a traves de las Teclas de la placa EDUCIAA poder ir probando cada una de ella.
    TEC1-carga la configuracion seleccionada
    TEC2-Setea una velocidad inicial nula
    TEC3-Setea una velocidad particular para cada modo
    TEC4-Borra la configuracion y permite probar las restantes

