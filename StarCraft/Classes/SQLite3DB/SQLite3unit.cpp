
#include <string.h>
#include <time.h>
#include <fstream>
//#pragma hdrstop 
#include "cocos2d.h"
using namespace cocos2d;

#include "SQLite3unit.h"
//#pragma package(smart_init)

// Named constant for passing to TSQLite3Exception when passing it a string
// that cannot be deleted.
static const bool DONT_DELETE_MSG = false;
// Timeout values on busy or lock conditions
static const int SQLTM_TIMEOUT   =  6000; // 6 seconds

#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
//class  TSQLite3Exception
////////////////////////////////////////////////////////////////////////////////
TSQLite3Exception::TSQLite3Exception(const int nErrCode,
	const char *szErrMess,
	bool bDeleteMsg) :
mnErrCode(nErrCode)
{
	mpszErrMess = sqlite3_mprintf("%s[%d]: %s",
		errorCodeAsString(nErrCode),
		nErrCode,
		szErrMess ? szErrMess : "");

	if(bDeleteMsg && szErrMess)
	{
		sqlite3_free(mpszErrMess);
	}
}
//---------------------------------------------------------------------------
TSQLite3Exception::TSQLite3Exception(const TSQLite3Exception  &e) :
mnErrCode(e.mnErrCode)
{
	mpszErrMess = 0;

	if(e.mpszErrMess)
	{
		mpszErrMess = sqlite3_mprintf("%s", e.mpszErrMess);
	}
}
//---------------------------------------------------------------------------
const char *TSQLite3Exception::errorCodeAsString(int nErrCode)
{
	switch(nErrCode)
	{
	case SQLITE_OK          :
		return "Successful result";
	case SQLITE_ERROR       :
		return "SQL error or missing database";
	case SQLITE_INTERNAL    :
		return "Internal logic error in SQLite ";
	case SQLITE_PERM        :
		return "Access permission denied";
	case SQLITE_ABORT       :
		return "Callback routine requested an abort";
	case SQLITE_BUSY        :
		return "The database file is locked";
	case SQLITE_LOCKED      :
		return "A table in the database is locked";
	case SQLITE_NOMEM       :
		return "A malloc() failed";
	case SQLITE_READONLY    :
		return "Attempt to write a readonly database";
	case SQLITE_INTERRUPT   :
		return "Operation terminated by sqlite3_interrupt()";
	case SQLITE_IOERR       :
		return "Some kind of disk I/O error occurred ";
	case SQLITE_CORRUPT     :
		return "The database disk image is malformed";
	case SQLITE_NOTFOUND    :
		return "NOT USED. Table or record not found";
	case SQLITE_FULL        :
		return "Insertion failed because database is full";
	case SQLITE_CANTOPEN    :
		return "Unable to open the database file";
	case SQLITE_PROTOCOL    :
		return "Database lock protocol error";
	case SQLITE_EMPTY       :
		return "Database is empty";
	case SQLITE_SCHEMA      :
		return "The database schema changed";
	case SQLITE_TOOBIG      :
		return "String or BLOB exceeds size limit";
	case SQLITE_CONSTRAINT  :
		return "Abort due to constraint violation";
	case SQLITE_MISMATCH    :
		return "Data type mismatch";
	case SQLITE_MISUSE      :
		return "Library used incorrectly";
	case SQLITE_NOLFS       :
		return "Uses OS features not supported on host";
	case SQLITE_AUTH        :
		return "Authorization denied";
	case SQLITE_FORMAT      :
		return "Auxiliary database format error";
	case SQLITE_RANGE       :
		return "2nd parameter to sqlite3_bind out of range";
	case SQLITE_NOTADB      :
		return "File opened that is not a database file";
	case SQLITE_ROW         :
		return "sqlite3_step() has another row ready";
	case SQLITE_DONE        :
		return "sqlite3_step() has finished executing";
	case TSQLITE_ERROR    :
		return "TSQLITE_ERROR";
	default:
		return "UNKNOWN_ERROR";
	}
}
//---------------------------------------------------------------------------
TSQLite3Exception::~TSQLite3Exception()
{
	if(mpszErrMess)
	{
		sqlite3_free(mpszErrMess);
		mpszErrMess = 0;
	}
}
#endif

