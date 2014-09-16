#ifndef  _UIMORTAR_H
#define  _UIMORTAR_H

#include "UIPiercingCannon.h"

typedef struct _ParabolyCfg
{   
	//��ʼ��
	Point beginPos;
	//�����
	Point shootPos;
	//������
	Point endPos;
}ParabolyCfg;

//�����߶���
class ParabolyTo : public ActionInterval
{
public:
	ParabolyTo();
	static ParabolyTo * create(float t,const ParabolyCfg cfg);
	bool  init(float t,ParabolyCfg cfg);
	virtual void update( float time );
	virtual ActionInterval* reverse() const override;
	virtual ActionInterval * clone() const override;

protected:
	float getPosY(float posX);
	ParabolyCfg _config;
	//��Ҫ�����
	float       _offsetX;
	//б����90�������
    float       _offsetY;
	Point       _currentPos;

};



//�Ȼ���
class UIMortar : public UIPiercingCannon 
{
public:
	UIMortar(){}
	~UIMortar(){}
	CREATE_FUNC(UIMortar);
	virtual void doFly();
	
};


#endif



