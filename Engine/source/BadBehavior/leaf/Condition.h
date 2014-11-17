#ifndef _BB_CONDITION_H_
#define _BB_CONDITION_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Condition - evaluates a short comparison statement when run
   //---------------------------------------------------------------------------
   class Condition : public LeafNode
   {
      typedef LeafNode Parent;
   
   protected:
      // condition function to be evaluated
      String mConditionFunction;
  
   public:

      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      // do the evaluation
      Status evaluate(SimObject *owner);

      static void initPersistFields();

      DECLARE_CONOBJECT(Condition);
   };

   //---------------------------------------------------------------------------
   // Condition task
   //---------------------------------------------------------------------------
   class ConditionTask : public Task
   {
      typedef Task Parent;
   
   protected:
      virtual Task* update();
      
   public:
      ConditionTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);
   };

} // namespace BadBehavior
#endif