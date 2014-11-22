#ifndef _BB_RUNNER_H_
#define _BB_RUNNER_H_

#ifndef _BB_CORE_H_
#include "Core.h"
#endif
#ifndef _SIMOBJECT_H_
#include "console/simObject.h"
#endif
#ifndef _ITICKABLE_H_
#include "core/iTickable.h"
#endif
#ifndef _TVECTOR_H_
#include "util/tVector.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // BehaviorTreeRunner - handles the evaluation of the tree
   //---------------------------------------------------------------------------
   class BehaviorTreeRunner : public SimObject, public virtual ITickable
   {
      typedef SimObject Parent;

   private:
      // list of tasks to be processed
      VectorPtr<Task*> mTasks;

      // the root node of the tree
      SimObjectPtr<CompositeNode> mRootNode;
      
      // the task associated with the root node
      Task *mRootTask;

      // the game object that is using this tree
      SimObjectPtr<SimObject> mOwner;

      // setters for the script interface
      static bool _setRootNode( void *object, const char *index, const char *data );
      static bool _setOwner( void *object, const char *index, const char *data );

   public:
      /*Ctor*/ BehaviorTreeRunner();
      /*Dtor*/ ~BehaviorTreeRunner();

      // public setters for the script interface
      void setOwner(SimObject *owner);
      void setRootNode(CompositeNode *root);
      
      // for script control
      void stop();
      void start();
      void reset();

      // Itickable interface, only using processTick atm
      virtual void interpolateTick( F32 ){}
      virtual void advanceTime( F32 ){}
      virtual void processTick();

      // script interface
      static void initPersistFields();

      DECLARE_CONOBJECT(BehaviorTreeRunner);
   };

} // namespace BadBehavior

#endif