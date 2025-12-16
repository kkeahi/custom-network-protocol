#include <cstdint>
#include <vector>
#include <string>

#define MIN_PACKET_SIZE 3
#define MAX_PACKET_SIZE 255

#define VERSION_1       0x00
#define VERSION_UNSET   0xFF

#define TYPE_DATA       0x00
#define TYPE_ACK        0x01
#define TYPE_UNSET      0xFF

#define REPLY_SUCCESS   0x00
#define REPLY_ERROR     0x01
#define REPLY_UNSET     0xFF

struct message {
    uint8_t version;
    uint8_t type;
    // uint16_t length; // in bytes - in buffer not struct

    std::vector<std::string> payload;
    int payload_len; // num of words/payloads
};

struct result {
    int size; // size of buffer
    uint8_t response;
};

void reset_result(struct result *res);
void reset_message(struct message *msg);

void encode(uint8_t buffer[MAX_PACKET_SIZE], struct message *msg, struct result *res);
void decode(uint8_t buffer[MAX_PACKET_SIZE], struct message *msg, struct result *res);

void print_message(struct message *msg);
