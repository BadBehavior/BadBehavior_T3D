#ifndef _BB_FAILALWAYS_H_
#define _BB_FAILALWAYS_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // FailAlways decorator
   // Returns FAILURE if child returns SUCCESS or FAILURE. RUNNING and INVALID are unchanged
   //---------------------------------------------------------------------------
   class FailAlways : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      virtual Task *createTask();
      
      DECLARE_CONOBJECT(FailAlways);
   };

   //---------------------------------------------------------------------------
   // FailAlways task
   //---------------------------------------------------------------------------
   class FailAlwaysTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      virtual Task* update();
      virtual void onInitialize();

   public:
      FailAlwaysTask(Node &node);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif