#include "Scheduler.hpp"
#include <queue>

Scheduler::Scheduler(Node* node)
{
	mMaxDepth = getMaxDepth(node);
	mTabPrio = new std::queue<Node*>[mMaxDepth];
}

int Scheduler::getMaxDepth(Node* node)
{

}

void Scheduler::notifyFinished(Node* node)
{
	for (int i = 0; i < node->mFathers.size(); i++)
	{
		Node* father = node->mFathers[i];
		father->mNbUnfinishedSons--;
		if (father->mNbUnfinishedSons == 0)
		{
			mTabPrio[father->mDepth].push(father);
		}
	}
}

Node* Scheduler::getNextTask()
{
	Node* task;
	for (int i = mMaxDepth; i >= 0; i--)
	{
		task = mTabPrio[i].front(); // TODO: check that front() return NULL if the queue is empty
		if (task)
		{
			mTabPrio[i].pop();
			break;
		}
	}
	return task;
}
