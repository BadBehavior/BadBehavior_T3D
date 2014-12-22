#include "console/engineAPI.h"

#include "Loop.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Loop decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Loop);

ImplementEnumType( LoopTerminationPolicy,
   "@brief The policy to use when determining if the loop should terminate early.\n\n"
   "@ingroup AI\n\n")
   { Loop::ON_FAILURE,  "ON_FAILURE",  "Will terminate and return FAILURE if child fails.\n" },
   { Loop::ON_SUCCESS,  "ON_SUCCESS",  "Will terminate and return SUCCESS if child succeeds.\n" },
EndImplementEnumType;

Loop::Loop()
   : mNumLoops(0), 
     mTerminationPolicy(ON_FAILURE) 
{
}

void Loop::initPersistFields()
{
   addGroup( "Behavior" );
   
   addProtectedField( "numLoops", TypeS32, Offset(mNumLoops, Loop), &_setNumLoops, &defaultProtectedGetFn,
      "The number of times to repeat the child behavior. 0 = infinite." );

   addField( "terminationPolicy", TYPEID< Loop::TerminationPolicy >(), Offset(mTerminationPolicy, Loop),
      "@brief The policy to use when deciding if the loop should terminate before completion.");
   
   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool Loop::_setNumLoops(void *object, const char *index, const char *data)
{
   Loop *node = static_cast<Loop *>( object );
   node->mNumLoops = getMax(0, dAtoi( data ));
   return false;
}

Task *Loop::createTask()
{
   return new LoopTask(*this);
}

//------------------------------------------------------------------------------
// Loop decorator task
//------------------------------------------------------------------------------
LoopTask::LoopTask(Node &node)
   : Parent(node), 
     mCurrentLoop(0)
{
}

void LoopTask::onInitialize()
{
   Parent::onInitialize();
   mCurrentLoop = 0;
   (*mCurrentChild)->reset();
}

Task* LoopTask::update()
{
   if( mIsComplete )
   {
      Loop *nodeRep = static_cast<Loop *>(mNodeRep);
      Loop::TerminationPolicy policy = nodeRep->mTerminationPolicy;
      S32 numLoops = nodeRep->mNumLoops;
   
      if( ((policy == Loop::ON_FAILURE) && (mStatus != FAILURE)) ||
          ((policy == Loop::ON_SUCCESS) && (mStatus != SUCCESS)) )
      {
         if ( (++mCurrentLoop < numLoops) || (numLoops == 0) )
         {
            mIsComplete = false;
            mStatus = RUNNING;
         }
      }
      
      return NULL;
   }
   
   return (*mCurrentChild); 
}

void LoopTask::onChildComplete(Status s)
{
   mStatus = s;
   mIsComplete = true;
}
