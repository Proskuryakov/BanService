cmake_minimum_required(VERSION 3.27)
project(BanService C)

set(CMAKE_C_STANDARD 23)

include_directories(src/server)
include_directories(src)
include_directories(external/mongoose)

file(COPY external/swagger-ui DESTINATION ./)
configure_file(resources/swagger/swagger.json ./swagger.json COPYONLY)

link_directories(/usr/lib /usr/lib/x86_64-linux-gnu)

add_executable(BanService main.c
        external/mongoose/mongoose.c
        external/mongoose/mongoose.h
        src/server/db/driver/postgres/postgres_database.c
        src/server/db/driver/postgres/postgres_database.h
        src/server/domain/ban/ban.c
        src/server/domain/ban/ban.h
        src/server/repository/ban_repository.c
        src/server/repository/ban_repository.h
        src/server/dto/ban/ban_dto.c
        src/server/dto/ban/ban_dto.h
        src/utils.h
        src/server/controller/routes.h
        src/server/controller/routes.c
        src/server/controller/swagger/swagger_controller.h
        src/server/controller/swagger/swagger_controller.c
        src/env.h
        src/env.c
        src/server/dto/ban_request/ban_request.c
        src/server/dto/ban_request/ban_request.h
        src/server/service/ban_service.c
        src/server/service/ban_service.h
        src/utils.c
        src/server/controller/ban/ban_controller.c
        src/server/controller/ban/ban_controller.h
)

target_link_libraries(BanService -lpq -lcjson)