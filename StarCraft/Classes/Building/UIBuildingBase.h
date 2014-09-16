#ifndef __UIBUILDINGBASE_H
#define __UIBUILDINGBASE_H

#include "cocos2d.h"
using namespace cocos2d;
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "UITouchButton.h"
#include "MapHelper.h"
#include "BattleLogic.h"

class DBuilding;
class CBuilding;
class BuyListConfig;
class UIPlaceBuildingLayer;

class UIBuildingBase : public Layer
{
public:
	UIBuildingBase();
	~UIBuildingBase();
public:
	CC_SYNTHESIZE_RETAIN(Sprite*,_grassbg,GrassBg);
	CC_SYNTHESIZE_RETAIN(Armature*,_buildingArmature,BuildingArmature);
	CC_SYNTHESIZE_RETAIN(ActionInterval*,_bigeraction,BigAction);
	CC_SYNTHESIZE_RETAIN(ActionInterval*,_sharkaction,SharkAction);
	CC_SYNTHESIZE_RETAIN(Sprite*,_arr1,Arr1);
	CC_SYNTHESIZE_RETAIN(Sprite*,_arr2,Arr2);
	CC_SYNTHESIZE_RETAIN(Sprite*,_arr3,Arr3);
	CC_SYNTHESIZE_RETAIN(Sprite*,_arr4,Arr4);
	CC_SYNTHESIZE_RETAIN(Sprite*,_processbg,ProcessBg);
	CC_SYNTHESIZE_RETAIN(ProgressTimer*,_processTimer,ProcessTimer);
	CC_SYNTHESIZE_RETAIN(UITouchButton*,_btnyes,BtnBuyYES);
	CC_SYNTHESIZE_RETAIN(UITouchButton*,_btnno,BtnBuyNo);
	CC_SYNTHESIZE_RETAIN(ProgressTimer*,_hpProcess,HPProcess);
	CC_SYNTHESIZE_RETAIN(Sprite*,_hpgb,HPBg);
	CC_SYNTHESIZE_RETAIN(Armature*,_buildingproc,BuildingProc);
public:
	virtual void setCoor(Coord coor);
	Coord getCoor(){return _coor;}
	int  getGridCount() {return m_nGridCount;}
	virtual void onBuyDone(Ref * ref){}
	//���½�����״̬
	void updateUpgradeStatus();
	
	//�������Ƿ���Ŀ����
	//TODO�ɸĽ�ʹ�ó�ǽ��ѡ��
	virtual bool isPointInRect(const Point& point);
	UIPlaceBuildingLayer* getPlaceLayer();
	//������������Ҫ������ò�ſ���ʹ��
	virtual void addBuildPoint();
	virtual bool checkaddPoint();

protected:
	void  setSharkAction();
	void  setBigerAction();
	void  bigActionBack();

	//���½�������ʾ״̬
	virtual void updateStatus(){}
	//����������
	void  createProcessTimer();
	//����Ѫ��
	void  createHPProcess();
	//�����������
	void  createBuildProc();
	//��������ť
    void createBuyButton();

protected:
	//����
	Coord  _coor;
	//������
	int	   m_nGridCount;
};

//�༭�߼�
typedef std::function<void(Ref*,bool)> ccYesOrNoCallBack;
class EditLogic
{
public:
	EditLogic();
	~EditLogic();
public:

    void setClickTarget( const ccMenuCallback& callback);
	bool isSelected(){return _Selected;}
	virtual void selected(){}
	virtual void unselected(){}

	virtual void setMove(bool isMove){};
	bool getIsMove(){return _bMove;}

	//����λ���Ƿ�Ƿ�
	virtual void setillegal(bool illeagal){}
	//����ģʽ
	virtual void setBuy(bool isBuy = false,const ccYesOrNoCallBack& callback = nullptr){}

	virtual void setBuidingData(DBuilding * db){};
	DBuilding * getBuidingData() {return _db;}
	BuyListConfig * getBuyCfg() {return _buyCfg;}

	int getLevel();
	int getBType(){return _type;}
	int getBStatus();

protected:
	ccMenuCallback  _callBack;//ѡ�лص�
	//ԭʼ����
	Coord _orginCoor;
	//�Ƿ�ѡ��
	bool  _Selected;
	//�Ƿ��ƶ���
	bool  _bMove;
	//������
	DBuilding *       _db;
	//��������
	BuyListConfig *   _buyCfg;
	//�Ƿ��ڹ���ģʽ
	bool              _isInBuy;
	 // λ�÷Ƿ�
	bool                _isillegal;
	//���찴ť�ص�
	ccYesOrNoCallBack   _yesorno;
	int                 _type;
	int                 _level;
	int                 _bstatus;

};


class Building : public UIBuildingBase,public EditLogic,public BattleLogic
{

//�༭����
public:
	virtual bool initWithDB(DBuilding * db);
	//virtual bool initWithBDB(BBuilding * db);
	virtual bool initWithBuyDB(BuyListConfig * db);
public:
	virtual void removeBuildPoint();
	virtual void selected();
	virtual void unselected();
	virtual bool isNeedShowGrid();
	virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
	virtual void onDraw(const kmMat4 &transform, bool transformUpdated); 
	virtual void setMove( bool isMove );
	virtual void setillegal( bool illeagal );
	virtual void updateStatus();
	virtual void setBuy( bool isBuy = false,const ccYesOrNoCallBack& callback = nullptr );
	virtual void onBuyDone( Ref * ref );
	virtual void setBuidingData( DBuilding * db );
	virtual void update(float delta);
protected:
	bool initWithFile(const char* filename,const int gridCount);
	CustomCommand _customCommand;

//ս������
public:
	//TODO
	virtual bool initWithBattleLogic(DBuilding * db);
	
	virtual bool subHp( int hp );
	virtual void updateHpProgress();
	virtual void dead();
	virtual Coord getMidCoord();

	

private:
	vector<int> _attackRange;//x*MAX_GRID_COUNT+y

};



#endif



