#ifndef _BB_TICKER_H_
#define _BB_TICKER_H_

#ifndef _BB_CORE_H_
#include "BadBehavior/core/Core.h"
#endif

namespace BadBehavior
{
   //---------------------------------------------------------------------------
   // Ticker decorator
   // Limits the frequency at which it's children are updated.
   //---------------------------------------------------------------------------
   class Ticker : public DecoratorNode
   {
      typedef DecoratorNode Parent;

   protected:
      static bool _setFrequency(void *object, const char *index, const char *data);
  
   public:
      S32 mFrequencyMs;
      
      Ticker();

      virtual Task *createTask(SimObject &owner, BehaviorTreeRunner &runner);
      
      static void initPersistFields();

      DECLARE_CONOBJECT(Ticker);
   };

   //---------------------------------------------------------------------------
   // Ticker decorator task
   //---------------------------------------------------------------------------
   class TickerTask : public CompositeTask
   {
      typedef CompositeTask Parent;

   protected:
      S32 mNextTimeMs;

      virtual Task* update();
      virtual void onInitialize();
      
   public:
      TickerTask(Node &node, SimObject &owner, BehaviorTreeRunner &runner);

      virtual void onChildComplete(Status s);
   };

} // namespace BadBehavior

#endif