---
geometry: margin=2cm
output: pdf_document
fontfamily: helvet
header-includes:
  - \renewcommand{\familydefault}{\sfdefault}
  - \usepackage{datetime}
  - \usepackage[spanish]{babel}
  - \usepackage{pifont}
  - \usepackage{fancyhdr}
  - \pagestyle{fancy}
  - \fancyhead[R]{\MakeUppercase{Changelog Sistema Registros}}
  - \fancyhead[L]{\MakeUppercase{\textit{DigiControl Engineering S.L.}}}
  - \fancyfoot[L]{\today, \currenttime}
---

# Historial de cambios

## 03-sep-2024 PRELIMINAR v0.2.0

- Crea interfaz de la librería y la implementa.
  - Podemos procesar los eventos.
  - Podemos recuperar un registro dado una fecha.
- Implementa estructura de datos para almacenar y recuperar un registro.
  - Únicamente permitimos un registro por día.
  - Sigue una política FIFO.
  - Optimizada para memorias FRAM o similares.
- Implementa lógica de un registro.
  - Un registro sólo puede tener un día de una fecha.
  - La geolocalización se guarda como la media de los sitios en los que se ha vibrado un árbol.
  - El número de árboles vibrados es el contador de árboles del registro.
  - Las estadísticas de vibración se guardan como la media de vibraciones y entre vibraciones.
    - Para el tiempo medio entre árboles se ignoran aquellos tiempos entre árboles superiores a los 10 minutos.
- Implementa driver FRAM `MB85RS4MT`.

## 21-ago-2024 v0.1.0

- Decide qué datos guardar en los registros de trabajo.
- Decide qué criterio seguir para delimitar un registro de trabajo.
- Decide y diseña interfaz gráfica.
- Decide soporte de almacenamiento permanente.
- Decide forma de almacenamiento y recuperación de los datos.
