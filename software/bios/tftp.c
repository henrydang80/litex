#include <stdint.h>
#include <string.h>

#include "microudp.h"
#include "tftp.h"

#define PORT_OUT	69
#define PORT_IN		7642

enum {
	TFTP_RRQ	= 1,	/* Read request */
	TFTP_WRQ	= 2, 	/* Write request */
	TFTP_DATA	= 3,	/* Data */
	TFTP_ACK	= 4,	/* Acknowledgment */
	TFTP_ERROR	= 5,	/* Error */
};

static int format_request(uint8_t *buf, uint16_t op, const char *filename)
{
	int len = strlen(filename);

	*buf++ = op >> 8; /* Opcode */
	*buf++ = op;
	memcpy(buf, filename, len);
	buf += len;
	*buf++ = 0x00;
	*buf++ = 'o';
	*buf++ = 'c';
	*buf++ = 't';
	*buf++ = 'e';
	*buf++ = 't';
	*buf++ = 0x00;
	return 9+strlen(filename);
}

static int format_ack(uint8_t *buf, uint16_t block)
{
	*buf++ = 0x00; /* Opcode: Ack */
	*buf++ = TFTP_ACK;
	*buf++ = (block & 0xff00) >> 8;
	*buf++ = (block & 0x00ff);
	return 4;
}

static uint8_t *packet_data;
static int total_length;
static int transfer_finished;
static uint8_t *dst_buffer;

static void rx_callback(uint32_t src_ip, uint16_t src_port,
    uint16_t dst_port, void *_data, unsigned int length)
{
	uint8_t *data = _data;
	uint16_t opcode;
	uint16_t block;
	int i;
	int offset;
	
	if(length < 4) return;
	if(dst_port != PORT_IN) return;
	opcode = data[0] << 8 | data[1];
	block = data[2] << 8 | data[3];
	if(block < 1) return;
	if(opcode == TFTP_DATA) { /* Data */
		length -= 4;
		offset = (block-1)*512;
		for(i=0;i<length;i++)
			dst_buffer[offset+i] = data[i+4];
		total_length += length;
		if(length < 512)
			transfer_finished = 1;
		
		length = format_ack(packet_data, block);
		microudp_send(PORT_IN, src_port, length);
	}
	if(opcode == TFTP_ERROR) { /* Error */
		total_length = -1;
		transfer_finished = 1;
	}
}

int tftp_get(uint32_t ip, const char *filename, void *buffer)
{
	int len;
	int tries;
	int i;
	int length_before;
	
	if(!microudp_arp_resolve(ip))
		return -1;

	microudp_set_callback(rx_callback);

	packet_data = microudp_get_tx_buffer();
	dst_buffer = buffer;

	total_length = 0;
	transfer_finished = 0;
	tries = 5;
	while(1) {
		len = format_request(packet_data, TFTP_RRQ, filename);
		microudp_send(PORT_IN, PORT_OUT, len);
		for(i=0;i<2000000;i++) {
			microudp_service();
			if((total_length > 0) || transfer_finished) break;
		}
		if((total_length > 0) || transfer_finished) break;
		tries--;
		if(tries == 0) {
			microudp_set_callback(NULL);
			return -1;
		}
	}

	length_before = total_length;
	while(!transfer_finished) {
		if(length_before != total_length) {
			i = 12000000;
			length_before = total_length;
		}
		if(i-- == 0) {
			microudp_set_callback(NULL);
			return -1;
		}
		microudp_service();
	}

	microudp_set_callback(NULL);

	return total_length;
}
