//--- OBJECT WRITE BEGIN ---
new Root(PatrolBehavior) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new PrioritySelector() {
      canSave = "1";
      canSaveDynamicFields = "1";

      new Parallel() {
         returnPolicy = "REQUIRE_ALL";
         internalName = "patrol while player not seen";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Inverter() {
            internalName = "fail if we see player";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Loop() {
               numLoops = "0";
               terminationPolicy = "ON_SUCCESS";
               internalName = "until player seen";
               canSave = "1";
               canSaveDynamicFields = "1";

               new Condition() {
                  conditionFunc = "seePlayer";
                  internalName = "see player?";
                  canSave = "1";
                  canSaveDynamicFields = "1";
               };
            };
         };
         new Loop() {
            numLoops = "0";
            terminationPolicy = "ON_FAILURE";
            internalName = "do patrol";
            canSave = "1";
            canSaveDynamicFields = "1";

            new Sequence() {
               internalName = "Patrol sequence";
               canSave = "1";
               canSaveDynamicFields = "1";

               new Sequence() {
                  internalName = "move";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new Command() {
                     func = "moveAlongPath";
                     args[0] = "PatrolPath";
                     defaultReturnStatus = "FAILURE";
                     internalName = "start moving on path";
                     canSave = "1";
                     canSaveDynamicFields = "1";
                  };
                  new WaitForEvent() {
                     event = "onReachDestination";
                     internalName = "onReachDestination";
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
               new Sequence() {
                  internalName = "Look";
                  canSave = "1";
                  canSaveDynamicFields = "1";

                  new Delay() {
                     delayMinMs = "250";
                     delayMaxMs = "1500";
                     internalName = "pause";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Command() {
                        func = "alignWithCurrentNode";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "Look";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
                  new Delay() {
                     delayMinMs = "250";
                     delayMaxMs = "1500";
                     internalName = "pause again";
                     canSave = "1";
                     canSaveDynamicFields = "1";

                     new Command() {
                        func = "clearAim";
                        defaultReturnStatus = "SUCCESS";
                        internalName = "clear aim";
                        canSave = "1";
                        canSaveDynamicFields = "1";
                     };
                  };
               };
            };
         };
      };
      new Sequence() {
         internalName = "patrol cleanup";
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
            func = "clearNode";
            defaultReturnStatus = "SUCCESS";
            internalName = "clear node";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
         new Command() {
            func = "clearAim";
            defaultReturnStatus = "FAILURE";
            internalName = "clear aim";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
      };
   };
};
//--- OBJECT WRITE END ---
