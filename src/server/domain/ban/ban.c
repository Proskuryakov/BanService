//
// Created by eproskuryakov on 5/28/24.
//

#include <stdlib.h>
#include "ban.h"

// Function to free a single Ban object
void free_ban(Ban *ban) {
    if (ban) {
        free(ban->key.resource_type);
        free(ban->reason);
        free(ban);
    }
}

// Function to free an array of Ban objects
void free_ban_array(Ban **bans) {
    if (bans) {
        for (int i = 0; bans[i] != NULL; ++i) {
            free_ban(bans[i]);
        }
        free(bans);
    }
}