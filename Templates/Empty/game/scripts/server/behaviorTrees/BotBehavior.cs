//--- OBJECT WRITE BEGIN ---
new Root(BotBehavior) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new PrioritySelector() {
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
               internalName = "alive?";
               canSave = "1";
               canSaveDynamicFields = "1";
            };
         };
         new Loop() {
            numLoops = "0";
            terminationPolicy = "ON_FAILURE";
            internalName = "main behavior branch";
            canSave = "1";
            canSaveDynamicFields = "1";

            new PrioritySelector() {
               internalName = "choose action";
               canSave = "1";
               canSaveDynamicFields = "1";

               new SubTree() {
                  subTreeName = "BotAttackBehavior";
                  internalName = "attack";
                  canSave = "1";
                  canSaveDynamicFields = "1";
               };
               new SubTree() {
                  subTreeName = "BotPatrolBehavior";
                  internalName = "Patrol";
                  canSave = "1";
                  canSaveDynamicFields = "1";
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
