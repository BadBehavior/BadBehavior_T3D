
function BTEditorUndoManager::onAddUndo( %this )
{
   echo("Undo Added");
   BTEditor.updateUndoMenu();
}

//==============================================================================
// create a node
//==============================================================================
function BTCreateUndoAction::submit( %undoObject )
{
   // The instant group will try to add our
   // UndoAction if we don't disable it.   
   pushInstantGroup();
   
   // Create the undo action.     
   %action = new MECreateUndoAction()
   {
      className = "BTCreateUndoAction";
      actionName = "Create " @ %undoObject.getClassName();
   };
   
   // Restore the instant group.
   popInstantGroup();
   
   // Set the object to undo.
   %action.addObject( %undoObject );
   
   // Submit it.
   %action.addToManager( BTEditor.getUndoManager() );
}

function BTCreateUndoAction::onUndone( %this )
{
   //EWorldEditor.syncGui();
}

function BTCreateUndoAction::onRedone( %this )
{
   //EWorldEditor.syncGui();
}



//==============================================================================
// delete a node
//==============================================================================

/// A helper for submitting a delete undo action.
function BTDeleteUndoAction::submit( %deleteObject )
{
   // The instant group will try to add our
   // UndoAction if we don't disable it.   
   pushInstantGroup();
   
   // Create the undo action.     
   %action = new BTDeleteUndoAction()
   {
      actionName = "delete node";
   };

   // Restore the instant group.
   popInstantGroup();
   
   %action.deleteObject( %deleteObject );
   
   // Submit it.
   %action.addToManager( BTEditor.getUndoManager() );
}

function BTDeleteUndoAction::onUndone( %this )
{
   // for some reason this doesn't restore the correct order
   //BTEditor.getCurrentViewCtrl().buildVisibleTree(true);
   // so re-open the tree instead
   BTEditor.getCurrentViewCtrl().refresh();
}

function BTDeleteUndoAction::onRedone( %this )
{
   BTEditor.getCurrentViewCtrl().buildVisibleTree(true);
}

//==============================================================================
// reparent a node
//==============================================================================
function BTReparentUndoAction::create( %treeView )
{
   pushInstantGroup();
   %action = new UndoScriptAction()
   {
      class = "BTReparentUndoAction";
      actionName = "move node";
      control = %treeView;
   };
   popInstantGroup();
   
   return %action;
}

function BTReparentUndoAction::undo(%this)
{
   if(%this.newParent.isMember(%this.node))  
      %this.newParent.remove(%this.node);
   
   %this.oldParent.add(%this.node);  
   
   if(%this.oldPosition < %this.oldParent.getCount() - 1)
      %this.oldParent.reorderChild(%this.node, %this.oldParent.getObject(%this.oldPosition));
   
   %this.control.refresh();
}

function BTReparentUndoAction::redo(%this)
{
   if(%this.oldParent.isMember(%this.node))
      %this.oldParent.remove(%this.node);
   
   %this.newParent.add(%this.node);
   
   if(%this.newPosition < %this.newParent.getCount() - 1)
      %this.newParent.reorderChild(%this.node, %this.newParent.getObject(%this.newPosition));
   
   %this.control.refresh();
}

//==============================================================================
// Inspector field modified
//==============================================================================
function BTInspectorUndoAction::create(%object, %fieldName, %arrayIndex, %oldValue, %newValue )
{
   %nameOrClass = %object.getName();
   if ( %nameOrClass $= "" )
      %nameOrClass = %object.getClassname();
      
   pushInstantGroup();
   %action = new InspectorFieldUndoAction()
   {
      class = "BTInspectorUndoAction";
      actionName = %nameOrClass @ "." @ %fieldName @ " Change";
      
      objectId = %object.getId();
      fieldName = %fieldName;
      fieldValue = %oldValue;
      arrayIndex = %arrayIndex;
                  
      inspectorGui = BehaviorTreeInspector;
   };
   popInstantGroup();

   return %action;
}
