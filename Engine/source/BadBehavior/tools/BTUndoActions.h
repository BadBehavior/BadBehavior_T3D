#ifndef _BT_UNDO_ACTIONS_H_
#define _BT_UNDO_ACTIONS_H_

#ifndef _UNDO_H_
#include "util/undo.h"
#endif
#ifndef _CONSOLE_SIMOBJECTMEMENTO_H_
#include "console/simObjectMemento.h"
#endif

S32 getNextObjectInSet(SimObject *obj, SimGroup &group);

class BTDeleteUndoAction : public UndoAction
{
   typedef UndoAction Parent;

protected:

   struct ObjectState
   {
      /// The object we deleted and will restore in undo.
      SimObjectId id;

      /// The captured object state.
      SimObjectMemento memento;

      /// Keep track of the parent group.
      SimObjectId groupId;

      /// Keep track of the position within the parent group
      SimObjectId nextId;
   };

   /// The object we're deleting.
   ObjectState mObject;

public:

   DECLARE_CONOBJECT( BTDeleteUndoAction );
   static void initPersistFields();
   
   BTDeleteUndoAction( const UTF8* actionName = "Delete Object" );
   virtual ~BTDeleteUndoAction();

   ///
   void deleteObject( SimObject *object );
   
   // UndoAction
   virtual void undo();
   virtual void redo();
};



#endif // _BT_UNDO_ACTIONS_H_