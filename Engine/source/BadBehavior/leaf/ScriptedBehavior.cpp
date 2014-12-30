//-----------------------------------------------------------------------------
// Copyright (c) 2014 Guy Allard
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "console/engineAPI.h"
#include "platform/profiler.h"

#include "ScriptedBehavior.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// script enum for precondition mode
//------------------------------------------------------------------------------
ImplementEnumType( BehaviorPreconditionType,
   "@brief When should the precondition function be evaluated.\n\n"
   "@ingroup AI\n\n")
   { ONCE, "ONCE", "The first time the node is executed.\n" },
   { TICK, "TICK", "Each time the node is executed.\n" },
EndImplementEnumType;

//------------------------------------------------------------------------------
// ScriptedBehavior node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(ScriptedBehavior);

ScriptedBehavior::ScriptedBehavior()
   : mDefaultReturnStatus(FAILURE),
     mPreconditionMode(ONCE)
{
}

void ScriptedBehavior::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "defaultReturnStatus", TYPEID< BadBehavior::Status >(), Offset(mDefaultReturnStatus, ScriptedBehavior),
      "@brief The default value for this node to return.");

   addField( "preconditionMode", TYPEID< BadBehavior::PreconditionMode >(), Offset(mPreconditionMode, ScriptedBehavior),
      "@brief When to evaluate the precondition function.");

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *ScriptedBehavior::createTask()
{
   return new ScriptedBehaviorTask(*this);
}

bool ScriptedBehavior::precondition( SimObject *owner, bool firstRun )
{
   PROFILE_SCOPE( ScriptedBehavior_precondition);

   if( (mPreconditionMode == ONCE && firstRun) || (mPreconditionMode == TICK) )
      if(isMethod("precondition"))
         return dAtob(Con::executef(this, "precondition", owner->getId()));

   return true;
}

void ScriptedBehavior::onEnter( SimObject *owner )
{
   PROFILE_SCOPE( ScriptedBehavior_onEnter );
   if(isMethod("onEnter"))
      Con::executef(this, "onEnter", owner->getId());
}

void ScriptedBehavior::onExit( SimObject *owner )
{
   PROFILE_SCOPE( ScriptedBehavior_onExit );
   if(isMethod("onExit"))
      Con::executef(this, "onExit", owner->getId());
}

Status ScriptedBehavior::behavior( SimObject *owner )
{
   PROFILE_SCOPE( ScriptedBehavior_behavior );
   const char *result = NULL;
   
   if(isMethod("behavior"))
      result = Con::executef(this, "behavior", owner->getId());
      
   // if function didn't return a result, use our default return status
   if(!result || !result[0])
      return mDefaultReturnStatus;
      
   if(result[0] == '1' || result[0] == '0')
      // map true or false to SUCCEED or FAILURE
      return static_cast<Status>(dAtoi(result));
      
   // convert the returned value to our internal enum type
   return EngineUnmarshallData< BehaviorReturnType >()( result );
}

//------------------------------------------------------------------------------
// ScriptedBehavior task
//------------------------------------------------------------------------------
ScriptedBehaviorTask::ScriptedBehaviorTask(Node &node)
   : Parent(node)
{
}

Task* ScriptedBehaviorTask::update()
{  
   PROFILE_SCOPE(ScriptedBehaviorTask_update);

   ScriptedBehavior *node = static_cast<ScriptedBehavior*>(mNodeRep);
   
   // first check preconditions are valid
   bool precondition = node->precondition( mOwner, mStatus == INVALID );
   
   if(precondition)
   {
      // run onEnter if this is the first time the node is ticked
      if(mStatus == INVALID)
         node->onEnter(mOwner);
      
      // execute the main behavior and get its return value
      mStatus = node->behavior(mOwner);
   }
   else
   {
      mStatus = FAILURE;
   }

   mIsComplete = mStatus != RUNNING;

   if(mIsComplete)
      node->onExit(mOwner);

   return NULL; // leaves don't have children
}
