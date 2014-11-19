#ifndef _BB_PRIORITYSELECTOR_H_
#define _BB_PRIORITYSELECTOR_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Priority selector Node
   //---------------------------------------------------------------------------
   class PrioritySelector : public CompositeNode
   {
      typedef CompositeNode Parent;
         
   public:
      virtual Task *createTask();

      DECLARE_CONOBJECT(PrioritySelector);
   };

   //---------------------------------------------------------------------------
   // Priority selector Task
   //---------------------------------------------------------------------------
   class PrioritySelectorTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      virtual Task* update();
   
   public:
      PrioritySelectorTask(Node &node);

      virtual void onChildComplete(Status);
   };

} // namespace BadBehavior

#endif