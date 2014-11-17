//--- OBJECT WRITE BEGIN ---
new Root(CombatBehavior) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new PrioritySelector() {
      canSave = "1";
      canSaveDynamicFields = "1";

      new Parallel() {
         returnPolicy = "REQUIRE_ALL";
         internalName = "combat branch";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Loop() {
            numLoops = "0";
            terminationPolicy = "ON_FAILURE";
            internalName = "check combat preconditions";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Sequence() {
               canSave = "1";
               canSaveDynamicFields = "1";

               new Condition() {
                  conditionFunc = "hasTarget";
                  internalName = "has target?";
                  canSave = "1";
                  canSaveDynamicFields = "1";
               };
               new Condition() {
                  conditionFunc = "isTargetStillHere";
                  internalName = "target still here?";
                  canSave = "1";
                  canSaveDynamicFields = "1";
               };
               new Condition() {
                  conditionFunc = "isTargetAlive";
                  internalName = "target alive?";
                  canSave = "1";
                  canSaveDynamicFields = "1";
               };
            };
         };
         new Loop() {
            numLoops = "0";
            terminationPolicy = "ON_FAILURE";
            internalName = "movement";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Sequence() {
               internalName = "move";
               canSave = "1";
               canSaveDynamicFields = "1";

               new PrioritySelector() {
                  internalName = "choose move";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new Sequence() {
                     internalName = "target too far";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Condition() {
                        conditionFunc = "isTargetTooFar";
                        internalName = "target too far?";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new Command() {
                        func = "moveForward";
                        args[0] = "10";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "move forwards";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
                  new Sequence() {
                     internalName = "target too close";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Condition() {
                        conditionFunc = "isTargetTooClose";
                        internalName = "too close?";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new Command() {
                        func = "moveBackward";
                        args[0] = "10";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "move backwards";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
                  new RandomSelector() {
                     internalName = "pick random move";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Delay() {
                        delayMinMs = "750";
                        delayMaxMs = "1250";
                        internalName = "stand still";
                        canSave = "1";
                        canSaveDynamicFields = "1";

                        new Command() {
                           func = "stop";
                           defaultReturnStatus = "SUCCESS";
                           internalName = "stand in place";
                           canSave = "1";
                           canSaveDynamicFields = "1";
                        };
                     };
                     new Command() {
                        func = "moveForward";
                        args[0] = "10";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "move fwd";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new Command() {
                        func = "moveBackward";
                        args[0] = "10";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "move back";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new Command() {
                        func = "moveLeft";
                        args[0] = "10";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "move left";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                     new Command() {
                        func = "moveRight";
                        args[0] = "10";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "move right";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
               };
               new Delay() {
                  delayMinMs = "750";
                  delayMaxMs = "1250";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new Command() {
                     func = "stop";
                     defaultReturnStatus = "SUCCESS";
                     internalName = "done";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
               };
            };
         };
         new Loop() {
            numLoops = "0";
            terminationPolicy = "ON_FAILURE";
            internalName = "shooting";
            canSave = "1";
            canSaveDynamicFields = "1";

            new SucceedAlways() {
               canSave = "1";
               canSaveDynamicFields = "1";

               new Sequence() {
                  internalName = "shoot";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new Condition() {
                     conditionFunc = "isTargetInLOS";
                     internalName = "target in LOS?";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
                  new Command() {
                     func = "startShooting";
                     defaultReturnStatus = "SUCCESS";
                     internalName = "pull trigger";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
                  new Delay() {
                     delayMinMs = "250";
                     delayMaxMs = "1000";
                     internalName = "hold trigger then release";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Command() {
                        func = "stopShooting";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "release trigger";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
               };
            };
         };
      };
      new Sequence() {
         internalName = "cleanup";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Command() {
            func = "stopShooting";
            defaultReturnStatus = "SUCCESS";
            internalName = "stop shooting";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
         new Command() {
            func = "stop";
            defaultReturnStatus = "SUCCESS";
            internalName = "stop";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
      };
   };
};
//--- OBJECT WRITE END ---
