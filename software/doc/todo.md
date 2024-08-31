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
  - \fancyhead[R]{\MakeUppercase{Plan de Desarrollo}}
  - \fancyhead[L]{\MakeUppercase{\textit{DigiControl Engineering S.L.}}}
  - \fancyfoot[L]{\today, \currenttime}
---

# Plan de Desarrollo

~~1. **Diseña el comportamiento del sistema de registros (v0.1.0)**~~ \ding{51}

- ~~Decide qué datos guardar en los registros de trabajo.~~ \ding{51}
- ~~Decide qué criterio seguir para delimitar un registro de trabajo.~~ \ding{51}
- ~~Decide y diseña interfaz gráfica.~~ \ding{51}
- ~~Decide soporte de almacenamiento permanente.~~ \ding{51}
- ~~Decide forma de almacenamiento y recuperación de los datos.~~ \ding{51}

2. **Implementa comportamiento del sistema de registros (v0.2.0)**

- Realiza una estimación de costes y de tiempo.
- ~~Implementa reglas de un registro.~~ \ding{51}
- ~~Implementa driver FRAM `MB85RS4MT`.~~ \ding{51}
- Implementa reglas de almacenamiento y recuperación de registros.

3. **Implementa pegamento entre módulos (v0.3.0)**

- Añade soporte de las librerías para la plataforma objetivo.
- Añade conexión entre interfaz gráfica y sistema de registros.
