#include "console/engineAPI.h"

#include "SucceedAlways.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// SucceedAlways decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(SucceedAlways);

Task *SucceedAlways::createTask()
{
   return new SucceedAlwaysTask(*this);
}

void SucceedAlwaysTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->reset();
}

//------------------------------------------------------------------------------
// SucceedAlways decorator task
//------------------------------------------------------------------------------
SucceedAlwaysTask::SucceedAlwaysTask(Node &node)
   : Parent(node) 
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
   mStatus = (s == FAILURE) ? SUCCESS : s;
   mIsComplete = true;
}

