/**
 * Daniel Pedrosa Montes © 2024
 *
 * Platform-agnostic synchronous driver for Fujitsu's MB85RS4MT series.
 * 
 * GNU AFFERO GENERAL PUBLIC LICENSE
 */

#ifndef MB85RS4MT_H
#define MB85RS4MT_H

#include <stdint.h>

typedef enum {
    kMB85RS4MTBlockProtectNone = 0x0,
    kMB85RS4MTBlockProtect0x60000to0x7FFFF = 0x1,
    kMB85RS4MTBlockProtect0x40000to0x7FFFF = 0x2,
    kMB85RS4MTBlockProtectAll = 0x3,
} MB85RS4MT_block_protect_t;

typedef struct {
    uint8_t : 1;                         /* Bit fixed to zero. */
    uint8_t WEL : 1;                     /* Write Enable Latch. Enable writing to unprotected locations. */
    MB85RS4MT_block_protect_t BP : 2;    /* Block Protect. Protected blocks can never be written to, only read. */
    uint8_t : 3;                         /* Not used bits */
    uint8_t WPEN : 1;                    /* Status Register Write Protect. If set to false the status register can be written without WP pin. If set to true WP pin is required for writting to the status register. */
} MB85RS4MT_status_t;

typedef enum {
    kMB85RS4MTErrOk,
    kMB85RS4MTErrOutOfBounds,
    kMB85RS4MTErrBlockProtected,
} MB85RS4MT_err_t;

typedef void (*MB85RS4MT_setter_t)(void);
typedef void (*MB85RS4MT_spi_synchronous_write_t)(const uint8_t* data, uint32_t n_bytes);
typedef void (*MB85RS4MT_spi_synchronous_read_t)(uint8_t* data, uint32_t n_bytes);

typedef struct {
    /* Dependencies */
    const MB85RS4MT_setter_t _open;
    const MB85RS4MT_setter_t _close;
    const MB85RS4MT_setter_t _cs_assert;
    const MB85RS4MT_setter_t _cs_deassert;
    const MB85RS4MT_setter_t _wp_assert;
    const MB85RS4MT_setter_t _wp_deassert;
    const MB85RS4MT_spi_synchronous_write_t _write;
    const MB85RS4MT_spi_synchronous_read_t _read;

    /* Status Register. May need to be pulled/pushed. */
    MB85RS4MT_status_t status;
} MB85RS4MT_t;

MB85RS4MT_err_t MB85RS4MT_write(const MB85RS4MT_t* self, uint32_t addr, const uint8_t* data, uint32_t n_bytes);

void MB85RS4MT_read(const MB85RS4MT_t* self, uint32_t addr, uint8_t* data, uint32_t n_bytes);

void MB85RS4MT_status_push(const MB85RS4MT_t* self);

void MB85RS4MT_status_pull(const MB85RS4MT_t* self);

uint32_t MB85RS4MT_id_pull(const MB85RS4MT_t* self);

#endif // MB85RS4MT_H
