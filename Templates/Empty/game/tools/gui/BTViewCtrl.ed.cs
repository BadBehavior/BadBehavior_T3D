
function BTEdit(%root)
{
   Canvas.popDialog("BTViewCtrl");
   Canvas.pushDialog("BTViewCtrl");
   
   if(isObject(%root))
      BehaviorTreeView.open(%root);
   else if((BehaviorTreeView.getFirstRootItem() == 0) && (BehaviorTreeGroup.getCount()))
      BehaviorTreeView.open(BehaviorTreeGroup.getObject(0));
}

function BTViewCtrl::onWake(%this)
{
   if(!isObject(%this.nodeTypes))   
      %this.nodeTypes = new SimSet();
   else
      %this.nodeTypes.clear();
   
   %this.nodeTypes.add(new ScriptObject(){ type="SubTree"; isLeaf=true; });
   %this.nodeTypes.add(new ScriptObject(){ type="Sequence"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="Parallel"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="PrioritySelector"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="RandomSelector"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="Command"; isLeaf=true; });
   %this.nodeTypes.add(new ScriptObject(){ type="Condition"; isLeaf=true; });
   %this.nodeTypes.add(new ScriptObject(){ type="RunScript"; isLeaf=true; });
   %this.nodeTypes.add(new ScriptObject(){ type="Inverter"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="SucceedAlway"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="Delay"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="Ticker"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="Loop"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="Logger"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="WaitForEvent"; isLeaf=false; });
   %this.nodeTypes.add(new ScriptObject(){ type="LoopUntilEvent"; isLeaf=false; });
   
   %dropDown = %this-->BTCreateNode;
   %dropDown.clear();
   for(%i=0; %i<%this.nodeTypes.getCount(); %i++)
      %dropDown.add(%this.nodeTypes.getObject(%i).type, %i); 
   %dropDown.setFirstSelected(false);
   
   BehaviorTreeList.doMirror();
}

function BTViewCtrl::onButtonAdd(%this)
{
   %type =  %this-->BTCreateNode.getText();
   %this.createNode(%type, false);
}

function BTViewCtrl::onButtonInsert(%this)
{
   %type =  %this-->BTCreateNode.getText();
   %this.createNode(%type, true);
}

function BTViewCtrl::createNode(%this, %type, %insert)
{
   %selected = BehaviorTreeView.getSelectedObject();
   %rootId = BehaviorTreeView.getFirstRootItem(); 
   %rootObj = %rootId ? BehaviorTreeView.getItemValue(%rootId) : -1;
   
   // must have a root to add nodes to it
   if(!isObject(%rootObj))
      return;
   
   // determine the parent node
   if(isObject(%selected))
   {
      if(!%insert)
         %parent = %selected;
      else
         %parent = BehaviorTreeView.getItemValue(BehaviorTreeView.getParent(BehaviorTreeView.getSelectedItem()));
   }
   else if(!%insert)
      %parent = %rootObj;
   
   
   if(%insert && (!isObject(%parent) || !isObject(%selected)))
      return; // must have a parent and selected node to insert
   
   // cannot add another node to a leaf node
   if(isObject(%parent) && %parent.getClassName() $= "Leaf" && !%insert)
      return;
      
   // cannot create a leaf as the root node, and cannot insert leaf
   if((!isObject(%parent) || %insert) && %type $= "Leaf")
      return;
   
   pushInstantGroup(BehaviorTreeGroup);
   
   // create a node
   %node = new(%type)();

   popInstantGroup();
   
   if(!isObject(%node))
   {
      error("could not create node");
      return;
   }
   
   if(!isObject(%parent))
      BehaviorTreeView.open(%node);
   else
   {
      if(%insert)
      {
         %idx = %parent.getObjectIndex(%selected);
         %parent.remove(%selected);
         %parent.add(%node);
         %node.add(%selected);
         
         // move node to up to correct place in set
         // (surely there's an easier way than this, but I can't find it)
         if(%idx < %parent.getCount() - 1)
         {
            %sibling = %parent.getObject(%idx);            
            %parent.reorderChild(%node, %sibling);
         }
         
         BehaviorTreeView.buildVisibleTree(true);
         
         for(%i=0; %i < %parent.getCount() - %idx - 1; %i++)
            BehaviorTreeView.moveItemUp(BehaviorTreeView.findItemByObjectId(%node.getId()));         
         
         BehaviorTreeView.expandItem(BehaviorTreeView.findItemByObjectId(%node.getId()));
      }
      else
      {
         %parent.add(%node);
         BehaviorTreeView.expandItem(BehaviorTreeView.findItemByObjectId(%parent.getId()));
      }
   }
}

function BehaviorTreeView::onSelect(%this, %object)
{
   BehaviorTreeInspector.inspect(%object);
}

function BehaviorTreeView::expandAll(%this)
{
   %item = %this.getFirstRootItem();
   %this.recursiveExpand(%item);
   %this.buildVisibleTree();
}

function BehaviorTreeView::recursiveExpand(%this, %item)
{
   if(%item)
   {
      %this.expandItem(%item); 
      %this.buildVisibleTree(); 
      %this.recursiveExpand(%this.getChild(%item));
      
      %sibling = %this.getNextSibling(%item);
      while(%sibling)
      {
         %this.recursiveExpand(%sibling);
         %sibling = %this.getNextSibling(%sibling);
      }
   }
}

function BTViewCtrl::saveTree(%this)
{
   // check we actually have something to save
   %rootObj = BehaviorTreeView.getItemValue(BehaviorTreeView.getFirstRootItem());  
   if(!isObject(%rootObj))
      return;
      
   if((%file = %rootObj.getFileName()) !$= "")
   {
      %path = filePath(%file);  
   }
   else
   {
      %path = "scripts/server/behaviorTrees";
      %file = %path @ "/" @ %rootObj.name;
      
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
      %rootObj.save(%dlg.fileName);
   }
   %dlg.delete();
}

function BTViewCtrl::clearTree(%this)
{
   BehaviorTreeView.selectItem(BehaviorTreeView.getSelectedItem(), false);
   BehaviorTreeInspector.removeInspect(BehaviorTreeInspector.getInspectObject());
   BehaviorTreeView.clear();
}

function BTViewCtrl::newTree(%this)
{
   %this.clearTree();
   
   pushInstantGroup(BehaviorTreeGroup);
   %root = new Root();
   popInstantGroup();

   %root.setFileName("");

   BehaviorTreeView.open(%root);  
   BehaviorTreeList.clearSelection();
}

function BehaviorTreeList::onSelect( %this, %id, %text )
{
   %obj = %this.getItemObject( %id );   
   
   %itemNum = %this.getSelectedItem();
   if ( %itemNum == -1 )
      return;
   
   %tree = %this.getItemObject( %itemNum );   
   BTViewCtrl.clearTree();
   BehaviorTreeView.open(%tree);
}
