/**
 * Daniel Pedrosa Montes © 2024
 */

#include "registro.h"
#include <stdbool.h>
#include <math.h>

static registro_mov_t ca_estado = kRegistro_PinzaAbriendo;
static registro_mov_t cv_estado = kRegistro_Quieto;
static uint64_t cv_inicio_vibracion = 0;
static uint64_t cv_tiempo_vibrado = 0;
static uint64_t ct_ultima_apertura = 0;
static registro_mov_t ct_estado = 0;

static uint64_t fecha = 0;
static uint16_t num_arboles = 0;
static uint64_t tiempo_vibrando = 0;
static uint64_t tiempo_entre_arboles = 0;
static float latitud_dec = 0.0f;
static float longitud_dec = 0.0f;
static uint64_t ultimo_evento_ms = 0;

void registro_reset(void) {
    ca_estado = kRegistro_PinzaAbriendo;
    cv_estado = kRegistro_Quieto;
    cv_inicio_vibracion = 0;
    cv_tiempo_vibrado = 0;
    ct_ultima_apertura = 0;
    ct_estado = kRegistro_PinzaCerrando;

    fecha = 0;
    num_arboles = 0;
    tiempo_vibrando = 0;
    tiempo_entre_arboles = 0;
    latitud_dec = 0.0f;
    longitud_dec = 0.0f;
    ultimo_evento_ms = 0;
}

void registro_init(const registro_t* r) {
    registro_reset();
    fecha = r->fecha;
    num_arboles = r->num_arboles_vibrados;
    tiempo_vibrando = num_arboles * r->tiempo_medio_vibrando_cs*100;
    tiempo_entre_arboles = num_arboles * r->tiempo_medio_entre_arboles_cs*100;
    latitud_dec = r->latitud_dec;
    longitud_dec = r->longitud_dec;
}

static bool arbol_detectado(registro_mov_t mov) {
    switch (mov) {
    case kRegistro_Quieto:
    case kRegistro_Moviendo:
        break;
    case kRegistro_PinzaCerrando:
        if (ca_estado == kRegistro_PinzaAbriendo) {
            ca_estado = kRegistro_PinzaCerrando;
        }
        break;
    case kRegistro_Vibrando:
        if (ca_estado == kRegistro_PinzaCerrando) {
            ca_estado = kRegistro_Vibrando;
        }
        break;
    case kRegistro_PinzaAbriendo:
        if (ca_estado == kRegistro_Vibrando) {
            ca_estado = kRegistro_PinzaAbriendo;
            return true;
        }
        break;
    }
    return false;
}

static uint64_t milisegundos_vibrados(registro_mov_t mov, uint64_t ms_desde_encendido) {
    uint64_t t = 0;
    if (cv_estado != kRegistro_Vibrando && mov == kRegistro_Vibrando) {
        cv_inicio_vibracion = ms_desde_encendido;
    }
    if (cv_estado == kRegistro_Vibrando && mov != kRegistro_Vibrando) {
        t = ms_desde_encendido - cv_inicio_vibracion;
    }
    cv_estado = mov;
    return t;
}

static uint64_t milisegundos_entre_arboles(registro_mov_t mov, uint64_t ms_desde_encendido) {
    uint64_t tiempo_entre_arboles = 0;
    switch (mov) {
    case kRegistro_Quieto:
    case kRegistro_Moviendo:
        break;
    case kRegistro_Vibrando:
        ct_estado = kRegistro_Vibrando;
        break;
    case kRegistro_PinzaAbriendo:
        if (ct_estado == kRegistro_Vibrando) {
            ct_ultima_apertura = ms_desde_encendido;
            ct_estado = kRegistro_PinzaAbriendo;
        }
        break;
    case kRegistro_PinzaCerrando:
        if (ct_estado == kRegistro_PinzaAbriendo) {
            const uint64_t t = ms_desde_encendido - ct_ultima_apertura;
            const uint64_t ms_10m = 10*60*1000;
            if (t < ms_10m) {
                tiempo_entre_arboles = t;
            }
            ct_estado = kRegistro_PinzaCerrando;
        }
        break;
    }
    return tiempo_entre_arboles;
}

static float media_en_linea(float ultima_media, float nuevo_valor, uint32_t numero_muestras) {
    // https://nullbuffer.com/articles/welford_algorithm.html
    return ultima_media + (nuevo_valor - ultima_media) / numero_muestras;
}

int registro_procesa(const registro_evt_t* evt) {
    if (evt->ms_desde_encendido < ultimo_evento_ms) {
        return 1;
    }
    ultimo_evento_ms = evt->ms_desde_encendido;

    if (evt->fecha != fecha && fecha != 0) {
        return 2;
    }
    fecha = evt->fecha;

    tiempo_vibrando += milisegundos_vibrados(evt->movimiento, evt->ms_desde_encendido);
    tiempo_entre_arboles += milisegundos_entre_arboles(evt->movimiento, evt->ms_desde_encendido);
    if (arbol_detectado(evt->movimiento) && num_arboles != UINT16_MAX) {
        num_arboles++;
        latitud_dec = media_en_linea(latitud_dec, evt->latitud_dec, num_arboles);
        longitud_dec = media_en_linea(longitud_dec, evt->longitud_dec, num_arboles);
    }
    return 0;
}

static uint64_t calcula_tiempo_medio_vibrando_ms(void) {
    if (num_arboles) {
        return roundf((float)tiempo_vibrando / (float)num_arboles);
    }
    return tiempo_vibrando;
}

static uint64_t calcula_tiempo_medio_entre_arboles_ms(void) {
    if (num_arboles) {
        return roundf((float)tiempo_entre_arboles / (float)num_arboles);
    }
    return tiempo_entre_arboles;
}

registro_t registro_estado(void) {
    return (registro_t){
        .fecha = fecha,
        .latitud_dec = latitud_dec,
        .longitud_dec = longitud_dec,
        .num_arboles_vibrados = num_arboles,
        .tiempo_medio_vibrando_cs = (uint16_t)(calcula_tiempo_medio_vibrando_ms() / 100),
        .tiempo_medio_entre_arboles_cs = (uint16_t)(calcula_tiempo_medio_entre_arboles_ms() / 100),
    };
}
