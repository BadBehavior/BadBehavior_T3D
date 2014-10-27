#include "console/engineAPI.h"
#include "math/mMathFn.h"

#include "Delay.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Delay decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Delay);

Delay::Delay() 
   : mDelayMinMs(0), 
     mDelayMaxMs(99999) 
{
}

void Delay::initPersistFields()
{
   addGroup( "Behavior" );
   
   addProtectedField( "delayMinMs", TypeS32, Offset(mDelayMinMs, Delay), &_setDelayMin, &defaultProtectedGetFn,
      "The minimum delay period in ms to wait before executing the child node." );

   addProtectedField( "delayMaxMs", TypeS32, Offset(mDelayMaxMs, Delay), &_setDelayMax, &defaultProtectedGetFn,
      "The maximum delay period in ms to wait before executing the child node." );
   
   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool Delay::_setDelayMin(void *object, const char *index, const char *data)
{
   Delay *node = static_cast<Delay *>( object );
   node->mDelayMinMs = getMin(node->mDelayMaxMs, dAtoi( data ));
   return false;
}

bool Delay::_setDelayMax(void *object, const char *index, const char *data)
{
   Delay *node = static_cast<Delay *>( object );
   node->mDelayMaxMs = getMax(node->mDelayMinMs, dAtoi( data ));
   return false;
}

Task *Delay::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new DelayTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Delay decorator task
//------------------------------------------------------------------------------
DelayTask::DelayTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner), 
     mCompleteMs(0) 
{
}

void DelayTask::onInitialize()
{
   Parent::onInitialize();
   Delay *nodeRep = static_cast<Delay*>(mNodeRep);
   mCompleteMs = Sim::getCurrentTime() + mRandI(nodeRep->mDelayMinMs, nodeRep->mDelayMaxMs);

   (*mCurrentChild)->setStatus(INVALID);
}

Task* DelayTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;
      
      return NULL;
   }

   if(Sim::getCurrentTime() < mCompleteMs)
   {
      mStatus = RUNNING;
      return NULL;
   }
   
   return (*mCurrentChild); 
}
      
void DelayTask::onChildComplete(Status s)
{
   mStatus = s;
   mIsComplete = true;
}

