#ifndef __UIMSGLAYER_H__
#define __UIMSGLAYER_H__
/*
	������UI
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
	//��ǰ��Node
	CC_SYNTHESIZE_RETAIN(Node*,_contentNode,ConTentNode);
	//ͷ������
	CC_SYNTHESIZE_RETAIN(Node*,_contentTitle,ContentTitle);
	//��Ϣ�ڵ� 
	CC_SYNTHESIZE_RETAIN(Node*,_contentInfoNode,ContentInfoNode);
	//�����ʱ�ڵ� �ڵ�ǰ��Ϣ�ڵ�����
	CC_SYNTHESIZE_RETAIN(Node*,m_pTmpInfoNode,ContentTmpInfoNode);
	
	//��ק�ڵ�
	CC_SYNTHESIZE_RETAIN(ScrollView*,m_scrollView,ContentScrollView);

	void setCloseCallback( const ccMenuCallback& callback);

	//�����½���
	void onPopInfoNode();
	//��Ϣ�ڵ㵯������
	void PopInfoNodeAction(Point Pos);

	virtual void onClickLeftBtnCallBack(Ref* sender);

	virtual void onClickRightBtnCallBack(Ref* sender);

	//�������Ұ�ť�Ƿ�ɼ�
	void setVIsibelLeftRightBtn(bool bTrue);

	//������קҳ�� 
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


