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

	//��������
	void createUI();
	//�ײ��±���
	void createBottomBar();
	//����ײ��±�����ѡ��ʿ��
	void selectArmy(Ref* sender);
	//�±�
	void addArmy(Point point);


	// ����
	virtual void doLongPress(PointArray* array);
	// ���
	virtual void doClick(PointArray* array);
	// ����Ƿ��ܵ�����߳���
	virtual bool checkIsCanClickOrLongPress(PointArray* array);

	static Node* getPlaceLayer();

	void onFrameEvent(Bone *bone, const char *evt, int originFrameIndex, int currentFrameIndex); 
	void animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID);
	void dataLoaded(float percent);
private:
	void initTowerObserver();
	void onBackToTsetScence(Ref * ref);

public:
	int m_selectedSoldierType;		//�ײ��±�������ǰѡ�еı���



	//map<int, deque<DSoldier *>> m_SoldierInCamp;//��Ӫ�е�ʿ��
	//vector<Soldier *> m_SoldierInBattle;//���ϳ���ʿ��



};

#endif // __BATTLESCENE_H__
