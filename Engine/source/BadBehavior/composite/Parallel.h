#ifndef _BB_PARALLEL_H_
#define _BB_PARALLEL_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Parallel sequence node
   // Runs all of its children irrespective of their return status
   // The final return status depends on the chosen policy
   // (not a true parallel, as branches are actually evaluated sequentially)
   //---------------------------------------------------------------------------
   class Parallel: public CompositeNode
   {
      typedef CompositeNode Parent;

   public:
      // parallel return policies
      enum ParallelPolicy
      {
         REQUIRE_NONE,
         REQUIRE_ONE,
         REQUIRE_ALL,
      };
      
      ParallelPolicy mReturnPolicy;

      Parallel();

      virtual Task *createTask();

      static void initPersistFields();
      
      DECLARE_CONOBJECT(Parallel);
   };

   //---------------------------------------------------------------------------
   // Parallel Task
   //---------------------------------------------------------------------------
   class ParallelTask: public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      bool mHasFailure;
      bool mHasSuccess;
      bool mHasRunning;
      
      virtual void onInitialize();
      virtual Task* update();
      Task *pickChild();

   public:
      ParallelTask(Node &node);

      virtual void onChildComplete(Status);
   };

} // namespace BadBehavior

// make the return policy enum accessible from script
typedef BadBehavior::Parallel::ParallelPolicy ParallelReturnPolicy;
DefineEnumType( ParallelReturnPolicy );

#endif