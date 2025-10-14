#include "user_io_board.h"
#include "spi.h"
#include <util/delay.h>

// ---- CONFIG: set this to your real slave id for the IO-board (not the OLED) ----
#ifndef SPI_SLAVE_IO
#define SPI_SLAVE_IO  1
#endif

// Command defined by IO-board firmware
#define CMD_BUTTONS   0x04

// ---- Optional fixes if your nav bits come inverted or swapped ----
#define NAV_ACTIVE_HIGH   1   // 1 = bit=1 means pressed (most boards). Set to 0 if your board is active-low.
#define NAV_SWAP_U_AND_B  1   // 1 = swap Up and Button (you said U/B looked inverted)

// Full-duplex transfer using your SPI write/read API
static inline uint8_t txrx(uint8_t v){ spi_write(v); return spi_read(); }

// Deselect helper (adapt if your SPI API has a specific deselect)
static inline void deselect_all(void){
    // If you have spi_deselectAll(), call that instead.
    spi_selectSlave(255);
}

// Read the 3 raw bytes from the IO-board (with robust timing)
static inline void read_raw(uint8_t *b0, uint8_t *b1, uint8_t *b2){
    spi_selectSlave(SPI_SLAVE_IO);
    (void)txrx(CMD_BUTTONS);
    _delay_us(80);                 // >= 40 us (be generous)

    *b0 = txrx(0x00);              // Byte 0
    _delay_us(10);                 // >= 2 us (be generous)
    *b1 = txrx(0x00);              // Byte 1
    _delay_us(10);
    *b2 = txrx(0x00);              // Byte 2
    deselect_all();
}

void user_io_init(void){
    // nothing extra; just ensure you never select OLED and IO at the same time
}

// Returns 0 on success
int user_io_read_buttons(Buttons *btn){
    if(!btn) return -1;

    uint8_t b0, b1, b2;
    read_raw(&b0, &b1, &b2);

    // Your board appears to be: NAV first, then RIGHT, then LEFT
    uint8_t nav   = b0;
    uint8_t right = b1;
    uint8_t left  = b2;

    // If your board is active-low (rare for this), flip all three:
#if NAV_ACTIVE_HIGH == 0
    nav   = ~nav;
    right = ~right;
    left  = ~left;
#endif

    // Map RIGHT: [0 0 R6 R5 R4 R3 R2 R1]
    btn->R1 = !!(right & (1<<0));
    btn->R2 = !!(right & (1<<1));
    btn->R3 = !!(right & (1<<2));
    btn->R4 = !!(right & (1<<3));
    btn->R5 = !!(right & (1<<4));
    btn->R6 = !!(right & (1<<5));

    // Map LEFT: [0 L7 L6 L5 L4 L3 L2 L1]
    btn->L1 = !!(left  & (1<<0));
    btn->L2 = !!(left  & (1<<1));
    btn->L3 = !!(left  & (1<<2));
    btn->L4 = !!(left  & (1<<3));
    btn->L5 = !!(left  & (1<<4));
    btn->L6 = !!(left  & (1<<5));
    btn->L7 = !!(left  & (1<<6));

    // NAV: doc says [0 0 0 Btn Right Left Down Up]
    uint8_t up    = !!(nav & (1<<0));
    uint8_t down  = !!(nav & (1<<1));
    uint8_t leftn = !!(nav & (1<<2));
    uint8_t rightn= !!(nav & (1<<3));
    uint8_t btnn  = !!(nav & (1<<4));

#if NAV_SWAP_U_AND_B
    // Some boards/prints observed U and B swapped: fix it here if needed
    uint8_t tmp = up; up = btnn; btnn = tmp;
#endif

    btn->NU = up;
    btn->ND = down;
    btn->NL = leftn;
    btn->NR = rightn;
    btn->NB = btnn;

    return 0;
}