//class TSQLite3Query
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
TSQLite3Query::TSQLite3Query()
{
	mpDB = 0;
	mpVM = 0;
	mbEof = true;
	mnCols = 0;
	mbOwnVM = false;
}
//---------------------------------------------------------------------------
TSQLite3Query::TSQLite3Query(const TSQLite3Query &rQuery)
{
	mpDB = rQuery.mpDB;
	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<TSQLite3Query &>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
}
//---------------------------------------------------------------------------
TSQLite3Query::TSQLite3Query(sqlite3 *pDB,
	sqlite3_stmt *pVM,
	bool bEof,
	bool bOwnVM)
{
	mpDB = pDB;
	mpVM = pVM;
	mbEof = bEof;
	mnCols = sqlite3_column_count(mpVM);
	mbOwnVM = bOwnVM;
}
//---------------------------------------------------------------------------
TSQLite3Query::~TSQLite3Query()
{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
	try
	{
		finalize();
	}
	catch(...)
	{
	}
#else
	finalize();
#endif
}
//---------------------------------------------------------------------------
TSQLite3Query &TSQLite3Query::operator=(const TSQLite3Query &rQuery)
{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
	try
	{
		finalize();
	}
	catch(...)
	{
	}
#else
	finalize();
#endif

	mpDB = rQuery.mpDB;
	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<TSQLite3Query &>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
	return *this;
}
//---------------------------------------------------------------------------
int TSQLite3Query::numFields()
{
	checkVM();
	return mnCols;
}
//---------------------------------------------------------------------------
const char *TSQLite3Query::fieldValue(int nField)
{
	checkVM();

	if(nField < 0 || nField > mnCols - 1)
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(TSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
#endif
		return NULL;
	}

	return (const char *)sqlite3_column_text(mpVM, nField);
}
//---------------------------------------------------------------------------
const char *TSQLite3Query::fieldValue(const char *szField)
{
	int nField = fieldIndex(szField);
	return (const char *)sqlite3_column_text(mpVM, nField);
}
//---------------------------------------------------------------------------
int TSQLite3Query::getIntField(int nField, int nNullValue)
{
	if(fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int(mpVM, nField);
	}
}
//---------------------------------------------------------------------------
int TSQLite3Query::getIntField(const char *szField, int nNullValue)
{
	int nField = fieldIndex(szField);
	return getIntField(nField, nNullValue);
}
//---------------------------------------------------------------------------
long long TSQLite3Query::getInt64Field( int nField, long long llNullValue /*= 0*/ )
{
	if(fieldDataType(nField) == SQLITE_NULL)
	{
		return llNullValue;
	}
	else
	{
		return sqlite3_column_int64(mpVM, nField);
	}
}
//---------------------------------------------------------------------------
long long TSQLite3Query::getInt64Field( const char *szField, long long llNullValue /*= 0*/ )
{
	int nField = fieldIndex(szField);
	return getInt64Field(nField, llNullValue);
}
//---------------------------------------------------------------------------
double TSQLite3Query::getFloatField(int nField, double fNullValue)
{
	if(fieldDataType(nField) == SQLITE_NULL)
	{
		return fNullValue;
	}
	else
	{
		return sqlite3_column_double(mpVM, nField);
	}
}
//---------------------------------------------------------------------------
double TSQLite3Query::getFloatField(const char *szField, double fNullValue)
{
	int nField = fieldIndex(szField);
	return getFloatField(nField, fNullValue);
}
//---------------------------------------------------------------------------
const char *TSQLite3Query::getStringField(int nField, const char *szNullValue)
{
	if(fieldDataType(nField) == SQLITE_NULL)
	{
		return szNullValue;
	}
	else
	{
		return (const char *)sqlite3_column_text(mpVM, nField);
	}
}
//---------------------------------------------------------------------------
const char *TSQLite3Query::getStringField(const char *szField, const char *szNullValue)
{
	int nField = fieldIndex(szField);
	return getStringField(nField, szNullValue);
}
//---------------------------------------------------------------------------
const unsigned char *TSQLite3Query::getBlobField(int nField, int &nLen)
{
	checkVM();

	if(nField < 0 || nField > mnCols - 1)
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(TSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
#endif
		return NULL;
	}

	nLen = sqlite3_column_bytes(mpVM, nField);
	return (const unsigned char *)sqlite3_column_blob(mpVM, nField);
}
//---------------------------------------------------------------------------
const unsigned char *TSQLite3Query::getBlobField(const char *szField, int &nLen)
{
	int nField = fieldIndex(szField);
	return getBlobField(nField, nLen);
}
//---------------------------------------------------------------------------
bool TSQLite3Query::fieldIsNull(int nField)
{
	return (fieldDataType(nField) == SQLITE_NULL);
}
//---------------------------------------------------------------------------
bool TSQLite3Query::fieldIsNull(const char *szField)
{
	int nField = fieldIndex(szField);
	return (fieldDataType(nField) == SQLITE_NULL);
}
//---------------------------------------------------------------------------
int TSQLite3Query::fieldIndex(const char *szField)
{
	checkVM();

	if(szField)
	{
		for(int nField = 0; nField < mnCols; nField++)
		{
			const char *szTemp = sqlite3_column_name(mpVM, nField);

			if(strcmp(szField, szTemp) == 0)
			{
				return nField;
			}
		}
	}

#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
	throw TSQLite3Exception(TSQLITE_ERROR,
		"Invalid field name requested",
		DONT_DELETE_MSG);
#endif
	return -1;
}
//---------------------------------------------------------------------------
const char *TSQLite3Query::fieldName(int nCol)
{
	checkVM();

	if(nCol < 0 || nCol > mnCols - 1)
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(TSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
#endif
		return NULL;
	}

	return sqlite3_column_name(mpVM, nCol);
}
//---------------------------------------------------------------------------
const char *TSQLite3Query::fieldDeclType(int nCol)
{
	checkVM();

	if(nCol < 0 || nCol > mnCols - 1)
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(TSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
#endif
		return NULL;
	}

	return sqlite3_column_decltype(mpVM, nCol);
}
//---------------------------------------------------------------------------
int TSQLite3Query::fieldDataType(int nCol)
{
	checkVM();

	if(nCol < 0 || nCol > mnCols - 1)
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(TSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
#endif
		return -1;
	}

	return sqlite3_column_type(mpVM, nCol);
}
//---------------------------------------------------------------------------
bool TSQLite3Query::eof()
{
	checkVM();
	return mbEof;
}
//---------------------------------------------------------------------------
void TSQLite3Query::nextRow()
{
	checkVM();
	int nRet = sqlite3_step(mpVM);

	if(nRet == SQLITE_DONE)
	{
		// no rows
		mbEof = true;
	}
	else if(nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = sqlite3_reset(mpVM);
		mpVM = 0;
		const char *szError = sqlite3_errmsg(mpDB);
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(nRet,
			(char *)szError,
			DONT_DELETE_MSG);
#endif
	}
}
//---------------------------------------------------------------------------
void TSQLite3Query::finalize()
{
	if(mpVM && mbOwnVM)
	{
		int nRet = sqlite3_finalize(mpVM);
		mpVM = 0;

		if(nRet != SQLITE_OK)
		{
			const char *szError = sqlite3_errmsg(mpDB);
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
			throw TSQLite3Exception(nRet, (char *)szError, DONT_DELETE_MSG);
#endif
		}
	}
	else if(mpVM && !mbOwnVM)
	{
		sqlite3_reset(mpVM);
	}
}
//---------------------------------------------------------------------------
void TSQLite3Query::checkVM()
{
	if(mpVM == 0)
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(TSQLITE_ERROR,
			"Null Virtual Machine pointer",
			DONT_DELETE_MSG);
#endif
	}
}


