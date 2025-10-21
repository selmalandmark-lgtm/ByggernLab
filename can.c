#include "can.h"

void can_init(){
    mcp2515_init();
    CAN_init_loopback();
}
void can_messenger_send(can_message* msg){
    //mcp2515_write(uint8_t address, uint8_t *data, uint8_t len)
    mcp2515_write(MCP_TXB0SIDH, ((msg->id)/8)); //de 8 første tallene blir med i h
    mcp2515_write(MCP_TXB0SIDH, ((msg->id)%8)<<5);
    //sende lengden
    mcp2515_write(MCP_TXB0DLC, msg->data_length);

    for (int i=0; i < msg->data_length; i++){
        mcp2515_write(MCP_TXB0D0+i, msg->data[i]);
    }
    mcp2515_request_to_send(0);
}


void can_data_receive(can_message* message){
    //message id
    uint8_t id_l = mcp2515_read(MCP_RXB0SIDH)/0b100000;
    uint8_t id_h = mcp2515_read(MCP_RXB0SIDL);
    message->id = (id_h*0b1000) + id_l;
    //message length
    message->data_length = mcp2515_read(MCP_RXB0DLC);

    for (uint8_t i=0;i<message->data_length;i++){
        message->data[i]=mcp2515_read(MCP_RXB0D0+i);
    }

}
//void can_int_vect(){}



uint8_t CAN_init_loopback(){
    mcp2515_reset();
    mcp2515_set_mode(MODE_CONFIG);
    mcp2515_write(0b00101010, 0x03);
    mcp2515_write(0b00101001, 0x90);
    mcp2515_write(0b00101000, 0x02);
    mcp2515_write(0b00101011, 0x03);
    mcp2515_set_mode(MODE_LOOPBACK);
    return 1;
}

