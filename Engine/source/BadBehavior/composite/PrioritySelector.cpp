#include "console/engineAPI.h"
#include "PrioritySelector.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Priority selector node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(PrioritySelector);

Task *PrioritySelector::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new PrioritySelectorTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Priority selector task
//------------------------------------------------------------------------------
PrioritySelectorTask::PrioritySelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

void PrioritySelectorTask::onChildComplete(Status s)
{
   mStatus = s;

   if(s == FAILURE)
      ++mCurrentChild;
   else
      mIsComplete = true;
}

Task* PrioritySelectorTask::update()
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

   // return child
   if(mStatus != RUNNING)
      (*mCurrentChild)->setStatus(INVALID);
   
   return (*mCurrentChild);   
}

