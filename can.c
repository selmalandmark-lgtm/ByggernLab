#include "can.h"
#include "mcp2515.h"
#include <stdint.h>

#ifndef MCP_TXB0CTRL
#define MCP_TXB0CTRL 0x30
#endif
#ifndef MCP_CANINTF
#define MCP_CANINTF  0x2C
#endif
#ifndef MCP_RX0IF
#define MCP_RX0IF    0x01
#endif
#ifndef MCP_TX0IF
#define MCP_TX0IF    0x04
#endif
#ifndef MCP_TXREQ_BIT
#define MCP_TXREQ_BIT 0x08
#endif
#ifndef MCP_CNF1
#define MCP_CNF1     0x2A
#endif
#ifndef MCP_CNF2
#define MCP_CNF2     0x29
#endif
#ifndef MCP_CNF3
#define MCP_CNF3     0x28
#endif
#ifndef MCP_CANINTE
#define MCP_CANINTE  0x2B
#endif
#ifndef MODE_LOOPBACK
#define MODE_LOOPBACK 0x40
#endif

// Wait until TXB0 is ready for a new message
static inline void txb0_wait_idle(void){
    while (mcp2515_read(MCP_TXB0CTRL) & MCP_TXREQ_BIT) { }
}

void can_init(){
    mcp2515_init();
    CAN_init_loopback();
}

void can_messenger_send(can_message* msg){
    uint8_t sidh = (uint8_t)(msg->id >> 3);
    uint8_t sidl = (uint8_t)((msg->id & 0x07) << 5);

    // Wait for TX buffer to be free before loading a new message
    txb0_wait_idle();

    mcp2515_write(MCP_TXB0SIDH, sidh);
    mcp2515_write(MCP_TXB0SIDL, sidl);

    mcp2515_write(MCP_TXB0DLC, (uint8_t)(msg->data_length & 0x0F));

    for (uint8_t i = 0; i < msg->data_length; i++){
        mcp2515_write(MCP_TXB0D0 + i, msg->data[i]);
    }

    mcp2515_request_to_send(0);   // transmit the message

    txb0_wait_idle();             // optional, ensures TX complete
    mcp2515_bit_modify(MCP_CANINTF, MCP_TX0IF, 0); // clear TX flag
}

void can_data_receive(can_message* message){
    uint8_t sidh = mcp2515_read(MCP_RXB0SIDH);
    uint8_t sidl = mcp2515_read(MCP_RXB0SIDL);
    message->id = ((uint16_t)sidh << 3) | (sidl >> 5);

    uint8_t dlc = mcp2515_read(MCP_RXB0DLC) & 0x0F;
    message->data_length = dlc;

    for (uint8_t i = 0; i < dlc; i++){
        message->data[i] = mcp2515_read(MCP_RXB0D0 + i);
    }

    mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0); // clear RX flag
}

uint8_t CAN_init_loopback(){
    mcp2515_reset();
    mcp2515_set_mode(MODE_CONFIG);

    mcp2515_write(MCP_CNF1, 0x03);
    mcp2515_write(MCP_CNF2, 0x90);
    mcp2515_write(MCP_CNF3, 0x02);
    mcp2515_write(MCP_CANINTE, 0x03);

    mcp2515_set_mode(MODE_LOOPBACK);
    return 1;
} 