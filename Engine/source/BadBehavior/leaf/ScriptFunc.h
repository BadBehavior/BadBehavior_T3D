#ifndef _BB_SCRIPTFUNC_H_
#define _BB_SCRIPTFUNC_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   static const U8 MAX_COMMAND_ARGS = 4;

   //---------------------------------------------------------------------------
   // ScriptFunc - executes a function on the owner object
   //---------------------------------------------------------------------------
   class ScriptFunc : public LeafNode
   {
      typedef LeafNode Parent;

      // the function to call
      StringTableEntry mScriptFunction;
      
      // the arguments for the function
      StringTableEntry mScriptArgs[MAX_COMMAND_ARGS];

   public:

      // status to return if the command does not return a value
      Status mDefaultReturnStatus;

      ScriptFunc();

      virtual Task *createTask();
      
      static void initPersistFields();

      // execute the command
      Status evaluate(SimObject *owner);

      DECLARE_CONOBJECT(ScriptFunc);
   };

   //---------------------------------------------------------------------------
   // ScriptFunc task
   //---------------------------------------------------------------------------
   class ScriptFuncTask : public Task
   {
      typedef Task Parent;
   
   protected:
      virtual Task* update();
        
   public:
      ScriptFuncTask(Node &node);
   };

} // namespace BadBehavior
#endif