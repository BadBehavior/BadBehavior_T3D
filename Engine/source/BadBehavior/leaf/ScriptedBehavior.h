#ifndef _BB_SCRIPTEDBEHAVIOR_H_
#define _BB_SCRIPTEDBEHAVIOR_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   // specify when the precondition function should be executed
   enum PreconditionMode
   {
      ONCE,    // the first time the behavior is evaluated
      TICK     // every tick
   };

   //---------------------------------------------------------------------------
   // ScriptedBehavior - A structured behavior leaf node which defines a series
   // Of scripted callbacks
   //---------------------------------------------------------------------------
   class ScriptedBehavior : public LeafNode
   {
      typedef LeafNode Parent;
   
   public:

      // status to return if the beahvior does not return a value
      Status mDefaultReturnStatus;

      // how often should we valuate the precondition
      PreconditionMode mPreconditionMode;

      ScriptedBehavior();

      virtual Task *createTask();
      
      static void initPersistFields();

      DECLARE_CONOBJECT(ScriptedBehavior);
   };

   //---------------------------------------------------------------------------
   // ScriptedBehavior task
   //---------------------------------------------------------------------------
   class ScriptedBehaviorTask : public Task
   {
      typedef Task Parent;
   
   protected:
      virtual Task* update();
      virtual void onTerminate();

   public:
      ScriptedBehaviorTask(Node &node);
   };

} // namespace BadBehavior

// make the return status enum accessible from script
typedef BadBehavior::PreconditionMode BehaviorPreconditionType;
DefineEnumType( BehaviorPreconditionType );

#endif