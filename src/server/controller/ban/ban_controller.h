//
// Created by eproskuryakov on 5/30/24.
//

#ifndef BANSERVICE_BAN_CONTROLLER_H
#define BANSERVICE_BAN_CONTROLLER_H
#include "mongoose.h"

// Function declarations for the Ban controller
//int ban_controller_handle_get_bans(struct mg_connection *nc, struct mg_http_message *hm, char **params);

int ban_controller_handle_get_ban_by_id(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int ban_controller_handle_create_ban(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int ban_controller_handle_update_ban(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int ban_controller_handle_delete_ban_by_id(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int ban_controller_handle_annul_ban(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int ban_controller_handle_get_actual_bans_by_user_id(struct mg_connection *nc, struct mg_http_message *hm, char **params);
int ban_controller_handle_get_actual_bans_by_resource(struct mg_connection *nc, struct mg_http_message *hm, char **params);


void ban_controller_start();
void ban_controller_stop();
#endif //BANSERVICE_BAN_CONTROLLER_H
