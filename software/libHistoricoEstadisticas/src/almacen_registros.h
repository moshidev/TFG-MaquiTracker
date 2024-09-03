/**
 * Daniel Pedrosa Montes © 2024
 */

#ifndef ALMACEN_REGISTROS_H
#define ALMACEN_REGISTROS_H

#include <stdint.h>
#include "registro.h"

typedef void (*ar_write_t)(uint32_t addr, const void* data, uint32_t len);
typedef void (*ar_read_t)(uint32_t addr, void* data, uint32_t len);

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
typedef struct {
    uint32_t addr;  // offset en bytes del primer registro físico con respecto al primer registro lógico
    uint32_t len;   // número de registros almacenados
    uint32_t size;  // size = (num_bytes - sizeof(ar_ring_buffer_t)) / sizeof(registro_t)
    uint32_t cks;   // checksum
} ar_ring_buffer_t;
#pragma GCC diagnostic pop

typedef struct {
    ar_write_t _write;
    ar_read_t _read;
    ar_ring_buffer_t _rb;
    uint32_t _cache;
} ar_t;

typedef enum {
    kArErr_Ok,
    kArErr_Invalid,
} ar_err_t;

void ar_init(ar_t* ar);

/**
 * Devuelve:
 *   - kArErr_Ok cuando crea un nuevo registro.
 *   - kArErr_Invalid cuando se intenta registrar una fecha que no es
 *     mas nueva que la última o que es igual a cero.
 */
ar_err_t ar_nuevo(ar_t* ar, uint64_t fecha);

/**
 * Devuelve:
 *   - Una estructura válida cuando existe un registro para la fecha dada.
 *   - Un registro con fecha 0 en caso contrario.
 */
registro_t ar_recupera(ar_t* ar, uint64_t fecha);

/**
 * Devuelve:
 *   - kArErr_Ok cuando actualiza el registro.
 *   - kArErr_Invalid no se ha encontrado un registro con la fecha
 *     indicada.
 */
ar_err_t ar_actualiza(ar_t* ar, const registro_t* r);

#endif // ALMACEN_REGISTROS_H
