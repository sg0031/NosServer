#include "stdafx.h"
#include "Monster.h"

Monster::Monster()
{

}


Monster::~Monster()
{
}

bool Monster::searchChasePlayer()
{
	float dist = (targetPosition.x - monsterPosition.x)
		*(targetPosition.x - monsterPosition.x)
		+ (targetPosition.z - monsterPosition.z)
		* (targetPosition.z - monsterPosition.z);
	if (dist <= chaseRange * chaseRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Monster::attackPlayer()
{
	float dist = (targetPosition.x - monsterPosition.x)
		*(targetPosition.x - monsterPosition.x)
		+ (targetPosition.z - monsterPosition.z)
		* (targetPosition.z - monsterPosition.z);
	if (dist <= attackRange * attackRange)
	{
		return true;
	}
	else
		return false;
}
void Monster::setTarget(int id, D3DXVECTOR3 playerPosition)
{
	if(-1==target)
		this->target = id;
	this->targetPosition = playerPosition;
}
void Monster::move()
{

}
void Monster::chaseTarget()
{
	D3DXVECTOR3 direction= this->targetPosition - this->monsterPosition;
	D3DXVec3Normalize(&direction, &direction);
	if (monsterDirection != direction)
		monsterDirection = direction;
	this->monsterPosition= monsterPosition + (monsterVelocity * monsterDirection * dealtaTime);
	//std::cout << ID << "�߰��� : " << monsterPosition.x<<","<< monsterPosition.z<< std::endl;
}
void Monster::attackTarget()
{

}
void Monster::returnMove()
{
	D3DXVECTOR3 direction = this->monsterReturnPos - this->monsterPosition;
	D3DXVec3Normalize(&direction, &direction);
	if(monsterDirection !=direction)
		monsterDirection = direction;
	this->monsterPosition = monsterPosition + (monsterVelocity * monsterDirection * dealtaTime);
	if ((monsterReturnPos.x + 10 >= monsterPosition.x) && (monsterReturnPos.x - 10 <= monsterPosition.x) &&
		(monsterReturnPos.z + 10 >= monsterPosition.z) && (monsterReturnPos.z - 10 <= monsterPosition.z))
	{
		monsterPosition = monsterReturnPos;
		myState = waitPosState;
	}
	//std::cout << ID << "���ư��� �� : " << monsterPosition.x << "," << monsterPosition.z << std::endl;
}
void Monster::upDate()
{
	switch (myState)
	{
	case waitPosState:
	{
		if (false == getAlive())
			myState = deadState;
		//����ϸ鼭 ���������� �÷��̾ ��ó�� �ֳ� �˻��Ѵ�.
		if (true == searchChasePlayer())
			myState = chaseTargetState;
		else
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
		}
		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "Ÿ�� : "<<target<<std::endl;
		break;
	}
	case attackTargetState:
	{
		if (false == getAlive())
			myState = deadState;
		//���ݻ��¿� ���� �÷��̾ �����Ѵ�.
		if (false == attackPlayer())
		{
			//this->target = -1;
			//this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = chaseTargetState;
			//�׾������ ��ü ����
		}
		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "Ÿ�� : " << target << std::endl;
		break;
	}
	case chaseTargetState:
	{
		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		if (true == attackPlayer()) 	//�߰��ϴٰ� �÷��̾ ���ݹ����� ������ �����Ѵ�.
			myState = attackTargetState;
		if (false == searchChasePlayer()) //�߰��ϴٰ� �߰ݹ����� �÷��̾ �����
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = returnPos;
		}
		else
			chaseTarget();

		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z <<" "<< myState << std::endl;
		//std::cout << "Ÿ�� : " << target << std::endl;
		break;
	}
	case returnPos:
	{
		//if (true == searchChasePlayer()) //�ڱ� �ڸ��� ���ư����߿� �÷��̾ �߰ݹ����� ������
		//	myState = chaseTargetState;
		//�÷��̾ �װų� �þ߿��� ����� �ǵ��ƿ´�.
		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		//if (((monsterReturnPos.x - 10 <= monsterPosition.x) && (monsterPosition.x <= (monsterReturnPos.x + 10)) &&
		//	(monsterReturnPos.z - 10 <= monsterPosition.z) && (monsterPosition.z <= (monsterReturnPos.z + 10))))
		//{
		//	myState = waitPosState;
		//	//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//}
		else
			returnMove();
		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "Ÿ�� : " << target << std::endl;
		break;
	}
	case deadState:
	{
		//std::cout << "dead" << std::endl;
		monsterReturnPos = monsterPosition;
		break;
	}
	default:
		std::cout << "monsterState Error" << std::endl;
		break;
	}
}




void OrcKing::upDate()
{
	switch (myState)
	{
	case waitPosState:
	{
		if (false == getAlive())
			myState = deadState;
		//����ϸ鼭 ���������� �÷��̾ ��ó�� �ֳ� �˻��Ѵ�.
		if (true == searchChasePlayer())
			myState = chaseTargetState;
		else
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
		}
		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "Ÿ�� : "<<target<<std::endl;
		break;
	}
	case attackTargetState:
	{
		if (health <= 1500)
			myState = revengeState;
		if (false == getAlive())
			myState = deadState;
		//���ݻ��¿� ���� �÷��̾ �����Ѵ�.
		if (false == attackPlayer())
		{
			//this->target = -1;
			//this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = chaseTargetState;
			//�׾������ ��ü ����
		}
		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "Ÿ�� : " << target << std::endl;
		break;
	}
	case chaseTargetState:
	{

		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		if (true == attackPlayer()) 	//�߰��ϴٰ� �÷��̾ ���ݹ����� ������ �����Ѵ�.
			myState = attackTargetState;
		if (false == searchChasePlayer()) //�߰��ϴٰ� �߰ݹ����� �÷��̾ �����
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = returnPos;
		}
		else
			chaseTarget();

		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z <<" "<< myState << std::endl;
		//std::cout << "Ÿ�� : " << target << std::endl;
		break;
	}
	case returnPos:
	{
		//if (true == searchChasePlayer()) //�ڱ� �ڸ��� ���ư����߿� �÷��̾ �߰ݹ����� ������
		//	myState = chaseTargetState;
		//�÷��̾ �װų� �þ߿��� ����� �ǵ��ƿ´�.
		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		if (monsterReturnPos == monsterPosition) // ���� ��ġ�Ǿ��� ��ġ�� ���� ��ġ�� ������
			myState = waitPosState;
		else
			returnMove();
		//std::cout << ID << "���� : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "Ÿ�� : " << target << std::endl;
		break;
	}
	case deadState:
	{
		//std::cout << "dead" << std::endl;
		monsterReturnPos = monsterPosition;
		break;
	}
	case revengeState:
	{
		revengeMode();
		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		if (true == attackPlayer()) 	//���� ���ʹ� �г� ���¿����� ���ݹ����� �÷��̾ ������ ���޽� ���·� �ٲ��.
			myState = smashState;
		if (false == searchChasePlayer()) //�߰��ϴٰ� �߰ݹ����� �÷��̾ �����
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = returnPos;
		}
		break;
	}
	case smashState:
	{
		if (false == getAlive())
			myState = deadState;
		break;
	}
	default:
		std::cout << "monsterState Error" << std::endl;
		break;
	}
}