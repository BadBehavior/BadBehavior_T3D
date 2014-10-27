#ifndef _BB_LOGGER_H_
#define _BB_LOGGER_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // logger decorator
   // outputs the return status to the console,
   //---------------------------------------------------------------------------
   class Logger : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      DECLARE_CONOBJECT(Logger);
   };

   //---------------------------------------------------------------------------
   // logger decorator task
   //---------------------------------------------------------------------------
   class LoggerTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      virtual Task* update();
      virtual void onInitialize();

   public:
      LoggerTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif