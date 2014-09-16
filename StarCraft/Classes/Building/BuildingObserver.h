#ifndef __BUILDINGOBSERVER_H__
#define __BUILDINGOBSERVER_H__
#include <vector>
#include <map>
#include "BattleLogic.h"

using namespace std;

///// 抽象观察者类，为所有具体观察者定义一个接口，在得到通知时更新自己
//class BuildingObserver
//{
//public:
//	void Update();
//}
//
//// 抽象主题类
//class BuildingSubject
//{
//public:
//	void Attach(BuildingObserver observer);// 增加观察者
//	void Detach(BuildingObserver observer);// 移除观察者
//	void Notify();// 向观察者（们）发出通知
//private:
//	vector<BuildingObserver *> observers;
//}


class BuildingObserver
{
public:
	static BuildingObserver* getInstance();
	static void release();

	void addObserver(int grid, int buildingId);// 增加观察者
	void deleteObserver(BattleLogic* building);// 移除观察者
	void notifyObserver(int grid, int soldierId);// 向观察者（们）发出通知

	bool check(int grid, int soldierId);

private:
	map<int, vector<int>> observers;//key=grid,value=buildingid
};




#endif



