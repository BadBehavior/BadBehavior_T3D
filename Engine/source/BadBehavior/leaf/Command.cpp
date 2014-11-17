#include "console/engineAPI.h"
#include "platform/profiler.h"

#include "Command.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Command node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Command);

Command::Command()
   : mDefaultReturnStatus(FAILURE),
     mCommandFunction(StringTable->insert(""))
{
   for(U8 i = 0; i < MAX_COMMAND_ARGS; ++i)
      mCommandArgs[i] = StringTable->insert("");
}


void Command::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "func", TypeCaseString, Offset(mCommandFunction, Command),
      "@brief The function to execute when the leaf is ticked." );

   addField( "args", TypeCaseString, Offset(mCommandArgs, Command), MAX_COMMAND_ARGS,
      "@brief The arguments to be passed to the function to be executed." );

   addField( "defaultReturnStatus", TYPEID< BadBehavior::Status >(), Offset(mDefaultReturnStatus, Command),
      "@brief The default value for this node to return.");

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *Command::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new CommandTask(*this, owner, runner);
}

Status Command::evaluate( SimObject *owner )
{
   PROFILE_SCOPE(Command_evaluate);

   if(!owner)
      return INVALID;

   if(!mCommandFunction || !mCommandFunction[0])
      return mDefaultReturnStatus;

   S32 argc = 0;

   const char *args[MAX_COMMAND_ARGS + 2];
   args[0] = mCommandFunction;

   while(argc < MAX_COMMAND_ARGS && (mCommandArgs[argc] && mCommandArgs[argc][0]))
   {
      args[argc + 2] = mCommandArgs[argc];
      ++argc;
   }

   argc += 2;

   // get the result
   const char *result = Con::execute(owner, argc, args);

   // if function didn't return a result, return our default status
   if(!result || !result[0])
      return mDefaultReturnStatus;
   
   // map true or false to SUCCEED or FAILURE
   if(result[0] == '1' || result[0] == '0') 
      return static_cast<Status>(dAtoi(result));
   
   // convert the returned value to our internal enum type
   return EngineUnmarshallData< BehaviorReturnType >()( result );
}

//------------------------------------------------------------------------------
// Command task
//------------------------------------------------------------------------------
CommandTask::CommandTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner)
{
}

Task* CommandTask::update()
{
   mStatus = static_cast<Command*>(mNodeRep)->evaluate( mOwner );

   return NULL; // leaves don't have children
}