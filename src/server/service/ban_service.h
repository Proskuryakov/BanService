//
// Created by eproskuryakov on 5/29/24.
//

#ifndef BANSERVICE_BAN_SERVICE_H
#define BANSERVICE_BAN_SERVICE_H

#include "dto/ban/ban_dto.h"

// Function declarations
BanDTO *create_ban(const BanDTO *ban_dto);
BanDTO *get_ban(long long resource_id, long long user_id, const char *resource_type);
BanDTO *update_ban(const BanDTO *ban_dto);
void delete_ban(long long resource_id, long long user_id, const char *resource_type);
BanDTO **find_all_bans_by_user_id(long long user_id);
BanDTO **find_all_bans_by_resource_type_and_id(const char *resource_type, long long resource_id);
void annul_ban(long long resource_id, long user_id, const char *resource_type);
BanDTO ** find_all_actual_bans_for_user_id(long long user_id);
BanDTO ** find_all_actual_bans_for_resource(const char *resource_type, long long resource_id);

#endif //BANSERVICE_BAN_SERVICE_H
