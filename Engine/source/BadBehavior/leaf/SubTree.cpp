#include "console/engineAPI.h"

#include "SubTree.h"

using namespace BadBehavior;


//------------------------------------------------------------------------------
// SubTree node - links to another behavior tree
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(SubTree);

SubTree::SubTree()
   : mSubTreeName(0)
{
}

void SubTree::initPersistFields()
{
   addGroup( "Behavior" );

   addField( "subTreeName", TypeString, Offset(mSubTreeName, SubTree),
      "@brief The name of the behavior tree that this node links to.  Max 255 characters." );

   endGroup( "Behavior" );

   Parent::initPersistFields();
}

Task *SubTree::createTask()
{
   if(!mSubTreeName || mSubTreeName[0] == 0)
   {
      Con::errorf("SubTree::onInitialize: no sub tree specified");
      return NULL;
   }

   SimObject *subTreeNode;

   if(!Sim::findObject(mSubTreeName, subTreeNode))
   {
      Con::errorf("SubTree:onInitialize: the specified sub tree does not exist");
      return NULL;
   }

   Node *node = dynamic_cast<Node*>(subTreeNode);
   if(!node)
   {
      Con::errorf("SubTree::onInitialize: the specified sub tree is not a behavior tree node");
      return NULL;
   }

   return node->createTask();
}