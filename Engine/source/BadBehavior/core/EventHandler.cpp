#include "core/stringTable.h"
//#include "console/simEvents.h"

#include "BadBehavior/core/Core.h"
#include "EventHandler.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// BehaviorTreeEvent 
//------------------------------------------------------------------------------

BehaviorTreeEvent::BehaviorTreeEvent(EventListenerTask *task)
   : mTask(task) 
{
}

//------------------------------------------------------------------------------
// BehaviorTreeEventHandler
//------------------------------------------------------------------------------

BehaviorTreeEventHandler::BehaviorTreeEventHandler()
{
   mEventQueue = new VectorPtr<BehaviorTreeEvent*>;
   mPendingEventQueue = new VectorPtr<BehaviorTreeEvent*>;
}

// clear out the pending queue on deletion
BehaviorTreeEventHandler::~BehaviorTreeEventHandler()
{
   BehaviorTreeEvent *evt = NULL;
   while(mEventQueue->size())
   {
      evt = mEventQueue->back();
      mEventQueue->pop_back();
      delete evt;
      evt = NULL;
   }

   while(mPendingEventQueue->size())
   {
      evt = mPendingEventQueue->back();
      mPendingEventQueue->pop_back();
      delete evt;
      evt = NULL;
   }

   delete mEventQueue;
   delete mPendingEventQueue;
}

// check if the specified event is registered
bool BehaviorTreeEventHandler::isEventRegistered(const char *event_)
{
   StringTableEntry eventName = StringTable->insert( event_ );
   return mEvents.contains(eventName);
}

// register and event to be handled
void BehaviorTreeEventHandler::registerEvent(const char *event_)
{
   // check event_ has a name
   if(!event_ || !event_[0])
      return;

   // Make sure the event has not been registered yet.
   if(!isEventRegistered( event_ ) )
   {
      // Add to the event list.
      mEvents.push_back( StringTable->insert( event_ ) );

      // Create a list of subscribers for this event.
      mSubscribers.insert( new VectorPtr<EventListenerTask*>, event_ );
   }
}

// register a subscriber to an event
void BehaviorTreeEventHandler::registerSubscriber(const char *event_, EventListenerTask *subscriber)
{
   // must have a subscriber
   if(!subscriber)
      return;

   // and an event name
   if(!event_ || !event_[0])
      return;

   // register a new event if this one hasn't already been registered
   if(!isEventRegistered(event_))
      registerEvent(event_);

   // add the subscriber if it's not already registered for this event
   VectorPtr<EventListenerTask*>* subscribers = mSubscribers.retreive( event_ );

   if(!subscribers->contains(subscriber))
      subscribers->push_back(subscriber);
}

// unregister a subscriber from an event
void BehaviorTreeEventHandler::unregisterSubscriber(const char *event_, EventListenerTask *subscriber)
{
   // check if the subscriber exists and that the event is actually registered
   if(!subscriber || !isEventRegistered(event_))
      return;

   // find the subscriber and remove it
   VectorPtr<EventListenerTask*>* subscribers = mSubscribers.retreive( event_ );

   for( VectorPtr<EventListenerTask *>::iterator iter = subscribers->begin(); iter != subscribers->end(); ++iter )
   {
      if( *iter == subscriber )
      {
         subscribers->erase_fast( iter );
         break;
      }
   }
}

// post an event
void BehaviorTreeEventHandler::postEvent(const char *event_)
{
   // event must be registered
   if(!isEventRegistered(event_))
      return;

   // dispatch a SimEvent to each of the subscribers to this event
   VectorPtr<EventListenerTask*>* subscribers = mSubscribers.retreive( event_ );

   for( VectorPtr<EventListenerTask *>::iterator iter = subscribers->begin(); iter != subscribers->end(); ++iter )
   {
      mPendingEventQueue->push_front(new BehaviorTreeEvent((*iter)));
   }
}

// dispatch queued events
void BehaviorTreeEventHandler::dispatchEvents()
{
   if(!mEventQueue->size() && !mPendingEventQueue->size())
      return;

   BehaviorTreeEvent *evt = NULL;

   while(mEventQueue->size())
   {
      evt = mEventQueue->back();
      mEventQueue->pop_back();
      evt->mTask->onEvent();
      evt = NULL;
   }

   // swap the queues
   VectorPtr<BehaviorTreeEvent*> *tmp = mEventQueue;
   mEventQueue = mPendingEventQueue;
   mPendingEventQueue = tmp;
}