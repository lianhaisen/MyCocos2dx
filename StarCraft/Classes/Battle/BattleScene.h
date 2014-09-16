#ifndef __BATTLESCENE_H__
#define __BATTLESCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "UIPanZoomLayer.h"

using namespace cocos2d;
using namespace cocostudio;

class BattleScene : public Scene,public UILongPressOrClickDelegate
{
public:
    BattleScene(void);
    ~BattleScene(void);
	virtual void onEnter() override;
    virtual bool init();
    CREATE_FUNC(BattleScene);
	void initBattleData();

	void battleUpdate(float dt);

	//创建背景
	void createUI();
	//底部下兵条
	void createBottomBar();
	//点击底部下兵条，选中士兵
	void selectArmy(Ref* sender);
	//下兵
	void addArmy(Point point);


	// 长按
	virtual void doLongPress(PointArray* array);
	// 点击
	virtual void doClick(PointArray* array);
	// 检查是否能点击或者长按
	virtual bool checkIsCanClickOrLongPress(PointArray* array);

	static Node* getPlaceLayer();

	void onFrameEvent(Bone *bone, const char *evt, int originFrameIndex, int currentFrameIndex); 
	void animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID);
	void dataLoaded(float percent);
private:
	void initTowerObserver();
	void onBackToTsetScence(Ref * ref);

public:
	int m_selectedSoldierType;		//底部下兵栏，当前选中的兵种



	//map<int, deque<DSoldier *>> m_SoldierInCamp;//兵营中的士兵
	//vector<Soldier *> m_SoldierInBattle;//已上场的士兵



};

#endif // __BATTLESCENE_H__
