#include "console/engineAPI.h"
#include "platform/profiler.h"

#include "Condition.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Condition node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Condition);

void Condition::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "conditionFunc", TypeRealString, Offset(mConditionFunction, Condition),
      "@brief The comparison function to call when the leaf is ticked.  Max 255 characters." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *Condition::createTask()
{
   return new ConditionTask(*this);
}

Status Condition::evaluate( SimObject *owner )
{
   PROFILE_SCOPE(Condition_evaluate);

   if(!owner)
      return INVALID;

   if(mConditionFunction.isEmpty())
      return FAILURE;

   const char *result = Con::executef(owner, mConditionFunction.c_str());
   return static_cast<Status>(dAtob(result));
}

//------------------------------------------------------------------------------
// Condition task
//------------------------------------------------------------------------------
ConditionTask::ConditionTask(Node &node)
   : Parent(node) 
{
}

Task* ConditionTask::update()
{
   mStatus = static_cast<Condition*>(mNodeRep)->evaluate( mOwner );

   return NULL; // leaves don't have children
}
