#include "BTUndoActions.h"
#include "console/consoleTypes.h"
#include "console/simSet.h"

IMPLEMENT_CONOBJECT( BTDeleteUndoAction );

ConsoleDocClass( BTDeleteUndoAction,
				"@brief Behavior Tree Editor delete undo instance\n\n"
				"Not intended for game development, for editors or internal use only.\n\n "
				"@internal");

BTDeleteUndoAction::BTDeleteUndoAction( const UTF8 *actionName )
   :  UndoAction( actionName )
{
}

BTDeleteUndoAction::~BTDeleteUndoAction()
{
}

void BTDeleteUndoAction::initPersistFields()
{
   Parent::initPersistFields();
}

void BTDeleteUndoAction::deleteObject( SimObject *object )
{
   AssertFatal( object, "BTDeleteUndoAction::deleteObject() - Got null object!" );
   AssertFatal( object->isProperlyAdded(), 
      "BTDeleteUndoAction::deleteObject() - Object should be registered!" );

   // Capture the object id.
   mObject.id = object->getId();

   // Save the state.
   mObject.memento.save( object );

   // Store the group.
   SimGroup *group = object->getGroup();
   if ( group )
   {
      mObject.groupId = group->getId();

      group->lock();
      if(object == group->last())
         mObject.nextId = -1;
      else
      for( SimSet::iterator i = group->begin(); i != group->end(); i++)
      {
         if( *i == object )
         {
            //++i;
            mObject.nextId = (*++i)->getId();
            break;
         }
      }
      group->unlock();
   }
   
   // Now delete the object.
   object->deleteObject();
}

ConsoleMethod( BTDeleteUndoAction, deleteObject, void, 3, 3, "( SimObject obj )")
{
   SimObject *obj = NULL;
   if ( Sim::findObject( argv[2], obj ) && obj )
   	object->deleteObject( obj );
}

void BTDeleteUndoAction::undo()
{
   // Create the object.
   SimObject::setForcedId(mObject.id); // Restore the object's Id
   SimObject *object = mObject.memento.restore();
   if ( !object )
      return;

   // Now restore its group.
   SimGroup *group;
   if ( Sim::findObject( mObject.groupId, group ) )
   {
      group->addObject( object );

      // restore its position in the group
      SimObject *nextObj;
      if ( Sim::findObject( mObject.nextId, nextObj ) )
      {
         group->reOrder(object, nextObj);
      }
   }
   
   Con::executef( this, "onUndone" );
}

void BTDeleteUndoAction::redo()
{
   SimObject *object = Sim::findObject( mObject.id );
   if ( object )
      object->deleteObject();
   
   Con::executef( this, "onRedone" );
}
