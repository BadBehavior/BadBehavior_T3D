
//==============================================================================
// INIT
//==============================================================================
function BTEditCanvas::onAdd( %this )
{
   %this.onCreateMenu();
}

function BTEditCanvas::onRemove( %this )
{
   if( isObject( BehaviorTreeEditorGui.menuGroup ) )
      BehaviorTreeEditorGui.delete();

   // cleanup
   %this.onDestroyMenu();
   
   // close any open tab book pages
   BTEditorTabBook.deleteAllObjects();
}

function BTEditCanvas::quit( %this )
{
   // we must not delete a window while in its event handler, or we foul the event dispatch mechanism
   %this.schedule(10, delete);
   
   Canvas.setContent(BTEditor.lastContent);
   $InBehaviorTreeEditor = false;
}

//==============================================================================
// MENU
//==============================================================================
function BTEditCanvas::onCreateMenu(%this)
{
   if(isObject(%this.menuBar))
      return;
   
   //set up %cmdctrl variable so that it matches OS standards
   if( $platform $= "macos" )
   {
      %cmdCtrl = "cmd";
      %redoShortcut = "Cmd-Shift Z";
   }
   else
   {
      %cmdCtrl = "Ctrl";
      %redoShort = "Ctrl Y";
   }
   
   // Menu bar
   %this.menuBar = new MenuBar()
   {
      dynamicItemInsertPos = 3;
      
      new PopupMenu()
      {
         superClass = "MenuBuilder";
         barTitle = "File";
         internalName = "FileMenu";
         
         item[0] = "New Tree..." TAB %cmdCtrl SPC "N" TAB "BTEditor.createTree();";
         item[1] = "Open..." TAB %cmdCtrl SPC "O" TAB %this @ ".open();";
         item[2] = "Save Tree" TAB %cmdCtrl SPC "S" TAB "BTEditor.saveTree( BTEditor.getCurrentRootNode(), false );";
         item[3] = "Save Tree As..." TAB %cmdCtrl @ "-Shift S" TAB "BTEditor.saveTree( BTEditor.getCurrentRootNode(), true );";
         item[4] = "-";
         item[5] = "Close Editor" TAB "F9" TAB %this @ ".quit();";
         item[6] = "Quit" TAB %cmdCtrl SPC "Q" TAB "quit();";
      };

      new PopupMenu()
      {
         superClass = "MenuBuilder";
         barTitle = "Edit";
         internalName = "EditMenu";
         
         item[0] = "Undo" TAB %cmdCtrl SPC "Z" TAB "BTEditor.undo();";
         item[1] = "Redo" TAB %redoShortcut TAB "BTEditor.redo();";
      };
      
      new PopupMenu()
      {
         superClass = "MenuBuilder";
         barTitle = "View";
         internalName = "ViewMenu";
         
         item[0] = "Expand All" TAB %cmdCtrl SPC "=" TAB "BTEditor.expandAll();";
         item[1] = "Collapse All" TAB %cmdCtrl SPC "-" TAB "BTEditor.collapseAll();";
      };
      
      new PopupMenu()
      {
         superClass = "MenuBuilder";
         internalName = "HelpMenu";

         barTitle = "Help";

         Item[0] = "Help will arrive soon......";
      };
   };
   %this.menuBar.attachToCanvas(Canvas, 0);
}

function BTEditCanvas::onDestroyMenu(%this)
{
   if( !isObject( %this.menuBar ) )
      return;

   // Destroy menus      
   while( %this.menuBar.getCount() != 0 )
      %this.menuBar.getObject( 0 ).delete();
   
   %this.menuBar.removeFromCanvas();
   %this.menuBar.delete();
}

