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

#include "ActiveSelector.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Active selector node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(ActiveSelector);

ActiveSelector::ActiveSelector()
   //:  mRecheckFrequency(0)
{
}

Task *ActiveSelector::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new ActiveSelectorTask(*this, owner, runner);
}

void ActiveSelector::initPersistFields()
{
   //addGroup( "Behavior" );

   //addField( "recheckFrequency", TypeS32, Offset(mRecheckFrequency, ActiveSelector),
   //   "@brief The minimum time period in milliseconds between re-evaluations of higher priority branches.");

   //endGroup( "Behavior" );

   Parent::initPersistFields();
}

//------------------------------------------------------------------------------
// Active selector task
//------------------------------------------------------------------------------
ActiveSelectorTask::ActiveSelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner) 
{
}

void ActiveSelectorTask::onInitialize()
{
   Parent::onInitialize();

   if(mBranches.empty())
   {
      for (VectorPtr<Task*>::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
      {
         mBranches.push_back(BehaviorTreeBranch(*i));
      }
   }
   else
   {
      for (Vector<BehaviorTreeBranch>::iterator it = mBranches.begin(); it != mBranches.end(); ++it)
      {
         it->reset();
      }
   }

   mCurrentBranch = mBranches.begin();
   mRunningBranch = mBranches.end();
}


Task* ActiveSelectorTask::update()
{
   if(mBranches.empty())
   {
      mStatus = INVALID;
      return NULL;
   }

   for(mCurrentBranch = mBranches.begin(); mCurrentBranch != mBranches.end(); ++mCurrentBranch)
   {
      mStatus = mCurrentBranch->update();

      if(mStatus == FAILURE)
         continue;
      
      break;
   }

   if( (mStatus != RUNNING && mStatus != SUSPENDED) || mCurrentBranch == mBranches.end() )
      mIsComplete = true;

   return NULL;
   //if(mCurrentChild == mChildren.end())
   //{
   //   mIsComplete = true;
   //}

   //if( mIsComplete )
   //{
   //   if(mStatus == RUNNING || mStatus == SUSPENDED)
   //      mIsComplete = false;
   //   else
   //      mRunningChild = mChildren.end();

   //   mCurrentChild = mChildren.begin();

   //   return NULL;
   //}

   //// return child
   //if(mCurrentChild != mRunningChild)
   //   (*mCurrentChild)->reset();
   //
   //return (*mCurrentChild);   
}

Status ActiveSelectorTask::getStatus()
{
   if(mCurrentBranch != mBranches.end())
      return mCurrentBranch->getStatus();

   return mStatus;
}




//void ActiveSelectorTask::onChildComplete(Status s)
//{
//   mStatus = s;
//
//   if(mStatus == FAILURE)
//      ++mCurrentChild;
//   else
//   {
//      if(mStatus == RUNNING)
//         mRunningChild = mCurrentChild;
//      
//      mIsComplete = true;
//   }
//}