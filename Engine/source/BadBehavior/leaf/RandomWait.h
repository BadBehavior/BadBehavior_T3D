#ifndef _BB_RANDOMWAIT_H_
#define _BB_RANDOMWAIT_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // RandomWait leaf
   // Pauses for a random period of time between delayMin and delayMax ms before completing.
   //---------------------------------------------------------------------------
   class RandomWait : public LeafNode
   {
      typedef LeafNode Parent;

   protected:
      static bool _setWaitMin(void *object, const char *index, const char *data);
      static bool _setWaitMax(void *object, const char *index, const char *data);

   public:
      S32 mWaitMinMs;
      S32 mWaitMaxMs;

      RandomWait();
   
      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      static void initPersistFields();

      DECLARE_CONOBJECT(RandomWait);
   };

   //---------------------------------------------------------------------------
   // RandomWait leaf task
   //---------------------------------------------------------------------------
   class RandomWaitTask : public Task
   {
      typedef Task Parent;

   protected:
      S32 mCompleteMs;

      virtual void onInitialize();
      virtual Task* update();
      
   public:
      RandomWaitTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);
   };

} // namespace BadBehavior

#endif