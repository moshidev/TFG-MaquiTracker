/**
 * Daniel Pedrosa Montes © 2024
 */

#ifdef TEST

#include "unity.h"

#include "registro.h"
#include "historico_estadisticas.h"
#include "almacen_registros.h"

#include <stdint.h>
#include <string.h>

#define N_REGISTROS 3
#define MEM_SIZE (size_t)(sizeof(ar_ring_buffer_t)+N_REGISTROS*sizeof(registro_t))

static uint8_t mem[MEM_SIZE];

static const uint64_t dia = 24*60*60;

static void write(uint32_t addr, const void* data, uint32_t len) {
    TEST_ASSERT(addr + len <= (uint64_t)MEM_SIZE);
    memcpy(mem+addr, data, len);
}

static void read(uint32_t addr, void* data, uint32_t len) {
    TEST_ASSERT(addr + len <= (uint64_t)MEM_SIZE);
    memcpy(data, mem+addr, len);
}

void setUp(void) {
    const he_deps_t deps = {
        .read = read,
        .write = write,
        .mem_size = MEM_SIZE,
    };
    historico_estadisticas_inicializa(&deps);
}

void tearDown(void) {
    historico_estadisticas_reset();
}

static uint64_t vibra_arbol(registro_evt_t evt, uint64_t ms) {
    evt.movimiento = kRegistro_Quieto;
    evt.ms_desde_encendido = 0 + ms;
    historico_estadisticas_procesa(&evt);

    evt.movimiento = kRegistro_Moviendo;
    evt.ms_desde_encendido = 500 + ms;
    historico_estadisticas_procesa(&evt);

    evt.movimiento = kRegistro_PinzaCerrando;
    evt.ms_desde_encendido = 1000 + ms;
    historico_estadisticas_procesa(&evt);

    evt.movimiento = kRegistro_Vibrando;
    evt.ms_desde_encendido = 2000 + ms;
    historico_estadisticas_procesa(&evt);

    evt.movimiento = kRegistro_PinzaAbriendo;
    evt.ms_desde_encendido = 4000 + ms;
    historico_estadisticas_procesa(&evt);

    return 4000 + ms;
}

void test_CuandoNingunRegistro_AlRecibirEvento_CreaNuevoRegistro(void) {
    registro_evt_t evt = {
        .fecha = 1727740800, // 01 de septiembre de 2024
        .latitud_dec = 1.0f,
        .longitud_dec = -1.0f,
        .movimiento = kRegistro_Quieto,
        .ms_desde_encendido = 0,
    };
    historico_estadisticas_procesa(&evt);

    uint64_t ms = vibra_arbol(evt, 0);
    registro_t r = historico_estadisticas_recupera(evt.fecha);
    TEST_ASSERT_EQUAL(evt.fecha, r.fecha);
    TEST_ASSERT_EQUAL(evt.latitud_dec, r.latitud_dec);
    TEST_ASSERT_EQUAL(evt.longitud_dec, r.longitud_dec);
    TEST_ASSERT_EQUAL(20, r.tiempo_medio_vibrando_cs);
}

void test_CuandoRecibeEventoAntiguo_EntoncesIgnoraEvento(void) {
    registro_evt_t evt = {
        .fecha = 1727740800, // 01 de septiembre de 2024
        .latitud_dec = 1.0f,
        .longitud_dec = -1.0f,
        .movimiento = kRegistro_Quieto,
        .ms_desde_encendido = 0,
    };
    uint64_t ms = vibra_arbol(evt, 0); // crea un registro
    evt.fecha += dia;
    ms = vibra_arbol(evt, ms); // crea otro registro para el dia siguiente con dos arboles vibrados
    ms = vibra_arbol(evt, ms);
    evt.fecha -= dia;
    ms = vibra_arbol(evt, ms); // intenta vibrar un árbol para el día anterior

    registro_t r1 = historico_estadisticas_recupera(evt.fecha);
    registro_t r2 = historico_estadisticas_recupera(evt.fecha+dia);
    TEST_ASSERT_EQUAL(1, r1.num_arboles_vibrados);
    TEST_ASSERT_EQUAL(2, r2.num_arboles_vibrados);
}

void test_SigueRegistrandoEnElMismoDiaDespuesDeReiniciar(void) {
    registro_evt_t evt = {
        .fecha = 1727740800, // 01 de septiembre de 2024
        .latitud_dec = 1.0f,
        .longitud_dec = -1.0f,
        .movimiento = kRegistro_Quieto,
        .ms_desde_encendido = 0,
    };
    uint64_t ms = vibra_arbol(evt, 0); // crea un registro
    registro_reset();
    ms = vibra_arbol(evt, 0);
    registro_t r = historico_estadisticas_recupera(evt.fecha);
    TEST_ASSERT_EQUAL(2, r.num_arboles_vibrados);
}

void test_IgnoraFechasInexistentes(void) {
    registro_evt_t evt = {
        .fecha = 0, // 01 de septiembre de 2024
        .latitud_dec = 1.0f,
        .longitud_dec = -1.0f,
        .movimiento = kRegistro_Quieto,
        .ms_desde_encendido = 0,
    };
    uint64_t ms = vibra_arbol(evt, 0);
    registro_t r = historico_estadisticas_recupera(evt.fecha);
    TEST_ASSERT_EQUAL(0, r.fecha);
    TEST_ASSERT_EQUAL(0, r.num_arboles_vibrados);
}

#endif // TEST
