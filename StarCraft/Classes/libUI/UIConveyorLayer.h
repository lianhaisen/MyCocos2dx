#ifndef __UIConveyorLayer_H__
#define __UIConveyorLayer_H__
/*
	�����
*/
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "UITouchButton.h"
#include "UISoldierHeadLayer.h"
#include "UITipLayer.h"
using namespace cocos2d;
using namespace ui;
#define CONVEYORID 700001 //�����id
class UIConveyorLayer :public UITipLayer
{
public:

public:
	UIConveyorLayer();
	~UIConveyorLayer();
    CREATE_FUNC(UIConveyorLayer);
	virtual bool init();
	//static UIBarracks * CreateFromDB(DBuilding * db);
private:
	void initWithForm();

	Node* getOneItem(Node* Itme1,Node* Item2);
	//������б�
	void createConveyorList();

	//���������Ϣ
	void showConveyorInfoLayer();
	void UpDataConveyorInfo(int id,int TypeId);

	//ʿ����Ϣ�б�
	void showSoldierInfoLayer();
	void UpDataSoldierInfo(int id,int TypeId);

	//���ʿ���������
	void addSoldier(Ref *sender);
	//���������ʿ��
	void ReductionSoldier(Ref *sender);
private:
	int m_iCurState;
	ListView* m_ConveyorInfolist;
	ListView* m_SoldierInfolist;


};




#endif //  __UIHeroPub_H__


