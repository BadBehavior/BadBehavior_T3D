#ifndef _BB_DELAY_H_
#define _BB_DELAY_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Delay decorator
   // Pauses for a random period of time between delayMin and delayMax ms before executing its child.
   //---------------------------------------------------------------------------
   class Delay : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   protected:
      static bool _setDelayMin(void *object, const char *index, const char *data);
      
      static bool _setDelayMax(void *object, const char *index, const char *data);
  
   public:
      S32 mDelayMinMs;
      S32 mDelayMaxMs;

      Delay();
   
      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      static void initPersistFields();

      DECLARE_CONOBJECT(Delay);
   };

   //---------------------------------------------------------------------------
   // Delay decorator task
   //---------------------------------------------------------------------------
   class DelayTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      S32 mCompleteMs;

      virtual void onInitialize();
      virtual Task* update();
      
   public:
      DelayTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif