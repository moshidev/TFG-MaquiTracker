/**
 * Daniel Pedrosa Montes © 2024
 */

#ifndef HISTORICO_ESTADISTICAS_H
#define HISTORICO_ESTADISTICAS_H

#include <stdint.h>

typedef struct {
    void (*read)(uint32_t addr, void* data, uint32_t len);
    void (*write)(uint32_t addr, const void* data, uint32_t len);
    uint32_t mem_size;
} he_deps_t;

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

void historico_estadisticas_inicializa(const he_deps_t* he);

void historico_estadisticas_reset(void);

void historico_estadisticas_procesa(const registro_evt_t* evt);

registro_t historico_estadisticas_recupera(uint64_t fecha);

#endif // HISTORICO_ESTADISTICAS_H
