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

#include "Sequence.h"


using namespace BadBehavior;

//------------------------------------------------------------------------------
// Sequence node
//------------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Sequence);

Task *Sequence::createTask()
{
   return new SequenceTask(*this);
}

//------------------------------------------------------------------------------
// Sequence Task
//------------------------------------------------------------------------------
SequenceTask::SequenceTask(Node &node) 
   : Parent(node) 
{
}

void SequenceTask::onChildComplete(Status s)
{
   mStatus = s;

   if(s == SUCCESS)
      ++mCurrentChild;
   else
      mIsComplete = true;
}

Task* SequenceTask::update()
{
   if (mCurrentChild == mChildren.end())
      mIsComplete = true;

   if( mIsComplete )
   {
      if(mStatus == RUNNING)
         mIsComplete = false;

      return NULL;
   }

   if(mStatus != RUNNING)
      (*mCurrentChild)->reset();

   // return child
   return (*mCurrentChild);   
}
