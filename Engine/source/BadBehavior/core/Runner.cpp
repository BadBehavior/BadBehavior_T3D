#include "console/engineAPI.h"
#include "platform/profiler.h"

#include "Runner.h"


using namespace BadBehavior;

IMPLEMENT_CONOBJECT(BehaviorTreeRunner);

BehaviorTreeRunner::BehaviorTreeRunner()
   : mOwner(NULL), 
     mRootNode(NULL), 
     mRootTask(NULL) 
{}


BehaviorTreeRunner::~BehaviorTreeRunner()
{
   delete mRootTask;
}


void BehaviorTreeRunner::initPersistFields()
{
   addGroup( "Behavior" );

   addProtectedField("rootNode", TYPEID< SimObject >(), Offset(mRootNode, BehaviorTreeRunner),
      &_setRootNode, &defaultProtectedGetFn, "@brief The root node of the tree to be run.");

   addProtectedField("ownerObject", TYPEID< SimObject >(), Offset(mOwner, BehaviorTreeRunner),
      &_setOwner, &defaultProtectedGetFn, "@brief The object that owns the tree to be run.");

   endGroup( "Behavior" );

   Parent::initPersistFields();
}


// processTick is where the magic happens :)
void BehaviorTreeRunner::processTick()
{
   PROFILE_SCOPE(BehaviorTreeTicker_Tick);

   // check that we are setup to run
   if(mOwner.isNull() || mRootNode.isNull())
      return;

   if(!mRootTask)
   {
      if((mRootTask = mRootNode->createTask(*mOwner, *this)) == NULL)
      {
         Con::errorf("BehaviorTreeTicker::processTick, no task for root node");
         return;
      }
   }

   if(mTasks.empty())
   {
      // init the root node
      mRootTask->setup();//mOwner, this);

      // add it to the task list
      mTasks.push_back(mRootTask);
   }

   // some vars we will use
   Task *currentTask = NULL;
   Task *nextTask = NULL;
   
   // loop through the tasks in the task list
   while(mTasks.size())
   {
      // get a task
      currentTask = mTasks.back();

      // tick the task, and get its child and status
      nextTask = currentTask->tick();
      
      // if task returned no children, it has completed
      if(!nextTask)
      {
         // so remove it from the list
         mTasks.pop_back();
         if(mTasks.size())
            // and tell its parent that it completed
            mTasks.back()->onChildComplete(currentTask->getStatus());

         currentTask->finish();
      }
      else
      {
         // add the child as a task
         nextTask->setup();
         mTasks.push_back(nextTask);
      }
   }  
}


bool BehaviorTreeRunner::_setRootNode( void *object, const char *index, const char *data )
{
   BehaviorTreeRunner *runner = reinterpret_cast<BehaviorTreeRunner *>( object );   
   CompositeNode* root = NULL;
   Sim::findObject( data, root );
   runner->setRootNode(root);
   return false;
}

bool BehaviorTreeRunner::_setOwner( void *object, const char *index, const char *data )
{
   BehaviorTreeRunner *runner = reinterpret_cast<BehaviorTreeRunner *>( object );   
   SimObject* owner = NULL;
   Sim::findObject( data, owner );
   runner->setOwner(owner);
   return false;
}


void BehaviorTreeRunner::setOwner(SimObject *owner) 
{ 
   mOwner = owner; 
}


void BehaviorTreeRunner::setRootNode(CompositeNode *root) 
{ 
   mRootNode = root; 
}
