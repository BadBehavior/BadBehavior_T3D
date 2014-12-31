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
// ScriptedBehavior node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(ScriptedBehavior);

ScriptedBehavior::ScriptedBehavior()
   : mDefaultReturnStatus(FAILURE)
{
}

void ScriptedBehavior::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "defaultReturnStatus", TYPEID< BadBehavior::Status >(), Offset(mDefaultReturnStatus, ScriptedBehavior),
      "@brief The default value for this node to return.");

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

bool ScriptedBehavior::precondition( SimObject *owner )
{
   PROFILE_SCOPE( ScriptedBehavior_precondition);

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

