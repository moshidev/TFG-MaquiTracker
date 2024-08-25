#ifdef TEST

#include "unity.h"

#include "registro.h"

void setUp(void)
{
}

void tearDown(void)
{
    registro_reset();
}

#define eq(a, b)  TEST_ASSERT_EQUAL(a, b)
#define evt_mov(m, ms) \
do { \
    evt.movimiento = m;\
    evt.ms_desde_encendido = ms; \
    registro_procesa(&evt); \
} while (0)
#define arboles_vibrados() registro_estado().num_arboles_vibrados
#define cs_vibracion_media() registro_estado().tiempo_medio_vibrando_cs
#define cs_entre_arboles_medio() registro_estado().tiempo_medio_entre_arboles_cs

void test_registro_DetectaVibrarArbol(void)
{
    registro_evt_t evt = { 0 };
    evt_mov(kRegistro_PinzaCerrando, 0);
    eq(0, arboles_vibrados());
    evt_mov(kRegistro_Vibrando, 0);
    eq(0, arboles_vibrados());
    evt_mov(kRegistro_PinzaAbriendo, 0);
    eq(1, arboles_vibrados());

    evt_mov(kRegistro_PinzaCerrando, 0);
    eq(1, arboles_vibrados());
    evt_mov(kRegistro_Vibrando, 0);
    eq(1, arboles_vibrados());
    evt_mov(kRegistro_PinzaAbriendo, 0);
    eq(2, arboles_vibrados());
}

void test_registro_calculaTiempoMedioDeVibracion(void) {
    registro_evt_t evt = { 0 };
    evt_mov(kRegistro_PinzaCerrando, 0);
    evt_mov(kRegistro_Moviendo, 100);

    evt_mov(kRegistro_Vibrando, 1000);
    evt_mov(kRegistro_Moviendo, 6000);

    evt_mov(kRegistro_Vibrando, 10000);
    eq(50, cs_vibracion_media());
    evt_mov(kRegistro_PinzaAbriendo, 15000);
    eq(100, cs_vibracion_media());
    for (int i = 1; i < 10; i++) {
        const uint64_t dt = i*100000;
        evt_mov(kRegistro_PinzaCerrando, 0 + dt);
        evt_mov(kRegistro_Moviendo, 100 + dt);

        evt_mov(kRegistro_Vibrando, 1000 + dt);
        evt_mov(kRegistro_Moviendo, 6000 + dt);

        evt_mov(kRegistro_Vibrando, 10000 + dt);
        evt_mov(kRegistro_PinzaAbriendo, 15000 + dt);
        eq(100, cs_vibracion_media());
    }
}

void test_registro_calculaTiempoEntreArboles(void) {
    registro_evt_t evt = { 0 };
    evt_mov(kRegistro_Quieto, 0);
    evt_mov(kRegistro_PinzaCerrando, 100);
    evt_mov(kRegistro_Vibrando, 200);
    evt_mov(kRegistro_PinzaAbriendo, 1000);
    eq(0, cs_entre_arboles_medio());
    evt_mov(kRegistro_Moviendo, 1000);
    evt_mov(kRegistro_PinzaCerrando, 1100);
    eq(1, cs_entre_arboles_medio());
    evt_mov(kRegistro_Vibrando, 1200);
    evt_mov(kRegistro_PinzaAbriendo, 2000);
    evt_mov(kRegistro_PinzaCerrando, 2300);
    eq(2, cs_entre_arboles_medio());
}

void test_registro_noCalculaTiempoEntreArbolesSiPasanMasDeDiezMinutos(void) {
    // no cuenta tiempo si ha pasado más de 10 minutos entre árboles
    registro_evt_t evt = { 0 };
    const uint64_t diez_minutos_ms = 10*60*1000;
    evt_mov(kRegistro_Quieto, 0);
    evt_mov(kRegistro_PinzaCerrando, 100);
    evt_mov(kRegistro_Vibrando, 200);
    evt_mov(kRegistro_PinzaAbriendo, 1000);
    eq(0, cs_entre_arboles_medio());
    evt_mov(kRegistro_Moviendo, 1000);
    evt_mov(kRegistro_PinzaCerrando, diez_minutos_ms + 1000);
    eq(0, cs_entre_arboles_medio());
}

void test_registro_almacenaSoloPrimeraFecha(void) {
    registro_evt_t evt = {
        .fecha = 1,
        .latitud_dec = 0.0f,
        .longitud_dec = 0.0f,
        .movimiento = kRegistro_Quieto,
        .ms_desde_encendido = 0,
    };
    eq(0, registro_procesa(&evt));
    eq(1, registro_estado().fecha);

    evt.fecha = 2;
    eq(2, registro_procesa(&evt));
    eq(1, registro_estado().fecha);
}

void test_registro_calculaCoordenadasAproximadas(void) {
    registro_evt_t evt = { 0 };

    for (int i = 0; i < UINT16_MAX; i++) {
        evt.latitud_dec = 2.5f * i;
        evt.longitud_dec = 2.5f * i;

        // vibra árbol
        evt_mov(kRegistro_PinzaCerrando, 1000 + i*1000);
        evt_mov(kRegistro_Vibrando, 1200 + i*1000);
        evt_mov(kRegistro_PinzaAbriendo, 1700 + i*1000);

        // (progresión aritmética) / número de muestras = media
        TEST_ASSERT_EQUAL_FLOAT((evt.latitud_dec * (i+1.0f)/2) / (i+1.0f), registro_estado().latitud_dec);
        TEST_ASSERT_EQUAL_FLOAT((evt.longitud_dec * (i+1.0f)/2) / (i+1.0f), registro_estado().longitud_dec);
    }

}

#endif // TEST
