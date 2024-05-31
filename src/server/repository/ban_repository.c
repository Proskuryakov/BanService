//
// Created by eproskuryakov on 5/28/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ban_repository.h"
#include "db/driver/postgres/postgres_database.h"

// Static database connection variable
static PGconn *db_conn;

int ban_repo_init(const char *conninfo) {
    db_conn = db_open(conninfo);
    return db_conn != NULL ? 1 : 0;
}

// Function to close database connection
void ban_repo_close() {
    if (db_conn != NULL) {
        db_close(db_conn);
        db_conn = NULL;
    }
}

// CRUD operations

Ban* ban_repo_create(const Ban *ban) {
    const char *sql = "INSERT INTO bans (resource_id, user_id, resource_type, reason, moderator_id, created_at, updated_at, expiration_date) VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    const char *param_values[8] = {NULL};
    param_values[0] = (char *) malloc(32); // Allocate memory for resource_id
    sprintf((char *) param_values[0], "%ld", ban->key.resource_id);
    param_values[1] = (char *) malloc(32); // Allocate memory for user_id
    sprintf((char *) param_values[1], "%ld", ban->key.user_id);
    param_values[2] = strdup(ban->key.resource_type);
    param_values[3] = strdup(ban->reason);
    param_values[4] = (char *) malloc(32); // Allocate memory for moderator_id
    sprintf((char *) param_values[4], "%ld", ban->moderator_id);
    param_values[5] = (char *) malloc(32); // Allocate memory for created_at
    sprintf((char *) param_values[5], "%lld", ban->created_at);
    param_values[6] = (char *) malloc(32); // Allocate memory for updated_at
    sprintf((char *) param_values[6], "%lld", ban->updated_at);
    param_values[7] = (char *) malloc(32); // Allocate memory for expiration_date
    sprintf((char *) param_values[7], "%lld", ban->expiration_date);

    int err = db_execute_param(db_conn, sql, 8, param_values);

    // Free allocated memory
    for (int i = 0; i < 8; i++) {
        free(param_values[i]);
    }

    if (err) {
        return NULL;
    }

    Ban *created_ban = (Ban *)malloc(sizeof(Ban));
    if (!created_ban) return NULL;

    // Populate the created ban entity (just for demonstration)
    created_ban->key.resource_id = ban->key.resource_id;
    created_ban->key.user_id = ban->key.user_id;
    created_ban->key.resource_type = strdup(ban->key.resource_type);
    created_ban->reason = strdup(ban->reason);
    created_ban->moderator_id = ban->moderator_id;
    created_ban->created_at = ban->created_at;
    created_ban->updated_at = ban->updated_at;
    created_ban->expiration_date = ban->expiration_date;

    return created_ban;
}

Ban* ban_repo_update(const Ban *ban) {
    const char *sql = "UPDATE bans SET reason=$1, moderator_id=$2, created_at=$3, updated_at=$4, expiration_date=$5 WHERE resource_id=$6 AND user_id=$7 AND resource_type=$8";
    const char *param_values[8] = {NULL};
    param_values[0] = strdup(ban->reason);
    param_values[1] = (char *) malloc(32); // Allocate memory for moderator_id
    sprintf((char *) param_values[1], "%ld", ban->moderator_id);
    param_values[2] = (char *) malloc(32); // Allocate memory for created_at
    sprintf((char *) param_values[2], "%lld", ban->created_at);
    param_values[3] = (char *) malloc(32); // Allocate memory for updated_at
    sprintf((char *) param_values[3], "%lld", ban->updated_at);
    param_values[4] = (char *) malloc(32); // Allocate memory for expiration_date
    sprintf((char *) param_values[4], "%lld", ban->expiration_date);
    param_values[5] = (char *) malloc(32); // Allocate memory for resource_id
    sprintf((char *) param_values[5], "%ld", ban->key.resource_id);
    param_values[6] = (char *) malloc(32); // Allocate memory for user_id
    sprintf((char *) param_values[6], "%ld", ban->key.user_id);
    param_values[7] = strdup(ban->key.resource_type);

    int err = db_execute_param(db_conn, sql, 8, param_values);

    // Free allocated memory
    for (int i = 1; i < 8; i++) {
        free(param_values[i]);
    }

    if (err) {
        return NULL;
    }

    Ban *updated_ban = (Ban *)malloc(sizeof(Ban));
    if (!updated_ban) return NULL;

    // Populate the created ban entity (just for demonstration)
    updated_ban->key.resource_id = ban->key.resource_id;
    updated_ban->key.user_id = ban->key.user_id;
    updated_ban->key.resource_type = strdup(ban->key.resource_type);
    updated_ban->reason = strdup(ban->reason);
    updated_ban->moderator_id = ban->moderator_id;
    updated_ban->created_at = ban->created_at;
    updated_ban->updated_at = ban->updated_at;
    updated_ban->expiration_date = ban->expiration_date;

    return updated_ban;
}

