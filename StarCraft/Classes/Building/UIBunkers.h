#ifndef  _UIBUNKERS_H
#define  _UIBUNKERS_H

#include "UIPiercingCannon.h"
//�ﱤ��
class UIBunkers : public UIPiercingCannon
{
public:
	UIBunkers(){}
	~UIBunkers(){}
	CREATE_FUNC(UIBunkers);
	virtual void doFly();
	virtual void flyEndCallBack( Node* ref );
	virtual void updateFlyWays( float dt );

};


#endif


