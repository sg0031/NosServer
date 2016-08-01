#include "stdafx.h"
#include "Object.h"

Object::Object()
{

}


Object::~Object()
{
}

bool Object::searchChasePlayer()
{
	float dist = (targetPosition.x - objectPosition.x)
		*(targetPosition.x - objectPosition.x)
		+ (targetPosition.z - objectPosition.z)
		* (targetPosition.z - objectPosition.z);
	if (dist <= chaseRange * chaseRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Object::attackPlayer()
{
	float dist = (targetPosition.x - objectPosition.x)
		*(targetPosition.x - objectPosition.x)
		+ (targetPosition.z - objectPosition.z)
		* (targetPosition.z - objectPosition.z);
	if (dist <= attackRange * attackRange)
	{
		return true;
	}
	else
		return false;
}
void Object::setTarget(int id, D3DXVECTOR3 playerPosition)
{
	if(-1==target)
		this->target = id;
	this->targetPosition = playerPosition;
}
void Object::move()
{

}
void Object::chaseTarget()
{
	D3DXVECTOR3 direction= this->targetPosition - this->objectPosition;
	D3DXVec3Normalize(&direction, &direction);
	if (objectDirection != direction)
		objectDirection = direction;
	this->objectPosition= objectPosition + (objectVelocity * objectDirection * dealtaTime);
	//std::cout << ID << "�߰��� : " << monsterPosition.x<<","<< monsterPosition.z<< std::endl;
}
void Object::attackTarget()
{

}
void Object::returnMove()
{
	D3DXVECTOR3 direction = this->objectReturnPos - this->objectPosition;
	D3DXVec3Normalize(&direction, &direction);
	if(objectDirection !=direction)
		objectDirection = direction;
	this->objectPosition = objectPosition + (objectVelocity * objectDirection * dealtaTime);
	if ((objectReturnPos.x + 10 >= objectPosition.x) && (objectReturnPos.x - 10 <= objectPosition.x) &&
		(objectReturnPos.z + 10 >= objectPosition.z) && (objectReturnPos.z - 10 <= objectPosition.z))
	{
		objectPosition = objectReturnPos;
		myState = waitPosState;
	}
	//std::cout << ID << "���ư��� �� : " << monsterPosition.x << "," << monsterPosition.z << std::endl;
}
void Object::upDate()
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
		objectReturnPos = objectPosition;
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
		if (objectReturnPos == objectPosition) // ���� ��ġ�Ǿ��� ��ġ�� ���� ��ġ�� ������
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
		objectReturnPos = objectPosition;
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