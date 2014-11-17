#include "console/engineAPI.h"
#include "math/mMathFn.h"

#include "RandomSelector.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Random selector node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(RandomSelector);

Task *RandomSelector::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new RandomSelectorTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Random selector task
//------------------------------------------------------------------------------
RandomSelectorTask::RandomSelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

void RandomSelectorTask::onInitialize()
{
   Parent::onInitialize();

   mChildSelection.clear();

   // populate the list of child indices
   for(U32 i=0; i < mChildren.size(); i++)
      mChildSelection.push_back(i);

   pickRandomChild();
}

void RandomSelectorTask::pickRandomChild()
{
   if(mChildSelection.size() > 0)
   {
      U32 index = mRandI(0, mChildSelection.size() - 1);
      U32 child = mChildSelection[index];
      mChildSelection.erase(index, 1);
      mCurrentChild = mChildren.begin() + child;
   }
   else
      mCurrentChild = mChildren.end(); 
}

Task* RandomSelectorTask::update()
{
   if(mCurrentChild == mChildren.end())
   {
      mIsComplete = true;
   }
   
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;

      return NULL;
   }

   if(mStatus != RUNNING)
      (*mCurrentChild)->setStatus(INVALID);
   
   // return child
   return (*mCurrentChild);   
}

void RandomSelectorTask::onChildComplete(Status s)
{
   mStatus = s;

   if(s == FAILURE)
      pickRandomChild();
   else
      mIsComplete = true;
}
