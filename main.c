#include <stdio.h>
#include <stdlib.h>

#include <libpq-fe.h>

#define HOST "localhost"
#define PORT "5432"

#define DBNAME "postgres"
#define USER "arthur"
#define PASS "12345"

const char *conn_info
    = "postgresql://" USER ":" PASS "@" HOST ":" PORT "/" DBNAME;

int
main (void)
{
  PGconn *conn = PQconnectdb (conn_info);

  if (PQstatus (conn) != CONNECTION_OK)
    abort ();

  const char *cmd = "SELECT * FROM users;";
  PGresult *res = PQexecParams (conn, cmd, 0, NULL, NULL, NULL, NULL, 0);

  if (PQresultStatus (res) != PGRES_TUPLES_OK)
    abort ();

  int ntpls = PQntuples (res);

  for (int i = 0; i < ntpls; i++)
    {
      printf ("(id: %s, ", PQgetvalue (res, i, 0));
      printf ("name: %s, ", PQgetvalue (res, i, 1));
      printf ("age: %s)\n", PQgetvalue (res, i, 2));
    }

  PQclear (res);

  PQfinish (conn);
}
