#pragma once

#include <cstdint>
#include <vector>
#include <string>

#define MIN_PACKET_SIZE 3
#define MAX_PACKET_SIZE 255

#define VERSION_1       0x00

#define TYPE_DATA       0x00
#define TYPE_ACK        0x01

#define REPLY_SUCCESS   0x00
#define REPLY_ERROR     0x01
#define REPLY_UNSET     0x02

struct message {
    uint8_t version;
    uint8_t type;
    // uint16_t length; // in bytes - in buffer not struct

    std::vector<std::string> payload;
    int payload_len; // num of words/payloads
};

struct serialize_result {
    int size;
    uint8_t response;
};