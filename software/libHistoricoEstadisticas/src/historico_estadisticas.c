/**
 * Daniel Pedrosa Montes © 2024
 */

#include "historico_estadisticas.h"
#include "almacen_registros.h"
#include "registro.h"

static ar_t ar;

void historico_estadisticas_inicializa(const he_deps_t* he) {
    const uint32_t rb_size = (he->mem_size - sizeof(ar_ring_buffer_t)) / sizeof(registro_t);
    ar = (ar_t){
        ._read = he->read,
        ._write = he->write,
        ._rb = (ar_ring_buffer_t) {
            .addr = 0,
            .len = 0,
            .size = rb_size,
            .cks = rb_size,
        },
        ._cache = UINT32_MAX,
    };
}

void historico_estadisticas_reset(void) {
    ar = (ar_t){0};
}

void historico_estadisticas_procesa(const registro_evt_t* evt) {
    const registro_t r_ar = ar_recupera(&ar, evt->fecha);
    if (r_ar.fecha == 0) {
        int retc = ar_nuevo(&ar, evt->fecha);
        if (retc != 0) {
            return;
        }
        registro_reset();
    }
    else if (registro_estado().fecha == 0){
        registro_init(&r_ar);
    }

    registro_procesa(evt);
    const registro_t r = registro_estado();
    ar_actualiza(&ar, &r);
}

registro_t historico_estadisticas_recupera(uint64_t fecha) {
    return ar_recupera(&ar, fecha);
}
