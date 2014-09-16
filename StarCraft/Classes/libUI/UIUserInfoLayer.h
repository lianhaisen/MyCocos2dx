#ifndef  __UIUSERINFOLAYER_H
#define  __UIUSERINFOLAYER_H

//TODO  �����û����ݽ����
// ��ҳ��
// ��������,���ø���ֵ
// ������Դ��ֵ��ʱ����Բ������⶯��~
//
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class UIUserInfoLayer :public Layer
{
public:
	static UIUserInfoLayer* create(const string &BgFile,float iScaleX, float iScaleY,string HeadFile = " ", string ExpFile = " ");
	//�������ø��ֻ��/**/
	//����ͷ��
	void setHerdImg(const string &filename);
	Sprite* getHerdImg(){return (Sprite*)this->getChildByTag(2);}

	//��������
	void setName(const string& text);
	Label* getNameLayer(){return (Label*)this->getChildByTag(3);}//static_cast<Sprite*>( LayerTop->getChildByTag(2) );

	//���þ�����
	ProgressTimer* getExpLayer(){return (ProgressTimer*)this->getChildByTag(5);}
	//�������� Ŀǰ������  //@Num �������ӵ����� 
	void setAddNum(int Num); 
	//û�ж����� ����ֵ
	void setAddNumNoAction(int Num);
	Label* getInfoString(){return (Label*)this->getChildByTag(4);}
	//�������� ����
	//void setInfoString(const string& text);

	//���ø���ͷ���Ƿ�Ť��
	void setHeadAction(bool b);

	//������ֵ
	int getMaxNum(){return m_nMaxNum;}
	void setMaxNum(int nMaxNum,const string& text = " ");
	//��õ�ǰֵ
	int getCurNum(){return m_nCurNum;}
	//���ó�ʼֵ
	void setinitNum(int nCurNum,int nMaxNum,const string& text = " ");

	//���ó���������ɫ��
	//void setFormImg(Sprite* sp);
	//Sprite* getFormImg(){return (Sprite*)this->getChildByTag(1);}
	//���ó���
	//void setBg(const string &filename);
	//Sprite* getBg(){return (Sprite*)this->getChildByTag(2);}
public:
	int m_nCurNum;			//��ǰֵ
	int m_nAddNum;			//����ֵ
	int m_nMaxNum;			//����ֵ
	bool m_bIsAction;		//����ʱͷ���Ƿ��ж���  Ĭ�� ͷ�񲻶� 
	float m_fScaleX;
	float m_fScaleY;
private:
	virtual bool init(const string &BgFile,float iScaleX, float iScaleY, string &HeadFile,string &ExpFile);  
	void createOneItme(const string &BgFile,float iScaleX, float iScaleY, string &HeadFile,string &ExpFile);//������ͼ �м��������layer  ����ͼ��
	void updataHerditemLayer(float delta);//������Ҿ���ֵ 
private:
	int m_nSpeed;			//�����ٶ�
};
#endif



