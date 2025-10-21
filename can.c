
#include "can.h"

void can_init(){
    mcp_init();
}
void can_messenger_send(struct can_message* msg){
    //mcp2515_write(uint8_t address, uint8_t *data, uint8_t len)
    mcp2515_write(MCP_TXB0SIDH, (can_message->id>>3)); //de 8 første tallene blir med i h
    mcp2515_write(MCP_TXB0SIDH, (can_message->id));
    //sende lengden
    mcp_write(MCP_TXB0DLC, can_message->data_length);

    for (int i=0;i<can_message->data_length;i++){
        mcp2515_write(MCP_TXB0D0+i, can_message->data[i]);
    }
    mcp2515_request_to_send(0);
}
void can_data_receive(){
    
}
void can_int_vect(){}

