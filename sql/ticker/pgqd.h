
#ifndef __PGQD_H__
#define __PGQD_H__

#include <string.h>

#define Assert(x)

#include <usual/list.h>
#include <usual/statlist.h>
#include <usual/logging.h>
#include <usual/pgsocket.h>

enum DbState {
	DB_CLOSED,
	DB_TICKER_CHECK_PGQ,
	DB_TICKER_CHECK_VERSION,
	DB_TICKER_RUN,
	DB_MAINT_TEST_VERSION,
	DB_MAINT_LOAD_OPS,
	DB_MAINT_OP,
	DB_MAINT_LOAD_QUEUES,
	DB_MAINT_ROT1,
	DB_MAINT_ROT2,
	DB_MAINT_VACUUM_LIST,
	DB_MAINT_DO_VACUUM,
};

struct MaintOp;

struct PgDatabase {
	struct List head;
	const char *name;
	struct PgSocket *c_ticker;
	struct PgSocket *c_maint;
	struct PgSocket *c_retry;
	bool has_pgq;
	enum DbState state;
	enum DbState maint_state;
	bool dropped;

	struct StrList *maint_item_list;
	struct StatList maint_op_list;
	struct MaintOp *cur_maint;
};

struct Config {
	const char *config_file;
	const char *pidfile;
	const char *logfile;
	const char *base_connstr;
	const char *initial_database;
	const char *database_list;
	int syslog;
	double retry_period;
	double check_period;
	double maint_period;
	double ticker_period;
};

extern struct Config cf;


void launch_ticker(struct PgDatabase *db);
void launch_maint(struct PgDatabase *db);
void launch_retry(struct PgDatabase *db);

void free_maint(struct PgDatabase *db);

const char *make_connstr(const char *dbname);

#endif

