#include "console/engineAPI.h"

#include "BadBehavior/core/Runner.h"
#include "WaitForEvent.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Wait for event decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(WaitForEvent);

Task *WaitForEvent::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new WaitForEventTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Wait for event decorator task
//------------------------------------------------------------------------------
WaitForEventTask::WaitForEventTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner)
{
}

void WaitForEventTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->setStatus(INVALID);
}

Task* WaitForEventTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;
      
      return NULL;
   }

   if(!mEventReceived)
   {
      mStatus = RUNNING;
      return NULL;
   }

   return (*mCurrentChild); 
}
      
void WaitForEventTask::onChildComplete(Status s)
{
   mStatus = s;
   mIsComplete = true;
}
