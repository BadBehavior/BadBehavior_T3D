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

Task::Task(Node &node, SimObject &owner, BehaviorTreeRunner &runner)
   : mStatus(INVALID), 
     mIsComplete(false), 
     mNodeRep(&node),
     mOwner(&owner),
     mRunner(&runner)
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

void Task::reset()
{
   mStatus = INVALID;
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
         mChildren.push_back(static_cast<Node*>(*i)->createTask(*mOwner, *mRunner));
      }
   }
   
   mStatus = INVALID;
   mCurrentChild = mChildren.begin();
}

void CompositeTask::onTerminate()
{
   mStatus = INVALID;
}
