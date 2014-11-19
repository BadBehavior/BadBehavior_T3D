#ifndef _BB_SEQUENCE_H_
#define _BB_SEQUENCE_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Sequence Node
   //---------------------------------------------------------------------------
   class Sequence : public CompositeNode
   {
      typedef CompositeNode Parent;

   public:
      virtual Task* createTask();
      
      DECLARE_CONOBJECT(Sequence);
   };

   //---------------------------------------------------------------------------
   // Sequence Task
   //---------------------------------------------------------------------------
   class SequenceTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      virtual Task* update();
   
   public:
      SequenceTask(Node &node);

      virtual void onChildComplete(Status);
   };

} // namespace BadBehavior

#endif