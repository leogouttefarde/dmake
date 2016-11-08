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
	for (int i = 0; i < node->vFathers.size(); i++)
	{
		Node* father = node->vFathers[i];
		father->nbUnfinishedSons--;
		if (father->nbUnfinishedSons == 0)
		{
			mTabPrio[father->depth].push(father);
		}
	}
}

Node* Scheduler::getNextTask()
{
	Node* task;
	for (int i = mMaxDepth; i >= 0; i--)
	{
		task = mTabPrio[i].front(); // TODO: vérifier que front retourne NULL si vide
		if (task)
		{
			mTabPrio[i].pop();
			break;
		}
	}
	return task;
}
