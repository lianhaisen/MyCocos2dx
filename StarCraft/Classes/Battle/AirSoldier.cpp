#include "AirSoldier.h"


//����ֱ����700011
SoldierDnzsj::SoldierDnzsj( int id ) : Soldier(id)
{

}

SoldierDnzsj::~SoldierDnzsj( void )
{

}

void SoldierDnzsj::runWalkAction()
{
	string bodyName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
}

//����ս��700012
SoldierYlzj::SoldierYlzj( int id ) : Soldier(id)
{

}

SoldierYlzj::~SoldierYlzj( void )
{

}

void SoldierYlzj::runWalkAction()
{
	string bodyName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
}

//�����700013
SoldierYsj::SoldierYsj( int id ) : Soldier(id)
{

}

SoldierYsj::~SoldierYsj( void )
{

}

void SoldierYsj::runWalkAction()
{
	string bodyName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
}

//������ս��700014
SoldierHbfzj::SoldierHbfzj( int id ) : Soldier(id)
{

}

SoldierHbfzj::~SoldierHbfzj( void )
{

}

void SoldierHbfzj::runWalkAction()
{
	string bodyName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
}

//��ӥս��700015
SoldierTyzj::SoldierTyzj( int id ) : Soldier(id)
{

}

SoldierTyzj::~SoldierTyzj( void )
{

}

void SoldierTyzj::runWalkAction()
{
	string bodyName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
}

//ս�н�700016
SoldierZlj::SoldierZlj( int id ) : Soldier(id)
{

}

SoldierZlj::~SoldierZlj( void )
{

}

void SoldierZlj::runWalkAction()
{
	string bodyName = "";
	switch (m_direction)
	{
	case e_battle_dir_east :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_es :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_south :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_ws :
		bodyName = "01_walk04";
		break;
	case e_battle_dir_west :
		bodyName = "01_walk01";
		break;
	case e_battle_dir_wn :
		bodyName = "01_walk02";
		break;
	case e_battle_dir_north :
		bodyName = "01_walk03";
		break;
	case e_battle_dir_en :
		bodyName = "01_walk05";
		break;
	default:
		break;
	}

	getBody()->getAnimation()->play(bodyName);
}