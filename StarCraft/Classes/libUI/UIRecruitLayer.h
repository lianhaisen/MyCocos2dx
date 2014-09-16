
#ifndef __UIRECRUITLAYER_H__
#define __UIRECRUITLAYER_H__
/*
	招募UI
*/

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "UIUserInfoLayer.h"
#include "UITouchButton.h"
#include "UIMsgLayer.h"
#include "BuildingData.h"
#include "UISoldierHeadLayer.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;
class CSoldier;
class TrainingData;
//兵营1，兵营2，军工厂
//军工厂2，飞行基地1，飞行基地2 
class UIRecruitLayer :public UIMsgLayer
{
//TODO:
public: 
	static UIRecruitLayer* createFromDB(DBuilding * db);

public:
	//建造中的士兵列表
	typedef struct _Recruit_Soldier
	{
		int ntime;				//所需时间
		int nNum;				//创建数量
		int nType;				//类型
		int nOrder;				//顺序
		int nCurtime;			//剩余时间
		Sprite* imgHerd;		//头像
		Label* lbNum;			//显示数量
		string Path;			//图片地址

	}RecruitSoldier;
public: 
	UIRecruitLayer();
	~UIRecruitLayer();
	virtual bool init();
	static UIRecruitLayer* create();

	//创建招募节点 IsPop 加载的是否是弹窗
	void createRecruitLayer(bool IsPop = false);

	//设置总时间
	void UpDataAllTIme(const string& text);
	//更新金额
	void UpDataMoney(const string& text);
	//                  设置创建中的士兵                  //
	//创建或刷新建造中士兵列表
	void RefreshCreateSoldierList(int nOrder,int nType,int nNum,int ntime,int nCurtime);
	//数量
	void setOneRecruitSoldierNum(int nType,int nNum);
	//时间
	void setOneRecruitSoldierTime(int nType,int nTime,int nCurtime = 0);
	//列队第一的单个士兵的进度
	void dataFirstSoldierProcess(int nExp);

	/*             设置士兵列表            */
	void createSoldierLayer(CSoldier * data ,int nNum,bool v = false);

	UISoldierHeadLayer::Soldierinfo* getSoldierInfo(int nType);


	//执行定时器回调函数
	void setUnschedule( const ccMenuCallback& callback);

	//翻页回调
	virtual void onClickLeftBtnCallBack(Ref* sender);
	virtual void onClickRightBtnCallBack(Ref* sender);

	//定时器
    void updateWorkProcess(float dt);

private:
	//创建士兵招募
	void createSoldierRecruit(bool IsPop);
	//单机 增加建造士兵
	void addRecruitSoldier(Ref* sender);
	//删除创建中的士兵回调
	void DeleteSoldierList(Ref* sender);
	//删除创建中的士兵
	void DeleteSoldier(int nType);
	//点击查看士兵详细信息
	void onClickCallBack(Ref* sender);
	//创建士兵详细信息layer
	void createSoldierInfoLayer(CSoldier* data);
	//点击返回创建士兵
	void onClickBckaCallBack(Ref* sender);
	//点击加速
	void onClickJiaSuCallBack(Ref* sender);
	//关闭tip是开启招募
	void onClickCloseTip();
	//创建单个 @nOrder 列队位置 @nType 类型  @nNum 数量 @nTime所需时间 @nCurtime 当前已生产时间默认0
	void createRecruitSoldierLayer(int nOrder, int nType, int nNum,int ntime,int nCurtime = 0);

private:
	ccMenuCallback m_Unschedule;
	map<int,RecruitSoldier> _mSoldierList;//正在建造的士兵
	map<int,UISoldierHeadLayer*> _mSoldierInfo;//士兵信息列表
	Point PosEnd;			//最后一个创建中的士兵的位置
	UIUserInfoLayer* m_pUserInfoLayer;		//单个士兵建造进度
	Label* m_lbAllTime;						//建造士兵所需的总时间
	UITouchButton* m_btnUpMoney;			//加速按钮
	Scale9Sprite* _lbTopTime;						//建造士兵所需的总时间 Top文字
	bool _booltrue;							//是否是弹窗
private:
	DBuilding * _db;
	TrainingData * _traindata;
	bool           _isNeedFresh;

};

#endif //  __UIRECRUITLAYER_H__