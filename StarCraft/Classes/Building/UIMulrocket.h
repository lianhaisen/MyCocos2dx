#ifndef  _UIMULROCKET_H
#define  _UIMULROCKET_H

//�෢�����
#include "UIPiercingCannon.h"
class UIMulrocket : public UIPiercingCannon
{
public:
	UIMulrocket(){}
	~UIMulrocket(){}
	CREATE_FUNC(UIMulrocket);
	virtual void doFly();

};


#endif


