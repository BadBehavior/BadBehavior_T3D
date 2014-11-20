#include "console/engineAPI.h"
#include "platform/profiler.h"

#include "ScriptEval.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// ScriptEval node - warning, slow!
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(ScriptEval);

ScriptEval::ScriptEval()
   : mDefaultReturnStatus(FAILURE) 
{
}

void ScriptEval::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "behaviorScript", TypeCommand, Offset(mBehaviorScript, ScriptEval),
      "@brief The command to execute when the leaf is ticked.  Max 255 characters." );

   addField( "defaultReturnStatus", TYPEID< BadBehavior::Status >(), Offset(mDefaultReturnStatus, ScriptEval),
      "@brief The default value for this node to return.");

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *ScriptEval::createTask()
{
   return new ScriptEvalTask(*this);
}

Status ScriptEval::evaluateScript( SimObject *owner )
{
   PROFILE_SCOPE(ScriptEval_evaluateScript);

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
ScriptEvalTask::ScriptEvalTask(Node &node)
   : Parent(node)
{
}

Task* ScriptEvalTask::update()
{
   mStatus = static_cast<ScriptEval*>(mNodeRep)->evaluateScript( mOwner );

   return NULL; // leaves don't have children
}