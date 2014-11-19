#ifndef _BB_INVERTER_H_
#define _BB_INVERTER_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // inverter decorator
   // invert the return value of the child,
   // SUCCESS becomes FAILURE, FAILURE becomes SUCCESS, INVALID and RUNNING are unmodified
   //---------------------------------------------------------------------------
   class Inverter : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      virtual Task *createTask();
      
      DECLARE_CONOBJECT(Inverter);
   };

   //---------------------------------------------------------------------------
   // inverter decorator task
   //---------------------------------------------------------------------------
   class InverterTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      virtual Task* update();
      virtual void onInitialize();

   public:
      InverterTask(Node &node);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif