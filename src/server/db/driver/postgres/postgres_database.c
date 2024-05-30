//
// Created by eproskuryakov on 5/28/24.
//

#include <string.h>
#include "postgres_database.h"

PGconn* db_open(const char *conninfo) {
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }
    return conn;
}

void db_close(PGconn *conn) {
    PQfinish(conn);
}

int db_execute(PGconn *conn, const char *sql) {
    PGresult *res = PQexec(conn, sql);
    ExecStatusType status = PQresultStatus(res);
    PQclear(res);
    return status == PGRES_COMMAND_OK ? 0 : -1;
}

int db_execute_param(PGconn *conn, const char *sql, int param_count, const char **param_values) {
    PGresult *res = PQexecParams(conn, sql, param_count, NULL, param_values, NULL, NULL, 0);
    ExecStatusType status = PQresultStatus(res);
    PQclear(res);
    return status == PGRES_COMMAND_OK ? 0 : -1;
}

PGresult* db_query(PGconn *conn, const char *sql) {
    return PQexec(conn, sql);
}

PGresult* db_query_param(PGconn *conn, const char *sql, int param_count, const char **param_values) {
    return PQexecParams(conn, sql, param_count, NULL, param_values, NULL, NULL, 0);
}

void db_free_query_result(PGresult *res) {
    PQclear(res);
}