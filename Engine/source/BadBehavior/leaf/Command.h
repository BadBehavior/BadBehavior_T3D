#ifndef _BB_COMMAND_H_
#define _BB_COMMAND_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   static const U8 MAX_COMMAND_ARGS = 4;

   //---------------------------------------------------------------------------
   // Command - executes a function on the owner object
   //---------------------------------------------------------------------------
   class Command : public LeafNode
   {
      typedef LeafNode Parent;

      // the function to call
      StringTableEntry mCommandFunction;
      
      // the arguments for the function
      StringTableEntry mCommandArgs[MAX_COMMAND_ARGS];

   public:

      // status to return if the command does not return a value
      Status mDefaultReturnStatus;

      Command();

      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      static void initPersistFields();

      // execute the command
      Status evaluate(SimObject *owner);

      DECLARE_CONOBJECT(Command);
   };

   //---------------------------------------------------------------------------
   // Command task
   //---------------------------------------------------------------------------
   class CommandTask : public Task
   {
      typedef Task Parent;
   
   protected:
      virtual Task* update();
        
   public:
      CommandTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);
   };

} // namespace BadBehavior
#endif