int ban_repo_delete(const BanPK *key) {
    const char *sql = "DELETE FROM bans WHERE resource_id=$1 AND user_id=$2 AND resource_type=$3";
    const char *param_values[3] = {NULL};
    param_values[0] = (char *) malloc(32); // Allocate memory for resource_id
    sprintf((char *) param_values[0], "%ld", key->resource_id);
    param_values[1] = (char *) malloc(32); // Allocate memory for user_id
    sprintf((char *) param_values[1], "%ld", key->user_id);
    param_values[2] = strdup(key->resource_type);

    int result = db_execute_param(db_conn, sql, 3, param_values);

    // Free allocated memory
    for (int i = 0; i < 3; i++) {
        free(param_values[i]);
    }

    return result;
}

Ban *ban_repo_find(const BanPK *key) {
    const char *sql = "SELECT * FROM bans WHERE resource_id=$1 AND user_id=$2 AND resource_type=$3";
    const char *param_values[3] = {NULL};
    param_values[0] = (char *) malloc(32); // Allocate memory for resource_id
    sprintf((char *) param_values[0], "%ld", key->resource_id);
    param_values[1] = (char *) malloc(32); // Allocate memory for user_id
    sprintf((char *) param_values[1], "%ld", key->user_id);
    param_values[2] = strdup(key->resource_type);

    PGresult *res = db_query_param(db_conn, sql, 3, param_values);

    // Free allocated memory
    for (int i = 0; i < 3; i++) {
        free(param_values[i]);
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        return NULL;
    }

    Ban *ban = (Ban *) malloc(sizeof(Ban));
    ban->key.resource_id = atol(PQgetvalue(res, 0, 0));
    ban->key.user_id = atol(PQgetvalue(res, 0, 1));
    ban->key.resource_type = strdup(PQgetvalue(res, 0, 2));
    ban->reason = strdup(PQgetvalue(res, 0, 3));
    ban->moderator_id = atol(PQgetvalue(res, 0, 4));
    ban->created_at = atoll(PQgetvalue(res, 0, 5));
    ban->updated_at = atoll(PQgetvalue(res, 0, 6));
    ban->expiration_date = atoll(PQgetvalue(res, 0, 7));

    PQclear(res);
    return ban;
}

Ban **ban_repo_find_all_by_user_id(long user_id) {
    const char *sql = "SELECT * FROM bans WHERE user_id=$1";
    const char *param_values[1] = {NULL};
    param_values[0] = (char *) malloc(32); // Allocate memory for user_id
    sprintf((char *) param_values[0], "%ld", user_id);

    PGresult *res = db_query_param(db_conn, sql, 1, param_values);

    // Free allocated memory
    free(param_values[0]);

    int num_rows = PQntuples(res);
    Ban **bans = (Ban **) malloc((num_rows + 1) * sizeof(Ban *)); // Allocate memory for ban pointers
    bans[num_rows] = NULL; // Null-terminate the array

    for (int i = 0; i < num_rows; ++i) {
        bans[i] = (Ban *) malloc(sizeof(Ban)); // Allocate memory for each ban
        bans[i]->key.resource_id = atol(PQgetvalue(res, i, 0));
        bans[i]->key.user_id = atol(PQgetvalue(res, i, 1));
        bans[i]->key.resource_type = strdup(PQgetvalue(res, i, 2));
        bans[i]->reason = strdup(PQgetvalue(res, i, 3));
        bans[i]->moderator_id = atol(PQgetvalue(res, i, 4));
        bans[i]->created_at = atoll(PQgetvalue(res, i, 5));
        bans[i]->updated_at = atoll(PQgetvalue(res, i, 6));
        bans[i]->expiration_date = atoll(PQgetvalue(res, i, 7));
    }

    PQclear(res);
    return bans;
}

Ban **ban_repo_find_all_by_resource_type_and_id(const char *resource_type, long resource_id) {
    const char *sql = "SELECT * FROM bans WHERE resource_type=$1 AND resource_id=$2";
    const char *param_values[2] = {NULL};
    param_values[0] = resource_type;
    param_values[1] = (char *) malloc(32); // Allocate memory for resource_id
    sprintf((char *) param_values[1], "%ld", resource_id);

    PGresult *res = db_query_param(db_conn, sql, 2, param_values);

    // Free allocated memory
    free(param_values[1]);

    int num_rows = PQntuples(res);
    Ban **bans = (Ban **) malloc((num_rows + 1) * sizeof(Ban *)); // Allocate memory for ban pointers
    bans[num_rows] = NULL; // Null-terminate the array

    for (int i = 0; i < num_rows; ++i) {
        bans[i] = (Ban *) malloc(sizeof(Ban)); // Allocate memory for each ban
        bans[i]->key.resource_id = atol(PQgetvalue(res, i, 0));
        bans[i]->key.user_id = atol(PQgetvalue(res, i, 1));
        bans[i]->key.resource_type = strdup(PQgetvalue(res, i, 2));
        bans[i]->reason = strdup(PQgetvalue(res, i, 3));
        bans[i]->moderator_id = atol(PQgetvalue(res, i, 4));
        bans[i]->created_at = atoll(PQgetvalue(res, i, 5));
        bans[i]->updated_at = atoll(PQgetvalue(res, i, 6));
        bans[i]->expiration_date = atoll(PQgetvalue(res, i, 7));
    }

    PQclear(res);
    return bans;
}

