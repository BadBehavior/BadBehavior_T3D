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

#include "BadBehavior\core\Runner.h"
#include "WaitForSignal.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// WaitForSignal leaf node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(WaitForSignal);

WaitForSignal::WaitForSignal() 
{
}

void WaitForSignal::initPersistFields()
{
   addGroup( "Behavior" );
   
   addField( "signalName", TypeRealString, Offset(mSignalName, WaitForSignal),
      "The time in ms that the node should wait before completion." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *WaitForSignal::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new WaitForSignalTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Wait task
//------------------------------------------------------------------------------
WaitForSignalTask::WaitForSignalTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner)
{
}

WaitForSignalTask::~WaitForSignalTask()
{
   unsubscribe();
}

void WaitForSignalTask::subscribe()
{
   mRunner->subscribeToSignal(static_cast<WaitForSignal *>(mNodeRep)->getSignalName().c_str(), this);
}

void WaitForSignalTask::unsubscribe()
{
   mRunner->unsubscribeFromSignal(static_cast<WaitForSignal *>(mNodeRep)->getSignalName().c_str(), this);
}

void WaitForSignalTask::onSignal()
{
   onResume();
}

void WaitForSignalTask::onInitialize()
{
   Parent::onInitialize();
   subscribe();
}

void WaitForSignalTask::onTerminate()
{
   Parent::onTerminate();
   unsubscribe();
}

Task* WaitForSignalTask::update() 
{ 
   if(mStatus == RESUME)
   {
      mStatus = SUCCESS;
      mIsComplete = true;
   }
   else if (mStatus == INVALID)
   {
      mStatus = SUSPENDED;
   }

   return NULL; 
}
