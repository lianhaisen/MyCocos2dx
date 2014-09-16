#ifndef __SQLITEHELPER_H__
#define __SQLITEHELPER_H__

#include "SQLite3unit.h"

class SqliteHelper
{
public:
	SqliteHelper(void);
	~SqliteHelper(void);

	static TSQLite3DB* getSqliteHandler();
	
};


#endif //__UIGRID_H__