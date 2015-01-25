//-----------------------------------------------------------------------------
// Copyright (c) 2014 Guy Allard
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Base frequency for behavior tree ticks (in milliseconds)
//-----------------------------------------------------------------------------
$BotTickFrequency = 100;

// ----------------------------------------------------------------------------
// make the local player invisible to the AI
//-----------------------------------------------------------------------------
function setGodMode(%val)
{
   LocalClientConnection.player.isGod = %val;
}

//-----------------------------------------------------------------------------
// bot datablock
//-----------------------------------------------------------------------------
datablock PlayerData(BadBotData : DefaultPlayerData)
{
   // max visible distance
   VisionRange = 40;
   
   // vision field of view
   VisionFov = 120;
   
   // max range to look for items
   findItemRange = 20;
   
   // the type of object to search for when looking for targets
   targetObjectTypes = $TypeMasks::PlayerObjectType;
   
   // the type of object to search for when looking for items
   itemObjectTypes = $TypeMasks::itemObjectType;
   
   // some numbers for testing
   
   // distance the bot wants to be from its target when using the Ryder
   optimalRange["Ryder"] = 8;
   
   // distance the bot wants to be from its target when using the Lurker
   optimalRange["Lurker"] = 12;
   
   // +/- deviation from optimal range that is tolerated
   rangeTolerance = 3;
   
   // probability that the bot will switch from its current target to another, closer target
   switchTargetProbability = 0.1;
   
   // number of milliseconds to hold the trigger down
   burstLength = 750;
   
   // disable other weapons, we don't know how to use them yet
   maxInv[LurkerGrenadeLauncher] = 0;
   maxInv[LurkerGrenadeAmmo] = 0;
   maxInv[ProxMine] = 0;
   maxInv[DeployableTurret] = 0;
};


//=============================================================================
// Supporting functions for an AIPlayer driven by a behavior tree
//=============================================================================

// Spawn a bot called %name located at %startpos
function BadBot::spawn(%name, %startPos)
{
   // create the bot
   %bot = new AIPlayer(%name) {
      dataBlock = BadBotData; 
      class = "BadBot";
   };
   
   // give it a name
   if(%name !$= "")
      %bot.setShapeName(%name);
   
   // set its position, or use the default if no position is given
   if(isObject(%startPos))
   {
      %startPos = %startPos.position;
   }
   else if(%startPos $= "")
   {
      %spawnPoint = pickPlayerSpawnPoint(PlayerDropPoints);
      if(isObject(%spawnPoint))
         %startPos = %spawnPoint.getPosition();
   }
   
   %bot.setPosition(%startPos);
   
   // tetherpoint will give the bot a place to call home
   %bot.tetherPoint = %startPos;
   
   // hack for aiplayer trigger index out of bounds in 3.6
   %bot.allowSprinting(false);
   
   return %bot;      
}


// override getMuzzleVector so that the bots aim at where they are looking
function BadBot::getMuzzleVector(%this, %slot)
{
   return %this.getEyeVector();
}


// use onAdd to equip the bot
function BadBotData::onAdd(%data, %obj)
{
   // give him the standard player loadout
   game.loadout(%obj);
   
   // randomly pick Ryder or Lurker
   if(getRandom(0,1))
      %obj.cycleWeapon("next");
}


// Override onDisabled so we can stop running the behavior tree
function BadBotData::onDisabled(%this, %obj, %state)
{
   %obj.behaviorTree.stop();
   Parent::onDisabled(%this, %obj, %state);
}


// simple function to spawn a given number of bots, and get them fighting each other
function botMatch(%numBots)
{
   // Avoid having lots of dead bodies lying around.
   $CorpseTimeoutValue = 2000;

   if(!isObject(BotMatch))
      new ScriptObject(botMatch);
   
   botMatch.numBots = %numBots;
   botMatch.setBehavior(botMatchTree, 250);
}


// stop the match and delete the bots
function cancelBotmatch()
{
   if(isObject(botMatch))
      botMatch.behaviorTree.postSignal("onBotmatchCancel");
}


function BotSet::onRemove(%this)
{
   cancelBotmatch();  
}


// moveTo command, %dest can be either a location or an object
function BadBot::moveTo(%this, %dest, %slowDown)
{
   %pos = isObject(%dest) ? %dest.getPosition() : %dest;
   %this.setMoveDestination(%pos, %slowDown);
   %obj.atDestination = false;
}

// forward onReachDestination to the behavior tree as a signal
function BadBotData::onReachDestination(%data, %obj)
{
   if(isObject(%obj.behaviorTree))
      %obj.behaviorTree.postSignal("onReachDestination");
      
   %obj.atDestination = true;
}