//class TSQLite3DB
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
TSQLite3DB::TSQLite3DB()
{
	mpDB = 0;
	mnBusyTimeoutMs = SQLTM_TIMEOUT;
}
//---------------------------------------------------------------------------
TSQLite3DB::TSQLite3DB(const TSQLite3DB &db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = SQLTM_TIMEOUT;
}
//---------------------------------------------------------------------------
TSQLite3DB::~TSQLite3DB()
{
	close();
}
//---------------------------------------------------------------------------
TSQLite3DB &TSQLite3DB::operator=(const TSQLite3DB &db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = SQLTM_TIMEOUT;
	return *this;
}
//---------------------------------------------------------------------------
void TSQLite3DB::open(const char *szFile)
{
	int nRet = sqlite3_open(szFile, &mpDB);

	if(nRet != SQLITE_OK)
	{
		const char *szError = sqlite3_errmsg(mpDB);
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(nRet, (char *)szError, DONT_DELETE_MSG);
#endif
	}
	
	//setBusyTimeout(mnBusyTimeoutMs);
}

#ifdef SQLITE_HAS_CODEC
void TSQLite3DB::open(const char* szFile,const char* szPass)
{
	int nRet = sqlite3_open(szFile, &mpDB);

	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(mpDB);
		throw TSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
	if(szPass != NULL) 
	{
		int nRet = sqlite3_key(mpDB, szPass, strlen(szPass));

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw TSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}

	setBusyTimeout(mnBusyTimeoutMs);
}

