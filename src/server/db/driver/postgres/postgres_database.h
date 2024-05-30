//
// Created by eproskuryakov on 5/28/24.
//

#ifndef BANSERVICE_POSTGRES_DATABASE_H
#define BANSERVICE_POSTGRES_DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>

PGconn* db_open(const char *conninfo);
void db_close(PGconn *conn);
int db_execute(PGconn *conn, const char *sql);
int db_execute_param(PGconn *conn, const char *sql, int param_count, const char **param_values);
PGresult* db_query(PGconn *conn, const char *sql);
PGresult* db_query_param(PGconn *conn, const char *sql, int param_count, const char **param_values);
void db_free_query_result(PGresult *res);


#endif //BANSERVICE_POSTGRES_DATABASE_H