// get the index of the closest node on the specified path
function BadBot::getClosestNodeOnPath(%this, %path)
{
   if(isObject(%path) && %path.isMemberOfClass("SimSet") && (%numNodes = %path.getCount()) > 0)
   {
      %bestNode = 0;
      %bestDist = VectorDist(%path.getObject(%bestNode).position, %this.position);
      
      for(%i=1; %i < %numNodes; %i++)
      {
         %node = %path.getObject(%i);
         %dist = VectorDist(%node.position, %this.position);
         
         if(%dist < %bestDist)
         {
            %bestNode = %i;
            %bestDist = %dist;  
         }
      }
      
      return %bestNode;
   }
   return -1;
}


// send a chat message from the bot
function BadBot::say(%this, %message)
{
   chatMessageAll(%this, '\c3%1: %2', %this.getShapeName(), %message);  
}


//=============================Global Utility==================================
function RandomPointOnCircle(%center, %radius)
{
   %randVec = (getRandom() - 0.5) SPC (getRandom() - 0.5) SPC "0";
   %randVec = VectorNormalize(%randVec);
   %randVec = VectorScale(%randVec, %radius);
   return VectorAdd(%center, %randVec);  
}

//===========================ScriptedBehavior Tasks=============================
/*
   ScriptedBehavior Tasks are composed of four (optional) parts:
   1) precondition - this function should return a boolean indicating whether
      or not the behavior should continue. If precondition returns true, the
      rest of the behavior is evaluated. If precondition returns false, the
      behavior will abort.
      
      There are two options for the evaluation of the precondition that can be 
      set in the editor:
      ONCE - The precondition is run the first time the behavior becomes active
      TICK - The precondition is run each time the behavior is ticked (if latent)
   
   2) onEnter - This is called the first time the behavior is run if the 
      precondition was successful. onEnter does not use a return value.
      
   3) onExit - This is called if the behavior reaches completion. onExit does
      not use a return value.
   
   4) behavior - This is the main behavior function, evaluated each tick.
      behavior must return a status (SUCCES / FAILURE / RUNNING).
*/

//==============================================================================
// wander behavior task
//==============================================================================
function wanderTask::behavior(%this, %obj)
{
   %obj.clearAim();
   %basePoint = %obj.tetherPoint !$= "" ? %obj.tetherPoint : %obj.position;
   %obj.moveTo(RandomPointOnCircle(%basePoint, 10));
   return SUCCESS;
}


//==============================================================================
// Move to closest node task
//==============================================================================
function moveToClosestNodeTask::precondition(%this, %obj)
{
   return isObject(%obj.path);  
}

function moveToClosestNodeTask::onEnter(%this, %obj)
{
   %obj.clearAim();  
}

function moveToClosestNodeTask::behavior(%this, %obj)
{
   %obj.currentNode = %obj.getClosestNodeOnPath(%obj.path);
   %obj.moveTo(patrolPath.getObject(%obj.currentNode));
   return SUCCESS;
}

//==============================================================================
// Patrol behavior task
//==============================================================================
function patrolTask::precondition(%this, %obj)
{
//   echo("patrolTask::precondition");
   return isObject(%obj.path);
}

function patrolTask::onEnter(%this, %obj)
{
//   echo("patrolTask::onEnter");
   %obj.clearAim();
}

function patrolTask::behavior(%this, %obj)
{
   %obj.moveToNextNode();
   return SUCCESS;
}

//=============================================================================
// findHealth task
//=============================================================================
function findHealthTask::behavior(%this, %obj)
{
   // search for a health item
   %bestDist = 9999;
   %bestItem = -1;
   %db = %obj.dataBlock;
   
   initContainerRadiusSearch( %obj.position, %db.findItemRange, %db.itemObjectTypes );
   while ( (%item = containerSearchNext()) != 0 )
   {
      if(%item.dataBlock.category !$= "Health" || !%item.isEnabled() || %item.isHidden())
         continue;
      
      // broken in 3.6
      //if(%obj.checkInFov(%item, %db.visionFov))
      // this does the same thing.....
      if(VectorDot(VectorNormalize(VectorSub(%item.position, %obj.position)), %obj.getForwardVector()) > mCos(%db.visionFov / 2))
      {
         %dist = VectorDist(%obj.position, %item.position);
         if(%dist < %bestDist)
         {
            %bestItem = %item;
            %bestDist = %dist;
         }
      }
   }
   
   %obj.targetItem = %bestItem;
   
   return isObject(%obj.targetItem) ? SUCCESS : FAILURE;
}

