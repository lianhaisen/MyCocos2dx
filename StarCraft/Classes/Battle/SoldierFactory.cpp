#include "SoldierFactory.h"
#include "HumanSoldier.h"
#include "MachineSoldier.h"
#include "AirSoldier.h"
#include "SoldierData.h"

Soldier* SoldierFactory::createSoldier(int id)
{
	auto sd = SoldierData::getSoldierData(id);
	int type = sd->type/100;
	switch (type)
	{
	case 700001://机枪兵
		return new SoldierJqb(id);
		break;
	case 700002://炮兵
		return new SoldierPb(id);
		break;
	case 700003://护士
		return new SoldierHs(id);
		break;
	case 700004://自杀机器人
		return new SoldierZsjqr(id);
		break;
	case 700005://火枪兵
		return new SoldierHqb(id);
		break;
	case 700006://机械战警
		return new SoldierJxzj(id);
		break;
	case 700007://烈焰战车
		return new SoldierLyzc(id);
		break;
	case 700008://攻城车
		return new SoldierGcc(id);
		break;
	case 700009://坦克
		return new SoldierTk(id);
		break;
	case 700010://雷神机器人
		return new SoldierLsjqr(id);
		break;
	case 700011://大鸟直升机
		return new SoldierDnzsj(id);
		break;
	case 700012://幽灵战机
		return new SoldierYlzj(id);
		break;
	case 700013://运输机
		return new SoldierYsj(id);
		break;
	case 700014://黑蝙蝠战机
		return new SoldierHbfzj(id);
		break;
	case 700015://铁鹰战机
		return new SoldierTyzj(id);
		break;
	case 700016://战列舰
		return new SoldierZlj(id);
		break;
	case 800001://蚁虫
		break;
	case 800002://刺蛇
		break;
	case 800003://爆虫
		break;
	case 800004://速龙
		break;
	case 800005://蜂巢
		break;
	case 800006://蜂虫
		break;
	case 800007://飞龙
		break;
	case 800008://巢虫领主
		break;
	case 800009://翼候
		break;
	case 800010://自杀蝗虫
		break;
	case 800011://感染者
		break;
	case 800012://飞蛇
		break;
	case 800013://雷兽
		break;
	case 800014://地刺
		break;
	case 800015://腐蚀兽
		break;
	case 800016://虫后
		break;
	case 800017://跳虫
		break;
	case 800018://地狱兽
		break;
	default:
		return nullptr;
		break;
	}
}



