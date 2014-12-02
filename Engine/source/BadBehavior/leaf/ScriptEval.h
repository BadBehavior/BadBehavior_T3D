#ifndef _BB_SCRIPTEVAL_H_
#define _BB_SCRIPTEVAL_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // ScriptEval - evaluates a set of Torque Script commands when run
   // Warning - slow, handle with care!
   //---------------------------------------------------------------------------
   class ScriptEval : public LeafNode
   {
      typedef LeafNode Parent;
   
   public:

      // status to return if the command does not return a value
      Status mDefaultReturnStatus;

      // the torque script to evaluate
      String mBehaviorScript;

      ScriptEval();

      virtual Task *createTask();
      
      static void initPersistFields();

      // execute the script
      Status evaluateScript(SimObject *owner);

      DECLARE_CONOBJECT(ScriptEval);
   };

   //---------------------------------------------------------------------------
   // ScriptEval task
   //---------------------------------------------------------------------------
   class ScriptEvalTask : public Task
   {
      typedef Task Parent;
   
   protected:
      virtual Task* update();
        
   public:
      ScriptEvalTask(Node &node);
   };

} // namespace BadBehavior
#endif