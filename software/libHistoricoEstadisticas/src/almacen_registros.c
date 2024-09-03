/**
 * Daniel Pedrosa Montes © 2024
 */

#include "almacen_registros.h"

#define calc_addr(i) sizeof(ar_ring_buffer_t) + \
                        ( ar->_rb.addr + \
                        (i)*sizeof(registro_t) ) % \
                        (ar->_rb.size*sizeof(registro_t))

static void almacena_rb(ar_t* ar) {
    ar->_rb.cks = ar->_rb.addr + ar->_rb.len + ar->_rb.size;
    ar->_write(0, &ar->_rb, sizeof(ar_ring_buffer_t));
}

static void recupera_rb(ar_t* ar) {
    ar_ring_buffer_t rb = {0};
    ar->_read(0, &rb, sizeof(ar_ring_buffer_t));
    if (rb.addr + rb.len + rb.size != ar->_rb.cks) {
        almacena_rb(ar);
        return;
    }
    ar->_read(0, &ar->_rb, sizeof(ar_ring_buffer_t));
}

static uint32_t obten(ar_t* ar, uint64_t fecha) {
    recupera_rb(ar);

    // comprueba si el registro coincide con la última llamada
    if (ar->_cache < ar->_rb.len) {
        registro_t r = {0};
        const uint32_t addr = calc_addr(ar->_cache);
        ar->_read(addr, &r, sizeof(registro_t));
        if (r.fecha == fecha) {
            return addr;
        }
    }

    // búsqueda binaria
    uint32_t min = 0;
    uint32_t max = ar->_rb.len;
    while (min < max) {
        uint32_t p = (max - min) / 2 + min;
        const uint32_t addr = calc_addr(p);
        registro_t r = {0};
        ar->_read(addr, &r, sizeof(registro_t));
        if (r.fecha < fecha) {
            if (min == p) {
                return UINT32_MAX;  // bajo esta condición deducimos que la fecha a consultar es anterior a cualquier otra en la estructura de datos
            }
            min = p;
        }
        else if (r.fecha > fecha) {
            max = p;
        }
        else if (r.fecha == fecha) {
            ar->_cache = addr;
            return addr;
        }
    }

    return UINT32_MAX;
}

static uint32_t obten_ultimo(ar_t* ar) {
    recupera_rb(ar);
    if (ar->_rb.len == 0) {
        return UINT32_MAX;
    }

    const uint32_t addr = calc_addr(ar->_rb.len-1);
    return addr;
}

static void nuevo(ar_t* ar, uint64_t fecha) {
    recupera_rb(ar);

    const uint32_t addr = calc_addr(ar->_rb.len);
    registro_t r = {0};
    r.fecha = fecha;
    ar->_write(addr, &r, sizeof(registro_t));

    if (ar->_rb.len < ar->_rb.size) {
        ar->_rb.len += 1;
    }
    else {
        ar->_rb.addr = (ar->_rb.addr + sizeof(registro_t)) % (ar->_rb.size*sizeof(registro_t));
    }
    almacena_rb(ar);
}

ar_err_t ar_nuevo(ar_t* ar, uint64_t fecha) {
    if (fecha == 0) {
        return kArErr_Invalid;
    }

    const uint32_t addr = obten_ultimo(ar);
    if (addr != UINT32_MAX) {
        registro_t r = {0};
        ar->_read(addr, &r, sizeof(registro_t));
        if (fecha <= r.fecha) {
            return kArErr_Invalid;
        }
    }

    nuevo(ar, fecha);
    return kArErr_Ok;
}

registro_t ar_recupera(ar_t* ar, uint64_t fecha) {
    const uint32_t addr = obten(ar, fecha);
    if (UINT32_MAX == addr) {
        return (registro_t){0};
    }

    registro_t r = {0};
    ar->_read(addr, &r, sizeof(registro_t));
    return r;
}

ar_err_t ar_actualiza(ar_t* ar, const registro_t* r) {
    const uint32_t addr = obten(ar, r->fecha);
    if (addr == UINT32_MAX) {
        return kArErr_Invalid;
    }

    ar->_write(addr, r, sizeof(registro_t));
    return kArErr_Ok;
}
