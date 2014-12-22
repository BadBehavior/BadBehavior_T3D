#include "console/engineAPI.h"

#include "Monitor.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Monitor decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Monitor);

Task *Monitor::createTask()
{
   return new MonitorTask(*this);
}

//------------------------------------------------------------------------------
// Logger decorator task
//------------------------------------------------------------------------------
MonitorTask::MonitorTask(Node &node)
   : Parent(node) 
{
}

void MonitorTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->reset();
}

Task* MonitorTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;

      return NULL;
   }
   
   return (*mCurrentChild); 
}
      
void MonitorTask::onChildComplete(Status s)
{
   mStatus = s;
   Con::printf("%s child returning %s", static_cast<Monitor *>(mNodeRep)->getInternalName(), EngineMarshallData< BehaviorReturnType > (mStatus));

   mIsComplete = true;
}