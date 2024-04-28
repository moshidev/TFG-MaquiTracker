# Problema a resolver

Títulos adaptados de Steinbeck, Reinhold. 2011. “El Design Thinking Como
Estrategia de Creatividad En La Distancia.” Comunicar 19 (37): 27–35.

## Conocimientos sobre los usuarios y el problema general (Comprender)

Existen empresas y trabajadores los cuales, a cambio de dinero, trabajan la
tierra de un tercero con su maquinaria.

**Quien contrata el servicio** quiere reportes/resúmenes acerca de la actividad
de la máquina dado un intervalo de tiempo.

Quiere conocer, a partir de una fecha y hora de inicio y otra de final:

1. Tiempo medio y mediano entre vibraciones.
1. Tiempo medio y mediano vibrando.
1. Posición de los árboles vibrados.
1. Tiempo de trabajo efectivo.

**El dueño de la máquina** quiere conocer los fallos de funcionamiento
registrados y el número de veces que se ha detectado cada uno de estos para
detectar si la máquina está funcionando incorrectamente, pudiendo marcar como
leídos estos avisos y poniéndolos a 0.

**El fabricante de la máquina** quiere conocer los fallos de funcionamiento
registrados y el número de veces que se ha detectado cada uno de estos para
decidir si una avería se tramita por garantía o no, pudiendo marcar como leídos
estos avisos y poniéndolos a 0.

**Todos** quieren tener una seguridad alta en que los reportes/resúmenes son
verdaderos y que no los ha modificado el trabajador/empresa.

La maquinaria se controla mediante un sistema de control dedicado con las
siguientes características:

1. Renesas RA6M1.
    - Arm® Cortex®-M4.
    - 8kB Data FLASH.
    - 128kB SRAM.
    - USB2.0 FS.
1. Conexión mediante UART a módulo de comunicaciones.
    - BMD-380. (Sin firmware).
    - ESP32-C3. (Firmware desarrollado: comunicación emulando UART a través de
        BLE).
    - Digi XBee Pro. (Firmware de fábrica: UART a través de ZigBee.)
1. FRAM Fujitsu 4kiB. (Posibilidad de otros tamaños de almacenamiento). Driver
   desarrollado.
1. Posibilidad de incluir otra FRAM Fujitsu o FLASH que sea compatible con la
   interfaz del integrado.
1. Smart GNSS antenna module UBLOX CAM-M8Q. Driver desarrollado.
1. Entradas 4-20mA. Driver desarrollado.
1. Entradas digitales. Driver desarrollado.
1. Entradas CAN 2.0. Driver desarrollado.
1. Alimentación a 12V.

## Empatía con los usuarios mirándoles de cerca (Observar)

[ Pendiente ]

## Usuario típico (Definir el punto de vista)

Empresa o trabajador que se dedica a tratar los árboles de campos a un tercero.

Suelen ser hombres que están acostumbrados al trabajo físico. Operan maquinaria
agrícola.

[ Pendiente detalle ]

## Generar todas las ideas posibles (Idear)

## Construir prototipos de algunas de las ideas más prometedoras (Construir prototipos)

[ Pendiente ]

## Aprender del uso de los usuarios de los distintos prototipos (Probar)

[ Pendiente ]