// Function to find active bans by user id
Ban **ban_repository_find_active_bans_by_user_id(long long user_id, long long time) {
    PGresult *res = NULL;
    const char *sql = "SELECT * FROM bans WHERE user_id = $1 AND (expiration_date > $2 OR expiration_date = 0)";
    char *param_values[2] = {NULL};
    param_values[0] = (char *) malloc(32);
    param_values[1] = (char *) malloc(32);
    sprintf(param_values[0], "%lld", user_id);
    sprintf(param_values[1], "%lld", time);

    // Execute the query
    res = db_query_param(db_conn, sql, 2, (const char **) param_values);
    if (!res) {
        fprintf(stderr, "Failed to execute query: %s\n", sql);
        return NULL;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        fprintf(stderr, "No bans found for user id: %lld\n", user_id);
        db_free_query_result(res);
        return NULL;
    }

    Ban **bans = (Ban **)malloc((rows + 1) * sizeof(Ban *));
    if (!bans) {
        fprintf(stderr, "Memory allocation failed\n");
        db_free_query_result(res);
        return NULL;
    }

    // Process the query result
    for (int i = 0; i < rows; ++i) {
        Ban *ban = (Ban *)malloc(sizeof(Ban));
        if (!ban) {
            fprintf(stderr, "Memory allocation failed\n");
            db_free_query_result(res);
            free_ban_array(bans);
            return NULL;
        }

        ban->key.resource_id = atol(PQgetvalue(res, i, 0));
        ban->key.user_id = atol(PQgetvalue(res, i, 1));
        ban->key.resource_type = strdup(PQgetvalue(res, i, 2));
        ban->reason = strdup(PQgetvalue(res, i, 3));
        ban->moderator_id = atol(PQgetvalue(res, i, 4));
        ban->created_at = atoll(PQgetvalue(res, i, 5));
        ban->updated_at = atoll(PQgetvalue(res, i, 6));
        ban->expiration_date = atoll(PQgetvalue(res, i, 7));

        bans[i] = ban;
    }
    bans[rows] = NULL;

    db_free_query_result(res);
    return bans;
}

// Function to find active bans by resource type and id
Ban **ban_repository_find_active_bans_by_resource_type_and_id(const char *resource_type, long long resource_id, long long time) {
    PGresult *res = NULL;
    const char *sql = "SELECT * FROM bans WHERE resource_type = $1 AND resource_id = $2 AND (expiration_date > $3 OR expiration_date = 0)";
    char *param_values[3] = {NULL};
    param_values[0] = strdup(resource_type);
    param_values[1] = (char *) malloc(32);
    param_values[2] = (char *) malloc(32);

    sprintf(param_values[1], "%lld", resource_id);
    sprintf(param_values[2], "%lld", time);


    // Execute the query
    res = db_query_param(db_conn, sql, 3, (const char **) param_values);
    for (int i = 0; i < 3; ++i) {
        free(param_values[i]);
    }
    if (!res) {
        fprintf(stderr, "Failed to execute query: %s\n", sql);
        return NULL;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        fprintf(stderr, "No bans found for resource type: %s, resource id: %lld\n", resource_type, resource_id);
        db_free_query_result(res);
        return NULL;
    }

    Ban **bans = (Ban **)malloc((rows + 1) * sizeof(Ban *));
    if (!bans) {
        fprintf(stderr, "Memory allocation failed\n");
        db_free_query_result(res);
        return NULL;
    }

    // Process the query result
    for (int i = 0; i < rows; ++i) {
        Ban *ban = (Ban *)malloc(sizeof(Ban));
        if (!ban) {
            fprintf(stderr, "Memory allocation failed\n");
            db_free_query_result(res);
            free_ban_array(bans);
            return NULL;
        }

        ban->key.resource_id = atol(PQgetvalue(res, i, 0));
        ban->key.user_id = atol(PQgetvalue(res, i, 1));
        ban->key.resource_type = strdup(PQgetvalue(res, i, 2));
        ban->reason = strdup(PQgetvalue(res, i, 3));
        ban->moderator_id = atol(PQgetvalue(res, i, 4));
        ban->created_at = atoll(PQgetvalue(res, i, 5));
        ban->updated_at = atoll(PQgetvalue(res, i, 6));
        ban->expiration_date = atoll(PQgetvalue(res, i, 7));

        bans[i] = ban;
    }
    bans[rows] = NULL;

    db_free_query_result(res);
    return bans;
}