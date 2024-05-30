//
// Created by eproskuryakov on 5/28/24.
//

#ifndef BANSERVICE_BAN_DTO_H
#define BANSERVICE_BAN_DTO_H
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>

// Define BanDTO structure
typedef struct {
    long resource_id;
    long user_id;
    char *resource_type;
    char *reason;
    long moderator_id;
    long long created_at; // Timestamp
    long long updated_at; // Timestamp
    long long expiration_date; // Timestamp
    int is_permanent;
} BanDTO;


// Functions to convert between BanDTO and JSON
cJSON *ban_dto_to_json(const BanDTO *ban);
BanDTO *json_to_ban_dto(const cJSON *json);

// Functions to handle arrays of BanDTO
cJSON *ban_dto_array_to_json(BanDTO **bans);
BanDTO **json_to_ban_dto_array(const cJSON *json);

// Functions to free BanDTO and BanDTO arrays
void free_ban_dto(BanDTO *ban);
void free_ban_dto_array(BanDTO **bans);

#endif //BANSERVICE_BAN_DTO_H
