#ifndef  _UISOLDIERCREATOR_H
#define  _UISOLDIERCREATOR_H
#include "UIBuildingBase.h"

class UISoldierCreator : public Building
{
public:
	UISoldierCreator(){}
	~UISoldierCreator(){}
    CREATE_FUNC(UISoldierCreator); 
	virtual void setBuidingData( DBuilding * db );
private:
	void updateWorkStatus(float dt);

	

};


#endif



