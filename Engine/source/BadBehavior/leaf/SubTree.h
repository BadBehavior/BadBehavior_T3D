#ifndef _BB_SUBTREE_H_
#define _BB_SUBTREE_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // SubTree decorator
   // Turns a named Behavior into a subtree of this node.
   // Does not actually have a child, so is a subclass of leaf
   //---------------------------------------------------------------------------
   class SubTree : public LeafNode
   {
      typedef LeafNode Parent;

   protected:
      const char* mSubTreeName;
   
   public:
      SubTree();

      virtual Task *createTask();
      
      static void initPersistFields();

      DECLARE_CONOBJECT(SubTree);
   };

} // namespace BadBehavior

#endif