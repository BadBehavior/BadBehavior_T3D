
function BehaviorTreeManager::create()
{
   if(isObject(BehaviorTreeManager))
      BehaviorTreeManager.delete();
      
   new ScriptObject(BehaviorTreeManager);
}

function BehaviorTreeManager::onAdd(%this)
{
   if(isObject(BehaviorTreeGroup))
      BehaviorTreeGroup.delete();
      
   new SimGroup(BehaviorTreeGroup);
   
   %this.loadTrees();
}

function BehaviorTreeManager::onRemove(%this)
{
   if(isObject(BehaviorTreeGroup))
      BehaviorTreeGroup.delete();
}

function BehaviorTreeManager::loadTrees(%this)
{
   if(!isDirectory("./BehaviorTrees"))
      return;
   
   pushInstantGroup(BehaviorTreeGroup);
   
   %pattern = "./BehaviorTrees/*.cs";   
   %file = findFirstFile( %pattern );
   if ( %file $= "" )
   {
      // Try for DSOs next.
      %pattern = "./BehaviorTrees/*.cs.dso";
      %file = findFirstFile( %pattern );
   }
   
   while( %file !$= "" )
   {      
      exec( %file );
      %file = findNextFile( %pattern );
   }  
   
   popInstantGroup();
}

function BehaviorTreeManager::assignTree(%this, %obj, %tree)
{
   if(!isObject(%obj))
   {
      error("BehaviorTreeManager::assignTree - object does not exist");
      return false;
   }
   
   if(!BehaviorTreeGroup.isMember(%tree))
   {
      error("BehaviorTreeManager::assignTree - tree is not a member of BehaviorTreeGroup");
      return false;
   }
   
   if(isObject(%obj.behaviorTree))
      %obj.behaviorTree.delete();
      
   %obj.behaviorTree = new BehaviorTreeRunner() {
      rootNode = %tree;
      ownerObject = %obj;
   };
}

BehaviorTreeManager::create();

