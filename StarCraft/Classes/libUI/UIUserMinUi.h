#ifndef __UIUSERMINUI_H__
#define __UIUSERMINUI_H__
/*
	主界面UI
*/
#include "cocos2d.h"
using namespace cocos2d;
#include "UIUserInfoLayer.h"
#include "UITouchButton.h"

class UIUserMinUi :public Layer
{
public:
	enum class MINDOWN_UI_ID
	{
		DownUI_PK			= 1,			//PK战
		DownUI_StarWar		= 2,			//星战战
		DownUI_NPC			= 3,			//NPC战
		DownUI_Friendly		= 4,			//友谊赛
	};

public:
	UIUserMinUi();
	~UIUserMinUi();
    CREATE_FUNC(UIUserMinUi);
	//当前实例 
	static UIUserMinUi* getInstance();

	//各种设置
	//设置玩家等级
	 void setHeroAddLv(int iLv);
	//设置玩家增加经验
	void setHeroAddExp(int nNum );
	//玩家初始经验
	void InitHeroExp(int nCurNum,int nMaxNum);
	//设置玩家最大经验值
	void setHeroMaxExp(int nNum);

	//设置上Top增加减少值 @nType == 1:   ，2:   ，3:
	void addTopUiNum(int nType,int nNum);
	//初始化TopUi 当前值 上限值 名字
	void InitTopUiNum(int nType,int nCurNum, int nMaxNum, const string& text = " ");
	//TopUi 设置最大值
	void setTopUiMaxNum(int nType, int nMaxNum);

	//设置右边Right增加减少值 @nType == 1:   ，2:   ，3:     4:
	void addRightUiNum(int nType,int nNum);
	//初始化RightUi 当前值 上限值 名字
	void InitRightUiNum(int nType,int nCurNum, int nMaxNum, const string& text = " ");
	//RightUi 设置最大值
	void setRightUiMaxNum(int nType, int nMaxNum);
	//设置聊天框内容
	void setChatString(const string& text);
	//设置聊天提示
	void setChatTip(int nType,int Num);

	//设置选择战斗类型回调
	void setStaticWarCallBack(const ccMenuCallback& callback);
private:
	virtual bool init();  
	//测试UI布置
	void createUIScene();
	//点击TOP回调
	void onClickTopUiCallback(Ref* sender );
	//创建头部UI
	void createTopUi();
	//创建右边UI
	void creatRightUi();
	//头像UI
	void createHerdUi();
	//底部UI
	void createDownUi();
	//底部UI回调
	void onClickDownUiCallbacke(Ref* sender);
	//聊天UI
	void createChat();

	//选择战斗类型
	void onClickStaticWar(Ref* sender);

	//移动按钮弹出动作结束回调
	void callBackPopUpBtnOk();
	//弹回动作结束回调
	void callBackBounceBtnOk();

private:
	bool _bPopUp;
	Vector<UITouchButton*> m_vWarBtnUi;
	Vector<UIUserInfoLayer*> m_vTopUi;
	Vector<UIUserInfoLayer*> m_vRightUi;
	UIUserInfoLayer* m_HeadLayer;
	Label* _lbLv; //等级Lb
	Label* _lbChat; //聊天
	UITouchButton* _btnChat;
	ccMenuCallback _btnStaticWarCallBack;
};




#endif //  __UIUSERMINUI_H__


