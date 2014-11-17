//--- OBJECT WRITE BEGIN ---
new Root(WarningShotBehavior) {
   canSave = "1";
   canSaveDynamicFields = "1";

   new Sequence() {
      internalName = "warning shot sequence";
      canSave = "1";
      canSaveDynamicFields = "1";

      new Delay() {
         delayMinMs = "250";
         delayMaxMs = "500";
         internalName = "then play animation";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Command() {
            func = "setActionThread";
            args[0] = "Celebrate_01";
            defaultReturnStatus = "FAILURE";
            internalName = "play wave anim";
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
            internalName = "done";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
      };
      new Command() {
         func = "startShooting";
         defaultReturnStatus = "SUCCESS";
         internalName = "shoot";
         canSave = "1";
         canSaveDynamicFields = "1";
      };
      new Delay() {
         delayMinMs = "250";
         delayMaxMs = "500";
         internalName = "then stop shooting";
         canSave = "1";
         canSaveDynamicFields = "1";

         new Command() {
            func = "stopShooting";
            defaultReturnStatus = "SUCCESS";
            internalName = "stop shooting";
            canSave = "1";
            canSaveDynamicFields = "1";
         };
      };
      new Delay() {
         delayMinMs = "500";
         delayMaxMs = "750";
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