void TSQLite3DB::rekey(const char* szPass)
{
	if(szPass != NULL) 
	{
		int nRet = sqlite3_rekey(mpDB, szPass, strlen(szPass));

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw TSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
	setBusyTimeout(mnBusyTimeoutMs);
}
#endif
//---------------------------------------------------------------------------
void TSQLite3DB::close()
{
	if(mpDB)
	{
		int Result = sqlite3_close(mpDB);

		if(Result != SQLITE_OK)
		{
			const char *szError = sqlite3_errmsg(mpDB);
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
			throw TSQLite3Exception(Result, (char *)szError, DONT_DELETE_MSG);
#endif
		}

		mpDB = 0;
	}
}
//---------------------------------------------------------------------------
int TSQLite3DB::execDML(const char *szSQL)
{
	checkDB();
	char *szError = 0;
	int nRet = sqlite3_exec(mpDB, szSQL, 0, 0, &szError);

	if(nRet == SQLITE_OK)
	{
		return sqlite3_changes(mpDB);
	}
	else
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(nRet, szError);
#endif
		return -1;
	}
}
//---------------------------------------------------------------------------
TSQLite3Query TSQLite3DB::execQuery(const char *szSQL)
{
	checkDB();
	sqlite3_stmt *pVM = compile(szSQL);
	int nRet = sqlite3_step(pVM);

	if(nRet == SQLITE_DONE)
	{
		// no rows
		return TSQLite3Query(mpDB, pVM, true);
	}
	else if(nRet == SQLITE_ROW)
	{
		// at least 1 row
		return TSQLite3Query(mpDB, pVM, false);
	}
	else
	{
		nRet = sqlite3_finalize(pVM);
		const char *szError = sqlite3_errmsg(mpDB);
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(nRet, (char *)szError, DONT_DELETE_MSG);
#endif
	}
}
//---------------------------------------------------------------------------
sqlite_int64 TSQLite3DB::lastRowId()
{
	return sqlite3_last_insert_rowid(mpDB);
}
//---------------------------------------------------------------------------
void TSQLite3DB::setBusyTimeout(int nMillisecs)
{
	mnBusyTimeoutMs = nMillisecs;
	sqlite3_busy_timeout(mpDB, mnBusyTimeoutMs);
}
//---------------------------------------------------------------------------
void TSQLite3DB::checkDB()
{
	if(!mpDB)
	{
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(TSQLITE_ERROR,
			"Database not open",
			DONT_DELETE_MSG);
#endif
	}
}
//---------------------------------------------------------------------------
sqlite3_stmt *TSQLite3DB::compile(const char *szSQL)
{
	checkDB();
	const char *szTail = 0;
	sqlite3_stmt *pVM;
	int nRet;
	nRet = sqlite3_prepare_v2(mpDB, szSQL, -1, &pVM, &szTail);

	if(nRet != SQLITE_OK)
	{
		const char *szError = sqlite3_errmsg(mpDB);
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		throw TSQLite3Exception(nRet,
			(char *)szError,
			DONT_DELETE_MSG);
#endif
		return NULL;
	}

	return pVM;
}
//---------------------------------------------------------------------------
int TSQLite3DB::backupTable( const char* dbname, StrVector tbname )
{
	int nNum = 0;
	char szSQL[100];
	//attach database
	sprintf(szSQL, "ATTACH '%s' AS db2", dbname);
	this->execDML(szSQL);
	StrVector::iterator it = tbname.begin();
	while(it != tbname.end()){
		//drop table
		sprintf(szSQL, "DROP TABLE db2.%s", (*it).c_str());
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		try{
			this->execDML(szSQL);
		}
		catch(...){
			//nothing to to.
		}
#else
		this->execDML(szSQL);
#endif

		//create table and backup data
		sprintf(szSQL, "CREATE TABLE db2.%s as SELECT * FROM %s", (*it).c_str(), (*it).c_str());
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		try{
			this->execDML(szSQL);
			++nNum;
		}
		catch(...){
			//nothing to do.
		}
#else
		if( this->execDML(szSQL) > 0 ){
			++nNum;
		}
#endif

		it++;
	}
	//detach database
	this->execDML("DETACH db2");
	return nNum;
}


