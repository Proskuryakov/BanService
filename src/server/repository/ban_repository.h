//
// Created by eproskuryakov on 5/28/24.
//

#ifndef BANSERVICE_BAN_REPOSITORY_H
#define BANSERVICE_BAN_REPOSITORY_H

#include "domain/ban/ban.h"

// Initialize repository with database connection
int ban_repo_init(const char *conninfo);
void ban_repo_close();

// CRUD operations
Ban* ban_repo_create(const Ban *ban);
Ban* ban_repo_update(const Ban *ban);
int ban_repo_delete(const BanPK *key);
Ban* ban_repo_find(const BanPK *key);

// Additional methods
Ban** ban_repo_find_all_by_user_id(long user_id);
Ban** ban_repo_find_all_by_resource_type_and_id(const char *resource_type, long resource_id);

Ban **ban_repository_find_active_bans_by_user_id(long long user_id, long long time);
Ban **ban_repository_find_active_bans_by_resource_type_and_id(const char *resource_type, long long resource_id, long long time);


#endif //BANSERVICE_BAN_REPOSITORY_H
