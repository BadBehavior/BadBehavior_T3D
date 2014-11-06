function GuiBehaviorTreeViewCtrl::getRootNode(%this)
{
   %rootId = %this.getFirstRootItem(); 
   %rootObj = %rootId ? %this.getItemValue(%rootId) : -1;
   return %rootObj;
}

function GuiBehaviorTreeViewCtrl::getUndoManager( %this )
{
   if( !isObject( %this.undoManager ) )
   {
      %this.undoManager = new UndoManager(){ numLevels=20; };
      %this.undoManager.setClassNamespace("BTEditorUndoManager");
   }

   return %this.undoManager;
}

function GuiBehaviorTreeViewCtrl::onRemove(%this)
{
   if(isObject(%this.undoManager))
      %this.undoManager.delete();
}

function GuiBehaviorTreeViewCtrl::expandAll(%this)
{
   for(%i=1; %i<=%this.getItemCount(); %i++)
   {
      %this.expandItem(%i);
      %this.buildVisibleTree();
   }
}

function GuiBehaviorTreeViewCtrl::collapseAll(%this)
{
   for(%i=1; %i<=%this.getItemCount(); %i++)
      %this.expandItem(%i, false);
   %this.buildVisibleTree();
}

function GuiBehaviorTreeViewCtrl::refresh(%this)
{
   %root = %this.getRootNode();
   %this.open(%root);
   %this.expandAll();
   %this.selectItem(1);
}

//==============================================================================
// DELETE
//==============================================================================
// onDeleteSelection is called prior to deleting the selected object. 
function GuiBehaviorTreeViewCtrl::onDeleteSelection(%this)
{
   if(%this.getSelectedItem() > 1) // not root
      BTDeleteUndoAction::submit(%this.getSelectedObject());
   
   %this.clearSelection();
   
   BTEditorStatusBar.print( "Node deleted" );
}

//==============================================================================
// SELECT
//==============================================================================
function GuiBehaviorTreeViewCtrl::onSelect(%this, %item)
{
   BehaviorTreeInspector.inspect(%item);
}

function GuiBehaviorTreeViewCtrl::onBeginReparenting(%this)
{
   if( isObject( %this.reparentUndoAction ) )
      %this.reparentUndoAction.delete();
      
   %action = BTReparentUndoAction::create( %this );
   %this.reparentUndoAction = %action;
}

function GuiBehaviorTreeViewCtrl::onReparent(%this, %item, %old, %new)
{
   if( !isObject(%this.reparentUndoAction) ||
       %this.reparentUndoAction.node != %item )
   {
      warn( "Reparenting undo is borked :(" );
      if(isObject(%this.reparentUndoAction))
      {
         %this.reparentUndoAction.delete();
         %this.reparentUndoAction="";
      }
   }
   else
   {       
      %this.reparentUndoAction.oldParent = %old;
      %this.reparentUndoAction.newParent = %new;
      %this.reparentUndoAction.newPosition = %new.getObjectIndex(%item);
   }
}

function GuiBehaviorTreeViewCtrl::onEndReparenting( %this )
{
   %action = %this.reparentUndoAction;
   %this.reparentUndoAction = "";
   
   // Check that the reparenting went as planned, and undo it right now if not
   if(%action.node.getGroup() != %action.newParent)
   {
      %action.undo();
      %action.delete();
   }
   else
   {
      %action.addToManager( %this.getUndoManager() );
      BTEditorStatusBar.print( "Moved node" );
   }
}

function GuiBehaviorTreeViewCtrl::canAdd(%this, %obj, %target)
{
   if(!isObject(%target))
      return false;
      
   if( !%target.isMemberOfClass( "SimGroup" ) )
      return false;
      
   return %target.acceptsAsChild(%obj);
}

function GuiBehaviorTreeViewCtrl::isValidDragTarget(%this, %id, %obj)
{
   %selObj = %this.getSelectedObject();
   
   if(!%selObj)
      return false;
   
   return %this.canAdd(%selObj, %obj);
}






function BTInsertMenuBar::onAdd(%this)
{
   %this.addMenu(0, "A Menu");
   %this.addMenuItem(0, "Do Something");  
}
