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

#ifndef _BB_LOOP_H_
#define _BB_LOOP_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // loop decorator
   // repeats the child behaviour for n times, or until it fails
   // returns RUNNING until it completes
   //---------------------------------------------------------------------------
   class Loop : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      // loop termination policies
      enum TerminationPolicy
      {
         ON_FAILURE,
         ON_SUCCESS,
      };

   protected:
      static bool _setNumLoops(void *object, const char *index, const char *data);

   public:
      S32 mNumLoops;
      TerminationPolicy mTerminationPolicy;

      Loop();

      virtual Task *createTask();
      
      static void initPersistFields();

      DECLARE_CONOBJECT(Loop);
   };

   //---------------------------------------------------------------------------
   // loop task
   //---------------------------------------------------------------------------
   class LoopTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      S32 mCurrentLoop;
   
      virtual void onInitialize();
      virtual Task *update();

   public:
      LoopTask(Node &node);

      virtual void onChildComplete(Status s);
   };
   
} // namespace BadBehavior

// make the loop termination policy enum accessible from script
typedef BadBehavior::Loop::TerminationPolicy LoopTerminationPolicy;
DefineEnumType( LoopTerminationPolicy );

#endif