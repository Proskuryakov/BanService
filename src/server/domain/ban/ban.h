//
// Created by eproskuryakov on 5/28/24.
//

#ifndef BANSERVICE_BAN_H
#define BANSERVICE_BAN_H
typedef struct {
    long resource_id;
    long user_id;
    char *resource_type;
} BanPK;

typedef struct {
    BanPK key;
    char *reason;
    long moderator_id;
    long long created_at; // Timestamp
    long long updated_at; // Timestamp
    long long expiration_date; // Timestamp
} Ban;

// Function declarations
void free_ban(Ban *ban);
void free_ban_array(Ban **bans);

#endif //BANSERVICE_BAN_H
