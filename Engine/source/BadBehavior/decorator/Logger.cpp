#include "console/engineAPI.h"

#include "Logger.h"

using namespace BadBehavior;

//------------------------------------------------------------------------------
// Logger decorator node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Logger);

Task *Logger::createTask()
{
   return new LoggerTask(*this);
}

//------------------------------------------------------------------------------
// Logger decorator task
//------------------------------------------------------------------------------
LoggerTask::LoggerTask(Node &node)
   : Parent(node) 
{
}

void LoggerTask::onInitialize()
{
   Parent::onInitialize();
   (*mCurrentChild)->setStatus(INVALID);
}

Task* LoggerTask::update() 
{ 
   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;

      return NULL;
   }
   
   return (*mCurrentChild); 
}
      
void LoggerTask::onChildComplete(Status s)
{
   mStatus = s;
   Con::printf("%s child returning %s", static_cast<Logger *>(mNodeRep)->getInternalName(), EngineMarshallData< BehaviorReturnType > (mStatus));

   mIsComplete = true;
}