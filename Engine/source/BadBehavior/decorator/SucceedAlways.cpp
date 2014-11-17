#include "console/engineAPI.h"

#include "SucceedAlways.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// SucceedAlways decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(SucceedAlways);

Task *SucceedAlways::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new SucceedAlwaysTask(*this, owner, runner);
}

void SucceedAlwaysTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->setStatus(INVALID);
}

//------------------------------------------------------------------------------
// SucceedAlways decorator task
//------------------------------------------------------------------------------
SucceedAlwaysTask::SucceedAlwaysTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

Task* SucceedAlwaysTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;
      
      return NULL;
   }
   
   return (*mCurrentChild); 
}
      
void SucceedAlwaysTask::onChildComplete(Status s)
{
   if(s == SUCCESS || s == FAILURE)
      mStatus = SUCCESS;
   else
      mStatus = s;
 
   mIsComplete = true;
}

