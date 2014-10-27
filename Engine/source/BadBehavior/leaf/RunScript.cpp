#include "console/engineAPI.h"
#include "platform/profiler.h"

#include "RunScript.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// RunScript node - warning, slow!
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(RunScript);

RunScript::RunScript()
   : mDefaultReturnStatus(FAILURE) 
{
}

void RunScript::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "behaviorScript", TypeCommand, Offset(mBehaviorScript, RunScript),
      "@brief The command to execute when the leaf is ticked.  Max 255 characters." );

   addField( "defaultReturnStatus", TYPEID< BadBehavior::Status >(), Offset(mDefaultReturnStatus, RunScript),
      "@brief The default value for this node to return.");

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *RunScript::createTask(SimObject &owner, BehaviorTreeRunner &runner)
{
   return new RunScriptTask(*this, owner, runner);
}

Status RunScript::evaluateScript( SimObject *owner )
{
   PROFILE_SCOPE(RunScript_evaluateScript);

   if(mBehaviorScript.isEmpty())
      return mDefaultReturnStatus;

   // construct the string to be exec'd
   String execString;
   if(owner)
   {
      String idString(owner->getIdString());
      execString = "%obj = " + idString + "; ";
   }
   
   execString += mBehaviorScript;

   // get the result
   const char *result = Con::evaluate(execString.c_str());

   // if script didn't return a result, return our default status
   if(!result || !result[0])
      return mDefaultReturnStatus;
   
   // true or false map directly to SUCCEED or FAILURE
   if(result[0] == '1' || result[0] == '0') 
      return static_cast<Status>(dAtoi(result));
   
   // convert the returned value to our internal enum type
   return EngineUnmarshallData< BehaviorReturnType >()( result );
}

//------------------------------------------------------------------------------
// RunScript task
//------------------------------------------------------------------------------
RunScriptTask::RunScriptTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : Parent(node, owner, runner)
{
}

Task* RunScriptTask::update()
{
   mStatus = static_cast<RunScript*>(mNodeRep)->evaluateScript( mOwner );

   return NULL; // leaves don't have children
}