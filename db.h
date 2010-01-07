#if !defined(__DB_H)
#define __DB_H

#include <sqlite3.h>

#include "config.h"

#if !defined(DATABASE)
#	define	DATABASE	CONFIG_PATH "/bazylum.db"
#endif /* DATABASE */

sqlite3 *open_database();

#endif /* __DB_H */
