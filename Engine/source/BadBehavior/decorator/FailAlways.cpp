#include "console/engineAPI.h"

#include "FailAlways.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// FailAlways decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(FailAlways);

Task *FailAlways::createTask()
{
   return new FailAlwaysTask(*this);
}

void FailAlwaysTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->setStatus(INVALID);
}

//------------------------------------------------------------------------------
// FailAlways decorator task
//------------------------------------------------------------------------------
FailAlwaysTask::FailAlwaysTask(Node &node)
   : Parent(node) 
{
}

Task* FailAlwaysTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;
      
      return NULL;
   }
   
   return (*mCurrentChild); 
}
      
void FailAlwaysTask::onChildComplete(Status s)
{
   mStatus = (s == SUCCESS) ? FAILURE : s;
   mIsComplete = true;
}

