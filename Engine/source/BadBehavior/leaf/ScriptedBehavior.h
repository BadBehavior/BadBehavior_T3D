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

#ifndef _BB_SCRIPTEDBEHAVIOR_H_
#define _BB_SCRIPTEDBEHAVIOR_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   // specify when the precondition function should be executed
   enum PreconditionMode
   {
      ONCE,    // the first time the behavior is evaluated
      TICK     // every tick
   };

   //---------------------------------------------------------------------------
   // ScriptedBehavior - A structured behavior leaf node which defines a series
   // Of scripted callbacks
   //---------------------------------------------------------------------------
   class ScriptedBehavior : public LeafNode
   {
      typedef LeafNode Parent;
   
   protected:
      // status to return if the beahvior does not return a value
      Status mDefaultReturnStatus;

      // how often should we valuate the precondition
      PreconditionMode mPreconditionMode;

   public:
      ScriptedBehavior();

      virtual Task *createTask();
      
      static void initPersistFields();

      Status getDefaultReturnStatus() const { return mDefaultReturnStatus; }
      PreconditionMode getPreconditionMode() const { return mPreconditionMode; }

      virtual bool precondition( SimObject *owner, bool firstRun );
      virtual void onEnter( SimObject *owner );
      virtual void onExit( SimObject *owner );
      virtual Status behavior( SimObject *owner );

      DECLARE_CONOBJECT(ScriptedBehavior);
   };

   //---------------------------------------------------------------------------
   // ScriptedBehavior task
   //---------------------------------------------------------------------------
   class ScriptedBehaviorTask : public Task
   {
      typedef Task Parent;
   
   protected:
      virtual Task* update();

   public:
      ScriptedBehaviorTask(Node &node);
   };

} // namespace BadBehavior

// make the return precondition mode accessible from script
typedef BadBehavior::PreconditionMode BehaviorPreconditionType;
DefineEnumType( BehaviorPreconditionType );

#endif