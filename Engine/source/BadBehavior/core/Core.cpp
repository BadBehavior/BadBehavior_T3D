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

Task::Task(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : mStatus(INVALID), 
     mIsComplete(false), 
     mNodeRep(&node),
     mOwner(&owner),
     mRunner(&runner) 
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

void Task::setup()
{
   PROFILE_SCOPE(Task_setup);
   
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

Task* Task::getChild() 
{ 
   return NULL; 
}

void Task::onChildComplete(Status)
{
}

//===========================CompositeTask======================================

CompositeTask::CompositeTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner) 
   : Parent(node, owner, runner) 
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
         mChildren.push_back(dynamic_cast<Node*>(*i)->createTask(*mOwner, *mRunner));
      }
   }
   
   mStatus = INVALID;
   mCurrentChild = mChildren.begin();
}

void CompositeTask::onTerminate()
{
   mStatus = INVALID;
}
