#ifndef _BB_ACTIVESELECTOR_H_
#define _BB_ACTIVESELECTOR_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Active selector Node
   //---------------------------------------------------------------------------
   class ActiveSelector : public CompositeNode
   {
      typedef CompositeNode Parent;
         
   public:
      virtual Task *createTask();

      DECLARE_CONOBJECT(ActiveSelector);
   };

   //---------------------------------------------------------------------------
   // Active selector Task
   //---------------------------------------------------------------------------
   class ActiveSelectorTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      VectorPtr<Task*>::iterator mRunningChild;
      
      virtual Task* update();
   
   public:
      ActiveSelectorTask(Node &node);

      virtual void onChildComplete(Status);
   };

} // namespace BadBehavior

#endif