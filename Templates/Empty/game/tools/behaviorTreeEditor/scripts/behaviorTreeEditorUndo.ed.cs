
function BehaviorTreeEditorUndoManager::onAddUndo( %this )
{
   BTEditor.updateUndoMenu();
}

function UndoActionReparent::create(%item, %old, %new)
{
   pushInstantGroup();
   %act = new UndoScriptAction() { class = UndoReparent; node=%item; oldParent=%old; newParent=%new; };
   popInstantGroup();
   return %act;
}

function UndoReparent::undo( %this )
{
   BTEditor.reparent(%this.node, %this.newParent, %this.oldParent);
}

function UndoReparent::redo( %this )
{
   BTEditor.reparent(%this.node, %this.oldParent, %this.newParent);
}


/// A helper for submitting a delete undo action.
function BTDeleteUndoAction::submit( %deleteObject )
{
   // The instant group will try to add our
   // UndoAction if we don't disable it.   
   pushInstantGroup();
   
   // Create the undo action.     
   %action = new BTDeleteUndoAction()
   {
      actionName = "Delete";
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
   //BTEditor.buildVisibleTree(true);
   // so re-open the tree instead
   %root = BTEditor.getItemValue(BTEditor.getFirstRootItem());
   BTEditor.open(%root);
   BTEditor.expandAll();
}

function BTDeleteUndoAction::onRedone( %this )
{
   BTEditor.buildVisibleTree(true);
}