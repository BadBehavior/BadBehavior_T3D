//--- OBJECT WRITE BEGIN ---
new Root(WanderTree) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new Sequence() {
      canSave = "1";
      canSaveDynamicFields = "1";

      new ScriptedBehavior() {
         preconditionMode = "ONCE";
         class = "wanderTask";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
      new WaitForSignal() {
         signalName = "onReachDestination";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
   };
};
//--- OBJECT WRITE END ---
