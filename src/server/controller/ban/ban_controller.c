//
// Created by eproskuryakov on 5/30/24.
//

#include "ban_controller.h"
#include "service/ban_service.h"
#include "dto/ban/ban_dto.h"
#include "mongoose.h"
#include <stdlib.h>
#include <string.h>

// Utility function to extract JSON body from HTTP message
static char *extract_json_body(struct mg_http_message *hm) {
    char *json_body = (char *)malloc(hm->body.len + 1);
    if (!json_body) return NULL;
    strncpy(json_body, hm->body.buf, hm->body.len);
    json_body[hm->body.len] = '\0';
    return json_body;
}

//int ban_controller_handle_get_bans(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
//    BanDTO **bans = ban_service_find();
//    if (!bans) {
//        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error reading bans");
//        return 0;
//    }
//
//    char *response = bans_to_json_array(bans);
//    if (!response) {
//        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error creating JSON response");
//        return 0;
//    }
//
//    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
//    free(response);
//    free_ban_array(bans);
//    return 0;
//}

int ban_controller_handle_get_ban_by_id(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    long user_id = atol(params[0]);
    long resource_id = atol(params[1]);
    char *resource_type = params[2];

    BanDTO *ban = ban_service_get_ban(resource_id, user_id, resource_type);
    if (!ban) {
        mg_http_reply(nc, 404, "Content-Type: text/plain\r\n", "BanDTO not found");
        return 0;
    }

    char *response = ban_dto_to_json_str(ban);
    if (!response) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error creating JSON response");
        return 0;
    }

    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
    free(response);
    free_ban_dto(ban);
    return 0;
}

int ban_controller_handle_create_ban(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    char *json_body = extract_json_body(hm);
    if (!json_body) {
        mg_http_reply(nc, 400, "", "Invalid JSON\n");
        return 0;
    }

    BanDTO *ban_dto = json_str_to_ban_dto(json_body);
    free(json_body);
    if (!ban_dto) {
        mg_http_reply(nc, 400, "", "Invalid JSON\n");
        return 0;
    }

    BanDTO *ban = ban_service_create_ban(ban_dto);
    free_ban_dto(ban_dto);
    if (!ban) {
        mg_http_reply(nc, 500, "", "Failed to create ban\n");
        return 0;
    }

    char *response_json = ban_dto_to_json_str(ban);
    if (!response_json) {
        mg_http_reply(nc, 500, "", "Failed to serialize ban\n");
        free_ban_dto(ban);
        return 0;
    }

    mg_http_reply(nc, 201, "Content-Type: application/json\r\n", "%s", response_json);
    free(response_json);
    free_ban_dto(ban);
    return 0;
}

int ban_controller_handle_update_ban(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    char *json_body = extract_json_body(hm);
    if (!json_body) {
        mg_http_reply(nc, 400, "", "Invalid JSON\n");
        return 0;
    }

    BanDTO *ban_dto = json_str_to_ban_dto(json_body);
    free(json_body);
    if (!ban_dto) {
        mg_http_reply(nc, 400, "", "Invalid JSON\n");
        return 0;
    }

    BanDTO *ban = ban_service_update_ban(ban_dto);
    free_ban_dto(ban_dto);
    if (!ban) {
        mg_http_reply(nc, 500, "", "Failed to update ban\n");
        return 0;
    }

    char *response_json = ban_dto_to_json_str(ban);
    if (!response_json) {
        mg_http_reply(nc, 500, "", "Failed to serialize ban\n");
        free_ban_dto(ban);
        return 0;
    }

    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response_json);
    free(response_json);
    free_ban_dto(ban);
    return 0;
}

int ban_controller_handle_delete_ban_by_id(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    long user_id = atol(params[0]);
    long resource_id = atol(params[1]);
    char *resource_type = params[2];

    ban_service_delete_ban(resource_id, user_id, resource_type);
    mg_http_reply(nc, 202, "", "");
    return 0;
}

int ban_controller_handle_annul_ban(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    long resource_id = atoll(params[0]);
    long user_id = atol(params[1]);
    const char *resource_type = params[2];

    ban_service_annul_ban(resource_id, user_id, resource_type);
    mg_http_reply(nc, 200, "Content-Type: text/plain\r\n", "Ban annulled");
    return 0;
}

int ban_controller_handle_get_actual_bans_by_user_id(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    long user_id = atol(params[0]);
    BanDTO **bans = ban_service_find_all_actual_bans_for_user_id(user_id);
    if (!bans) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error reading bans");
        return 0;
    }

    char *response = ban_dto_array_to_json_str(bans);
    if (!response) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error creating JSON response");
        return 0;
    }

    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
    free(response);
    free_ban_dto_array(bans);
    return 0;
}

int ban_controller_handle_get_actual_bans_by_resource(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    const char *resource_type = params[0];
    long resource_id = atoll(params[1]);
    BanDTO **bans = ban_service_find_all_actual_bans_for_resource(resource_type, resource_id);
    if (!bans) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error reading bans");
        return 0;
    }

    char *response = ban_dto_array_to_json_str(bans);
    if (!response) {
        mg_http_reply(nc, 500, "Content-Type: text/plain\r\n", "Error creating JSON response");
        return 0;
    }

    mg_http_reply(nc, 200, "Content-Type: application/json\r\n", "%s", response);
    free(response);
    free_ban_dto_array(bans);
    return 0;
}

void ban_controller_start() {
    ban_service_start();
}

void ban_controller_stop() {
    ban_service_stop();
}

