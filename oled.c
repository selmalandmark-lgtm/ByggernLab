/*#include "oled.h"
#include "spi.h"
#include <util/delay.h>


void oled_init()
{
    spi_write(0xae);  // skjerm av
    spi_write(0xa1);  // segment remapping
    spi_write(0xda);  // vanlige pad-hardware: alternativ
    spi_write(0x12);
    spi_write(0xc8);  // utgangsskannretning: com63~com0
    spi_write(0xa8);  // multipleksforhold: 63
    spi_write(0x3f);
    spi_write(0xd5);  // visningsdelerforhold / oscillasjonsfrekvens-modus
    spi_write(0x80);
    spi_write(0x81);  // kontrastkontroll
    spi_write(0x50);
    spi_write(0xd9);  // forhåndsladningsperiode
    spi_write(0x21);
    spi_write(0x20);  // sett minneadresseringsmodus
    spi_write(0x02);
    spi_write(0xdb);  // VCOM-frakoblingsnivåmodus
    spi_write(0x30);
    spi_write(0xad);  // hovedkonfigurasjon
    spi_write(0x00);
    spi_write(0xa4);  // utdata følger RAM-innhold
    spi_write(0xa6);  // sett normal visning
    spi_write(0xaf);  // skjerm på
}

oled_reset();

oled_home();

oled_go_to_line(line);

oled_clear_line(line);

oled_pos(row, column);

oled_write_data(char); //volatile

oled_print(char*);

oled_set_brightness(lvl);*/


