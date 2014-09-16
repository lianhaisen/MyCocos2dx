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
	case 700001://��ǹ��
		return new SoldierJqb(id);
		break;
	case 700002://�ڱ�
		return new SoldierPb(id);
		break;
	case 700003://��ʿ
		return new SoldierHs(id);
		break;
	case 700004://��ɱ������
		return new SoldierZsjqr(id);
		break;
	case 700005://��ǹ��
		return new SoldierHqb(id);
		break;
	case 700006://��еս��
		return new SoldierJxzj(id);
		break;
	case 700007://����ս��
		return new SoldierLyzc(id);
		break;
	case 700008://���ǳ�
		return new SoldierGcc(id);
		break;
	case 700009://̹��
		return new SoldierTk(id);
		break;
	case 700010://���������
		return new SoldierLsjqr(id);
		break;
	case 700011://����ֱ����
		return new SoldierDnzsj(id);
		break;
	case 700012://����ս��
		return new SoldierYlzj(id);
		break;
	case 700013://�����
		return new SoldierYsj(id);
		break;
	case 700014://������ս��
		return new SoldierHbfzj(id);
		break;
	case 700015://��ӥս��
		return new SoldierTyzj(id);
		break;
	case 700016://ս�н�
		return new SoldierZlj(id);
		break;
	case 800001://�ϳ�
		break;
	case 800002://����
		break;
	case 800003://����
		break;
	case 800004://����
		break;
	case 800005://�䳲
		break;
	case 800006://���
		break;
	case 800007://����
		break;
	case 800008://��������
		break;
	case 800009://���
		break;
	case 800010://��ɱ�ȳ�
		break;
	case 800011://��Ⱦ��
		break;
	case 800012://����
		break;
	case 800013://����
		break;
	case 800014://�ش�
		break;
	case 800015://��ʴ��
		break;
	case 800016://���
		break;
	case 800017://����
		break;
	case 800018://������
		break;
	default:
		return nullptr;
		break;
	}
}



