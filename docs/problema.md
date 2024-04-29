# Problema a resolver

Títulos adaptados de Steinbeck, Reinhold. 2011. “El Design Thinking Como
Estrategia de Creatividad En La Distancia.” Comunicar 19 (37): 27–35.

## Conocimientos sobre los usuarios y el problema general (Comprender)

Existen empresas y trabajadores los cuales, a cambio de dinero, trabajan la
tierra de un tercero con su maquinaria.

El cliente pide resolver dos problemas para este sector. (Histórico de
trabajo, histórico de incidencias).

### Primer problema: Histórico de trabajo

**Quien contrata el servicio** quiere reportes/resúmenes acerca de la actividad
de la máquina dado un intervalo de tiempo.

Quiere conocer, a partir de una fecha y hora de inicio y otra de final:

1. Tiempo medio y mediano entre vibraciones.
1. Tiempo medio y mediano vibrando.
1. Posición de los árboles vibrados.
1. Tiempo de trabajo efectivo.

### Segundo problema: Histórico de incidencias

**El dueño de la máquina** quiere conocer los fallos de funcionamiento
registrados y el número de veces que se ha detectado cada uno de estos para
detectar si la máquina está funcionando incorrectamente, pudiendo marcar como
leídos estos avisos y poniéndolos a 0.

**El fabricante de la máquina** quiere conocer los fallos de funcionamiento
registrados y el número de veces que se ha detectado cada uno de estos para
decidir si una avería se tramita por garantía o no, pudiendo marcar como leídos
estos avisos y poniéndolos a 0.

### Restricciones del problema y requisitos

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
agrícola. Para comunicarse telemáticamente suelen utilizar Whatsapp y e-mail.

## Generar todas las ideas posibles (Idear)

### Histórico de trabajo (almacenamiento en control electrónico)

Aunque podría ser más lento de recuperar y de desarrollar potencialmente
es más conveniente para el cliente.

1. Almacenamiento de los datos.
   - Para almacenar se puede utilizar
   [FlashDB](https://github.com/armink/FlashDB).
   Único proyecto vivo profesional que encuentro apto para plataformas
   baremetal, listo para producción y de código abierto. Está mantenido
   activamente
   por una empresa. Permite TSDB y KVDB. Si vemos que no es suficiente
   al ser de código abierto podemos hacer una librería que dependa en
   el sistema o un fork con las mejoras en el software y un pull request
   de vuelta.
1. Generación de los datos de salida (con garantías).
   - Se puede generar un PDF en la tarjeta y firmarlo.
   - Se puede generar un bloque de texto en la tarjeta y firmarlo.
   - Se puede generar una estructura binaria y firmarla.
1. Transmisión de los datos a un *smartphone*, ordenador o similar.
   - Mediante UART-BLE. [5kB/s~10kB/s](https://stackoverflow.com/a/22919464).
      - Módulo piggyback ESP-C3 o BDM-380.
      - Requiere que al otro lado se ejecute un software nuestro.
   - Mediante WiFi Direct.
      - Diseñando un módulo piggyback con algún módulo
      compatible con este protocolo.
      - No es compatible con iPhone, al menos no de una forma fácil.
         - Requiere una aplicación.
      - No sabemos el porcentaje de móviles Android que lo implementan.
   - Mediante WiFi.
      - Diseñando un módulo piggyback con algún módulo
      compatible con este protocolo.
         - Requiere implementar un servidor SFTP, HTTP, FTP o similar.
      - Requiere que al otro lado se ejecute un software nuestro.
   - Mediante Bluetooth File Transfer profile.
      - Diseñando un módulo piggyback con algún módulo
      compatible con este protocolo.
      - No es compatible con iPhone.
   - Mediante Bluetooth Personal Area Network profile.
      - Diseñando un módulo piggyback con algún módulo
      compatible con este protocolo.
      - Es compatible con el iPhone sin necesidad de hacerse parte del
      programa MFi.
      - Requiere que al otro lado se ejecute un software nuestro.
   - Mediante USB.
      - Requiere utilizar algún stack que nos permita utilizar el
      protocolo USB.
      - Requiere utilizar algún stack que nos permita interactuar con
      un sistema de ficheros FAT o similar.
      - Requiere adaptar físicamente la placa y la caja, garantizando
      de que este agujero no permita que entre polvo ni agua al control.
1. Comprobación de la validez de los datos y visualización.
   - Mediante un servicio web que permita visualizar los datos en la
   propia página web.
      - Requeriría un mantenimiento durante muchos años por nuestra parte.
   - Mediante un servicio web que permita transformar los datos a un PDF.
      - Requeriría un mantenimiento durante muchos años por nuestra parte.
   - Mediante un software que permita transformar los datos a un PDF.
      - Requeriría distintos ports a distintas plataformas.

### Histórico de trabajo (almacenamiento en *smartphone*)

Aunque requeriría que el cliente estuviese atento de iniciar y finalizar
una sesión de trabajo desde su teléfono, potencialmente puede ser más
rápido y barato de desarrollar.

1. Transmisión de datos entre control y *smartphone*. (con garantías)
   - [ Pendiente ]
1. Almacenamiento de datos. (con garantías)
   - [ Pendiente ]
1. Comprobación de la validez de los datos y visualización.
   - [ Pendiente ]

## Construir prototipos de algunas de las ideas más prometedoras (Construir prototipos)

[ Pendiente ]

## Aprender del uso de los usuarios de los distintos prototipos (Probar)

[ Pendiente ]