int TSQLite3DB::restoreTable( const char* dbname, StrVector tbname )
{
	int nNum = 0;
	char szFields[2048] = {0};
	char szSQL[4096] = {0};
	char szTable[50] = {0};
	string tablename = "";
	
	//attach database，附加上旧数据库 => db2
	sprintf(szSQL, "ATTACH '%s' AS db2", dbname);
	this->execDML(szSQL);
	StrVector::iterator it = tbname.begin();
	while(it != tbname.end()){
		szFields[0] = '\0';

		szSQL[0] = '\0';
		tablename = *it;

		//判断新数据库中要恢复用户数据的表名是否存在
		if(tableIsExist(tablename.c_str())){
			sprintf(szTable, "db2.%s", tablename.c_str());
			StrVector fields = getFieldName(szTable);
			for(int i=0; i<fields.size(); i++){
				if(strlen(szFields)>0) strcat(szFields,",");
				strcat(szFields, fields[i].c_str());
			}
			if(strlen(szFields)<1){
				it++;
				continue;
			}
			//数据恢复之前，要清空表
			sprintf(szSQL, "DELETE FROM %s", tablename.c_str());
			this->execDML(szSQL);
			sprintf(szSQL, "INSERT INTO %s(%s) SELECT %s FROM db2.%s", tablename.c_str(), szFields, szFields, tablename.c_str());
		}
		else{
			//create table and restore data
			//sprintf(szSQL, "CREATE TABLE %s as SELECT * FROM db2.%s", tablename.c_str(), tablename.c_str());
			it++;
			continue;
		}

#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
		try{
			this->execDML(szSQL);
			++nNum;
		}
		catch(...){
			//nothing to do.
		}
#else
		if( this->execDML(szSQL) > 0){
			++nNum;
		}
#endif

		it++;
	}

	//detach database
	this->execDML("DETACH db2");
	return nNum;
}


