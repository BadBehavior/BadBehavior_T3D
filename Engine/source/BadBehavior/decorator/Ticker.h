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

#ifndef _BB_TICKER_H_
#define _BB_TICKER_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Ticker decorator
   // Limits the frequency at which it's children are updated.
   //---------------------------------------------------------------------------
   class Ticker : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   protected:
      static bool _setFrequency(void *object, const char *index, const char *data);
      
      // time between ticks (in ms)
      S32 mFrequencyMs;

      // status to return between tick
      Status mIdleReturnStatus;
      
   public:
      Ticker();

      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      static void initPersistFields();

      S32 getFrequencyMs() const { return mFrequencyMs; }
      Status getIdleReturnStatus() const { return mIdleReturnStatus; }

      DECLARE_CONOBJECT(Ticker);
   };

   //---------------------------------------------------------------------------
   // Ticker decorator task
   //---------------------------------------------------------------------------
   class TickerTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      S32 mNextTimeMs;

      virtual Task* update();
      virtual void onInitialize();
      
   public:
      TickerTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif