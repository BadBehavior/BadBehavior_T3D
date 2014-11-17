//==============================================================================
// inspector field changed
//==============================================================================
function BehaviorTreeInspector::onInspectorFieldModified( %this, %object, %fieldName, %arrayIndex, %oldValue, %newValue )
{
   //echo("onInspectorFieldModified" SPC %object SPC %fieldName SPC %arrayIndex SPC %oldValue SPC %newValue);
   
   // select the correct page
   %treeRoot = BTEditor.getTreeRoot(%object);
   %targetView = -1;
   foreach(%page in BTEditorTabBook)
   {
      %view = %page-->BTView;
      if(%view.getRootNode() == %treeRoot)
         %targetView = %view;
   }
   
   if(!isObject(%targetView))
   {
      warn("OOPS - something went wrong with the inspector!");
      return;
   }
   
   %action = BTInspectorUndoAction::create(%object, %fieldName, %arrayIndex, %oldValue, %newValue);
   %action.addToManager( %targetView.getUndoManager() );
   
   BTEditor.updateUndoMenu();
}
