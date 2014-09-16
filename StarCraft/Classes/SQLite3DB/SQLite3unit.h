
#ifndef SQLite3_unitH
#define SQLite3_unitH

#define SQLITE_ENABLE_EXTFUNC
#define SQLITE_HAS_CODEC 1
#include "sqlite3.h"


#include <list>
#include <vector>
#include <string>

#define TSQLITE_ERROR 1000

using namespace std;

typedef std::vector<std::string> StrVector;
typedef std::vector<int> IntVector;
//---------------------------------------------------------------------------
//异常类
//---------------------------------------------------------------------------
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
class TSQLite3Exception
{
public:

	TSQLite3Exception(const int nErrCode,
		const char *szErrMess,
		bool bDeleteMsg = true);

	TSQLite3Exception(const TSQLite3Exception  &e);

	virtual ~TSQLite3Exception();

	int errorCode() const
	{
		return mnErrCode;
	}

	char *errorMessage() const
	{
		return mpszErrMess;
	}

	static const char *errorCodeAsString(int nErrCode);

private:

	int mnErrCode;
	char *mpszErrMess;
};
#endif

//---------------------------------------------------------------------------
//查询类
//---------------------------------------------------------------------------
class TSQLite3Query
{
public:

	TSQLite3Query();

	TSQLite3Query(const TSQLite3Query &rQuery);

	TSQLite3Query(sqlite3 *pDB,
		sqlite3_stmt *pVM,
		bool bEof,
		bool bOwnVM = true);

	TSQLite3Query &operator=(const TSQLite3Query &rQuery);

	virtual ~TSQLite3Query();

	int numFields();

	int fieldIndex(const char *szField);
	const char *fieldName(int nCol);

	const char *fieldDeclType(int nCol);
	int fieldDataType(int nCol);

	const char *fieldValue(int nField);
	const char *fieldValue(const char *szField);

	int getIntField(int nField, int nNullValue = 0);
	int getIntField(const char *szField, int nNullValue = 0);

	double getFloatField(int nField, double fNullValue = 0.0);
	double getFloatField(const char *szField, double fNullValue = 0.0);

	long long getInt64Field(int nField, long long llNullValue = 0);
	long long getInt64Field(const char *szField, long long llNullValue = 0);

	const char *getStringField(int nField, const char *szNullValue = "");
	const char *getStringField(const char *szField, const char *szNullValue = "");

	const unsigned char *getBlobField(int nField, int &nLen);
	const unsigned char *getBlobField(const char *szField, int &nLen);

	bool fieldIsNull(int nField);
	bool fieldIsNull(const char *szField);

	bool eof();
	void nextRow();
	void finalize();

private:

	void checkVM();

	sqlite3 *mpDB;
	sqlite3_stmt *mpVM;
	bool mbEof;
	int mnCols;
	bool mbOwnVM;
};


//---------------------------------------------------------------------------
//执行数据操作
//---------------------------------------------------------------------------
class TSQLite3DB
{
public:
	TSQLite3DB();
	virtual ~TSQLite3DB();
	void open(const char *szFile);
#ifdef SQLITE_HAS_CODEC
	void open(const char* szFile, const char* szPass);
	void rekey(const char* szPass);
#endif
	void close();
	int execDML(const char *szSQL);
	TSQLite3Query execQuery(const char *szSQL);
	void setBusyTimeout(int nMillisecs);
	sqlite_int64 lastRowId();

	//backup table to tempbak.db
	int backupTable( const char* dbname, StrVector tbname );
	//restore table from tempbak.db
	int restoreTable( const char* dbname, StrVector tbname );
	//执行脚本文件，执行成功返回0，否则返回失败的SQL语句数量，
	//每条SQL语句以分号结束（即使是最后一条SQL语句）
	int execSQLFile( const char* szFileName);
	//获取表的字段名列表
	StrVector getFieldName( const char* tbname );
	//判断一表名是否存在
	bool tableIsExist( const char* tbname );
	//判断表中某一字段名称是否存在
	bool fieldIsExist( const char* tbname, const char* fieldname);

	void interrupt()
	{
		sqlite3_interrupt(mpDB);
	}

	bool isAutoCommit()
	{
		return sqlite3_get_autocommit(mpDB) != 0;
	}

	static const char *SQLiteVersion()
	{
		return SQLITE_VERSION;
	}

	sqlite3 *getDB()
	{
		return mpDB;
	}
private:

	TSQLite3DB(const TSQLite3DB &db);
	TSQLite3DB &operator=(const TSQLite3DB &db);

	sqlite3_stmt *compile(const char *szSQL);

	void checkDB();

	sqlite3 *mpDB;
	int mnBusyTimeoutMs;
};

//---------------------------------------------------------------------------

#endif
