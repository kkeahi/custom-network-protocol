#include <iostream>

#include "protocol.h"

using namespace std;

void reset_result(struct result *res) {
    res->size = 0;
    res->response = REPLY_UNSET;
}

void reset_message(struct message *msg) {
    msg->version = VERSION_UNSET;
    msg->type = TYPE_UNSET;
    msg->payload = {};
    msg->payload_len = 0;
}

void encode(uint8_t buffer[MAX_PACKET_SIZE], struct message *msg, struct result *res) {
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
            buffer[MIN_PACKET_SIZE + dataSize + j] = (uint8_t)msg->payload[i][j];

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

void decode(uint8_t buffer[MAX_PACKET_SIZE], struct message *msg, struct result *res) {
    reset_result(res);
    reset_message(msg);

    if (buffer[0] != VERSION_1) {
        res->response = REPLY_ERROR;
        return;
    }

    msg->version = buffer[0];

    if (buffer[1] != TYPE_DATA) {
        res->response = REPLY_ERROR;
        return;
    }

    msg->type = buffer[1];

    string temp_str;
    for (int i = MIN_PACKET_SIZE; i < MIN_PACKET_SIZE + buffer[2]; i++) {
        if (buffer[i] == '\0') {
            msg->payload.emplace_back(temp_str);
            temp_str = "";
            res->size++;
        }
        else {
            temp_str += buffer[i];
        }
    }

    msg->payload_len = msg->payload.size();
    res->response = REPLY_SUCCESS;
}

void print_message(struct message *msg) {
    cout << "\n------START MESSAGE------\n";

    cout << "Version: " << (int)msg->version << '\n';
    cout << "   Type: " << (int)msg->type << "\n";

    cout << "Payload:\n";
    for (int i = 0; i < msg->payload_len; i++) {
        cout << msg->payload[i] << "  ";
    }

    cout << "\n-------END MESSAGE-------\n\n";
}

// int main() {
//     message m {
//         VERSION_1,
//         TYPE_DATA,
//         {"hello\0", "world\0"},
//         2,
//     };
//
//     print_message(&m);
//
//     result res {
//         0,
//         REPLY_UNSET,
//     };
//
//     uint8_t buf[MAX_PACKET_SIZE];
//
//     // encode(buf, &m, &res);
//     // decode(buf, &m, &res);
//
//     reset_message(&m);
//
//     print_message(&m);
//
//     return 0;
// }
