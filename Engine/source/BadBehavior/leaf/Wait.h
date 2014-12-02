#ifndef _BB_WAIT_H_
#define _BB_WAIT_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Wait leaf
   // Pauses for a set time period.
   //---------------------------------------------------------------------------
   class Wait : public LeafNode
   {
      typedef LeafNode Parent;

   protected:
      static bool _setWait(void *object, const char *index, const char *data);

   public:
      S32 mWaitMs;

      Wait();
   
      virtual Task *createTask();
      
      static void initPersistFields();

      DECLARE_CONOBJECT(Wait);
   };

   //---------------------------------------------------------------------------
   // Wait leaf task
   //---------------------------------------------------------------------------
   class WaitTask : public Task
   {
      typedef Task Parent;

   protected:
      S32 mCompleteMs;

      virtual void onInitialize();
      virtual Task* update();
      
   public:
      WaitTask(Node &node);
   };

} // namespace BadBehavior

#endif