//
// Created by eproskuryakov on 5/29/24.
//

#ifndef BANSERVICE_BAN_REQUEST_H
#define BANSERVICE_BAN_REQUEST_H

#include <cjson/cJSON.h>

typedef struct {
    long resource_id;
    long user_id;
    char *resource_type;
    char *reason;
    long moderator_id;
    long long duration; // Timestamp
} BanRequest;


// Functions to convert between BanRequest and JSON
cJSON *ban_request_to_json(const BanRequest *request);
BanRequest *json_to_ban_request(const cJSON *json);
BanRequest *json_str_to_ban_request(const char *str);

// Functions to handle arrays of BanRequest
cJSON *ban_request_array_to_json(BanRequest **requests);
BanRequest **json_to_ban_request_array(const cJSON *json);

// Functions to free BanRequest and BanRequest arrays
void free_ban_request(BanRequest *request);
void free_ban_request_array(BanRequest **requests);
#endif //BANSERVICE_BAN_REQUEST_H
