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
	//std::cout << ID << "추격중 : " << monsterPosition.x<<","<< monsterPosition.z<< std::endl;
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
	//std::cout << ID << "돌아가는 중 : " << monsterPosition.x << "," << monsterPosition.z << std::endl;
}
void Monster::upDate()
{
	switch (myState)
	{
	case waitPosState:
	{
		if (false == getAlive())
			myState = deadState;
		//대기하면서 지속적으로 플레이어가 근처에 있나 검사한다.
		if (true == searchChasePlayer())
			myState = chaseTargetState;
		else
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
		}
		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "타겟 : "<<target<<std::endl;
		break;
	}
	case attackTargetState:
	{
		if (false == getAlive())
			myState = deadState;
		//공격상태에 들어가면 플레이어를 공격한다.
		if (false == attackPlayer())
		{
			//this->target = -1;
			//this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = chaseTargetState;
			//죽었을경우 객체 삭제
		}
		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "타겟 : " << target << std::endl;
		break;
	}
	case chaseTargetState:
	{
		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		if (true == attackPlayer()) 	//추격하다가 플레이어가 공격범위에 들어오면 공격한다.
			myState = attackTargetState;
		if (false == searchChasePlayer()) //추격하다가 추격범위를 플레이어가 벗어나면
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = returnPos;
		}
		else
			chaseTarget();

		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z <<" "<< myState << std::endl;
		//std::cout << "타겟 : " << target << std::endl;
		break;
	}
	case returnPos:
	{
		//if (true == searchChasePlayer()) //자기 자리로 돌아가던중에 플레이어가 추격범위에 있으면
		//	myState = chaseTargetState;
		//플레이어가 죽거나 시야에서 벗어나면 되돌아온다.
		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		//if (((monsterReturnPos.x - 10 <= monsterPosition.x) && (monsterPosition.x <= (monsterReturnPos.x + 10)) &&
		//	(monsterReturnPos.z - 10 <= monsterPosition.z) && (monsterPosition.z <= (monsterReturnPos.z + 10))))
		//{
		//	myState = waitPosState;
		//	//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//}
		else
			returnMove();
		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "타겟 : " << target << std::endl;
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
		//대기하면서 지속적으로 플레이어가 근처에 있나 검사한다.
		if (true == searchChasePlayer())
			myState = chaseTargetState;
		else
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
		}
		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "타겟 : "<<target<<std::endl;
		break;
	}
	case attackTargetState:
	{
		if (health <= 1500)
			myState = revengeState;
		if (false == getAlive())
			myState = deadState;
		//공격상태에 들어가면 플레이어를 공격한다.
		if (false == attackPlayer())
		{
			//this->target = -1;
			//this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = chaseTargetState;
			//죽었을경우 객체 삭제
		}
		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "타겟 : " << target << std::endl;
		break;
	}
	case chaseTargetState:
	{

		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		if (true == attackPlayer()) 	//추격하다가 플레이어가 공격범위에 들어오면 공격한다.
			myState = attackTargetState;
		if (false == searchChasePlayer()) //추격하다가 추격범위를 플레이어가 벗어나면
		{
			this->target = -1;
			this->targetPosition = D3DXVECTOR3(-100, -100, -100);
			myState = returnPos;
		}
		else
			chaseTarget();

		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z <<" "<< myState << std::endl;
		//std::cout << "타겟 : " << target << std::endl;
		break;
	}
	case returnPos:
	{
		//if (true == searchChasePlayer()) //자기 자리로 돌아가던중에 플레이어가 추격범위에 있으면
		//	myState = chaseTargetState;
		//플레이어가 죽거나 시야에서 벗어나면 되돌아온다.
		if (false == getAlive())
		{
			myState = deadState;
			break;
		}
		if (monsterReturnPos == monsterPosition) // 최초 배치되었던 위치와 현재 위치가 같으면
			myState = waitPosState;
		else
			returnMove();
		//std::cout << ID << "상태 : " << targetPosition.x << "," << targetPosition.z << " " << myState << std::endl;
		//std::cout << "타겟 : " << target << std::endl;
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
		if (true == attackPlayer()) 	//보스 몬스터는 분노 상태에서는 공격범위에 플레이어가 들어오면 스메쉬 상태로 바뀐다.
			myState = smashState;
		if (false == searchChasePlayer()) //추격하다가 추격범위를 플레이어가 벗어나면
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