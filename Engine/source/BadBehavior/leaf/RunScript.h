#ifndef _BB_RUNSCRIPT_H_
#define _BB_RUNSCRIPT_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // RunScript - evaluates a set of Torque Script commands when run
   // Warning - slow, handle with care!
   //---------------------------------------------------------------------------
   class RunScript : public LeafNode
   {
      typedef LeafNode Parent;
   
   public:

      // status to return if the command does not return a value
      Status mDefaultReturnStatus;

      // the torque script to evaluate
      String mBehaviorScript;

      RunScript();

      virtual Task *createTask();
      
      static void initPersistFields();

      // execute the script
      Status evaluateScript(SimObject *owner);

      DECLARE_CONOBJECT(RunScript);
   };

   //---------------------------------------------------------------------------
   // RunScript task
   //---------------------------------------------------------------------------
   class RunScriptTask : public Task
   {
      typedef Task Parent;
   
   protected:
      virtual Task* update();
        
   public:
      RunScriptTask(Node &node);
   };

} // namespace BadBehavior
#endif