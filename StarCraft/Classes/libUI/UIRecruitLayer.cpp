#include "UIRecruitLayer.h"
#include "AppMacros.h"
#include "BuildingFunc.h"
#include "UITipLayer.h"
#include "SoldierConfig.h"
#include "BarrackData.h"
#include "BuildingConfig.h"

static UIRecruitLayer* m_instance;
static int m_nCurrentStart = 1;
#define CON Configuration::getInstance()

UIRecruitLayer::UIRecruitLayer()
{
	PosEnd = Point(0,0);
	_booltrue = false;
	_db = nullptr;
	m_lbAllTime = nullptr;
	_lbTopTime = nullptr;
	m_pUserInfoLayer = nullptr;
	m_btnUpMoney = nullptr;
	_traindata = nullptr;
	_isNeedFresh = false;

}

UIRecruitLayer::~UIRecruitLayer()
{
	_mSoldierList.clear();
}

UIRecruitLayer* UIRecruitLayer::create()
{
	UIRecruitLayer *pRet = new UIRecruitLayer();
		if(pRet && pRet->init())
		{
			pRet->autorelease();

	
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
}
bool UIRecruitLayer::init()
{
	if (UIMsgLayer::init())
	{   
		this->setVIsibelLeftRightBtn(true);
		createRecruitLayer();
		return true;
	}
	//添加背景
	
	return false;
}
//左右按钮回调
void UIRecruitLayer::onClickLeftBtnCallBack(Ref* sender)
{
	auto item =  static_cast<MenuItem*>(sender);
	m_nCurrentStart -= 1;
	if(m_nCurrentStart < 1)
		m_nCurrentStart = BuildingFunc::getInstance()->getBarracks().size();
	item->setTag(m_nCurrentStart);

	createRecruitLayer(true);

	auto node = getContentInfoNode();
	auto pos = Point(-node->getContentSize().width,node->getPositionY());
	PopInfoNodeAction(pos);

}
void UIRecruitLayer::onClickRightBtnCallBack(Ref* sender)
{
	auto item =  static_cast<MenuItem*>(sender);
	m_nCurrentStart += 1;
	if(m_nCurrentStart > (int)BuildingFunc::getInstance()->getBarracks().size())
		m_nCurrentStart = 1;
	item->setTag(m_nCurrentStart);

	createRecruitLayer(true);

	auto node = getContentInfoNode();
	getContentTmpInfoNode()->setPosition(Point(-node->getContentSize().width,node->getPositionY()));
	auto pos = Point(node->getContentSize().width,node->getPositionY());
	PopInfoNodeAction(pos);
}
//创建招募layer
void UIRecruitLayer::createRecruitLayer(bool IsPop)
{
	//初始化值
	_db = BuildingFunc::getInstance()->getBarrackFromIndex(m_nCurrentStart);
	auto train =  TrainingData::getTrainingDataFromId(_db->id);
	_traindata = train;

	if(m_lbAllTime)
		m_lbAllTime->removeFromParent();
	m_lbAllTime = nullptr;
	_mSoldierList.clear();

	createSoldierRecruit(IsPop);
}
//创建或者尾部添加招募士兵节点
void UIRecruitLayer::createSoldierRecruit(bool IsPop)
{
	if(!_db) 
		return ;
	//头部信息
	auto cfg = _db->getCurBindCfg();
	auto str = String::createWithFormat(CON->getValue("des").asString().c_str(),cfg->name.c_str(),cfg->id % 100);
	this->setTitle(str->getCString());

	//内容
	auto infoNode = getContentInfoNode();//默认信息节点
	_booltrue = IsPop;
	if(IsPop)		//是否添加弹窗
	{
		auto node = getContentInfoNode();
		auto Parent = node->getParent();

		setContentTmpInfoNode(Node::create());
		auto m_pTmpInfoNode = getContentTmpInfoNode();
		m_pTmpInfoNode->setContentSize(getContentInfoNode()->getContentSize());
		m_pTmpInfoNode->setPosition(Point(getContentInfoNode()->getContentSize().width,0));
		Parent->addChild(m_pTmpInfoNode);
		infoNode = m_pTmpInfoNode;
	}
	auto size = infoNode->getContentSize();
	m_pUserInfoLayer = nullptr;
	//头像位置
	PosEnd = Point(0,0);
	//背景
	auto layer = Scale9Sprite::create("form/form001.png");
	layer->setAnchorPoint(Point(0.5,0));
	infoNode->addChild(layer);
	layer->setContentSize(Size(infoNode->getContentSize().width*0.98,infoNode->getContentSize().height*0.58));
	layer->setPosition(infoNode->getContentSize().width/2,10);


	//招募兵种
	if(_db){
		_mSoldierInfo.clear();
		auto vc = BuildingFunc::getInstance()->getSolidListFromDBuilding(_db);
		for (auto sitem : vc)
		{  
			createSoldierLayer(sitem,0,true);
		}
		//刷新创建中的士兵列表
		auto train =  TrainingData::getTrainingDataFromId(_db->id);
		_traindata = train;
		if(train)
		{
			auto i = 1;
			for (auto sitem : train->soldieritems )
			{
				RefreshCreateSoldierList(i,sitem->soldierid/100,sitem->number,sitem->trainingtime,sitem->trainingtime - train->curtraininglavetime);
				i++;
			}
		}

	}
	_booltrue =false;



}
//点击建造士兵
void UIRecruitLayer::addRecruitSoldier(Ref* sender)
{
	auto item =  static_cast<UISoldierHeadLayer*>(sender);
	auto tag = item->getSoldierinfo()->nType;
	int nOrder = 0;
	
	map<int,RecruitSoldier>::iterator it = _mSoldierList.begin();
	for(it; it != _mSoldierList.end();it++)
	{
		if(tag == it->second.nType)
		{
			auto info =  it->second;
			nOrder = it->second.nOrder;
			
			RefreshCreateSoldierList(nOrder,tag,info.nNum,info.ntime,info.nCurtime);
			break;
		}
	}
	if(nOrder == 0 || _mSoldierList.size() == 0 )
	{
		RefreshCreateSoldierList(_mSoldierList.size()+1,tag,1,getSoldierInfo(tag)->nTime,0);
	}
	
	if (m_Unschedule)//启动定时器
	{
		m_Unschedule(this);
	}
	_isNeedFresh = true;

}

//添加建造中的士兵
void UIRecruitLayer::RefreshCreateSoldierList(int nOrder,int nType,int nNum,int ntime,int nCurtime)
{
	map<int,RecruitSoldier>::iterator it = _mSoldierList.find(nOrder);
	if (it != _mSoldierList.end())
	{
		auto info = it->second;
		if(info.imgHerd)					//更新
		{
			info.nNum += 1;
			auto str = String::createWithFormat("%d",info.nNum); 
			info.lbNum->setString(str->getCString());
			_mSoldierList[nOrder]= info;
		}
		else								//刷新界面
		{
			auto info = _mSoldierList[nOrder];
			createRecruitSoldierLayer(nOrder,nType,info.nNum,info.ntime,info.nCurtime);
		}
	}
	else									//创建
	{
		createRecruitSoldierLayer((int)_mSoldierList.size()+1,nType,nNum,ntime,nCurtime);
	}

	if(!m_lbAllTime){
		//总时间
		auto infoNode = getContentInfoNode();
		if(_booltrue)
			infoNode = getContentTmpInfoNode();
		auto size = getContentInfoNode()->getContentSize();
		m_btnUpMoney =  UITouchButton::create("btn/btn_jiasu.png");
			//背景
		auto layer = Scale9Sprite::create("form/form002.png");
		layer->setAnchorPoint(Point(0.5,1));
		infoNode->addChild(layer);
		layer->setContentSize(Size(m_btnUpMoney->getContentSize().width*0.8,m_btnUpMoney->getContentSize().height*0.9));
		layer->setPosition(size.width/1.3+layer->getContentSize().width/2,size.height-5);
		_lbTopTime = layer;

		auto lbTopTime = Label::createWithSystemFont("AllTime","",18);
		layer->addChild(lbTopTime);
		lbTopTime->setAnchorPoint(Point(0.5,0.5));
		lbTopTime->setPosition(layer->getContentSize().width/2,layer->getContentSize().height/1.4);
		m_lbAllTime = Label::createWithSystemFont(" ","",24);
		m_lbAllTime->setColor(Color3B::BLACK);
		layer->addChild(m_lbAllTime);
		m_lbAllTime->setAnchorPoint(Point(0.5,0.5));
		m_lbAllTime->setPosition(layer->getContentSize().width/2,layer->getContentSize().height/3.1);

		infoNode->addChild(m_btnUpMoney);
		m_btnUpMoney->setPosition(layer->getPositionX(),layer->getPositionY()-layer->getContentSize().height*2);
		m_btnUpMoney->setClickTarget(CC_CALLBACK_1(UIRecruitLayer::onClickJiaSuCallBack,this));
		auto spMoney = Sprite::create("ui/diamond.png");			//图
		auto lbMoney = Label::createWithSystemFont("50","",24);	//数值
		m_btnUpMoney->addChild(lbMoney,10,1);
		lbMoney->setPosition(-spMoney->getContentSize().width/2,0);
		m_btnUpMoney->addChild(spMoney,10,2);
		spMoney->setPosition(lbMoney->getPositionX()+lbMoney->getContentSize().width/2+spMoney->getContentSize().width/2+5,lbMoney->getPositionY());
	}
	if (m_Unschedule)//启动定时器
	{
		m_Unschedule(this);
	}
}

//删除正在建造中的士兵
void UIRecruitLayer::DeleteSoldierList(Ref* sender)
{
	auto item =  static_cast<MenuItem*>(sender);
	int tag = item->getTag();
	DeleteSoldier(tag);
	_isNeedFresh = true;
}
void UIRecruitLayer::DeleteSoldier(int nType)
{
	map<int,RecruitSoldier>::iterator it = _mSoldierList.find(nType);
	if(it != _mSoldierList.end())
	{	
		auto info = it->second;
		info.nNum -= 1;
		if(info.nNum <= 0)
		{
			if(m_pUserInfoLayer)
				m_pUserInfoLayer->removeFromParent();
			m_pUserInfoLayer = nullptr;
			int iVisibleNum = 0; //建造第五个以后都看不见
			info.imgHerd->removeFromParent();
			_mSoldierList.erase(it);
			map<int,RecruitSoldier>::iterator iter = _mSoldierList.begin();
			for(iter;iter!= _mSoldierList.end(); )
			{
				iVisibleNum += 1;
				auto info = iter->second;
				int Page = iter->first;
				auto size = getContentInfoNode()->getContentSize();
				if(iter == _mSoldierList.begin())
					PosEnd = Point(size.width/1.6,size.height/2+info.imgHerd->getContentSize().height*1.2);

				info.imgHerd->setPosition(PosEnd);
				PosEnd = Point(PosEnd.x - info.imgHerd->getContentSize().width*1.15,PosEnd.y);
				info.imgHerd->setVisible(true);
				if(iVisibleNum > 5)
					info.imgHerd->setVisible(false);
				if(iter == _mSoldierList.begin() && !m_pUserInfoLayer)
				{ 
					m_pUserInfoLayer = UIUserInfoLayer::create("ui/processbg.png",1,1.3f," ","ui/processbg3.png");
					info.imgHerd->addChild(m_pUserInfoLayer,0,info.nType);
					m_pUserInfoLayer->setinitNum(info.nCurtime,info.ntime);
					m_pUserInfoLayer->getNameLayer()->setVisible(false);
					auto str = String::createWithFormat("%d",info.ntime-info.nCurtime);
					m_pUserInfoLayer->getInfoString()->setString(str->getCString());
					m_pUserInfoLayer->setPosition(Point(info.imgHerd->getContentSize().width/2-m_pUserInfoLayer->getContentSize().width/2,-m_pUserInfoLayer->getContentSize().height*1.2));
				}
				if(iVisibleNum != info.nOrder)
				{
					info.imgHerd->getChildByTag(info.nOrder)->setTag(iVisibleNum);
					info.nOrder = iVisibleNum;
					_mSoldierList[iVisibleNum] = info;
					iter = _mSoldierList.erase(iter);
				}
				else
					iter++;
			}
		}
		else 
		{
			char str[64] = {0};
			sprintf(str, "%d",info.nNum);
			info.lbNum->setString(str);
			_mSoldierList[nType]= info;
		}
	}
	if((int)_mSoldierList.size() < 1)
	{
		if(_lbTopTime){
			m_btnUpMoney->removeFromParent();
			m_btnUpMoney= nullptr;
			_lbTopTime->removeFromParent();
			_lbTopTime= nullptr;
			m_lbAllTime = nullptr;
		}
	}
}
//点击查看士兵详细信息
void UIRecruitLayer::onClickCallBack(Ref* sender)
{
	auto item =  static_cast<UISoldierHeadLayer*>(sender);
	auto tag = item->getSoldierinfo()->nType;
	createSoldierInfoLayer(item->getData());
}

//创建士兵详细信息layer
void UIRecruitLayer::createSoldierInfoLayer(CSoldier * data)
{    
	auto tip = SoldierTip::createSoldierTip(data);
	addChild(tip->getTipLayer(),100);
}

//void UIRecruitLayer::onClickBckaCallBack(Ref* sender)
//{
//	this->setVisible(true);
//}

void UIRecruitLayer::setUnschedule( const ccMenuCallback& callback )
{
	m_Unschedule = callback;
}

void UIRecruitLayer::onClickJiaSuCallBack(Ref* sender)
{
	/*测试用*/
	char str[128] = {0};
	sprintf(str, "%d",5000);
	UpDataMoney(str);
}
//                  设置创建中的士兵                  //
void UIRecruitLayer::createRecruitSoldierLayer(int nOrder, int nType, int nNum,int ntime,int nCurtime)
{
	RecruitSoldier info = {ntime,nNum,nType,nOrder,nCurtime};
	info.Path = _mSoldierInfo[info.nType]->getSoldierinfo()->Path;
	info.imgHerd = Sprite::create("btn/btn_hero_head02.png");
	auto imgHerd = Sprite::create(info.Path);
	//数值
	auto mask = Sprite::create("ui/mask_01.png");
	mask->setAnchorPoint(Point(0.5,0));
	mask->setPosition(info.imgHerd->getContentSize().width/2,5);
	info.lbNum = Label::createWithSystemFont(" ","",24);
	info.imgHerd->addChild(mask,10);
	auto str = String::createWithFormat("%d",info.nNum)->getCString();
	info.lbNum->setString(str);
	//info.lbNum->setScale(mask->getContentSize().height/info.lbNum->getContentSize().height-0.1f);
	info.lbNum->setAnchorPoint(Point(0.5f,0.5f));
	info.lbNum->setPosition(mask->getContentSize().width/2,mask->getContentSize().height/2);
	mask->addChild(info.lbNum);

	auto btnClear = UITouchButton::create("ui/clear.png");
	info.imgHerd->addChild(btnClear,10);
	btnClear->setClickTarget(CC_CALLBACK_1(UIRecruitLayer::DeleteSoldierList,this));
	btnClear->setIsNeedLongTouch(true);
	btnClear->setLongPressTarget(CC_CALLBACK_1(UIRecruitLayer::DeleteSoldierList,this));
	btnClear->setPosition(info.imgHerd->getContentSize().width-btnClear->getContentSize().width/4,info.imgHerd->getContentSize().height- btnClear->getContentSize().height/4);
	btnClear->setTag(info.nOrder);
	//进度条
	if(nOrder == 1 ){
		m_pUserInfoLayer= UIUserInfoLayer::create("ui/processbg.png",1,1.3f," ","ui/processbg3.png");
		info.imgHerd->addChild(m_pUserInfoLayer,0,info.nType);
		m_pUserInfoLayer->setinitNum(nCurtime,info.ntime);
		m_pUserInfoLayer->getNameLayer()->setVisible(false);
		auto str = String::createWithFormat("%d",info.ntime-info.nCurtime);
		m_pUserInfoLayer->getInfoString()->setString(str->getCString());
		m_pUserInfoLayer->setPosition(Point(info.imgHerd->getContentSize().width/2 - m_pUserInfoLayer->getContentSize().width/2,-m_pUserInfoLayer->getContentSize().height*1.2));
	}
	imgHerd->setPosition(info.imgHerd->getContentSize().width/2,info.imgHerd->getContentSize().height/2);
	info.imgHerd->addChild(imgHerd);

	auto size = this->getContentInfoNode()->getContentSize();
	if(PosEnd.x == 0 || _mSoldierList.size() < 1)
		PosEnd = Point(size.width/1.6,size.height/2+info.imgHerd->getContentSize().height*1.2);

	if(_booltrue)
		this->getContentTmpInfoNode()->addChild(info.imgHerd);
	else 
		this->getContentInfoNode()->addChild(info.imgHerd);
	info.imgHerd->setPosition(PosEnd);

	PosEnd = Point(PosEnd.x - info.imgHerd->getContentSize().width*1.15,PosEnd.y);
	_mSoldierList[info.nOrder] = info;
	if(_mSoldierList.size() > 5)
	{
		info.imgHerd->setVisible(false);
	}
}
//数量
void UIRecruitLayer::setOneRecruitSoldierNum(int nType,int nNum)
{
	map<int,RecruitSoldier>::iterator it = _mSoldierList.find(nType);
	if(it != _mSoldierList.end())
	{
		auto info = it->second;
		info.nNum = nNum;
		auto str = String::createWithFormat( "%d",nNum);
		info.lbNum->setString(str->getCString());
		_mSoldierList[nType] = info;
	}
}
//时间
void UIRecruitLayer::setOneRecruitSoldierTime(int nType,int nTime,int nCurtime)
{
	map<int,RecruitSoldier>::iterator it = _mSoldierList.find(nType);
	if(it != _mSoldierList.end())
	{
		auto info = it->second;
		info.ntime = nTime;
		if(it == _mSoldierList.begin()){
			m_pUserInfoLayer->setinitNum(nCurtime,nTime);
			auto str = String::createWithFormat("%d",nTime-nCurtime);
			m_pUserInfoLayer->getInfoString()->setString(str->getCString());
		}
		_mSoldierList[nType] = info;
	}
}
//列队第一的单个士兵的进度
void UIRecruitLayer::dataFirstSoldierProcess(int nExp)
{
	if(m_pUserInfoLayer){
		m_pUserInfoLayer->setAddNumNoAction(nExp);
		auto str = String::createWithFormat("%d",_mSoldierList[1].ntime-nExp);
		m_pUserInfoLayer->getInfoString()->setString(str->getCString());
	}
}

/*                  设置士兵列表                     */
void UIRecruitLayer::createSoldierLayer(CSoldier * data ,int nNum,bool v)
{   
	auto layer = UISoldierHeadLayer::create(data,nNum,v);
    
	auto spacing = Size(layer->getContentSize().width,layer->getContentSize().height);
	layer->setRecruitCallBack(CC_CALLBACK_1(UIRecruitLayer::addRecruitSoldier,this));
	layer->setTipCallBack(CC_CALLBACK_1(UIRecruitLayer::onClickCallBack,this));
	layer->setIsRecruit(v);
	_mSoldierInfo[data->id/100]= layer;
	int nPage = (int)_mSoldierInfo.size();
	if(nPage<=5){
		layer->setPosition(Point(getContentInfoNode()->getContentSize().width/5*(nPage-1)+spacing.width/4+spacing.width/2.2,spacing.height*2));
	}
	else {
		layer->setPosition(Point(getContentInfoNode()->getContentSize().width/5*(nPage-6)+spacing.width/4+spacing.width/2.2,spacing.height*0.75f));
	}
	if(_booltrue)
		getContentTmpInfoNode()->addChild(layer);
	else 
	getContentInfoNode()->addChild(layer);
}
//获得士兵信息
UISoldierHeadLayer::Soldierinfo* UIRecruitLayer::getSoldierInfo(int nType)
{
	map<int,UISoldierHeadLayer*>::iterator it = _mSoldierInfo.find(nType);
	if(it != _mSoldierInfo.end())
	{
		auto info = it->second;
		return info->getSoldierinfo();
	}
	return NULL;
}
//设置总时间
void UIRecruitLayer::UpDataAllTIme(const string& text)
{  
	if (m_lbAllTime)
	{
		m_lbAllTime->setString(text);
	}
	
}
//更新加速金额
void UIRecruitLayer::UpDataMoney(const string& text)
{
	auto lbMoney = (Label*)m_btnUpMoney->getChildByTag(1);//
	lbMoney->setString(text);
	auto spMoney = m_btnUpMoney->getChildByTag(2);
	lbMoney->setPosition(-spMoney->getContentSize().width/2,0);
	spMoney->setPosition(lbMoney->getPositionX()+lbMoney->getContentSize().width/2+spMoney->getContentSize().width/2+5,lbMoney->getPositionY());
}

UIRecruitLayer* UIRecruitLayer::createFromDB( DBuilding * db )
{   
	
	m_nCurrentStart = BuildingFunc::getInstance()->getBarrackIndexFormDB(db);


	auto item = UIRecruitLayer::create();
	item->_db = db;
	if((int)BuildingFunc::getInstance()->getBarracks().size() < 2)
		item->setVIsibelLeftRightBtn(false);

	item->schedule(schedule_selector(UIRecruitLayer::updateWorkProcess),1.0f);
	return item;

}

void UIRecruitLayer::updateWorkProcess( float dt )
{   
	if (_isNeedFresh)
	{   
		_traindata->deletaitems();
		int i = 0;
		for (auto pair : _mSoldierList)
		{
			auto sitem = pair.second;
			auto item = new TrainItem();
			auto soldier = UserSoldierLevel::getUserSoldierLevelFromId(sitem.nType);

			item->soldierid    = soldier->getCurcfg()->id;
			item->number       = sitem.nNum;
			item->population   = soldier->getCurcfg()->population;
			item->trainingtime = soldier->getCurcfg()->train_time; 
			_traindata->soldieritems.push_back(item);

			if (i == 0)
			{
                if ( _traindata->curtraininglavetime >= item->trainingtime)
                {
					_traindata->curtraininglavetime = item->trainingtime - 1;
                }
                
			}

			i++;
		}
		
		auto totaltime = _traindata->getTotalTime();
		UpDataAllTIme(String::createWithFormat("%d",totaltime)->getCString());
		
		_isNeedFresh = false;
		return ;
	}

	if (!_mSoldierList.empty())
	{   
		//TODO 同步数据
		


		auto time =  _traindata->curtraininglavetime;
		auto first =  _mSoldierList.find(1);
		auto item = first->second;
		auto nextcanbuildtime = _traindata->getNextCanBuildTime();
		if (time == nextcanbuildtime || (nextcanbuildtime == 0  && time == 1 ) )
		{
			if(item.nNum - 1 <= 0)
			{
				DeleteSoldier(1);
			}
			else
			{
				setOneRecruitSoldierNum(1,item.nNum - 1);
				dataFirstSoldierProcess(item.ntime - time);
			}
			//添加建造完成士兵
			int Num = getSoldierInfo(item.nType)->nNum += 1;
			getSoldierInfo(item.nType)->lbNum->setString(String::createWithFormat("%d",Num)->getCString());
		}
		else
		{
			dataFirstSoldierProcess(item.ntime - time);
		}


		auto totaltime = _traindata->getTotalTime();
	    UpDataAllTIme(String::createWithFormat("%d",totaltime)->getCString());
		
		
	}
}


