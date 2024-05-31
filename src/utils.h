//
// Created by eproskuryakov on 5/28/24.
//

#ifndef BANSERVICE_UTILS_H
#define BANSERVICE_UTILS_H
// Macros for JSON extraction and assignment
#define JSON_GET_NUMBER(json, key, dest) do { \
    cJSON *item = cJSON_GetObjectItem(json, key); \
    if (item) dest = item->valuedouble; \
} while (0)

#define JSON_GET_STRING(json, key, dest) do { \
    cJSON *item = cJSON_GetObjectItem(json, key); \
    if (item && item->valuestring) dest = strdup(item->valuestring); \
} while (0)

#define JSON_GET_BOOL(json, key, dest) do { \
    cJSON *item = cJSON_GetObjectItem(json, key); \
    if (item) dest = cJSON_IsTrue(item); \
} while (0)

long long utils_time_in_milliseconds();

#endif //BANSERVICE_UTILS_H
