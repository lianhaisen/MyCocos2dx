//
//  UIGraySprite.cpp
//  TestProject
//  支持.png
//  
//
//

#include "UIGraySprite.h"



UIGraySprite::UIGraySprite(){
	_isGray = false;
}

UIGraySprite::~UIGraySprite(){
}
//create
UIGraySprite* UIGraySprite::create(const char* pszFileName){
	UIGraySprite *pobSprite = new UIGraySprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName)) {
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

//initWithFile
bool UIGraySprite::initWithFile(const char *pszFilename){
	m_pszFilename=pszFilename;
	return Sprite::initWithFile(pszFilename);
}

void UIGraySprite::setGray(bool isGray){

   if (_isGray != isGray)
   {
	   Image* finalImage=new Image();
	   bool isloadOK = finalImage->initWithImageFile(m_pszFilename);
	   if (isloadOK)
	   {
		   unsigned char *pData = finalImage->getData();
		   int iIndex = 0;
		   if(isGray){   
			   for (int i = 0; i < finalImage->getHeight(); i ++){
				   for (int j = 0; j < finalImage->getWidth(); j ++){
					   // gray

					   int iBPos = iIndex;
					   unsigned int iB = pData[iIndex];
					   iIndex ++;
					   unsigned int iG = pData[iIndex];
					   iIndex ++;
					   unsigned int iR = pData[iIndex];
					   iIndex ++;

					   iIndex ++; //原来的示例缺少
					   //CCLog("%d %d %d",iB,iG,iR);
					   pData[iBPos] = pData[iBPos + 1] = pData[iBPos + 2] = (unsigned char)(0.3 * iR + 0.4 * iG + 0.2 * iB);
				   }
			   }
		   }

		   Texture2D *pTexture = new Texture2D;
		   pTexture->initWithImage(finalImage);

		   Sprite::setTexture(pTexture);

		   delete finalImage;
		   pTexture->release();
	   }
	   
	 
   }
   _isGray = isGray;
}

void UIGraySprite::setTexture( Texture2D *texture ,const char* pszFileName )
{
	m_pszFilename = pszFileName;
	_isGray = false;
	Sprite::setTexture(texture);
}



