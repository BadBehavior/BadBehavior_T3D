#ifndef _BB_LOOP_H_
#define _BB_LOOP_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // loop decorator
   // repeats the child behaviour for n times, or until it fails
   // returns RUNNING until it completes
   //---------------------------------------------------------------------------
   class Loop : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      // loop termination policies
      enum TerminationPolicy
      {
         ON_FAILURE,
         ON_SUCCESS,
      };

   protected:
      static bool _setNumLoops(void *object, const char *index, const char *data);

   public:
      S32 mNumLoops;
      TerminationPolicy mTerminationPolicy;

      Loop();

      virtual Task *createTask();
      
      static void initPersistFields();

      DECLARE_CONOBJECT(Loop);
   };

   //---------------------------------------------------------------------------
   // loop task
   //---------------------------------------------------------------------------
   class LoopTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      S32 mCurrentLoop;
   
      virtual void onInitialize();
      virtual Task *update();

   public:
      LoopTask(Node &node);

      virtual void onChildComplete(Status s);
   };
   
} // namespace BadBehavior

// make the loop termination policy enum accessible from script
typedef BadBehavior::Loop::TerminationPolicy LoopTerminationPolicy;
DefineEnumType( LoopTerminationPolicy );

#endif