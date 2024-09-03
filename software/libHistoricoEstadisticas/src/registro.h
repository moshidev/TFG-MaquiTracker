/**
 * Daniel Pedrosa Montes © 2024
 */

#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdint.h>
#include "historico_estadisticas.h"

void registro_reset(void);

void registro_init(const registro_t* r);

int registro_procesa(const registro_evt_t* evento);

registro_t registro_estado(void);

#endif // REGISTRO_H
