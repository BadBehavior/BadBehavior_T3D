#include "console/engineAPI.h"

#include "Ticker.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Ticker decorator node
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
      "The time to wait between executions of this nodes child." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool Ticker::_setFrequency(void *object, const char *index, const char *data)
{
   Ticker *node = static_cast<Ticker *>( object );
   node->mFrequencyMs = getMax(0, dAtoi( data ));
   return false;
}

Task *Ticker::createTask()
{
   return new TickerTask(*this);
}

//------------------------------------------------------------------------------
// Ticker decorator task
//------------------------------------------------------------------------------
TickerTask::TickerTask(Node &node)
   : Parent(node), 
     mNextTimeMs(0) 
{
}

void TickerTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->setStatus(INVALID);
}

Task* TickerTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;
      
      return NULL;
   }

   if(Sim::getCurrentTime() < mNextTimeMs)
   {
      mStatus = RUNNING;
      return NULL;
   }
   
   mNextTimeMs = Sim::getCurrentTime() + static_cast<Ticker*>(mNodeRep)->mFrequencyMs;
   return (*mCurrentChild); 
}
      
void TickerTask::onChildComplete(Status s)
{
   mStatus = s;
   mIsComplete = true;
}

