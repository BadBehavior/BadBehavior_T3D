#ifndef _BB_ROOT_H_
#define _BB_ROOT_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Root decorator
   // A placeholder node to mark the start of a tree. 
   // Used by the editor, bypassed during tree evaluation.
   //---------------------------------------------------------------------------
   class Root : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   public:
      virtual Task* createTask();
      DECLARE_CONOBJECT(Root);
   };

} // namespace BadBehavior

#endif