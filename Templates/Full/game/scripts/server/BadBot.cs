//=============================================================================
// Supporting functions for an AIPlayer driven by a behavior tree
//=============================================================================

datablock PlayerData(BadBotData : DefaultPlayerData)
{
   shootingDelay = 2000;
};

function BadBot::spawn(%name, %startPos)
{
   %bot = new AIPlayer(%name) {
      dataBlock = BadBotData; 
      class = "BadBot";
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
   %bot.allowSprinting(false);
}

function BadBot::setBehavior(%this, %tree)
{
   BehaviorTreeManager.assignTree(%this, %tree); 
}

function BadBot::clearBehavior(%this)
{
   if(isObject(%this.behaviorTree))
      %this.behaviorTree.clear();
}

function BadBotData::onRemove(%data, %obj)
{
   if(isObject(%obj.behaviorTree))
      %obj.behaviorTree.delete();
}

//==============================Movement=======================================

function BadBot::moveTo(%this, %dest, %slowDown)
{
   %pos = isObject(%dest) ? %dest.getPosition() : %dest;
   %this.setMoveDestination(%pos, %slowDown);
   %this.atDestination = false;
}

function BadBotData::onReachDestination(%data, %obj)
{
   %obj.atDestination = true;
}


//=============================Global Utility==================================
function RandomPointOnCircle(%center, %radius)
{
   %randVec = (getRandom() - 0.5) SPC (getRandom() - 0.5) SPC "0";
   %randVec = VectorNormalize(%randVec);
   %randVec = VectorScale(%randVec, %radius);
   return VectorAdd(%center, %randVec);  
}

function BadBot::say(%this, %message)
{
   echo(%this.getShapeName() @ ": '" @ %message @ "'");  
}


//==============================================================================
// Flee Behavior Task
//==============================================================================

//------------------------------------------------------------------------------
// Precondition: 
// Check that the conditions required for this behavior are satisfied. 
//------------------------------------------------------------------------------
function fleeTask::precondition(%this, %obj)
{
   //echo("fleeTask::precondition");
   %player = localClientConnection.player;
   return %obj.isEnabled() && 
          isObject(%player) &&
          %player.isEnabled() &&
          VectorDist(%obj.position, %player.position) < 20; 
}

//------------------------------------------------------------------------------
// onEnter:
// Called prior to the main behavior evaluation.
// A good place to do any initializa``tion
//------------------------------------------------------------------------------
function fleeTask::onEnter(%this, %obj)
{
   //echo("fleeTask::onEnter");
}

//------------------------------------------------------------------------------
// behavior:
// The main behavior evaluation
//------------------------------------------------------------------------------
function fleeTask::behavior(%this, %obj)
{
   //echo("fleeTask::behavior");
   %player = localClientConnection.player;
   %awayVec = VectorSub(%obj.position, %player.position);
   %awayVec = VectorNormalize(%awayVec);
   %awayVec = VectorScale(%awayVec, 30);
   %obj.setMoveDestination(VectorAdd(%obj.position, %awayVec));
   return RUNNING;  
}

//------------------------------------------------------------------------------
// onExit:
// Called when the behavior has completed.
// A good place to do any clean-up
//------------------------------------------------------------------------------
function fleeTask::onExit(%this, %obj)
{
   //echo("fleeTask::onExit");  
}


//==============================================================================
// wander behavior task
//==============================================================================

function wanderTask::precondition(%this, %obj)
{
   //echo("wanderTask::precondition");
   %player = localClientConnection.player;
   if(isObject(%player))
      return VectorDist(%player.position, %obj.position) > 20;
   
   return true;
}

function wanderTask::onEnter(%this, %obj)
{
   //echo("wanderTask::onEnter");
   %obj.atDestination = true;   
}

function wanderTask::behavior(%this, %obj)
{
//   echo("wanderTask::behavior");
   if(%obj.atDestination)
   {
      %obj.moveTo(RandomPointOnCircle(%obj.position, 10));
   }
   return RUNNING;
}


//==============================================================================
// Patrol behavior task
//==============================================================================

function patrolTask::precondition(%this, %obj)
{
//   echo("patrolTask::precondition");
   %satisfied = %obj.isEnabled();
   
   %player = localClientConnection.player;
   if(isObject(%player) && %player.isEnabled())
      %satisfied = %satisfied && VectorDist(%player.position, %obj.position) > 20;
   
   return %satisfied;
}

function patrolTask::onEnter(%this, %obj)
{
//   echo("patrolTask::onEnter");
   %obj.clearAim();
   %obj.followPath(PatrolPath, -1);
   %obj.atDestination = 0;
}

function patrolTask::behavior(%this, %obj)
{
//   echo("patrolTask::behavior");
   if(%obj.atDestination)
   {
      %obj.moveToNextNode();
      %obj.atDestination = 0;
   }
   return RUNNING;
}

