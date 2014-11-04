//==============================================================================
// inspector field changed
//==============================================================================
function BehaviorTreeInspector::onInspectorFieldModified( %this, %object, %fieldName, %arrayIndex, %oldValue, %newValue )
{
   //echo("onInspectorFieldModified" SPC %object SPC %fieldName SPC %arrayIndex SPC %oldValue SPC %newValue);
   
   // lose undo if we are switching trees
   if(BTEditor.getTreeRoot(%object) != BTEditor.getRootNode())
      return;
   
   %action = BTInspectorUndoAction::create(%object, %fieldName, %arrayIndex, %oldValue, %newValue);
   %action.addToManager( BTEditor.getUndoManager() );
   
   BTEditor.updateUndoMenu();
}
