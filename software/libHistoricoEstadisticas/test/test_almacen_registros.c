/**
 * Daniel Pedrosa Montes © 2024
 */

#ifdef TEST

#include "unity.h"

#include "registro.h"
#include "almacen_registros.h"
#include <time.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define N_REGISTROS 3

static void* memoria;
static size_t memoria_len;

static void write(uint32_t addr, const void* data, uint32_t len) {
    TEST_ASSERT(addr + len <= (uint64_t)memoria_len);
    memcpy(memoria+addr, data, len);
}

static void read(uint32_t addr, void* data, uint32_t len) {
    TEST_ASSERT(addr + len <= (uint64_t)memoria_len);
    memcpy(data, memoria+addr, len);
}

static ar_t ar;

void setUp(void) {
    memoria_len = sizeof(ar_ring_buffer_t) + N_REGISTROS*sizeof(registro_t);
    memoria = malloc(memoria_len);
    TEST_ASSERT(memoria);
    memset(memoria, UINT8_MAX, memoria_len);
    ar = (ar_t){
        ._read = read,
        ._write = write,
        ._rb = {
            .addr = 0,
            .len = 0,
            .size = N_REGISTROS,
        },
    };
}

void tearDown(void) {
    free(memoria);
    memoria_len = 0;
}

static const uint64_t fecha = 1727740800; // 01 de septiembre de 2024
static const uint64_t dia = 24*60*60;

static void inicializa_registros(int n) {
    for (int i = 0; i < n; i++) {
        ar_nuevo(&ar, fecha+dia*i);
    }
}

void test_crea_registro(void) {
    ar_nuevo(&ar, fecha);
    const registro_t r = ar_recupera(&ar, fecha);
    TEST_ASSERT_EQUAL(fecha, r.fecha);
}

void test_actualiza_registro(void) {
    ar_nuevo(&ar, fecha);
    ar_nuevo(&ar, fecha+dia);
    ar_nuevo(&ar, fecha+dia*2);

    const registro_t nuevo = {
        .fecha = fecha+dia*2,
        .latitud_dec = 3,
        .longitud_dec = 4,
        .num_arboles_vibrados = 5,
        .tiempo_medio_entre_arboles_cs = 6,
        .tiempo_medio_vibrando_cs = 7,
    };
    ar_actualiza(&ar, fecha+dia*2, &nuevo);

    const registro_t obtenido = ar_recupera(&ar, fecha+dia*2);
    TEST_ASSERT_EQUAL(0, memcmp(&nuevo, &obtenido, sizeof(registro_t)));
}

void test_noPermiteCrearRegistroEnEpoch(void) {
    ar_nuevo(&ar, 0);
    TEST_ASSERT_EQUAL(0, ar._rb.len);
}

void test_cuandoFechaAnterior_EntoncesNoCreaRegistro(void) {
    ar_nuevo(&ar, fecha);
    TEST_ASSERT_EQUAL(1, ar._rb.len);
    ar_nuevo(&ar, fecha);
    TEST_ASSERT_EQUAL(1, ar._rb.len);
    ar_nuevo(&ar, fecha-dia);
    TEST_ASSERT_EQUAL(1, ar._rb.len);
    ar_nuevo(&ar, fecha+dia);
    TEST_ASSERT_EQUAL(2, ar._rb.len);
}

void test_cuandoExcedeAlmacenamiento_EntoncesConservaUltimosRegistros(void) {
    ar_nuevo(&ar, fecha);
    ar_nuevo(&ar, fecha+dia);
    ar_nuevo(&ar, fecha+dia*2);
    TEST_ASSERT_EQUAL(3, ar._rb.len);
    ar_nuevo(&ar, fecha+dia*3);
    TEST_ASSERT_EQUAL(3, ar._rb.len);
    ar_nuevo(&ar, fecha+dia*4);
    ar_nuevo(&ar, fecha+dia*5);
    ar_nuevo(&ar, fecha+dia*6);
    ar_nuevo(&ar, fecha+dia*7);

    TEST_ASSERT_NOT_EQUAL(0, ar_recupera(&ar, fecha+dia*7).fecha);
    TEST_ASSERT_NOT_EQUAL(0, ar_recupera(&ar, fecha+dia*6).fecha);
    TEST_ASSERT_NOT_EQUAL(0, ar_recupera(&ar, fecha+dia*5).fecha);
    TEST_ASSERT_EQUAL(0, ar_recupera(&ar, fecha+dia*4).fecha);
    TEST_ASSERT_EQUAL(0, ar_recupera(&ar, fecha+dia*3).fecha);
    TEST_ASSERT_EQUAL(0, ar_recupera(&ar, fecha+dia*2).fecha);
    TEST_ASSERT_EQUAL(0, ar_recupera(&ar, fecha+dia).fecha);
    TEST_ASSERT_EQUAL(0, ar_recupera(&ar, fecha).fecha);
    TEST_ASSERT_EQUAL(3, ar._rb.len);
}

#endif // TEST
