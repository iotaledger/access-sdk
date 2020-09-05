#include "config.h"
#include "../ntdb.h"
#include "../private.h"
#include "tap-interface.h"
#include "logging.h"
#include "helpapi-external-agent.h"

int main(int argc, char *argv[])
{
	unsigned int i;
	struct ntdb_context *ntdb;
	int flags[] = { NTDB_INTERNAL, NTDB_DEFAULT, NTDB_NOMMAP,
			NTDB_INTERNAL|NTDB_CONVERT, NTDB_CONVERT,
			NTDB_NOMMAP|NTDB_CONVERT };
	NTDB_DATA key = ntdb_mkdata("key", 3);
	NTDB_DATA data = ntdb_mkdata("data", 4);

	plan_tests(sizeof(flags) / sizeof(flags[0]) * 7 + 1);
	for (i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
		ntdb = ntdb_open("run-simple-delete.ntdb",
				 flags[i]|MAYBE_NOSYNC,
				 O_RDWR|O_CREAT|O_TRUNC, 0600, &tap_log_attr);
		ok1(ntdb);
		if (ntdb) {
			/* Delete should fail. */
			ok1(ntdb_delete(ntdb, key) == NTDB_ERR_NOEXIST);
			ok1(ntdb_check(ntdb, NULL, NULL) == 0);
			/* Insert should succeed. */
			ok1(ntdb_store(ntdb, key, data, NTDB_INSERT) == 0);
			ok1(ntdb_check(ntdb, NULL, NULL) == 0);
			/* Delete should now work. */
			ok1(ntdb_delete(ntdb, key) == 0);
			ok1(ntdb_check(ntdb, NULL, NULL) == 0);
			ntdb_close(ntdb);
		}
	}
	ok1(tap_log_messages == 0);
	return exit_status();
}
