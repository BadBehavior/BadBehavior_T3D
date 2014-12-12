#include "console/engineAPI.h"
#include "platform/profiler.h"
#include "Core.h"
#include "Runner.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// script enum for return type
//------------------------------------------------------------------------------
ImplementEnumType( BehaviorReturnType,
   "@brief The return status for a behavior.\n\n"
   "@ingroup AI\n\n")
   // not needed script side { BadBehavior::INVALID, "INVALID", "The behavior could not be evaluated.\n" },
   { SUCCESS, "SUCCESS", "The behavior succeeded.\n" },
   { FAILURE, "FAILURE", "The behavior failed.\n" },
   { RUNNING, "RUNNING", "The behavior is still running.\n" },
EndImplementEnumType;


//================================LeafNode======================================

//------------------------------------------------------------------------------
// don't allow objects to be added
//------------------------------------------------------------------------------
void LeafNode::addObject(SimObject *object)
{
}

bool LeafNode::acceptsAsChild( SimObject *object ) const 
{ 
   return false; 
}


//============================CompositeNode=====================================

//------------------------------------------------------------------------------
// override addObject to only allow behavior tree nodes to be added
//------------------------------------------------------------------------------
void CompositeNode::addObject(SimObject *object)
{
   if(dynamic_cast<LeafNode*>(object) || dynamic_cast<CompositeNode*>(object))
      Parent::addObject(object);
}

bool CompositeNode::acceptsAsChild( SimObject *object ) const 
{ 
   return (dynamic_cast<LeafNode*>(object) || dynamic_cast<CompositeNode*>(object)); 
}


//===============================DecoratorNode==================================

//------------------------------------------------------------------------------
//override for decorators to only allow 1 child
//------------------------------------------------------------------------------
void DecoratorNode::addObject(SimObject *obj)
{
   if(empty())
      Parent::addObject(obj);
}

bool DecoratorNode::acceptsAsChild( SimObject *object ) const 
{
   return (Parent::acceptsAsChild(object) && empty());
}


//==================================Task========================================

Task::Task(Node &node)
   : mStatus(INVALID), 
     mIsComplete(false), 
     mNodeRep(&node) 
{
}

Task::~Task()
{
}

void Task::onInitialize()
{
}

void Task::onTerminate()
{
}

Task* Task::tick()
{
   PROFILE_SCOPE(Task_Tick);
   
   return update();
}

void Task::setup(SimObject *owner, BehaviorTreeRunner *runner)
{
   PROFILE_SCOPE(Task_setup);
   mRunner = runner;
   mOwner = owner;

   if(mStatus != RUNNING)
      onInitialize();
   
   mIsComplete = false;
}

void Task::finish()
{
   if(mIsComplete)
      onTerminate();
}

Status Task::getStatus() 
{ 
   return mStatus; 
}

void Task::setStatus(Status newStatus) 
{ 
   mStatus = newStatus; 
}

void Task::onChildComplete(Status)
{
}

//===========================CompositeTask======================================

CompositeTask::CompositeTask(Node &node) 
   : Parent(node) 
{
}

CompositeTask::~CompositeTask()
{
   while(mChildren.size())
   {
      Task *child = mChildren.back();
      mChildren.pop_back();
      delete child;
   }
}

void CompositeTask::onInitialize()
{
   if(mChildren.empty())
   {
      CompositeNode *node = static_cast<CompositeNode *>(mNodeRep);
      for(SimSet::iterator i = node->begin(); i != node->end(); ++i)
      {
         mChildren.push_back(static_cast<Node*>(*i)->createTask());
      }
   }
   
   mStatus = INVALID;
   mCurrentChild = mChildren.begin();
}

void CompositeTask::onTerminate()
{
   mStatus = INVALID;
}
