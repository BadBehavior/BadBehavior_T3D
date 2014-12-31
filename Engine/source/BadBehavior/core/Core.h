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

#ifndef _BBCORE_H_
#define _BBCORE_H_

#ifndef _DYNAMIC_CONSOLETYPES_H_
#include "console/dynamicTypes.h"
#endif
#ifndef _SIMSET_H_
#include "console/simSet.h"
#endif
#ifndef _SIMOBJECT_H_
#include "console/simObject.h"
#endif
#ifndef _SIMSET_H_
#include "console/simSet.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // return status values 
   //---------------------------------------------------------------------------
   enum Status
   {
       INVALID = -1,
       FAILURE,
       SUCCESS,
       RUNNING,
   };
   
   class Task;
   class BehaviorTreeRunner;
   
   //---------------------------------------------------------------------------
   // node base class
   // derived from SimGroup for easy editor integration
   //---------------------------------------------------------------------------
   class Node : public SimGroup
   {
      typedef SimGroup Parent;

   public:
      // create a runtime task for this node
      virtual Task* createTask(SimObject &owner, BehaviorTreeRunner &runner) = 0;
   };

   //---------------------------------------------------------------------------
   // Leaf node base class - for nodes without children
   //---------------------------------------------------------------------------
   class LeafNode : public Node
   {
      typedef Node Parent;

   public:
      virtual void addObject(SimObject *obj);
      virtual bool acceptsAsChild( SimObject *object ) const;
   };

   //---------------------------------------------------------------------------
   // Composite node base class - for nodes with children
   //---------------------------------------------------------------------------
   class CompositeNode : public Node
   {
      typedef Node Parent;

   public:
      // override addObject and acceptsAsChild to only allow behavior tree nodes to be added as children
      virtual void addObject(SimObject *obj);
      virtual bool acceptsAsChild( SimObject *object ) const;
   };

   //---------------------------------------------------------------------------
   // Decorator node base class
   // using CompositeNode as baseclass as this node has a child
   //---------------------------------------------------------------------------
   class DecoratorNode : public CompositeNode
   {
      typedef CompositeNode Parent;

   public:
      // only allow 1 child node to be added
      virtual void addObject(SimObject *obj);
      virtual bool acceptsAsChild( SimObject *object ) const;
   };


   //---------------------------------------------------------------------------
   // base class for all behavior tree tasks
   //---------------------------------------------------------------------------
   class Task
   {
   protected:

      // the current status
      Status mStatus;

      // has the task finished
      bool mIsComplete;

      // the node associated with this task
      Node *mNodeRep;

      // the object that owns us
      SimObjectPtr<SimObject> mOwner;

      // the object running us
      BehaviorTreeRunner *mRunner;

      // update
      virtual Task* update() = 0;
      
      // initialize
      virtual void onInitialize();

      // terminate
      virtual void onTerminate();
   
   public:
      // tasks are instantiated with a reference to their associated node
      Task(Node &node, SimObject &owner, BehaviorTreeRunner &runner);
      virtual ~Task();
      
      // status sets and gets
      Status getStatus();
      void setStatus(Status newStatus);
      
      // run the task
      Task* tick();
      
      // called when child task finishes
      virtual void onChildComplete(Status);

      // prepare the task
      void setup();

      // finish the task
      void finish();

      // reset the task
      void reset();
   };

   //---------------------------------------------------------------------------
   // Composite task base class
   //---------------------------------------------------------------------------
   class CompositeTask : public Task
   {
      typedef Task Parent;

   protected:
      // vector of pointers to child tasks
      VectorPtr<Task*> mChildren;

      // the current child task
      VectorPtr<Task*>::iterator mCurrentChild;

      CompositeTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);
      virtual ~CompositeTask();
      virtual void onInitialize();
      virtual void onTerminate();
   };

} // namespace BadBehavior

// make the return status enum accessible from script
typedef BadBehavior::Status BehaviorReturnType;
DefineEnumType( BehaviorReturnType );

#endif