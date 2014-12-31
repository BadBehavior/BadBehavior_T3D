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
#include "PrioritySelector.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Priority selector node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(PrioritySelector);

Task *PrioritySelector::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new PrioritySelectorTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Priority selector task
//------------------------------------------------------------------------------
PrioritySelectorTask::PrioritySelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

void PrioritySelectorTask::onChildComplete(Status s)
{
   mStatus = s;

   if(s == FAILURE)
      ++mCurrentChild;
   else
      mIsComplete = true;
}

Task* PrioritySelectorTask::update()
{
   if(mCurrentChild == mChildren.end())
   {
      mIsComplete = true;
   }

   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;

      return NULL;
   }

   // move on to next child
   if(mStatus != RUNNING)
      (*mCurrentChild)->reset();
   
   return (*mCurrentChild);   
}

