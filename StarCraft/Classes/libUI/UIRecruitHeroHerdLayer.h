#ifndef __UIRecruitHeroHerdLayer_H__
#define __UIRecruitHeroHerdLayer_H__
/*
	ÕÐÄ¼Ó¢ÐÛ¡ªÓ¢ÐÛÍ·Ïñ
*/
#include "cocos2d.h"
#include "UITouchButton.h"
#include "BuildingConfig.h"
#include "HeroConfig.h"
using namespace cocos2d;
using namespace std;
class UIRecruitHeroHerdLayer :public Layer
{
public:
	enum class RecruitType
    {
        BIG = 1,	//´óÍ¼±ê
		SMALL		//Ð¡Í¼±ê
    };
public:
	UIRecruitHeroHerdLayer();
	~UIRecruitHeroHerdLayer();
	static UIRecruitHeroHerdLayer* create(int nHeroId,RecruitType nType);
	virtual bool init(int nHeroId,RecruitType nType);

	void setTopName(const string str);
	void setDownName(const string str);
	void setCallBack(const ccMenuCallback& callback);
	HeroInfo* getHeroInfo(){return _db;}
private:
	void initWithForm(int nHeroId);
	void CallBack(Ref* sender);
	CC_SYNTHESIZE_RETAIN(Label*,m_TopName,TopName);
	CC_SYNTHESIZE_RETAIN(Label*,m_DownName,DownName);
	CC_SYNTHESIZE_RETAIN(UITouchButton*,m_Headbtn,Headbtn);
private:
	HeroInfo* _db;
	RecruitType m_nType;
	ccMenuCallback _callBack;

};




#endif //  __UIRecruitHeroHerdLayer_H__


