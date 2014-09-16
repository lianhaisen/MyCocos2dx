#ifndef __UIMSGLAYER_H__
#define __UIMSGLAYER_H__
/*
	主界面UI
*/
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "UIUserInfoLayer.h"
#include "UITouchButton.h"
#include "UIPopupLayer.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;
class UIMsgLayer :public UIPopupLayer
{
public:
	UIMsgLayer();
	~UIMsgLayer();
	virtual bool init();
	//static UIMsgLayer* create(const Size size);
	CREATE_FUNC(UIMsgLayer);
	void initwithSize();
	//当前主Node
	CC_SYNTHESIZE_RETAIN(Node*,_contentNode,ConTentNode);
	//头部标题
	CC_SYNTHESIZE_RETAIN(Node*,_contentTitle,ContentTitle);
	//信息节点 
	CC_SYNTHESIZE_RETAIN(Node*,_contentInfoNode,ContentInfoNode);
	//获得临时节点 在当前信息节点外面
	CC_SYNTHESIZE_RETAIN(Node*,m_pTmpInfoNode,ContentTmpInfoNode);
	
	//拖拽节点
	CC_SYNTHESIZE_RETAIN(ScrollView*,m_scrollView,ContentScrollView);

	void setCloseCallback( const ccMenuCallback& callback);

	//弹出新界面
	void onPopInfoNode();
	//信息节点弹出动作
	void PopInfoNodeAction(Point Pos);

	virtual void onClickLeftBtnCallBack(Ref* sender);

	virtual void onClickRightBtnCallBack(Ref* sender);

	//设置左右按钮是否可见
	void setVIsibelLeftRightBtn(bool bTrue);

	//设置拖拽页数 
	void setDragPage(int Page);

private:
	void onClose();
	
private:
	int m_CurrentStart;
	ccMenuCallback m_CloseCallback;
	UITouchButton* m_LeftBtn;
	UITouchButton* m_RightBtn;
};




#endif //  __UIMSGLAYER_H__


