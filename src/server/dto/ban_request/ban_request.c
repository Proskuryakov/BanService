//
// Created by eproskuryakov on 5/29/24.
//

#include "ban_request.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

// Function to convert BanRequest to JSON
cJSON *ban_request_to_json(const BanRequest *request) {
    cJSON *json = cJSON_CreateObject();
    if (!json) return NULL;

    cJSON_AddNumberToObject(json, "resource_id", request->resource_id);
    cJSON_AddNumberToObject(json, "user_id", request->user_id);
    cJSON_AddStringToObject(json, "resource_type", request->resource_type);
    cJSON_AddStringToObject(json, "reason", request->reason);
    cJSON_AddNumberToObject(json, "moderator_id", request->moderator_id);
    cJSON_AddNumberToObject(json, "duration", request->duration);

    return json;
}

// Function to convert JSON to BanRequest
BanRequest *json_to_ban_request(const cJSON *json) {
    BanRequest *request = (BanRequest *) malloc(sizeof(BanRequest));
    if (!request) return NULL;

    JSON_GET_NUMBER(json, "resource_id", request->resource_id);
    JSON_GET_NUMBER(json, "user_id", request->user_id);
    JSON_GET_STRING(json, "resource_type", request->resource_type);
    JSON_GET_STRING(json, "reason", request->reason);
    JSON_GET_NUMBER(json, "moderator_id", request->moderator_id);
    JSON_GET_NUMBER(json, "duration", request->duration);

    return request;
}

// Function to convert array of BanRequest to JSON
cJSON *ban_request_array_to_json(BanRequest **requests) {
    cJSON *json_array = cJSON_CreateArray();
    if (!json_array) return NULL;

    for (int i = 0; requests[i] != NULL; ++i) {
        cJSON *json = ban_request_to_json(requests[i]);
        if (!json) {
            cJSON_Delete(json_array);
            return NULL;
        }
        cJSON_AddItemToArray(json_array, json);
    }

    return json_array;
}

// Function to convert JSON to array of BanRequest
BanRequest **json_to_ban_request_array(const cJSON *json) {
    int size = cJSON_GetArraySize(json);
    BanRequest **requests = (BanRequest **) malloc((size + 1) * sizeof(BanRequest *));
    if (!requests) return NULL;

    for (int i = 0; i < size; ++i) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        BanRequest *request = json_to_ban_request(item);
        if (!request) {
            for (int j = 0; j < i; ++j) {
                free_ban_request(requests[j]);
            }
            free(requests);
            return NULL;
        }
        requests[i] = request;
    }
    requests[size] = NULL;

    return requests;
}

// Function to free a single BanRequest
void free_ban_request(BanRequest *request) {
    if (request) {
        free(request->resource_type);
        free(request->reason);
        free(request);
    }
}

// Function to free an array of BanRequest
void free_ban_request_array(BanRequest **requests) {
    if (requests) {
        for (int i = 0; requests[i] != NULL; ++i) {
            free_ban_request(requests[i]);
        }
        free(requests);
    }
}

BanRequest *json_str_to_ban_request(const char *str) {
    cJSON *json = cJSON_Parse(str);
    BanRequest *dto = json_to_ban_request(json);
    cJSON_Delete(json);
    return dto;
}

