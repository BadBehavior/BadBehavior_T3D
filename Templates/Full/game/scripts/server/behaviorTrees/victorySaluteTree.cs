//--- OBJECT WRITE BEGIN ---
new Root(VictorySaluteBehavior) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new Parallel() {
      returnPolicy = "REQUIRE_ONE";
      internalName = "victory salute";
      canSave = "1";
      canSaveDynamicFields = "1";

      new Loop() {
         numLoops = "0";
         terminationPolicy = "ON_FAILURE";
         internalName = "victory salute preconditions";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Sequence() {
            canSave = "1";
            canSaveDynamicFields = "1";

            new Condition() {
               conditionFunc = "hasTarget";
               internalName = "target still exists?";
               canSave = "1";
               canSaveDynamicFields = "1";
            };
            new Inverter() {
               internalName = "is target dead?";
               canSave = "1";
               canSaveDynamicFields = "1";

               new Condition() {
                  conditionFunc = "isTargetAlive";
                  internalName = "target alive?";
                  canSave = "1";
                  canSaveDynamicFields = "1";
               };
            };
         };
      };
      new Sequence() {
         internalName = "victory salute";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Inverter() {
            internalName = "approach player";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Loop() {
               numLoops = "0";
               terminationPolicy = "ON_FAILURE";
               internalName = "until reached player";
               canSave = "1";
               canSaveDynamicFields = "1";

               new Sequence() {
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new Inverter() {
                     internalName = "approach?";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Condition() {
                        conditionFunc = "isTargetTooClose";
                        internalName = "target too close?";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
                  new Command() {
                     func = "moveForward";
                     args[0] = "10";
                     defaultReturnStatus = "SUCCESS";
                     internalName = "move";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
                  new Delay() {
                     delayMinMs = "500";
                     delayMaxMs = "500";
                     internalName = "pause after move";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Command() {
                        defaultReturnStatus = "SUCCESS";
                        internalName = "Null";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
               };
            };
         };
         new Command() {
            func = "stop";
            defaultReturnStatus = "SUCCESS";
            internalName = "stop";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
         new Delay() {
            delayMinMs = "250";
            delayMaxMs = "500";
            internalName = "pause then salute";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Command() {
               func = "setActionThread";
               args[0] = "Celebrate_01";
               defaultReturnStatus = "FAILURE";
               internalName = "play anim";
               canSave = "1";
               canSaveDynamicFields = "1";
            };
         };
         new WaitForEvent() {
            event = "animationDone";
            internalName = "animationDone";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Command() {
               defaultReturnStatus = "SUCCESS";
               internalName = "finished";
               canSave = "1";
               canSaveDynamicFields = "1";
            };
         };
      };
   };
};
//--- OBJECT WRITE END ---
