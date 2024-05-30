//
// Created by eproskuryakov on 5/28/24.
//

#ifndef BANSERVICE_ENV_H
#define BANSERVICE_ENV_H

#include <stdlib.h>

// Macro to get the environment variable value
#define GET_ENV_VAR(name) getenv(name)

// Macro to get the environment variable value or a default value if not set
#define GET_ENV_VAR_OR_DEFAULT(name, default_value) (getenv(name) ? getenv(name) : default_value)



#endif //BANSERVICE_ENV_H
