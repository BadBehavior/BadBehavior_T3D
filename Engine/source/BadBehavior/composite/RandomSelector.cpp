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

#include "console/engineAPI.h"
#include "math/mMathFn.h"

#include "RandomSelector.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Random selector node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(RandomSelector);

Task *RandomSelector::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new RandomSelectorTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Random selector task
//------------------------------------------------------------------------------
RandomSelectorTask::RandomSelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

void RandomSelectorTask::onInitialize()
{
   Parent::onInitialize();

   mChildSelection.clear();

   // populate the list of child indices
   for(U32 i=0; i < mChildren.size(); i++)
      mChildSelection.push_back(i);

   pickRandomChild();
}

void RandomSelectorTask::pickRandomChild()
{
   if(mChildSelection.size() > 0)
   {
      U32 index = mRandI(0, mChildSelection.size() - 1);
      U32 child = mChildSelection[index];
      mChildSelection.erase(index, 1);
      mCurrentChild = mChildren.begin() + child;
   }
   else
      mCurrentChild = mChildren.end(); 
}

Task* RandomSelectorTask::update()
{
   if(mCurrentChild == mChildren.end())
   {
      mIsComplete = true;
   }
   
   if( mIsComplete )
   {
      if(mStatus == RUNNING || mStatus == SUSPENDED)
         mIsComplete = false;

      return NULL;
   }

   if(mStatus != RUNNING && mStatus != SUSPENDED)
      (*mCurrentChild)->reset();
   
   // return child
   return (*mCurrentChild);   
}

void RandomSelectorTask::onChildComplete(Status s)
{
   mStatus = s;

   if(s == FAILURE)
      pickRandomChild();
   else
      mIsComplete = true;
}
