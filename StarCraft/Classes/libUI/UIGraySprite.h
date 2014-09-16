
#ifndef __UIGraySprite_H__
#define __UIGraySprite_H__

#include "cocos2d.h"
USING_NS_CC;

class UIGraySprite : public Sprite{
public:
    UIGraySprite();
    virtual ~UIGraySprite();
    static  UIGraySprite* create(const char* pszFileName);
	void setTexture( Texture2D *texture ,const char* pszFileName) ;
    void setGray(bool isGray);

private:  
    std::string m_pszFilename;   
    bool initWithFile(const char *pszFilename);

	

	bool _isGray;
};

#endif //__UIBUTTON_H__
