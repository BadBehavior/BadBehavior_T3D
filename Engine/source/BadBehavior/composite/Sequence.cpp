#include "console/engineAPI.h"

#include "Sequence.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Sequence node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Sequence);

Task *Sequence::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new SequenceTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Sequence Task
//------------------------------------------------------------------------------
SequenceTask::SequenceTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner) 
   : Parent(node, owner, runner) 
{
}

void SequenceTask::onChildComplete(Status s)
{
   mStatus = s;

   if(s == SUCCESS)
      ++mCurrentChild;
   else
      mIsComplete = true;
}

Task* SequenceTask::update()
{
   if (mCurrentChild == mChildren.end())
      mIsComplete = true;

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
