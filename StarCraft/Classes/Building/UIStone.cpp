#include "UIStone.h"

void UIStone::updateStatus()
{
	auto str = String::createWithFormat("dizuo_%02d",m_nGridCount);
	getBuildingArmature()->getAnimation()->play(str->getCString(),-1,0);
}
void UIStone::removeBuildPoint()
{  
}
void UIStone::addBuildPoint()
{
}
