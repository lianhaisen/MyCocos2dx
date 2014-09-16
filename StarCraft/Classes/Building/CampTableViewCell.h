#ifndef _CAMPTABLEVIEWCELL_H
#define _CAMPTABLEVIEWCELL_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d;

class CampTableViewCell : public TableViewCell
{
public:
	CampTableViewCell();
	~CampTableViewCell();
	virtual bool init();
	CREATE_FUNC(CampTableViewCell);
	void setInfo(int id,int lv,int count);
	void disableInfo();

private:
	CC_SYNTHESIZE_RETAIN(Sprite*,_bg,Bg);
	CC_SYNTHESIZE_RETAIN(Node*,_contentNode,ContentNode);

};



#endif



