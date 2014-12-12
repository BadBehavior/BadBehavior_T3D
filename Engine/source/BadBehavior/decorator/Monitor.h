#ifndef _BB_MONITOR_H_
#define _BB_MONITOR_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Monitor decorator
   // outputs the return status to the console,
   //---------------------------------------------------------------------------
   class Monitor : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      virtual Task *createTask();
      
      DECLARE_CONOBJECT(Monitor);
   };

   //---------------------------------------------------------------------------
   // Monitor decorator task
   //---------------------------------------------------------------------------
   class MonitorTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      virtual Task* update();
      virtual void onInitialize();

   public:
      MonitorTask(Node &node);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif