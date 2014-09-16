#include "SqliteHelper.h"
#include "cocos2d.h"

using namespace cocos2d;
static TSQLite3DB *sqliteHandler;

SqliteHelper::SqliteHelper(void)
{
}

SqliteHelper::~SqliteHelper(void)
{
}


TSQLite3DB* SqliteHelper::getSqliteHandler()
{
	if (sqliteHandler == NULL)
	{
		sqliteHandler = new TSQLite3DB();
		std::string dbFile = FileUtils::getInstance()->fullPathForFilename("db/starcraft.db"); 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		std::string realpath = FileUtils::getInstance()->getWritablePath() + "/starcraft.db";
		if(FileUtils::getInstance()->isFileExist(realpath))
		{
			dbFile = realpath;
		}
		else
		{
			//TOOD copy file;
		    auto object = FileUtils::getInstance()->getDataFromFile(dbFile);
			FILE * file = fopen(realpath.c_str(),"w");
			if (file)
			{
				fwrite((const void *)object.getBytes(),object.getSize(),1,file);
				fclose(file);
				dbFile = realpath;
			}
		}

#endif

/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
		sqliteHandler->open(dbFile.c_str(),"mogeloftcom123456www");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//´ÓDocumentsÄ¿Â¼¶ÁÈ¡
		dds::CString dbPath;
		dbPath.Format("%s%s", CCFileUtils::getWriteablePath().c_str(), USING_DB_FILE);
		sqliteHandler->open(dbPath.c_str(), "mogeloftcom123456www");
#else
		sqliteHandler->open(dbFile.c_str());
#endif
*/
		sqliteHandler->open(dbFile.c_str());
	}

	return sqliteHandler;
}

