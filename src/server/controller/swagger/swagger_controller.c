//
// Created by eproskuryakov on 5/26/24.
//

#include "swagger_controller.h"
#include "mongoose.h"
#include <stdio.h>

static const char *swagger_ui_path = "./swagger-ui/dist";
static const char *swagger_json_path = "./swagger.json";

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

// Function to serve static files
static void serve_static(struct mg_connection *c, struct mg_http_message *hm, const char *file_path, int use_root) {
    struct mg_http_serve_opts opts = {};
    if (use_root) {
        opts.root_dir = swagger_ui_path;
    }
    mg_http_serve_file(c, hm, file_path, &opts);
}

// Handler for serving the Swagger UI and Swagger JSON
int swagger_handle_index(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    serve_static(nc, hm, "/index.html", 1);
    return 0;
}

int swagger_handle_static_swagger(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    char file_path[256];
    char *path = extract_http_info(hm->uri);
    snprintf(file_path, sizeof(file_path), "%s", path + 8); // +8 to skip "/swagger"
    serve_static(nc, hm, file_path, 1);
    free(path);
    return 0;
}

int swagger_handle_static(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    char file_path[256];
    char *path = extract_http_info(hm->uri);
    snprintf(file_path, sizeof(file_path), "%s", path); // +8 to skip "/swagger"
    serve_static(nc, hm, file_path, 1);
    free(path);
    return 0;
}

int swagger_handle_api(struct mg_connection *nc, struct mg_http_message *hm, char **params) {
    serve_static(nc, hm, swagger_json_path, 0);
    return 0;
}