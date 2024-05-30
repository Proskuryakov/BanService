//
// Created by eproskuryakov on 5/29/24.
//

#ifndef BANSERVICE_BAN_SERVICE_H
#define BANSERVICE_BAN_SERVICE_H

#include "dto/ban/ban_dto.h"

void ban_service_start();
void ban_service_stop();

// Function declarations
BanDTO *ban_service_create_ban(const BanDTO *ban_dto);
BanDTO *ban_service_get_ban(long long resource_id, long long user_id, const char *resource_type);
BanDTO *ban_service_update_ban(const BanDTO *ban_dto);
void ban_service_delete_ban(long long resource_id, long long user_id, const char *resource_type);
BanDTO **ban_service_find_all_bans_by_user_id(long long user_id);
BanDTO **ban_service_find_all_bans_by_resource_type_and_id(const char *resource_type, long long resource_id);
void ban_service_annul_ban(long long resource_id, long user_id, const char *resource_type);
BanDTO ** ban_service_find_all_actual_bans_for_user_id(long long user_id);
BanDTO ** ban_service_find_all_actual_bans_for_resource(const char *resource_type, long long resource_id);

#endif //BANSERVICE_BAN_SERVICE_H
