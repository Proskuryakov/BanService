//
// Created by eproskuryakov on 5/26/24.
//

#ifndef CHATSERVER_SWAGGER_CONTROLLER_H
#define CHATSERVER_SWAGGER_CONTROLLER_H

#include "mongoose.h"

int swagger_handle_index(struct mg_connection *nc, struct mg_http_message *hm, char **params);

int swagger_handle_static_swagger(struct mg_connection *nc, struct mg_http_message *hm, char **params);

int swagger_handle_api(struct mg_connection *nc, struct mg_http_message *hm, char **params);

int swagger_handle_static(struct mg_connection *nc, struct mg_http_message *hm, char **params);

#endif //CHATSERVER_SWAGGER_CONTROLLER_H
