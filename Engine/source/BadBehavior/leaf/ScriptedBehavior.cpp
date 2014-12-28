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
   bool precondition = true;
   if( (node->mPreconditionMode == ONCE && mStatus == INVALID) || (node->mPreconditionMode == TICK) )
      if(node->isMethod("precondition"))
         precondition = dAtob(Con::executef(node, "precondition", mOwner->getId()));

   if(precondition)
   {
      // run onEnter if this is the first time the node is ticked
      if(mStatus == INVALID && node->isMethod("onEnter"))
      {
         Con::executef(node, "onEnter", mOwner->getId());
      }

      // execute the main behavior and get its return value
      const char *result = Con::executef(node, "behavior", mOwner->getId());
      
      // if function didn't return a result, use our default return status
      if(!result || !result[0])
         mStatus = node->mDefaultReturnStatus;
      else if(result[0] == '1' || result[0] == '0')
         // map true or false to SUCCEED or FAILURE
         mStatus = static_cast<Status>(dAtoi(result));
      else
         // convert the returned value to our internal enum type
         mStatus = EngineUnmarshallData< BehaviorReturnType >()( result );
   }
   else
   {
      mStatus = FAILURE;
   }

   mIsComplete = mStatus != RUNNING;

   return NULL; // leaves don't have children
}


void ScriptedBehaviorTask::onTerminate()
{
   // behavior is complete, call the script side onExit function
   ScriptedBehavior *node = static_cast<ScriptedBehavior*>(mNodeRep);
   if(node->isMethod("onExit"))
      Con::executef(node, "onExit", mOwner->getId());
}