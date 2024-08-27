/**
 * Daniel Pedrosa Montes © 2024
 * 
 * GNU AFFERO GENERAL PUBLIC LICENSE
 */

#include <MB85RS4MT.h>
#include "test.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE    32
#endif

void watchdog_refresh(void);

static uint8_t expected[BLOCK_SIZE];
static uint8_t got[BLOCK_SIZE];

void MB85RS4MT_test_write_and_read_all_bytes(MB85RS4MT_t* fram) {
    const uint32_t len = 512 * 1024;
    for (uint32_t i = 0; i < len/BLOCK_SIZE ; i++) {
        expected[0] = 0;
        got[0] = 1;
        for (uint32_t j = 1; j < BLOCK_SIZE ; j++) {
            expected[j] = (uint8_t)(i*BLOCK_SIZE + j);
            got[j] = 0;
        }
        MB85RS4MT_write(fram, i, expected, BLOCK_SIZE);
        MB85RS4MT_read(fram, i, got, BLOCK_SIZE);
        assert(0 == memcmp(expected, got, BLOCK_SIZE));
        watchdog_refresh();
    }
}

void MB85RS4MT_test_block_protect_err(MB85RS4MT_t* fram) {
    fram->status = (MB85RS4MT_status_t){
        .WPEN = 1,
        .BP = kMB85RS4MTBlockProtect0x40000to0x7FFFF,
        .WEL = 0,
    };
    MB85RS4MT_status_push(fram);

    const uint8_t a = 'a';
    assert(kMB85RS4MTErrBlockProtected == MB85RS4MT_write(fram, 0x40000, &a, 1));
    assert(kMB85RS4MTErrOk == MB85RS4MT_write(fram, 0x39999, &a, 1));
    assert(kMB85RS4MTErrOutOfBounds == MB85RS4MT_write(fram, 0x7FFFF, &a, 2));
}

void MB85RS4MT_test_push_and_pull_status_register(MB85RS4MT_t* fram) {
    MB85RS4MT_status_t a = (MB85RS4MT_status_t){
        .WPEN = 1,
        .BP = kMB85RS4MTBlockProtect0x60000to0x7FFFF,
        .WEL = 0,
    };
    MB85RS4MT_status_t b = (MB85RS4MT_status_t){
        .WPEN = 1,
        .BP = kMB85RS4MTBlockProtectNone,
        .WEL = 0,
    };

    fram->status = a;
    MB85RS4MT_status_push(fram);
    MB85RS4MT_status_pull(fram);
    assert(*(uint8_t*)&fram->status == *(uint8_t*)&a);

    fram->status = b;
    MB85RS4MT_status_push(fram);
    MB85RS4MT_status_pull(fram);
    assert(*(uint8_t*)&fram->status == *(uint8_t*)&b);
}

void MB85RS4MT_test_pull_ID(MB85RS4MT_t* fram) {
    const uint32_t mask = 0xFFFF1F00;
    const uint32_t expected_id = 0x047F4903;
    uint32_t id = MB85RS4MT_id_pull(fram);
    assert((expected_id&mask) == (id&mask));
}
