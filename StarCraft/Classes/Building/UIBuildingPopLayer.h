
#ifndef _UIBUILDINGPOPLAYER_
#define _UIBUILDINGPOPLAYER_

#include "cocos2d.h"
class UITouchButton;
USING_NS_CC;

class UIBuildingPopLayer :public Layer
{
public:
	enum class PopStatus
	{
		FREE = 0,
		SHOW = 1,
		UNSHOW = 2
	};
public: 
	UIBuildingPopLayer();
	~UIBuildingPopLayer();
	CREATE_FUNC(UIBuildingPopLayer);
    
	virtual void update(float dt);

	virtual void onEnter();
	virtual void onExit();
	virtual bool init();
    
	
	//************************************
	// Method:    setStatus 设置显示状态
	// FullName:  UIBuildingPopLayer::setStatus
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: PopStatus status
	//************************************
	void setStatus(PopStatus status){_statusvec.push_back(status);}

	//************************************
	// Method:    addButtons 添加按钮
	// FullName:  UIBuildingPopLayer::addButtons
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Vector<UITouchButton * > buttons
	//************************************
	void addButtons(Vector<UITouchButton*> buttons);
    
	//获得提示文字
	Label* getTitleLabel();
	//设置提示文字
	void   setTitleLabel(Label* lb);

private:
	
	void actionCallBack(Node* node);//按钮数组

	Vector<UITouchButton*> _buttons;
	Vector<UITouchButton*> _tempbuttons;
	
	//提示文字
	Label* _titlelabel;

	void  showBtnEnd();
	void  unshowBtnEnd();
	void  doShow();//显示按钮
	void  doUnShow();//消失按钮
	bool _isActionEnd; //动作是否结束 
    std::vector<PopStatus> _statusvec;//动作队列


};




#endif