//
// Created by eproskuryakov on 5/25/24.
//

// routes.c

#include "routes.h"
#include <stdlib.h>
#include <string.h>

#define MAX_ROUTES 100
#define MAX_PARAMS 10

#include "swagger/swagger_controller.h"
#include "controller/ban/ban_controller.h"

typedef struct {
    const char *method;
    const char *pattern;
    RequestHandler handler;
} Route;

#define MAX_ROUTES 100

static Route specific_routes[MAX_ROUTES];
static Route wildcard_routes[MAX_ROUTES];
static int specific_route_count = 0;
static int wildcard_route_count = 0;

static char* extract_http_info(struct mg_str chunk) {
    // Allocate memory for the chunk string with null-terminator
    char *method_str = (char *)malloc(chunk.len + 1);
    if (method_str == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy chunk to the new string and add null-terminator
    strncpy(method_str, chunk.buf, chunk.len);
    method_str[chunk.len] = '\0';

    return method_str;
}

void add_route(const char *method, const char *pattern, RequestHandler handler) {
    if (strstr(pattern, "/*") != NULL) {
        if (wildcard_route_count < MAX_ROUTES) {
            wildcard_routes[wildcard_route_count].method = method;
            wildcard_routes[wildcard_route_count].pattern = pattern;
            wildcard_routes[wildcard_route_count].handler = handler;
            wildcard_route_count++;
        }
    } else {
        specific_routes[specific_route_count].method = method;
        specific_routes[specific_route_count].pattern = pattern;
        specific_routes[specific_route_count].handler = handler;
        specific_route_count++;
    }
}

static char **split_path(const char *path, int *count) {
    char *path_copy = strdup(path);
    char *token;
    char **tokens = NULL;
    int i = 0;

    token = strtok(path_copy, "/");
    while (token != NULL) {
        tokens = realloc(tokens, sizeof(char *) * (i + 1));
        tokens[i++] = strdup(token);
        token = strtok(NULL, "/");
    }
    free(path_copy);

    *count = i;
    return tokens;
}

static int match_route(const char *pattern, const char *url, char **params) {
    int pattern_count, url_count;
    char **pattern_parts = split_path(pattern, &pattern_count);
    char **url_parts = split_path(url, &url_count);

    int wildcard = (pattern_count > 0 && strcmp(pattern_parts[pattern_count - 1], "*") == 0);

    if (!wildcard && pattern_count != url_count) {
        for (int i = 0; i < pattern_count; i++) free(pattern_parts[i]);
        free(pattern_parts);
        for (int i = 0; i < url_count; i++) free(url_parts[i]);
        free(url_parts);
        return 0;
    }

    int param_index = 0;
    for (int i = 0; i < pattern_count; ++i) {
        if (wildcard && i == pattern_count - 1) {
            break; // Wildcard match
        }
        if (pattern_parts[i][0] == '{' && pattern_parts[i][strlen(pattern_parts[i]) - 1] == '}') {
            params[param_index++] = strdup(url_parts[i]);
        } else if (strcmp(pattern_parts[i], url_parts[i]) != 0) {
            for (int j = 0; j < pattern_count; j++) free(pattern_parts[j]);
            free(pattern_parts);
            for (int j = 0; j < url_count; j++) free(url_parts[j]);
            free(url_parts);
            return 0;
        }
    }

    for (int i = 0; i < pattern_count; i++) free(pattern_parts[i]);
    free(pattern_parts);
    for (int i = 0; i < url_count; i++) free(url_parts[i]);
    free(url_parts);
    return 1;
}

void route_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    char *params[MAX_PARAMS];

    if (ev == MG_EV_HTTP_MSG) {
        char *method_name = extract_http_info(hm->method);
        char *path = extract_http_info(hm->uri);
        for (int i = 0; i < specific_route_count; ++i) {
            if (strcmp(specific_routes[i].method, method_name) == 0 && match_route(specific_routes[i].pattern, path, params)) {
                if (specific_routes[i].handler(nc, hm, params) == 0) {
                    return;  // Successfully handled
                }
            }
        }

        RequestHandler best_match = NULL;
        int best_match_length = -1;
        for (int i = 0; i < wildcard_route_count; i++) {
            const char *wildcard_pos = strstr(wildcard_routes[i].pattern, "/*");
            if (wildcard_pos) {
                int base_length = wildcard_pos - wildcard_routes[i].pattern;
                if (strncmp(path, wildcard_routes[i].pattern, base_length) == 0) {
                    if (base_length > best_match_length) {
                        if (match_route(wildcard_routes[i].pattern, path, params)) {
                            best_match = wildcard_routes[i].handler;
                            best_match_length = base_length;
                        }
                    }
                }
            }
        }

        if (best_match >= 0) {
            if (best_match(nc, hm, params) == 0) {
                return;  // Successfully handled
            }
        }

        mg_http_reply(nc, 404, 0, "Content-Type: text/plain");
        mg_printf(nc, "%s", "Not Found");
        free(path);
        free(method_name);
    }
}


void register_routes() {
    add_route("GET", "/v1/ban/resource/{resource_type}/id/{resource_id}/user/{user_id}", ban_controller_handle_get_ban_by_id);
    add_route("POST", "/v1/ban/", ban_controller_handle_create_ban);
    add_route("PUT", "/v1/ban/", ban_controller_handle_update_ban);
    add_route("DELETE", "/v1/ban/resource/{resource_type}/id/{resource_id}/user/{user_id}", ban_controller_handle_delete_ban_by_id);
    add_route("GET", "/v1/ban/user/{user_id}/actual", ban_controller_handle_get_actual_bans_by_user_id);
    add_route("GET", "/v1/ban/resource/{resource_type}/id/{resource_id}/actual", ban_controller_handle_get_actual_bans_by_resource);
    add_route("PUT", "/v1/ban/resource/{resource_type}/id/{resource_id}/user/{user_id}/annul", ban_controller_handle_annul_ban);

    add_route("GET", "/swagger", swagger_handle_index);
    add_route("GET", "/swagger/*", swagger_handle_static_swagger);
    add_route("GET", "/*", swagger_handle_static);
    add_route("GET", "/swagger.json", swagger_handle_api);
}

void enable_controllers() {
    register_routes();
    ban_controller_start();
}

void free_controllers() {
    ban_controller_stop();
}
