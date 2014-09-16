#ifndef  _UIMORTAR_H
#define  _UIMORTAR_H

#include "UIPiercingCannon.h"

typedef struct _ParabolyCfg
{   
	//开始点
	Point beginPos;
	//发射点
	Point shootPos;
	//结束点
	Point endPos;
}ParabolyCfg;

//抛物线动作
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
	//主要用这个
	float       _offsetX;
	//斜率是90是用这个
    float       _offsetY;
	Point       _currentPos;

};



//迫击炮
class UIMortar : public UIPiercingCannon 
{
public:
	UIMortar(){}
	~UIMortar(){}
	CREATE_FUNC(UIMortar);
	virtual void doFly();
	
};


#endif



