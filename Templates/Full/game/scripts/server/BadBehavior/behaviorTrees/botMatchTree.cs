//--- OBJECT WRITE BEGIN ---
new Root(botMatchTree) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new Selector() {
      canSave = "1";
      canSaveDynamicFields = "1";

      new Sequence() {
         internalName = "run the match";
         canSave = "1";
         canSaveDynamicFields = "1";

         new ScriptEval() {
            behaviorScript = "if(isObject(%obj.botGroup))\n   %obj.botGroup.delete();\n\n%obj.botGroup = new SimGroup();\n%obj.countdown = 5;\n%obj.numBotsToSpawn = %obj.numBots;";
            defaultReturnStatus = "SUCCESS";
            internalName = "init";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
         new Sequence() {
            internalName = "countdown";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Ticker() {
               frequencyMs = "1000";
               canSave = "1";
               canSaveDynamicFields = "1";

               new Loop() {
                  numLoops = "5";
                  terminationPolicy = "ON_FAILURE";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new ScriptEval() {
                     behaviorScript = "centerPrintAll(\"BotMatch in\" SPC %obj.countdown, 2);\n%obj.countdown --;";
                     defaultReturnStatus = "SUCCESS";
                     internalName = "countdown message";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
               };
            };
            new ScriptEval() {
               behaviorScript = "centerPrintAll(\"GO!\",1);";
               defaultReturnStatus = "SUCCESS";
               internalName = "lets go";
               canSave = "1";
               canSaveDynamicFields = "1";
            };
         };
         new SucceedAlways() {
            canSave = "1";
            canSaveDynamicFields = "1";

            new Loop() {
               numLoops = "0";
               terminationPolicy = "ON_FAILURE";
               canSave = "1";
               canSaveDynamicFields = "1";

               new Sequence() {
                  internalName = "spawn bots";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new ScriptEval() {
                     behaviorScript = "%spawnpoint = PatrolPath.getRandom();\n%bot = BadBot::spawn(\"\", %spawnpoint);\n%bot.setbehavior(BotTree, $BotTickFrequency);\n%obj.botGroup.add(%bot);\n%obj.numBotsToSpawn --;";
                     defaultReturnStatus = "SUCCESS";
                     internalName = "spawn one bot";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
                  new ScriptEval() {
                     behaviorScript = "if(%obj.numBotsToSpawn == 0) return FAILURE;";
                     defaultReturnStatus = "SUCCESS";
                     internalName = "check if more bots to spawn";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
               };
            };
         };
         new FailAlways() {
            canSave = "1";
            canSaveDynamicFields = "1";

            new WaitForSignal() {
               signalName = "onBotmatchCancel";
               timeoutMs = "10000";
               internalName = "stop on cancel signal";
               canSave = "1";
               canSaveDynamicFields = "1";
            };
         };
      };
      new Sequence() {
         internalName = "end the match";
         canSave = "1";
         canSaveDynamicFields = "1";

         new ScriptEval() {
            behaviorScript = "%obj.botGroup.delete();";
            defaultReturnStatus = "SUCCESS";
            internalName = "remove bots";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
         new ScriptEval() {
            behaviorScript = "%obj.behaviorTree.schedule(10, stop);";
            defaultReturnStatus = "SUCCESS";
            internalName = "stop";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
      };
   };
};
//--- OBJECT WRITE END ---
