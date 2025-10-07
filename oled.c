#include "oled.h"



void OLED_init() // PDF:example appended to the datasheet
{ 

    // Slå AV først (trygt å konfigurere når skjermen er av)
    OLED_write_cmd(0xAE);                    // Display OFF

    OLED_write_cmd(0xD5); OLED_write_cmd(0x80); // Display clock div / osc
    OLED_write_cmd(0xA8); OLED_write_cmd(0x3F); // Multiplex ratio (64-1)
    OLED_write_cmd(0xD3); OLED_write_cmd(0x00); // Display offset = 0
    OLED_write_cmd(0x40);                       // Start line = 0

    // Hvis modul med intern VCC (vanlig): slå på charge pump
    OLED_write_cmd(0x8D); OLED_write_cmd(0x14); // Charge pump enable

    // Minnemodus: 0x00 = horizontal (greit for skriving radvis)
    // (Du kan bruke 0x02 = page om du vil.)
    OLED_write_cmd(0x20); OLED_write_cmd(0x00);

    // Orientering (du ønsket speil horisontalt + vertikal flip)
    OLED_write_cmd(0xA1); // Segment remap
    OLED_write_cmd(0xC8); // COM scan dir

    // COM pins hardware config (typisk for 128x64-moduler)
    OLED_write_cmd(0xDA); OLED_write_cmd(0x12);

    // Kontrast / precharge / VCOMH (trygge defaultverdier)
    OLED_write_cmd(0x81); OLED_write_cmd(0x7F); // Contrast
    OLED_write_cmd(0xD9); OLED_write_cmd(0xF1); // Pre-charge
    OLED_write_cmd(0xDB); OLED_write_cmd(0x40); // VCOMH deselect

    OLED_write_cmd(0xA4); // Følg RAM-innhold (ikke all-på)
    OLED_write_cmd(0xA6); // Normal display (ikke invert)

    // Rydd skjermbuffer (skriv 0 til hele RAM, se funksjon under)
    //OLED_clear(); // valgfritt men anbefalt

    // Slå PÅ til slutt
    OLED_write_cmd(0xAF);                    // Display ON
}


void OLED_home(){
    OLED_pos(0,0);
}


void OLED_goto_line(uint8_t line){
    uint8_t line2 = (0xb0)|(line); //?
    OLED_write_cmd(line);
}

void OLED_goto_column(uint8_t column){
    //ifølge databladet må kolonne deles i to, oppe og nede
    uint8_t lower = (0x00)| (column & 0x0F); //ved å ta column med operasjon OG med 0000 1111 får vi de fire nederste.
    //tar så eller med det som står i databladet
    uint8_t upper = (0x10) | (column & 0xF0); //tilsvarende over: OG med 1111 0000
    OLED_write_cmd(lower);
    OLED_write_cmd(upper);
}

void OLED_clear_line(uint8_t line){
    OLED_goto_line(line);
    OLED_goto_column(0);

    for (int i=0;i<=127;i++){
        
    }
}


void OLED_pos(uint8_t row, uint8_t column){
    OLED_goto_line(row);
    OLED_goto_column(column);
}

void OLED_write_cmd(uint8_t cmd){
    PORTB &= ~(1<<PB3); //dc=0
    //velg ss2- CS low, cs er lav så lenge vi skriver
    spi_selectSlave(1);
    spi_write(cmd); //skriver data
    PORTB |= (1<<PB3); //deselect slave 2
}

void OLED_write_data(uint8_t data) //volatile - slaven skriver
{OLED_write_cmd(0xa1); // segment remapping
OLED_write_cmd(0xda); // vanlige pad-hardware: alternativ
OLED_write_cmd(0x12);
OLED_write_cmd(0xc8); // utgangsskannretning: com63~com0
OLED_write_cmd(0xa8); // multipleksforhold: 63
OLED_write_cmd(0x3f);
OLED_write_cmd(0xd5); // visningsdelerforhold / oscillasjonsfrekvens-modus
OLED_write_cmd(0x80);
OLED_write_cmd(0x81); // kontrastkontroll
OLED_write_cmd(0x50);
OLED_write_cmd(0xd9); // forhåndsladningsperiode
OLED_write_cmd(0x21);

OLED_write_cmd(0xdb); // VCOM-frakoblingsnivåmodus
OLED_write_cmd(0x30);
OLED_write_cmd(0xad); // hovedkonfigurasjon
OLED_write_cmd(0x00);
OLED_write_cmd(0xa4); // utdata følger RAM-innhold
OLED_write_cmd(0xa6); // sett normal visning
OLED_write_cmd(0xaf); // skjerm på}
    //DC må være 1
    PORTB |=(1<<PB2);
    //velg ss2- CS low, cs er lav så lenge vi skriver
    spi_selectSlave(1);
    spi_write(data); //skriver data
    PORTB |= (1<<PB3); //deselect slave 2
}

void OLED_print_char(char letter){
    
    uint8_t col=0;
    OLED_pos(0,col);
    const unsigned char selected_font = font4;
    if (letter>32 && letter <127){
    //hente listen
        const unsigned char* liste = selected_font + (letter - 32) * 4; 
        for (int c=0;c<4;c++){
            uint8_t kolonne=liste[c];
            uint8_t data = pgm_read_byte(&liste[c]);
            OLED_write_data(data);
            col++;
            OLED_pos(0,col);
        }
    }

}

void OLED_print(char* word){
    while(*word != '\0'){
        OLED_print_char(*word++);
    }
}

void OLED_set_brightness(uint8_t lvl);




/*OLED_write_cmd(0xa1); // segment remapping
OLED_write_cmd(0xda); // vanlige pad-hardware: alternativ
OLED_write_cmd(0x12);
OLED_write_cmd(0xc8); // utgangsskannretning: com63~com0
OLED_write_cmd(0xa8); // multipleksforhold: 63
OLED_write_cmd(0x3f);
OLED_write_cmd(0xd5); // visningsdelerforhold / oscillasjonsfrekvens-modus
OLED_write_cmd(0x80);
OLED_write_cmd(0x81); // kontrastkontroll
OLED_write_cmd(0x50);
OLED_write_cmd(0xd9); // forhåndsladningsperiode
OLED_write_cmd(0x21);

OLED_write_cmd(0xdb); // VCOM-frakoblingsnivåmodus
OLED_write_cmd(0x30);
OLED_write_cmd(0xad); // hovedkonfigurasjon
OLED_write_cmd(0x00);
OLED_write_cmd(0xa4); // utdata følger RAM-innhold
OLED_write_cmd(0xa6); // sett normal visning
OLED_write_cmd(0xaf); // skjerm på}

//segment remap
OLED_write_cmd(0xA1);
//scan direction
OLED_write_cmd(0xC8);
//output enable
OLED_write_cmd(0xAF);

OLED_write_cmd(0x20); // sett minneadresseringsmodus
OLED_write_cmd(0x02);
//OLED_write_cmd(0xae); // skjerm av
}*/