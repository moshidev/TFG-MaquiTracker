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
    for (uint32_t i = 0; i < ar->_rb.len; i++) {
        const uint32_t addr = calc_addr(i);
        registro_t r = {0};
        ar->_read(addr, &r, sizeof(registro_t));
        if (r.fecha == fecha) {
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

void ar_nuevo(ar_t* ar, uint64_t fecha) {
    if (fecha == 0) {
        return;
    }

    const uint32_t addr = obten_ultimo(ar);
    if (addr != UINT32_MAX) {
        registro_t r = {0};
        ar->_read(addr, &r, sizeof(registro_t));
        if (fecha <= r.fecha) {
            return;
        }
    }

    nuevo(ar, fecha);
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

void ar_actualiza(ar_t* ar, uint64_t fecha, const registro_t* r) {
    if (fecha != r->fecha) {
        return;
    }

    const uint32_t addr = obten(ar, fecha);
    if (addr == UINT32_MAX) {
        return;
    }

    ar->_write(addr, r, sizeof(registro_t));
}
