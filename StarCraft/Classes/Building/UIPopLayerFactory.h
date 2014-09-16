#ifndef __UIPOPLAYERFACTORY_H 
#define __UIPOPLAYERFACTORY_H
#include "UIPopupLayer.h"
#include "UIButtonFactory.h"

class UIPopLayerFactory
{
public:
	UIPopLayerFactory(){}
	~UIPopLayerFactory();
	static UIPopupLayer * createLayerFromTypeAndDB(UIButtonFactory::UIButtonType type,DBuilding * db,Node * btn = nullptr);
};



#endif


