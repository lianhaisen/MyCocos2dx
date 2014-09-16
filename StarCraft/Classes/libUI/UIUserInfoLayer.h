#ifndef  __UIUSERINFOLAYER_H
#define  __UIUSERINFOLAYER_H

//TODO  返回用户数据界面层
// 主页面
// 开发方法,设置各种值
// 设置资源的值的时候可以播放特殊动画~
//
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class UIUserInfoLayer :public Layer
{
public:
	static UIUserInfoLayer* create(const string &BgFile,float iScaleX, float iScaleY,string HeadFile = " ", string ExpFile = " ");
	//各种设置各种获得/**/
	//设置头像
	void setHerdImg(const string &filename);
	Sprite* getHerdImg(){return (Sprite*)this->getChildByTag(2);}

	//设置名字
	void setName(const string& text);
	Label* getNameLayer(){return (Label*)this->getChildByTag(3);}//static_cast<Sprite*>( LayerTop->getChildByTag(2) );

	//设置经验条
	ProgressTimer* getExpLayer(){return (ProgressTimer*)this->getChildByTag(5);}
	//设置内容 目前仅数字  //@Num 所需增加的数量 
	void setAddNum(int Num); 
	//没有动作的 增加值
	void setAddNumNoAction(int Num);
	Label* getInfoString(){return (Label*)this->getChildByTag(4);}
	//设置内容 文字
	//void setInfoString(const string& text);

	//设置更新头像是否扭动
	void setHeadAction(bool b);

	//获得最大值
	int getMaxNum(){return m_nMaxNum;}
	void setMaxNum(int nMaxNum,const string& text = " ");
	//获得当前值
	int getCurNum(){return m_nCurNum;}
	//设置初始值
	void setinitNum(int nCurNum,int nMaxNum,const string& text = " ");

	//设置长条框内颜色条
	//void setFormImg(Sprite* sp);
	//Sprite* getFormImg(){return (Sprite*)this->getChildByTag(1);}
	//设置长条
	//void setBg(const string &filename);
	//Sprite* getBg(){return (Sprite*)this->getChildByTag(2);}
public:
	int m_nCurNum;			//当前值
	int m_nAddNum;			//增长值
	int m_nMaxNum;			//上限值
	bool m_bIsAction;		//更新时头像是否有动作  默认 头像不动 
	float m_fScaleX;
	float m_fScaleY;
private:
	virtual bool init(const string &BgFile,float iScaleX, float iScaleY, string &HeadFile,string &ExpFile);  
	void createOneItme(const string &BgFile,float iScaleX, float iScaleY, string &HeadFile,string &ExpFile);//左右有图 中间进度条的layer  创建图标
	void updataHerditemLayer(float delta);//更新玩家经验值 
private:
	int m_nSpeed;			//动画速度
};
#endif



