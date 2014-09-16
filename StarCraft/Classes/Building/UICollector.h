#ifndef  __UICOLLECTOR_H
#define  __UICOLLECTOR_H
#include "UIBuildingBase.h"

//采集器
class UICollector: public Building
{
public:
    UICollector();
	~UICollector();;
	CREATE_FUNC(UICollector);
	virtual void updateStatus();
	virtual void setBuidingData( DBuilding * db );
private:
    CC_SYNTHESIZE_RETAIN(UITouchButton*,_collectIcon,CollectIcon);
	void updateWorkStatus(float dt);
	//点击收集
	void onCollect();



};

#endif



