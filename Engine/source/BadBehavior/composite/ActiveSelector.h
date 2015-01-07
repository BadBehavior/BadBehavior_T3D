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

#ifndef _BB_ACTIVESELECTOR_H_
#define _BB_ACTIVESELECTOR_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

//==============================================================================
// Active Selector
// Re-evaluates its children from the beginning each tick. Lower priority
// children which previously returned RUNNING are resumed if re-selected
//
// ** NOTE **
// The "active" nature of this node means that it can only be fully SUSPENDED if
// it's highest priority child is suspended.
//
// USE SPARINGLY - place as high up in the tree as possible and try to early-out
// of high priority branches that are not going to be selected.
//==============================================================================

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Active selector Node
   //---------------------------------------------------------------------------
   class ActiveSelector : public CompositeNode
   {
      typedef CompositeNode Parent;
         
   public:
      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);

      DECLARE_CONOBJECT(ActiveSelector);
   };

   //---------------------------------------------------------------------------
   // Active selector Task
   //---------------------------------------------------------------------------
   class ActiveSelectorTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      VectorPtr<Task*>::iterator mRunningChild;
      
      virtual Task* update();
   
   public:
      ActiveSelectorTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status);
   };

} // namespace BadBehavior

#endif