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

#ifndef _BB_RANDOMWAIT_H_
#define _BB_RANDOMWAIT_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // RandomWait leaf
   // Pauses for a random period of time between delayMin and delayMax ms before completing.
   //---------------------------------------------------------------------------
   class RandomWait : public LeafNode
   {
      typedef LeafNode Parent;

   protected:
      static bool _setWaitMin(void *object, const char *index, const char *data);
      static bool _setWaitMax(void *object, const char *index, const char *data);

   public:
      S32 mWaitMinMs;
      S32 mWaitMaxMs;

      RandomWait();
   
      virtual Task *createTask();
      
      static void initPersistFields();

      DECLARE_CONOBJECT(RandomWait);
   };

   //---------------------------------------------------------------------------
   // RandomWait leaf task
   //---------------------------------------------------------------------------
   class RandomWaitTask : public Task
   {
      typedef Task Parent;

   protected:
      S32 mCompleteMs;

      virtual void onInitialize();
      virtual Task* update();
      
   public:
      RandomWaitTask(Node &node);
   };

} // namespace BadBehavior

#endif