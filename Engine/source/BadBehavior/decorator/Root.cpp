#include "console/engineAPI.h"

#include "Root.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Root decorator
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Root);

Task *Root::createTask()
{
   if(!size())
      return NULL;

   return dynamic_cast<Node *>(*begin())->createTask();
}