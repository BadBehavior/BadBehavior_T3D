#include "console/engineAPI.h"
#include "math/mMathFn.h"

#include "Wait.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Wait leaf node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Wait);

Wait::Wait() 
   : mWaitMs(1000)
{
}

void Wait::initPersistFields()
{
   addGroup( "Behavior" );
   
   addProtectedField( "waitMs", TypeS32, Offset(mWaitMs, Wait), &_setWait, &defaultProtectedGetFn,
      "The time in ms that the node should wait before completion." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool Wait::_setWait(void *object, const char *index, const char *data)
{
   Wait *node = static_cast<Wait *>( object );
   node->mWaitMs = getMax(0, dAtoi( data ));
   return false;
}

Task *Wait::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new WaitTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Wait task
//------------------------------------------------------------------------------
WaitTask::WaitTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner), 
     mCompleteMs(0) 
{
}

void WaitTask::onInitialize()
{
   Parent::onInitialize();
   Wait *nodeRep = static_cast<Wait*>(mNodeRep);
   mCompleteMs = Sim::getCurrentTime() + nodeRep->mWaitMs;
}

Task* WaitTask::update() 
{ 
   if(Sim::getCurrentTime() < mCompleteMs)
      mStatus = RUNNING;
   else
      mStatus = SUCCESS;

   return NULL; 
}
