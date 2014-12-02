#include "console/engineAPI.h"

#include "Parallel.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Parallel node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Parallel);

Parallel::Parallel() 
   : mReturnPolicy(REQUIRE_ALL) 
{
}

ImplementEnumType( ParallelReturnPolicy,
   "@brief The policy to use when determining the return status of a parallel node.\n\n"
   "@ingroup AI\n\n")
   { Parallel::REQUIRE_ALL,  "REQUIRE_ALL",  "Will return success if all children succeed.\n"
                                             "Will terminate and return failure if one child fails \n"},
   { Parallel::REQUIRE_NONE, "REQUIRE_NONE", "Will return success even if all children fail.\n" },
   { Parallel::REQUIRE_ONE,  "REQUIRE_ONE",  "Will terminate and return success when one child succeeds.\n" },
EndImplementEnumType;

void Parallel::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "returnPolicy", TYPEID< Parallel::ParallelPolicy >(), Offset(mReturnPolicy, Parallel),
      "@brief The policy to use when deciding the return status for the parallel sequence.");

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *Parallel::createTask()
{
   return new ParallelTask(*this);
}

//------------------------------------------------------------------------------
// Parallel Task
//------------------------------------------------------------------------------
ParallelTask::ParallelTask(Node &node)
   : Parent(node),
     mHasFailure(false),
     mHasSuccess(false),
     mHasRunning(false) 
{
}

void ParallelTask::onInitialize()
{
   mHasFailure = mHasSuccess = mHasRunning = false;
   Parent::onInitialize();
   for (VectorPtr<Task*>::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
      (*i)->setStatus(INVALID);
}


Task* ParallelTask::pickChild()
{
   while(mCurrentChild != mChildren.end())
   {
      if((*mCurrentChild)->getStatus() == RUNNING || (*mCurrentChild)->getStatus() == INVALID)
         return (*mCurrentChild);

      ++mCurrentChild;
   }
   return NULL;
}

void ParallelTask::onChildComplete(Status s)
{
   if(s == SUCCESS)
      mHasSuccess = true;
   else if(s == FAILURE)
      mHasFailure = true;
   else if(s == RUNNING)
      mHasRunning = true;

   ++mCurrentChild;
   pickChild();
}

Task* ParallelTask::update()
{
   bool atEnd = mCurrentChild == mChildren.end();
   
   if(atEnd)
   {
      mCurrentChild = mChildren.begin();

      switch(static_cast<Parallel *>(mNodeRep)->mReturnPolicy)
      {
      // REQUIRE_NONE
      // returns SUCCESS when all children have finished irrespective of their return status.
      case Parallel::REQUIRE_NONE:
         mStatus = mHasRunning ? RUNNING : SUCCESS;
         break;
      
      // REQUIRE_ONE
      // terminates and returns SUCCESS when any of its children succeed
      // returns FAILURE if no children succeed
      case Parallel::REQUIRE_ONE:
         mStatus = mHasSuccess ? SUCCESS : (mHasRunning ? RUNNING : FAILURE);
         break;

      // REQUIRE_ALL
      // returns SUCCESS if all of its children succeed.
      // terminates and returns failure if any of its children fail
      case Parallel::REQUIRE_ALL:
         mStatus = mHasFailure ? FAILURE : (mHasRunning ? RUNNING : SUCCESS);
         break;
      }

      if(!mHasRunning)
         mIsComplete = true;
      
      mHasRunning = false;
      return NULL;
   }

   return pickChild();
}
