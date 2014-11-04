$inBehaviorTreeEditor = false;

//==============================================================================
// INIT
//==============================================================================
function BTEdit( %val )
{
   if(%val != 0)
      return;

   if (!$InBehaviorTreeEditor)
   {
      if(!isObject(BTEditCanvas))
         new GuiControl(BTEditCanvas, EditorGuiGroup);
      
      BTEditor.startUp(Canvas.getContent());
      
      $InBehaviorTreeEditor = true;
   }
   else
   {
      BTEditCanvas.quit();
   }
}

function toggleBehaviorTreeEditor( %make )
{
   if( %make )
   {
      //if( EditorIsActive() && !GuiEditor.toggleIntoEditorGui )
      //   toggleEditor( true );
         
      BTEdit();
      
	  // Cancel the scheduled event to prevent
	  // the level from cycling after it's duration
	  // has elapsed.
      cancel($Game::Schedule);
   }
}

GlobalActionMap.bind( keyboard, "f9", toggleBehaviorTreeEditor );


function BTEditor::startUp(%this, %content)
{
   %this.lastContent=%content;
   Canvas.setContent( BehaviorTreeEditorGui );
   
   if(!isObject(BehaviorTreeGroup))
      new SimGroup(BehaviorTreeGroup);
   
   if(BehaviorTreeGroup.getCount() == 0)
   {
      %this.createTree("NewTree");
   }
   else
   {
      BTEditorContentList.populate();  
      BTEditorContentList.setFirstSelected();
   }
   
   %this.updateUndoMenu();
}

function BTEditor::getRootNode(%this)
{
   %rootId = %this.getFirstRootItem(); 
   %rootObj = %rootId ? %this.getItemValue(%rootId) : -1;
   return %rootObj;
}

function BTEditor::getTreeRoot(%this, %node)
{
   %current = %node;
   while(%current.getClassName() !$= "Root" && isObject(%current))
   {
      %current = %current.getGroup();
   }
   return %current;
}

function BTEditor::isDirty(%this)
{
   %undoManager = %this.getUndoManager();
   return %undoManager.getUndoCount() || %undoManager.getRedoCount();  
}

function BTEditor::createTree(%this, %name)
{
   pushInstantGroup(BehaviorTreeGroup);
   %root = new Root();
   popInstantGroup();

   %root.setFileName("");
   %root.setName(%name);
   BTEditorContentList.populate();
   BTEditorContentList.setSelected(BTEditorContentList.findText(%root.name));
}

function BTEditor::saveTree(%this, %tree)
{
   // check we actually have something to save
   if(!isObject(%tree))
      return;
      
   if((%file = %tree.getFileName()) !$= "")
   {
      %path = filePath(%file);  
   }
   else
   {
      %path = "scripts/server/behaviorTrees";
      %file = %path @ "/" @ %tree.name;
      
      if(!isDirectory(%path))
         createPath(%path @ "/");   
   }
      
   %dlg = new SaveFileDialog()
   {
      filters = "Torque script files (*.cs)|*.cs|";
      defaultPath = %path;
      defaultFile = %file;
      changePath = true;
      overwritePrompt = true;
   };
   
   if(%dlg.execute())
   {
      %tree.save(%dlg.fileName);
   }
   %dlg.delete();
}

//==============================================================================
// VIEW
//==============================================================================
function BTEditor::refresh(%this)
{
   %root = %this.getRootNode();
   %this.open(%root);
   %this.expandAll();
   %this.selectItem(1);
}

function BTEditor::setTree(%this, %tree)
{
   if(%tree != %this.getRootNode() && %this.isDirty())
   {
      MessageBoxYesNo( "Save Changes?", "Changes have been made to this tree." @
                             "Do you wish to save them?", "BTEditor.SaveTree(" @ BTEditor.getRootNode() @ ");", "");
      %this.getUndoManager().clearAll(); 
      %this.updateUndoMenu();
   }
   %this.open(%tree);
   %this.expandAll();
}

function BTEditor::expandAll(%this)
{
   for(%i=1; %i<=%this.getItemCount(); %i++)
   {
      %this.expandItem(%i);
      %this.buildVisibleTree();
   }
}

function BTEditor::collapseAll(%this)
{
   for(%i=1; %i<=%this.getItemCount(); %i++)
      %this.expandItem(%i, false);
   %this.buildVisibleTree();
}

//==============================================================================
// REPARENT
//==============================================================================
function BTEditor::onBeginReparenting(%this)
{
   if( isObject( %this.reparentUndoAction ) )
      %this.reparentUndoAction.delete();
      
   %action = BTReparentUndoAction::create( %this );
   %this.reparentUndoAction = %action;
}

function BTEditor::onReparent(%this, %item, %old, %new)
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

function BTEditor::onEndReparenting( %this )
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


//==============================================================================
// SELECT
//==============================================================================
function BTEditor::onSelect(%this, %item)
{
   BehaviorTreeInspector.inspect(%item);
}

function BTEditor::onUnselect(%this, %item)
{

}

function BTEditor::canAdd(%this, %obj, %target)
{
   if(!isObject(%target))
      return false;
      
   if( !%target.isMemberOfClass( "SimGroup" ) )
      return false;
      
   return %target.acceptsAsChild(%obj);
}

function BTEditor::isValidDragTarget(%this, %id, %obj)
{
   %selObj = %this.getSelectedObject();
   
   if(!%selObj)
      return false;
   
   return %this.canAdd(%selObj, %obj);
}

//==============================================================================
// DELETE
//==============================================================================
// onDeleteSelection is called prior to deleting the selected object. 
function BTEditor::onDeleteSelection(%this)
{
   if(%this.getSelectedItem() > 1) // not root
      BTDeleteUndoAction::submit(%this.getSelectedObject());
   
   %this.clearSelection();
   
   BTEditorStatusBar.print( "Node deleted" );
}

//==============================================================================
// UNDO
//==============================================================================
function BTEditor::getUndoManager( %this )
{
   if( !isObject( BehaviorTreeEditorUndoManager ) )
      new UndoManager( BehaviorTreeEditorUndoManager );
   
   return BehaviorTreeEditorUndoManager;
}

function BTEditor::updateUndoMenu(%this)
{
   %uman = %this.getUndoManager();
   %nextUndo = %uman.getNextUndoName();
   %nextRedo = %uman.getNextRedoName();
   
   %editMenu = BTEditCanvas.menuBar->editMenu;
   
   %editMenu.setItemName( 0, "Undo " @ %nextUndo );
   %editMenu.setItemName( 1, "Redo " @ %nextRedo );
   
   %editMenu.enableItem( 0, %nextUndo !$= "" );
   %editMenu.enableItem( 1, %nextRedo !$= "" );
}

function BTEditor::undo(%this)
{
   %action = %this.getUndoManager().getNextUndoName();
   
   %this.getUndoManager().undo();
   %this.updateUndoMenu();
   
   BTEditorStatusBar.print( "Undid '" @ %action @ "'" );
}

function BTEditor::redo(%this)
{
   %action = %this.getUndoManager().getNextRedoName();

   %this.getUndoManager().redo();
   %this.updateUndoMenu();
   
   BTEditorStatusBar.print( "Redid '" @ %action @ "'" );
}