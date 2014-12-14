#include "console/engineAPI.h"
#include "ActiveSelector.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Active selector node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(ActiveSelector);

Task *ActiveSelector::createTask()
{
   return new ActiveSelectorTask(*this);
}

//------------------------------------------------------------------------------
// Active selector task
//------------------------------------------------------------------------------
ActiveSelectorTask::ActiveSelectorTask(Node &node)
   : Parent(node) 
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
      (*mCurrentChild)->setStatus(INVALID);
   
   return (*mCurrentChild);   
}

