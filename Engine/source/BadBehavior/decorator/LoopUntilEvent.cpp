#include "console/engineAPI.h"

#include "LoopUntilEvent.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Loop until event decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(LoopUntilEvent);

LoopUntilEvent::LoopUntilEvent()
   : mNumLoops(0) 
{
}

void LoopUntilEvent::initPersistFields()
{
   addGroup( "Behavior" );
   
   addProtectedField( "numLoops", TypeS32, Offset(mNumLoops, LoopUntilEvent), &_setNumLoops, &defaultProtectedGetFn,
      "The number of times to repeat the child behavior. 0 = infinite." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool LoopUntilEvent::_setNumLoops(void *object, const char *index, const char *data)
{
   LoopUntilEvent *node = static_cast<LoopUntilEvent *>( object );
   node->mNumLoops = getMax(0, dAtoi( data ));
   return false;
}

Task *LoopUntilEvent::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new LoopUntilEventTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Loop until event decorator task
//------------------------------------------------------------------------------
LoopUntilEventTask::LoopUntilEventTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner) 
   : Parent(node, owner, runner), 
     mCurrentLoop(0) 
{
}

void LoopUntilEventTask::onInitialize()
{
   Parent::onInitialize();
   mCurrentLoop = 0;
   (*mCurrentChild)->setStatus(INVALID);
}

Task* LoopUntilEventTask::update()
{
   if(mEventReceived)
   {
      mStatus = SUCCESS;
      mIsComplete = true;
      return NULL;
   }

   if( mIsComplete )
   {
      LoopUntilEvent *nodeRep = static_cast<LoopUntilEvent *>(mNodeRep);
      S32 numLoops = nodeRep->mNumLoops;
   
      if( (numLoops <= 0) || ( (numLoops > 0) && (++mCurrentLoop < numLoops) ) )
      {
         mIsComplete = false;
         mStatus = RUNNING;
      }

      return NULL;
   }
   
   return (*mCurrentChild); 
}

void LoopUntilEventTask::onChildComplete(Status s)
{
   mStatus = s;
   mIsComplete = true;
}
