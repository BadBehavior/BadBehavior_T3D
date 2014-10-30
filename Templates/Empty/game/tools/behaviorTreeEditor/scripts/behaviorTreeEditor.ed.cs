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
   
   if(BehaviorTreeGroup.getCount() > 0)
   {
      %this.open(BehaviorTreeGroup.getObject(0));
      %this.expandAll();
   }
   else
   {
      %tree = new Root(NewTree);
      BehaviorTreeGroup.add(%tree);
      %this.open(%tree);
   }
   
   %this.updateUndoMenu();
}


//==============================================================================
// VIEW
//==============================================================================
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

function BTEditor::reparent(%this, %node, %oldParent, %newParent, %position)
{
   echo("reparent" SPC %node SPC %oldParent SPC %newParent);
   %oldParent.remove(%node);
   %newParent.add(%node);
   %this.buildVisibleTree();  
}

function BTEditor::onReparent(%this, %item, %old, %new)
{
   echo("onReparent" SPC %item SPC %old SPC %new);
   
   //%act = UndoActionReparent::create(%item, %oldParent, %newParent, %oldPos, %newPos);
   //%act.actionName = "reparent object";
   //%act.addToManager( %this.getUndoManager() );
   //%this.updateUndoMenu();
}

function BTEditor::onSelect(%this, %item)
{
   echo("onSelect" TAB %item);
   echo(%this.getSelectedItem());
}

function BTEditor::onUnselect(%this, %item)
{

}

function BTEditor::onDeleteSelection(%this)
{
   echo("onDeleteSelection");
   if(%this.getSelectedItem() > 1)
      BTDeleteUndoAction::submit(%this.getSelectedObjectList());
   
   %this.clearSelection();
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
   
   // BehaviorTreeEditorStatusBar.print( "Undid '" @ %action @ "'" );
}

function BTEditor::redo(%this)
{
   %action = %this.getUndoManager().getNextRedoName();

   %this.getUndoManager().redo();
   %this.updateUndoMenu();
   
   //BehaviorTreeEditorStatusBar.print( "Redid '" @ %action @ "'" );
}