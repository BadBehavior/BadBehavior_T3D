//-----------------------------------------------------------------------------
// Copyright (c) 2014 Guy Allard
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

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

Task *Parallel::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new ParallelTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Parallel Task
//------------------------------------------------------------------------------
ParallelTask::ParallelTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner),
     mHasFailure(false),
     mHasSuccess(false),
     mHasRunning(false),
     mHasSuspended(false)
{
}

void ParallelTask::onInitialize()
{
   mHasFailure = mHasSuccess = mHasRunning = mHasSuspended = false;
   Parent::onInitialize();
   for (VectorPtr<Task*>::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
      (*i)->reset();
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
   else if(s == SUSPENDED)
      mHasSuspended = true;

   ++mCurrentChild;
   pickChild();
}

Task* ParallelTask::update()
{
   bool atEnd = mCurrentChild == mChildren.end();
   
   if(atEnd)
   {
      mCurrentChild = mChildren.begin();

      switch(static_cast<Parallel *>(mNodeRep)->getReturnPolicy())
      {
      // REQUIRE_NONE
      // returns SUCCESS when all children have finished irrespective of their return status.
      case Parallel::REQUIRE_NONE:
         mStatus = mHasRunning ? RUNNING : (mHasSuspended ? SUSPENDED : SUCCESS);
         break;
      
      // REQUIRE_ONE
      // terminates and returns SUCCESS when any of its children succeed
      // returns FAILURE if no children succeed
      case Parallel::REQUIRE_ONE:
         mStatus = mHasSuccess ? SUCCESS : (mHasRunning ? RUNNING : (mHasSuspended ? SUSPENDED : FAILURE));
         break;

      // REQUIRE_ALL
      // returns SUCCESS if all of its children succeed.
      // terminates and returns failure if any of its children fail
      case Parallel::REQUIRE_ALL:
         mStatus = mHasFailure ? FAILURE : (mHasRunning ? RUNNING : (mHasSuspended ? SUSPENDED : SUCCESS));
         break;
      }

      if(mStatus != RUNNING && mStatus != SUSPENDED)
         mIsComplete = true;
      
      mHasRunning = false;
      mHasSuspended = false;
      return NULL;
   }

   return pickChild();
}