//=============================================================================
// getHealth task
//=============================================================================
function getHealthTask::precondition(%this, %obj)
{
   return (isObject(%obj.targetItem) && %obj.targetItem.isEnabled() && !%obj.targetItem.isHidden());  
}

function getHealthTask::onEnter(%this, %obj)
{
   %obj.moveTo(%obj.targetItem.position);  
}

function getHealthTask::behavior(%this, %obj)
{
   if(!%obj.atDestination)
      return RUNNING;
   
   return SUCCESS;
}

//=============================================================================
// pickTarget task
//=============================================================================
function pickTargetTask::precondition(%this, %obj)
{
   // decide if we should pick a new target or keep the old one
   return !(isObject(%obj.targetObject) && VectorDist(%obj, %obj.targetObject) <= %obj.dataBlock.visionRange &&
      getRandom() > %obj.dataBlock.switchTargetProbability);
}

function pickTargetTask::behavior(%this, %obj)
{
   %bestDist = 9999;
   %bestTarget = -1;
   %db = %obj.dataBlock;
   
   initContainerRadiusSearch( %obj.position, %db.VisionRange, %db.targetObjectTypes );
   while ( (%target = containerSearchNext()) != 0 )
   {
      if(%target == %obj || !%target.isEnabled() || %target.isGod)
         continue;
      
      //if(%obj.checkInFov(%target, %db.visionFov)) - broken in 3.6
      // this does the same thing.....
      if(VectorDot(VectorNormalize(VectorSub(%obj.targetObject.position, %obj.position)), %obj.getForwardVector()) > mCos(%db.visionFov / 2))
      {
         %dist = VectorDist(%obj.position, %target.position);
         if(%dist < %bestDist)
         {
            %bestTarget = %target;
            %bestDist = %dist;
         }
      }
   }
   
   %obj.targetObject = %bestTarget;
   
   return SUCCESS;
}

//=============================================================================
// aimAtTargetTask
//=============================================================================
function aimAtTargetTask::precondition(%this, %obj)
{
   return isObject(%obj.targetObject) && %obj.isEnabled();
}

function aimAtTargetTask::behavior(%this, %obj)
{
   %targetPos = %obj.targetObject.getWorldBoxCenter();
   %weaponImage = %obj.getMountedImage($WeaponSlot);
   %projectile = %weaponImage.projectile;
   %correction = "0 0 1";
   if(isObject(%projectile))
   {
      // simple target leading approximation (not for ballistics)
      %targetDist = VectorDist(%targetPos, %obj.position);
      %bulletVel = %projectile.muzzleVelocity;
      %targetVel = %obj.targetObject.getVelocity();      
      %correction = VectorAdd(%correction, VectorScale( %targetVel, (%targetDist / %bulletVel) ));
   }
   %obj.setAimObject(%obj.targetObject, %correction);
   
   return SUCCESS;
}

//=============================================================================
// shootAtTargetTask
//=============================================================================
function shootAtTargetTask::precondition(%this, %obj)
{
   return isObject(%obj.targetObject) && 
          %obj.checkInLos(%obj.targetObject) &&
          VectorDot(VectorNormalize(VectorSub(%obj.getAimLocation(), %obj.position)), %obj.getForwardVector()) > 0.9 &&
          %obj.getImageAmmo($WeaponSlot);  
}

function shootAtTargetTask::behavior(%this, %obj)
{
   if(isEventPending(%obj.triggerSchedule))
      cancel(%obj.triggerSchedule);
   
   %obj.setImageTrigger(0, true);
   %obj.triggerSchedule = %obj.schedule(%obj.dataBlock.burstLength, setImageTrigger, 0, false);

   return SUCCESS;
}


//=============================================================================
// combatMoveTask
//=============================================================================
function combatMoveTask::behavior(%this, %obj)
{
   %image = %obj.getMountedImage($WeaponSlot);
   %db = %obj.getDatablock();
   %optimalRange = %db.optimalRange[%image.item.description];
   %currentRange = VectorDist(%obj.position, %obj.targetObject.position);
   %rangeDelta = %currentRange - %optimalRange;

   %moveVec = "0 0 0";
   %fwd = %obj.getForwardVector();
   %right = %obj.getRightVector();
   
   // forward / back
   if(mAbs(%rangeDelta) > %db.rangeTolerance)
      %moveVec = VectorScale(%fwd, %rangeDelta);
   
   // side
   %moveVec = VectorAdd(%moveVec, VectorScale(%right, 5 * (getRandom(0,2) - 1)));
      
   %obj.moveTo(VectorAdd(%obj.position, %moveVec));
   
   return SUCCESS;
}
