#ifndef _BB_LOOPUNTILEVENT_H_
#define _BB_LOOPUNTILEVENT_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // loop until event decorator
   // repeats the child behaviour for n times, or until it receives an event
   // returns RUNNING until it completes
   //---------------------------------------------------------------------------
   class LoopUntilEvent : public EventListenerNode
   {
      typedef EventListenerNode Parent;

   protected:
      static bool _setNumLoops(void *object, const char *index, const char *data);

   public:
      S32 mNumLoops;
      
      LoopUntilEvent();

      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      static void initPersistFields();

      DECLARE_CONOBJECT(LoopUntilEvent);
   };

   //---------------------------------------------------------------------------
   // loop  until event task
   //---------------------------------------------------------------------------
   class LoopUntilEventTask : public EventListenerTask
   {
      typedef EventListenerTask Parent;

   protected:
      S32 mCurrentLoop;
   
      virtual void onInitialize();
      virtual Task *update();

   public:
      LoopUntilEventTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status s);
   };
   
} // namespace BadBehavior

#endif