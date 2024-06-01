//
// Created by eproskuryakov on 5/28/24.
//

#include "ban_dto.h"
#include "utils.h"

// Function to convert BanDTO to JSON
cJSON *ban_dto_to_json(const BanDTO *ban) {
    cJSON *json = cJSON_CreateObject();
    if (!json) return NULL;

    cJSON_AddNumberToObject(json, "resource_id", ban->resource_id);
    cJSON_AddNumberToObject(json, "user_id", ban->user_id);
    cJSON_AddStringToObject(json, "resource_type", ban->resource_type);
    cJSON_AddStringToObject(json, "reason", ban->reason);
    cJSON_AddNumberToObject(json, "moderator_id", ban->moderator_id);
    cJSON_AddNumberToObject(json, "created_at", ban->created_at);
    cJSON_AddNumberToObject(json, "updated_at", ban->updated_at);
    cJSON_AddNumberToObject(json, "expiration_date", ban->expiration_date);
    cJSON_AddBoolToObject(json, "is_permanent", ban->is_permanent);
    cJSON_AddBoolToObject(json, "expired", ban->expired);

    return json;
}

// Function to convert JSON to BanDTO
BanDTO *json_to_ban_dto(const cJSON *json) {
    BanDTO *ban = (BanDTO *)malloc(sizeof(BanDTO));
    if (!ban) return NULL;

    JSON_GET_NUMBER(json, "resource_id", ban->resource_id);
    JSON_GET_NUMBER(json, "user_id", ban->user_id);
    JSON_GET_STRING(json, "resource_type", ban->resource_type);
    JSON_GET_STRING(json, "reason", ban->reason);
    JSON_GET_NUMBER(json, "moderator_id", ban->moderator_id);
    JSON_GET_NUMBER(json, "created_at", ban->created_at);
    JSON_GET_NUMBER(json, "updated_at", ban->updated_at);
    JSON_GET_NUMBER(json, "expiration_date", ban->expiration_date);
    JSON_GET_BOOL(json, "is_permanent", ban->is_permanent);
    JSON_GET_BOOL(json, "expired", ban->expired);

    return ban;
}

// Function to convert array of BanDTO to JSON
cJSON *ban_dto_array_to_json(BanDTO **bans) {
    cJSON *json_array = cJSON_CreateArray();
    if (!json_array) return NULL;

    for (int i = 0; bans[i] != NULL; ++i) {
        cJSON *json = ban_dto_to_json(bans[i]);
        if (!json) {
            cJSON_Delete(json_array);
            return NULL;
        }
        cJSON_AddItemToArray(json_array, json);
    }

    return json_array;
}

// Function to convert JSON to array of BanDTO
BanDTO **json_to_ban_dto_array(const cJSON *json) {
    int size = cJSON_GetArraySize(json);
    BanDTO **bans = (BanDTO **)malloc((size + 1) * sizeof(BanDTO *));
    if (!bans) return NULL;

    for (int i = 0; i < size; ++i) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        BanDTO *ban = json_to_ban_dto(item);
        if (!ban) {
            for (int j = 0; j < i; ++j) {
                free(bans[j]);
            }
            free(bans);
            return NULL;
        }
        bans[i] = ban;
    }
    bans[size] = NULL;

    return bans;
}

// Function to free a single BanDTO
void free_ban_dto(BanDTO *ban) {
    if (ban) {
        free(ban->resource_type);
        free(ban->reason);
        free(ban);
    }
}

// Function to free an array of BanDTO
void free_ban_dto_array(BanDTO **bans) {
    if (bans) {
        for (int i = 0; bans[i] != NULL; ++i) {
            free_ban_dto(bans[i]);
        }
        free(bans);
    }
}

char *ban_dto_to_json_str(const BanDTO *ban) {
    cJSON *json = ban_dto_to_json(ban);
    char *json_str = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_str;
}

char *ban_dto_array_to_json_str(BanDTO **bans) {
    cJSON *json = ban_dto_array_to_json(bans);
    char *json_str = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_str;
}

BanDTO *json_str_to_ban_dto(const char *str) {
    cJSON *json = cJSON_Parse(str);
    BanDTO *dto = json_to_ban_dto(json);
    cJSON_Delete(json);
    return dto;
}

BanDTO **json_tr_to_ban_dto_array(const char *str) {
    return NULL;
}
