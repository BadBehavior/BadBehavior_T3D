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
#include "Ticker.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Ticker decorator node
// **** EXPERIMENTAL ****
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Ticker);

Ticker::Ticker()
   : mFrequencyMs(100)
{
}

void Ticker::initPersistFields()
{
   addGroup( "Behavior" );
   
   addProtectedField( "frequencyMs", TypeS32, Offset(mFrequencyMs, Ticker), &_setFrequency, &defaultProtectedGetFn,
      "The time to wait between evaluations of this nodes child." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool Ticker::_setFrequency(void *object, const char *index, const char *data)
{
   Ticker *node = static_cast<Ticker *>( object );
   node->mFrequencyMs = getMax(0, dAtoi( data ));
   return false;
}

Task *Ticker::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new TickerTask(*this, owner, runner);
}

//------------------------------------------------------------------------------
// Ticker decorator task
//------------------------------------------------------------------------------
TickerTask::TickerTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner), 
     mNextTimeMs(0),
     mTickEvent(0)
{
}

Task* TickerTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING || mStatus == SUSPENDED)
         mIsComplete = false;
      
      return NULL;
   }

   Ticker *node = static_cast<Ticker *>(mNodeRep);

   if(Sim::getCurrentTime() < mNextTimeMs)
   {
      if(!mIsComplete && mStatus != SUSPENDED)
      {
         mStatus = SUSPENDED;
         mTickEvent = Sim::postEvent(mRunner, new TaskReactivateEvent(*this), mNextTimeMs);
      }

      return NULL;
   }
   
   mNextTimeMs = Sim::getCurrentTime() + node->getFrequencyMs();
   return mStatus != SUSPENDED ? mChild : NULL; 
}
      
void TickerTask::onResume()
{
   if(Sim::isEventPending(mTickEvent))
      mStatus = SUSPENDED;
   else
      mStatus = RUNNING;

   Con::warnf("Resumed %s (%s)", 
               mNodeRep->getIdString(),
               EngineMarshallData(mStatus));
}