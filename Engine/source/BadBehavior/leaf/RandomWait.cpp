#include "console/engineAPI.h"
#include "math/mMathFn.h"

#include "RandomWait.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// RandomWait leaf node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(RandomWait);

RandomWait::RandomWait() 
   : mWaitMinMs(0), 
     mWaitMaxMs(99999) 
{
}

void RandomWait::initPersistFields()
{
   addGroup( "Behavior" );
   
   addProtectedField( "waitMinMs", TypeS32, Offset(mWaitMinMs, RandomWait), &_setWaitMin, &defaultProtectedGetFn,
      "The minimum time period in ms to wait before completion." );

   addProtectedField( "waitMaxMs", TypeS32, Offset(mWaitMaxMs, RandomWait), &_setWaitMax, &defaultProtectedGetFn,
      "The maximum time period in ms to wait before completion." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool RandomWait::_setWaitMin(void *object, const char *index, const char *data)
{
   RandomWait *node = static_cast<RandomWait *>( object );
   node->mWaitMinMs = getMin(node->mWaitMaxMs, dAtoi( data ));
   return false;
}

bool RandomWait::_setWaitMax(void *object, const char *index, const char *data)
{
   RandomWait *node = static_cast<RandomWait *>( object );
   node->mWaitMaxMs = getMax(node->mWaitMinMs, dAtoi( data ));
   return false;
}

Task *RandomWait::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new RandomWaitTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// RandomWait task
//------------------------------------------------------------------------------
RandomWaitTask::RandomWaitTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner), 
     mCompleteMs(0) 
{
}

void RandomWaitTask::onInitialize()
{
   Parent::onInitialize();
   RandomWait *nodeRep = static_cast<RandomWait*>(mNodeRep);
   mCompleteMs = Sim::getCurrentTime() + mRandI(nodeRep->mWaitMinMs, nodeRep->mWaitMaxMs);;
}

Task* RandomWaitTask::update() 
{ 
   if(Sim::getCurrentTime() < mCompleteMs)
      mStatus = RUNNING;
   else
      mStatus = SUCCESS;

   return NULL; 
}