//SQL脚本文件，每行注释行都必须用 # 或 // 开头，前面不能有空格
//每条SQL语句都必须新起一行开始，但是可以分行书写，并且每行总长度不能超过1000字符，
//每条SQL语句都必须以字符 ；结束
int TSQLite3DB::execSQLFile( const char* szFileName )
{
	char szData[1024] = {0};
	string strSQL = "";
	int nErrSQL = 0;
	//char ridof;

	ifstream in(szFileName);
	//if(in == NULL){
	//	return -1;
	//}

	//读掉文件的第一行，第一行固定必须为注释行
	if(!in.eof()){
		in.getline(szData, 1000);
	}

	/*
	//读掉文件开头隐藏的文件编码标识
	while(!in.eof()){
		ridof = in.get();
		if(ridof >= 0){
			break;
		}
	}

	if(in.eof()){
		return 0;
	}
	else{
		//回退一个字节
		in.seekg(-1, ios::cur);
	}
	*/

	//开始正常文件的读取
	while(!in.eof()){
		szData[0] = '\0';
		in.getline(szData, 1000);
		int nLen = strlen(szData);

		if(szData[0]=='#' || (szData[0]=='/' && szData[1]=='/') || szData[0]==13 || nLen<1){
			continue;
		}

		//去掉末尾的回车符 chr(13)
		if(szData[nLen-1] == 13){
			szData[nLen-1] = '\0';
		}
		strSQL.append(szData);
		//去掉最后的空格，并判断最后的字符是否以分号结束
		//strSQL.back()
		strSQL.erase(strSQL.find_last_not_of(" ")+1);

		if(strSQL.length() > 0){
			if(strSQL[strSQL.length()-1] == ';'){
#if !defined(MOGE_JURASSIC_NO_EXCEPTION)
				try{
					this->execDML(strSQL.c_str());
				}
				catch(...){
					++nErrSQL;
				}
#else
				if (this->execDML(strSQL.c_str()) < 0){
					++nErrSQL;
				}
#endif

				strSQL.clear();
			}
			else{
				strSQL.append(" ");
			}
		}
	}

	in.close();
	return nErrSQL;
}

StrVector TSQLite3DB::getFieldName( const char* tbname )
{
	char szSQL[100] = {0};
	int count = 0;
	StrVector Fields;

	sprintf(szSQL,"SELECT * FROM %s",tbname);
	sqlite3_stmt *stmt = compile(szSQL);
	//执行这个语句
	int nret = sqlite3_step(stmt);
	if(nret == SQLITE_DONE || nret == SQLITE_ROW){
		count = sqlite3_column_count(stmt);                
		for(int i=0; i<count; i++){
			Fields.push_back(sqlite3_column_name(stmt,i));
		}
	}
	sqlite3_finalize(stmt);

	return Fields; 
}

bool TSQLite3DB::tableIsExist( const char* tbname )
{
	if( tbname == NULL ){
		return false;
	}

	char szSQL[200] = {0};
	sprintf(szSQL, "SELECT COUNT(*) AS num FROM sqlite_master WHERE type='table' and name ='%s'", tbname);

	TSQLite3Query datset = execQuery(szSQL);
	if(datset.getIntField(0) > 0){
		return true;
	}
	return false;
}

bool TSQLite3DB::fieldIsExist( const char* tbname, const char* fieldname )
{
	StrVector fields;
	string strField = fieldname;
	fields = getFieldName(tbname);
	for (int i=0; i<fields.size(); i++){
		if(fields[i] == strField){
			return true;
		}
	}
	return false;
}
