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
#include "ActiveSelector.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Active selector node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(ActiveSelector);

Task *ActiveSelector::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new ActiveSelectorTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Active selector task
//------------------------------------------------------------------------------
ActiveSelectorTask::ActiveSelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

void ActiveSelectorTask::onChildComplete(Status s)
{
   mStatus = s;

   if(mStatus == FAILURE)
      ++mCurrentChild;
   else
   {
      if(mStatus == RUNNING)
         mRunningChild = mCurrentChild;
      
      mIsComplete = true;
   }
}

Task* ActiveSelectorTask::update()
{
   if(mCurrentChild == mChildren.end())
   {
      mIsComplete = true;
   }

   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;
      else
         mRunningChild = mChildren.end();

      mCurrentChild = mChildren.begin();

      return NULL;
   }

   // return child
   if(mCurrentChild != mRunningChild)
      (*mCurrentChild)->reset();
   
   return (*mCurrentChild);   
}

