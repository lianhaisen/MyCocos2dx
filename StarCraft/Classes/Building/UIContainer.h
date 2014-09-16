#ifndef __UIContainer_H
#define __UIContainer_H
#include "UIBuildingBase.h"

// ≤÷ø‚¿‡
class UIContainer:public Building
{
public:
	UIContainer();
	~UIContainer();
	CREATE_FUNC(UIContainer);
	virtual void setBuidingData( DBuilding * db );
	void setShowFullLabel(bool show);
public:
	CC_SYNTHESIZE_RETAIN(Sprite*,_fulllabel,FullLabel);
	virtual void updateStatus();



};

#endif



