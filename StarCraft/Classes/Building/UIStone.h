#ifndef  __UISTONE_H_
#define  __UISTONE_H_
#include "UIBuildingBase.h"
class UIStone : public Building
{
public:
	UIStone(){}
   ~UIStone(){}
   CREATE_FUNC(UIStone);
   virtual void updateStatus();
   virtual void removeBuildPoint();
   virtual void addBuildPoint();

};



#endif



