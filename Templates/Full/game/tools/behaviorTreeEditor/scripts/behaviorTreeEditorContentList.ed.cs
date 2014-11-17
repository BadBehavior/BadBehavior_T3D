function BTEditorContentList::refresh(%this)
{
   %this.clear();
   foreach(%tree in BehaviorTreeGroup)
      %this.add(%tree.name, %tree);
}

function BTEditorContentList::onSelect( %this, %tree )
{
   BTEditor.viewTree( %tree );
}



function BTEditor::newPage()
{
   %page = new GuiTabPageCtrl() {
      fitBook = "1";
      maxLength = "1024";
      docking = "client";
      margin = "-1 0 0 0";
      padding = "0 0 0 0";
      anchorTop = "1";
      anchorBottom = "0";
      anchorLeft = "1";
      anchorRight = "0";
      position = "0 0   ";
      extent = "200 200";
      minExtent = "8 2";
      horizSizing = "width";
      vertSizing = "height";
      profile = "GuiTabPageProfile";
      visible = "1";
      active = "1";
      tooltipProfile = "GuiToolTipProfile";
      hovertime = "1000";
      isContainer = "1";
      canSave = "0";
      canSaveDynamicFields = "0";
      
      new GuiScrollCtrl() {
         willFirstRespond = "1";
         hScrollBar = "dynamic";
         vScrollBar = "alwaysOn";
         lockHorizScroll = "0";
         lockVertScroll = "0";
         constantThumbHeight = "0";
         childMargin = "1 1";
         mouseWheelScrollSpeed = "-1";
         margin = "0 0 0 0";
         padding = "0 0 0 0";
         anchorTop = "1";
         anchorBottom = "0";
         anchorLeft = "1";
         anchorRight = "0";
         position = "0 0";
         extent = "400 400";
         minExtent = "8 2";
         horizSizing = "width";
         vertSizing = "height";
         profile = "GuiScrollProfile";
         visible = "1";
         active = "1";
         tooltipProfile = "GuiToolTipProfile";
         hovertime = "1000";
         isContainer = "1";
         canSave = "0";
         docking="client";
         canSaveDynamicFields = "0";

         new GuiBehaviorTreeViewCtrl() {
            internalName="BTView";
            tabSize = "17";
            textOffset = "8";
            fullRowSelect = "0";
            itemHeight = "21";
            destroyTreeOnSleep = "0";
            mouseDragging = "1";
            multipleSelections = "0";
            deleteObjectAllowed = "1";
            dragToItemAllowed = "1";
            clearAllOnSingleSelection = "1";
            showRoot = "1";
            useInspectorTooltips = "0";
            tooltipOnWidthOnly = "0";
            showObjectIds = "0";
            showClassNames = "1";
            showObjectNames = "1";
            showInternalNames = "1";
            showClassNameForUnnamedObjects = "0";
            compareToObjectID = "1";
            canRenameObjects = "1";
            renameInternal = "0";
            position = "1 1";
            extent = "267 231";
            minExtent = "8 2";
            horizSizing = "right";
            vertSizing = "bottom";
            profile = "GuiBehaviorTreeViewProfile";
            visible = "1";
            active = "1";
            tooltipProfile = "GuiToolTipProfile";
            hovertime = "1000";
            isContainer = "1";
            canSave = "0";
            canSaveDynamicFields = "";
         };
      };
   };
   return %page;
}