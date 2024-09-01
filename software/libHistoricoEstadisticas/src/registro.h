/**
 * Daniel Pedrosa Montes © 2024
 */

#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdint.h>

typedef struct {
    uint64_t fecha;
    float latitud_dec;
    float longitud_dec;
    uint16_t num_arboles_vibrados;
    uint16_t tiempo_medio_vibrando_cs;
    uint16_t tiempo_medio_entre_arboles_cs;
} registro_t;

typedef enum {
    kRegistro_Quieto,
    kRegistro_Moviendo,
    kRegistro_PinzaCerrando,
    kRegistro_Vibrando,
    kRegistro_PinzaAbriendo,
} registro_mov_t;

typedef struct {
    uint64_t ms_desde_encendido;
    uint64_t fecha;
    float latitud_dec;
    float longitud_dec;
    registro_mov_t movimiento;
} registro_evt_t;

void registro_reset(void);

int registro_procesa(const registro_evt_t* evento);

registro_t registro_estado(void);

#endif // REGISTRO_H
