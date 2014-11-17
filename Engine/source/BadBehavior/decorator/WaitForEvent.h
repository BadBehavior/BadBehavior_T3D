#ifndef _BB_WAITFOREVENT_H_
#define _BB_WAITFOREVENT_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Wait for event decorator
   // Pauses until the require event is received.
   //---------------------------------------------------------------------------
   class WaitForEvent : public EventListenerNode
   {
      typedef DecoratorNode Parent;

   public:
      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      DECLARE_CONOBJECT(WaitForEvent);
   };

   //---------------------------------------------------------------------------
   // WaitForEvent decorator task
   //---------------------------------------------------------------------------
   class WaitForEventTask : public EventListenerTask
   {
      typedef EventListenerTask Parent;

   protected:
      virtual void onInitialize();
      virtual Task* update();
      
   public:
      WaitForEventTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif