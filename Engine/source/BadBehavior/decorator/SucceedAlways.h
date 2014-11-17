#ifndef _BB_SUCCEEDALWAYS_H_
#define _BB_SUCCEEDALWAYS_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // SucceedAlways decorator
   // Returns SUCCESS if child returns SUCCESS or FAILURE. RUNNING and INVALID are unchanged
   //---------------------------------------------------------------------------
   class SucceedAlways : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      DECLARE_CONOBJECT(SucceedAlways);
   };

   //---------------------------------------------------------------------------
   // SucceedAlways task
   //---------------------------------------------------------------------------
   class SucceedAlwaysTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      virtual Task* update();
      virtual void onInitialize();

   public:
      SucceedAlwaysTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif