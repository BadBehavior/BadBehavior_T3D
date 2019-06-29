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

#include "Branch.h"
#include "Stepper.h"

using namespace BadBehavior;

BehaviorTreeBranch::BehaviorTreeBranch() 
   : mRootTask(NULL) 
{
}

BehaviorTreeBranch::BehaviorTreeBranch(Task *root) 
   : mRootTask(root), 
     mStatus(INVALID) 
{
}
         
Status BehaviorTreeBranch::getStatus() 
{
   if(!mTasks.empty())
      return mTasks.back()->getStatus();
            
   return mStatus;
}

Status BehaviorTreeBranch::update() 
{
   if(mRootTask)
   {
      if(mTasks.empty())
      {
         mRootTask->setup();
         mTasks.push_back(mRootTask);
      }
   }
   mStatus = BehaviorTreeStepper::stepThrough(mTasks);
   return mStatus;
}

void BehaviorTreeBranch::reset()
{
   mStatus = INVALID;
   mRootTask->reset();
   mTasks.clear();
}
