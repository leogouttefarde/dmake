class Scheduler
{
public:
	Scheduler(Node* node);
	void notifyFinished(Node* node);
	Node* getNextTask();
private:
	int getMaxDepth(Node* node);
	int					mMaxDepth;
	std::queue<Node*>*	mTabPrio
};
