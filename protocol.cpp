#include <iostream>
using namespace std;

#include "protocol.h"

void reset_result(struct serialize_result *res) {
    res->response = REPLY_UNSET;
}

void serialize(uint8_t buffer[MAX_PACKET_SIZE], struct message *msg, struct serialize_result *res) {
    reset_result(res);

    if (msg->version != VERSION_1) {
        res->response = REPLY_ERROR;
        return;
    }

    buffer[0] = msg->version;

    if (msg->type != TYPE_DATA) {
        res->response = REPLY_ERROR;
        return;
    }

    buffer[1] = msg->type;

    int dataSize = 0;
    int runningDataSize = 0;
    for (int i = 0; i < msg->payload_len; i++) {
        for (int j = 0; j < MAX_PACKET_SIZE - MIN_PACKET_SIZE - dataSize; j++) {

            runningDataSize++;
            buffer[MIN_PACKET_SIZE + dataSize + j] = msg->payload[i][j];

            if (msg->payload[i][j] == '\0') {
                break;
            }
        }

        dataSize += runningDataSize;
        runningDataSize = 0;
    }

    buffer[2] = dataSize;
    res->size = MIN_PACKET_SIZE + dataSize;
    res->response = REPLY_SUCCESS;
}



int main() {
    message m {
        .version = VERSION_1,
        .type = TYPE_DATA,
        .payload = {"hello", "world"},
        .payload_len = 2,
    };

    serialize_result res {
        .size = 0,
        .response = REPLY_UNSET,
    };

    uint8_t buf[MAX_PACKET_SIZE];

    serialize(buf, &m, &res);
    for (int i = 0; i < res.size; i++) {
        std::cout << int(buf[i]) << '\n';
    }
}