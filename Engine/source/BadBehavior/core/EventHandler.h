#ifndef _BB_EVENTHANDLER_H_
#define _BB_EVENTHANDLER_H_

#ifndef _TVECTOR_H_
#include "core/util/tVector.h"
#endif
#ifndef _TSIMPLEHASHTABLE_H
#include "core/tSimpleHashTable.h"
#endif


namespace BadBehavior
{
   class EventListenerTask;
   
   //------------------------------------------------------------------------------
   // BehaviorTreeEvent 
   //------------------------------------------------------------------------------
   class BehaviorTreeEvent
   {
   public:
      EventListenerTask *mTask;
      BehaviorTreeEvent(EventListenerTask *task);
   };

   //---------------------------------------------------------------------------
   // BehaviorTreeEventHandler
   // Simple event handler to pass events to tree node listeners
   //---------------------------------------------------------------------------
   class BehaviorTreeEventHandler
   {
   private:
      SimpleHashTable<VectorPtr<EventListenerTask*>> mSubscribers;
      Vector<StringTableEntry> mEvents;
      
      VectorPtr<BehaviorTreeEvent*> *mEventQueue;
      VectorPtr<BehaviorTreeEvent*> *mPendingEventQueue;

      void registerEvent(const char *event_);
      bool isEventRegistered(const char *event_);

   public:
      BehaviorTreeEventHandler();
      ~BehaviorTreeEventHandler();

      // register a scubscriber to an event
      void registerSubscriber(const char *event_, EventListenerTask *subscriber);

      // unregister a subscriber from an event
      void unregisterSubscriber(const char *event_, EventListenerTask *subscriber);

      // post an event
      void postEvent(const char *event_);

      // dispatch queued events
      void dispatchEvents();
   };

} // namespace BadBehavior
#endif