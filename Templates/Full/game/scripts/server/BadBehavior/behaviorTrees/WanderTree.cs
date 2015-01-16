//--- OBJECT WRITE BEGIN ---
new Root(WanderTree) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new Sequence() {
      canSave = "1";
      canSaveDynamicFields = "1";

      new ScriptedBehavior() {
         preconditionMode = "TICK";
         class = "wanderTask";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
      new RandomWait() {
         waitMinMs = "1000";
         waitMaxMs = "3000";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
   };
};
//--- OBJECT WRITE END ---
