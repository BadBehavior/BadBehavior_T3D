//--- OBJECT WRITE BEGIN ---
new Root(BotTree) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new PrioritySelector() {
      internalName = "alive or dead?";
      canSave = "1";
      canSaveDynamicFields = "1";

      new Parallel() {
         returnPolicy = "REQUIRE_ALL";
         internalName = "do while alive";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Loop() {
            numLoops = "0";
            terminationPolicy = "ON_FAILURE";
            internalName = "until dead";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Condition() {
               conditionFunc = "isAlive";
               internalName = "check alive";
               canSave = "1";
               canSaveDynamicFields = "1";
            };
         };
         new Loop() {
            numLoops = "0";
            terminationPolicy = "ON_FAILURE";
            internalName = "do things";
            canSave = "1";
            canSaveDynamicFields = "1";

            new PrioritySelector() {
               internalName = "pick action";
               canSave = "1";
               canSaveDynamicFields = "1";

               new SubTree() {
                  subTreeName = "PatrolBehavior";
                  internalName = "Patrol";
                  canSave = "1";
                  canSaveDynamicFields = "1";
               };
               new PrioritySelector() {
                  internalName = "attack branch";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new Sequence() {
                     internalName = "aggressive";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Condition() {
                        conditionFunc = "seePlayer";
                        internalName = "see player";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new Command() {
                        func = "targetNearestPlayer";
                        defaultReturnStatus = "FAILURE";
                        internalName = "target player";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new SubTree() {
                        subTreeName = "WarningShotBehavior";
                        internalName = "Warning shot";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new SubTree() {
                        subTreeName = "CombatBehavior";
                        internalName = "Combat";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new SubTree() {
                        subTreeName = "VictorySaluteBehavior";
                        internalName = "victory salute";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new Command() {
                        defaultReturnStatus = "FAILURE";
                        internalName = "complete";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
                  new Delay() {
                     delayMinMs = "500";
                     delayMaxMs = "1500";
                     internalName = "before cleanup";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Sequence() {
                        internalName = "cleanup";
                        canSave = "1";
                        canSaveDynamicFields = "1";

                        new Command() {
                           func = "stop";
                           defaultReturnStatus = "SUCCESS";
                           internalName = "stop";
                           canSave = "1";
                           canSaveDynamicFields = "1";
                        };
                        new Command() {
                           func = "clearTarget";
                           defaultReturnStatus = "SUCCESS";
                           internalName = "clear target";
                           canSave = "1";
                           canSaveDynamicFields = "1";
                        };
                        new Command() {
                           func = "stopShooting";
                           defaultReturnStatus = "SUCCESS";
                           internalName = "stop shooting";
                           canSave = "1";
                           canSaveDynamicFields = "1";
                        };
                     };
                  };
               };
            };
         };
      };
      new Delay() {
         delayMinMs = "5000";
         delayMaxMs = "5000";
         internalName = "Dead";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Command() {
            defaultReturnStatus = "SUCCESS";
            internalName = "do nothing";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
      };
   };
};
//--- OBJECT WRITE END ---
