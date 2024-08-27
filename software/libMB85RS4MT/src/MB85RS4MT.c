/**
 * Daniel Pedrosa Montes © 2024
 * 
 * GNU AFFERO GENERAL PUBLIC LICENSE
 */

#include <MB85RS4MT.h>
#include <stdbool.h>

#define CMD_WREN 0x06 // Write Enable Command. Sets Write Enable Latch.
#define CMD_WRDI 0x04 // Write Disable Command. Resets Write Enable Latch.

#define CMD_READ 0x03 // Read Bytes from FRAM.
#define CMD_WRITE 0x02 // Write Bytes to FRAM.

#define CMD_RDSR 0x05 // Read Status Register.
#define CMD_WRSR 0x01 // Write Status Register.

#define CMD_RDID 0x9F// Read Device ID.

static void write_enable(const MB85RS4MT_t* self) {
    const uint8_t cmd = CMD_WREN;
    self->_cs_assert();
    self->_write(&cmd, 1);
    self->_cs_deassert();
}

static void write_disable(const MB85RS4MT_t* self) {
    const uint8_t cmd = CMD_WRDI;
    self->_cs_assert();
    self->_write(&cmd, 1);
    self->_cs_deassert();
}

static bool is_within_bounds(uint32_t address, uint32_t n_bytes) {
    return address+n_bytes <= 512*1024;
}

static bool scopes_protected_blocks(MB85RS4MT_block_protect_t BP, uint32_t address, uint32_t n_bytes) {
    switch (BP) {
        case kMB85RS4MTBlockProtectNone:
            return false;
        case kMB85RS4MTBlockProtect0x60000to0x7FFFF:
            return address+n_bytes >= 0x60000;
        case kMB85RS4MTBlockProtect0x40000to0x7FFFF:
            return address+n_bytes >= 0x40000;
        case kMB85RS4MTBlockProtectAll:
            return true;
    }
    return true;
}

MB85RS4MT_err_t MB85RS4MT_write(const MB85RS4MT_t* self, uint32_t addr, const uint8_t* data, uint32_t len) {
    if (!is_within_bounds(addr, len)) {
        return kMB85RS4MTErrOutOfBounds;
    }
    if (scopes_protected_blocks(self->status.BP, addr, len)) {
        return kMB85RS4MTErrBlockProtected;
    }
    if (len < 1) {
        return kMB85RS4MTErrOk;
    }

    self->_open();
    write_enable(self);

    const uint8_t cmd[] = { CMD_WRITE, ((uint8_t*)&addr)[2], ((uint8_t*)&addr)[1], ((uint8_t*)&addr)[0] };

    self->_cs_assert();
    self->_write(cmd, sizeof(cmd));
    self->_write(data, len);
    self->_cs_deassert();

    write_disable(self);
    self->_close();

    return kMB85RS4MTErrOk;
}

void MB85RS4MT_read(const MB85RS4MT_t* self, uint32_t addr, uint8_t* data, uint32_t len) {
    if (len < 1) {
        return;
    }

    self->_open();

    const uint8_t cmd[] = { CMD_READ, ((uint8_t*)&addr)[2], ((uint8_t*)&addr)[1], ((uint8_t*)&addr)[0] };
    self->_cs_assert();
    self->_write(cmd, sizeof(cmd));
    self->_read(data, len);
    self->_cs_deassert();

    self->_close();
}

void MB85RS4MT_status_push(const MB85RS4MT_t* self) {
    const uint8_t cmd[] = { CMD_WRSR, *(uint8_t*)&self->status };

    self->_open();
    write_enable(self);
    self->_wp_assert();
    self->_cs_assert();

    self->_write(cmd, sizeof(cmd));

    self->_cs_deassert();
    self->_wp_deassert();
    write_disable(self);
    self->_close();
}

void MB85RS4MT_status_pull(const MB85RS4MT_t* self) {
    self->_open();
    self->_cs_assert();

    const uint8_t cmd = CMD_RDSR;
    self->_write(&cmd, 1);

    uint8_t rcv = UINT8_MAX;
    self->_read(&rcv, 1);
    if (0 == (rcv&0x1)) {
        *(uint8_t*)&self->status = rcv;
    }

    self->_cs_deassert();
    self->_close();
}

uint32_t MB85RS4MT_id_pull(const MB85RS4MT_t* self) {
    self->_open();
    self->_cs_assert();

    const uint8_t cmd = CMD_RDID;
    self->_write(&cmd, 1);

    uint8_t msb_id[4];
    self->_read(msb_id, 4);

    self->_cs_deassert();
    self->_close();

    return *(uint32_t*)(uint8_t[]){msb_id[3], msb_id[2], msb_id[1], msb_id[0]};
}
