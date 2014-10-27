#include "console/engineAPI.h"
#include "Inverter.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Inverter decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Inverter);

Task *Inverter::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new InverterTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Inverter decorator task
//------------------------------------------------------------------------------
InverterTask::InverterTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

void InverterTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->setStatus(INVALID);
}

Task* InverterTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;
      
      return NULL;
   }
   
   return (*mCurrentChild); 
}
      
void InverterTask::onChildComplete(Status s)
{
   // invert SUCCESS or FAILURE, leave INVALID and RUNNING unchanged
   if (s == SUCCESS)
      mStatus = FAILURE;
   else if (s == FAILURE)
      mStatus = SUCCESS;
   else
      mStatus = s;

   mIsComplete = true;
}