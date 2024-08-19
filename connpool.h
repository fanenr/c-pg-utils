#ifndef CONNPOOL_H
#define CONNPOOL_H

#include <libpq-fe.h>
#include <pthread.h>

#define CONNPOOL_DEFAULT 4

#define attr_nonnull(...) __attribute__ ((nonnull (__VA_ARGS__)))

enum
{
  CONNPOOL_ERR_MTX,
  CONNPOOL_ERR_COND,
  CONNPOOL_ERR_MALLOC,
  CONNPOOL_ERR_CONNECT,
};

typedef struct connpool_conn_t connpool_conn_t;
typedef struct connpool_t connpool_t;

struct connpool_t
{
  size_t size;
  const char *info;

  connpool_conn_t *head;
  connpool_conn_t *tail;

  pthread_cond_t cond;
  pthread_mutex_t mtx;
};

struct connpool_conn_t
{
  PGconn *conn;
  connpool_conn_t *next;
};

void connpool_free (connpool_t *pool);

PGconn *connpool_acquire (connpool_t *pool);

void connpool_release (connpool_t *pool, PGconn *c);

int connpool_init (connpool_t *pool, const char *info, size_t n);

#endif
