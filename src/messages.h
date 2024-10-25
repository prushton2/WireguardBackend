#include <cstdint>

enum Type {
	InitiatorToResponder=1,
	ResponderToInitiator=2,
	TransportDataMessage=3,
/*  */CookieReplyMessage=4,
};

//this will end up in a header file somewhere
struct InitiatorToResponder {
	uint8_t sender[4];
	uint8_t ephemeral[32];
	uint8_t _static[32];
	uint8_t timestamp[12];
	uint8_t mac1[16];
	uint8_t mac2[16];
};


struct ResponderToInitiator {
	uint8_t sender[4];
	uint8_t receiver[4];
	uint8_t ephemeral[32];
	uint8_t mac1[16];
	uint8_t mac2[16];
};


struct TransportDataMessage {
	uint8_t receiver[4];
	uint8_t counter[8];
	uint8_t packet[64]; //im not doing math to get this done no thank you
};

struct CookieReplyMessage {
	uint8_t receiver[4];
	uint8_t nonce[24];
	uint8_t cookie[16];
};

int processInitiatorToResponder(struct InitiatorToResponder* packet);