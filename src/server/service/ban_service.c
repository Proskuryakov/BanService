//
// Created by eproskuryakov on 5/29/24.
//

#include "ban_service.h"
#include "domain/ban/ban.h"
#include "repository/ban_repository.h"
#include <stdlib.h>
#include "utils.h"


// Function to convert BanDTO to Ban entity
static Ban *ban_dto_to_entity(const BanDTO *ban_dto) {
    if (!ban_dto) return NULL;

    Ban *ban_entity = (Ban *)malloc(sizeof(Ban));
    if (!ban_entity) return NULL;

    ban_entity->key.resource_id = ban_dto->resource_id;
    ban_entity->key.user_id = ban_dto->user_id;
    ban_entity->key.resource_type = strdup(ban_dto->resource_type);
    ban_entity->reason = strdup(ban_dto->reason);
    ban_entity->moderator_id = ban_dto->moderator_id;
    ban_entity->created_at = ban_dto->created_at;
    ban_entity->updated_at = ban_dto->updated_at;
    ban_entity->expiration_date = ban_dto->is_permanent ? 0 : ban_dto->expiration_date;

    return ban_entity;
}

// Function to convert Ban entity to BanDTO
static BanDTO *ban_entity_to_dto(const Ban *ban_entity) {
    if (!ban_entity) return NULL;

    BanDTO *ban_dto = (BanDTO *)malloc(sizeof(BanDTO));
    if (!ban_dto) return NULL;

    ban_dto->resource_id = ban_entity->key.resource_id;
    ban_dto->user_id = ban_entity->key.user_id;
    ban_dto->resource_type = strdup(ban_entity->key.resource_type);
    ban_dto->reason = strdup(ban_entity->reason);
    ban_dto->moderator_id = ban_entity->moderator_id;
    ban_dto->created_at = ban_entity->created_at;
    ban_dto->updated_at = ban_entity->updated_at;
    ban_dto->expiration_date = ban_entity->expiration_date;
    ban_dto->is_permanent = ban_entity->expiration_date ? 0 : 1;

    return ban_dto;
}

static BanDTO **bans_to_dtos(Ban **bans) {
    if (!bans) return NULL;

    int count = 0;
    while (bans[count] != NULL) count++;

    BanDTO **ban_dtos = (BanDTO **)malloc((count + 1) * sizeof(BanDTO *));
    if (!ban_dtos) {
        free_ban_array(bans);
        return NULL;
    }

    for (int i = 0; i < count; ++i) {
        ban_dtos[i] = ban_entity_to_dto(bans[i]);
    }
    ban_dtos[count] = NULL;

    free_ban_array(bans);

    return ban_dtos;
}

// Function to create a ban
BanDTO *create_ban(const BanDTO *ban_dto) {
    Ban *ban_entity = ban_dto_to_entity(ban_dto);
    if (!ban_entity) return NULL;

    long long timestamp = utils_time_in_milliseconds();
    ban_entity->created_at = timestamp;
    ban_entity->updated_at = timestamp;

    Ban *ban = ban_repo_create(ban_entity);
    free_ban(ban_entity);

    if (!ban) return NULL;
    BanDTO *created_ban = ban_entity_to_dto(ban);
    free_ban(ban_entity);
    free_ban(ban);

    return created_ban;
}

// Function to get a ban by resource id, user id, and resource type
BanDTO *get_ban(long long resource_id, long long user_id, const char *resource_type) {
    char *res_type = strdup(resource_type);
    BanPK pk = {resource_id, user_id, res_type};
    Ban *ban_entity = ban_repo_find(&pk);
    if (!ban_entity) return NULL;

    BanDTO *ban_dto = ban_entity_to_dto(ban_entity);
    free_ban(ban_entity);
    free(res_type);

    return ban_dto;
}

// Function to update a ban
BanDTO *update_ban(const BanDTO *ban_dto) {
    Ban *ban_entity = ban_dto_to_entity(ban_dto);
    if (!ban_entity) return NULL;

    long long timestamp = utils_time_in_milliseconds();
    ban_entity->updated_at = timestamp;

    Ban *updated_ban = ban_repo_update(ban_entity);
    free_ban(ban_entity);

    if (!updated_ban) return NULL;

    return ban_entity_to_dto(updated_ban);
}

// Function to delete a ban by resource id, user id, and resource type
void delete_ban(long long resource_id, long long user_id, const char *resource_type) {
    char *res_type = strdup(resource_type);
    BanPK pk = {resource_id, user_id, res_type};
    ban_repo_delete(&pk);
    free(res_type);
}

// Function to find all bans by user id
BanDTO **find_all_bans_by_user_id(long long user_id) {
    Ban **bans = ban_repo_find_all_by_user_id(user_id);
    return bans_to_dtos(bans);
}

// Function to find all bans by resource type and id
BanDTO **find_all_bans_by_resource_type_and_id(const char *resource_type, long long resource_id) {
    Ban **bans = ban_repo_find_all_by_resource_type_and_id(resource_type, resource_id);
    return bans_to_dtos(bans);
}

// Function to annul a ban by setting its expiration time to 1
void annul_ban(long long resource_id, long user_id, const char *resource_type) {
    BanDTO *ban_dto = get_ban(resource_id, user_id, resource_type);
    if (!ban_dto) return; // Ban does not exist
    ban_dto->expiration_date = 1;
    UNUSED_PTR(update_ban(ban_dto))
    free_ban_dto(ban_dto);
}

BanDTO **find_all_actual_bans_for_user_id(long long int user_id) {
    long long timestamp = utils_time_in_milliseconds();
    Ban **bans = ban_repository_find_active_bans_by_user_id(user_id, timestamp);
    return bans_to_dtos(bans);
}

BanDTO **find_all_actual_bans_for_resource(const char *resource_type, long long int resource_id) {
    long long timestamp = utils_time_in_milliseconds();
    Ban **bans = ban_repository_find_active_bans_by_resource_type_and_id(resource_type, resource_id, timestamp);
    return bans_to_dtos(bans);
}
