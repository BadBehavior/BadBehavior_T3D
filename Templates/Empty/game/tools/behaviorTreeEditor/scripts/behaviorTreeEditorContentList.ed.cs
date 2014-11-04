function BTEditorContentList::populate(%this)
{
   %this.clear();
   foreach(%tree in BehaviorTreeGroup)
      %this.add(%tree.name, %tree);
}

function BTEditorContentList::onSelect( %this, %tree )
{
   BTEditor.setTree( %tree );
}