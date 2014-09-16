#ifndef __UIUSERMINUI_H__
#define __UIUSERMINUI_H__
/*
	������UI
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
		DownUI_PK			= 1,			//PKս
		DownUI_StarWar		= 2,			//��սս
		DownUI_NPC			= 3,			//NPCս
		DownUI_Friendly		= 4,			//������
	};

public:
	UIUserMinUi();
	~UIUserMinUi();
    CREATE_FUNC(UIUserMinUi);
	//��ǰʵ�� 
	static UIUserMinUi* getInstance();

	//��������
	//������ҵȼ�
	 void setHeroAddLv(int iLv);
	//����������Ӿ���
	void setHeroAddExp(int nNum );
	//��ҳ�ʼ����
	void InitHeroExp(int nCurNum,int nMaxNum);
	//������������ֵ
	void setHeroMaxExp(int nNum);

	//������Top���Ӽ���ֵ @nType == 1:   ��2:   ��3:
	void addTopUiNum(int nType,int nNum);
	//��ʼ��TopUi ��ǰֵ ����ֵ ����
	void InitTopUiNum(int nType,int nCurNum, int nMaxNum, const string& text = " ");
	//TopUi �������ֵ
	void setTopUiMaxNum(int nType, int nMaxNum);

	//�����ұ�Right���Ӽ���ֵ @nType == 1:   ��2:   ��3:     4:
	void addRightUiNum(int nType,int nNum);
	//��ʼ��RightUi ��ǰֵ ����ֵ ����
	void InitRightUiNum(int nType,int nCurNum, int nMaxNum, const string& text = " ");
	//RightUi �������ֵ
	void setRightUiMaxNum(int nType, int nMaxNum);
	//�������������
	void setChatString(const string& text);
	//����������ʾ
	void setChatTip(int nType,int Num);

	//����ѡ��ս�����ͻص�
	void setStaticWarCallBack(const ccMenuCallback& callback);
private:
	virtual bool init();  
	//����UI����
	void createUIScene();
	//���TOP�ص�
	void onClickTopUiCallback(Ref* sender );
	//����ͷ��UI
	void createTopUi();
	//�����ұ�UI
	void creatRightUi();
	//ͷ��UI
	void createHerdUi();
	//�ײ�UI
	void createDownUi();
	//�ײ�UI�ص�
	void onClickDownUiCallbacke(Ref* sender);
	//����UI
	void createChat();

	//ѡ��ս������
	void onClickStaticWar(Ref* sender);

	//�ƶ���ť�������������ص�
	void callBackPopUpBtnOk();
	//���ض��������ص�
	void callBackBounceBtnOk();

private:
	bool _bPopUp;
	Vector<UITouchButton*> m_vWarBtnUi;
	Vector<UIUserInfoLayer*> m_vTopUi;
	Vector<UIUserInfoLayer*> m_vRightUi;
	UIUserInfoLayer* m_HeadLayer;
	Label* _lbLv; //�ȼ�Lb
	Label* _lbChat; //����
	UITouchButton* _btnChat;
	ccMenuCallback _btnStaticWarCallBack;
};




#endif //  __UIUSERMINUI_H__


