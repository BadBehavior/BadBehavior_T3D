#ifndef _BB_RANDOMSELECTOR_H_
#define _BB_RANDOMSELECTOR_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Random selector node
   // selects its children in a random order until one of them succeeds
   //---------------------------------------------------------------------------
   class RandomSelector : public CompositeNode
   {
      typedef CompositeNode Parent;

   public:
      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      DECLARE_CONOBJECT(RandomSelector);
   };

   //---------------------------------------------------------------------------
   // Random selector task
   //---------------------------------------------------------------------------
   class RandomSelectorTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected: 
      Vector<U32> mChildSelection; // list of child indices to pick from

      virtual Task* update();
      virtual void onInitialize();
      
      void pickRandomChild();
      
   public:
      RandomSelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status);
   };

} // namespace BadBehavior

#endif