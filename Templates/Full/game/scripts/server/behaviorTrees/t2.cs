//--- OBJECT WRITE BEGIN ---
new Root(t2) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new PrioritySelector() {
      canSave = "1";
      canSaveDynamicFields = "1";

      new SubTree() {
         subTreeName = "fleeBehavior";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
      new SubTree() {
         subTreeName = "PatrolBehavior";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
   };
};
//--- OBJECT WRITE END ---
