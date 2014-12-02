//=============================================================================
// Supporting functions for an AIPlayer driven by a behavior tree
//=============================================================================

datablock PlayerData(BadBotData : DefaultPlayerData)
{
   shootingDelay = 2000;
};

function BadBotData::onRemove(%this, %obj)
{
   if(isObject(%obj.behaviorTree))
      %obj.behaviorTree.delete();
}


function BadBot::spawn(%name, %startPos)
{
   %bot = new AIPlayer(%name) {
      dataBlock = BadBotData; 
      class = BadBot;
   };
   
   if(%name !$= "")
      %bot.setShapeName(%name);
   
   if(%startPos $= "")
   {
      %spawnPoint = pickPlayerSpawnPoint(PlayerDropPoints);
      if(isObject(%spawnPoint))
         %startPos = %spawnPoint.getPosition();
   }
   
   %bot.setPosition(%startPos);      
}

function BadBot::setBehavior(%this, %tree)
{
   BehaviorTreeManager.assignTree(%this, %tree); 
}