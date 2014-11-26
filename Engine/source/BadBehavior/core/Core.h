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
      virtual Task* createTask() = 0;
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
      Task(Node &node);
      virtual ~Task();
      
      // status sets and gets
      Status getStatus();
      void setStatus(Status newStatus);
      
      // run the task
      Task* tick();
      
      // called when child task finishes
      virtual void onChildComplete(Status);

      // prepare the task
      void setup(SimObject *owner, BehaviorTreeRunner *runner);

      // finish the task
      void finish();
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

      CompositeTask(Node &node);
      virtual ~CompositeTask();
      virtual void onInitialize();
      virtual void onTerminate();
   };

} // namespace BadBehavior

// make the return status enum accessible from script
typedef BadBehavior::Status BehaviorReturnType;
DefineEnumType( BehaviorReturnType );

#endif