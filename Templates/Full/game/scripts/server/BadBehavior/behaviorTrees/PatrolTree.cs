//--- OBJECT WRITE BEGIN ---
new Root(PatrolTree) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new Sequence() {
      canSave = "1";
      canSaveDynamicFields = "1";

      new ScriptedBehavior() {
         preconditionMode = "TICK";
         internalName = "patrol move";
         class = "patrolTask";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
   };
};
//--- OBJECT WRITE END ---
