function BTEditorStatusBar::print( %this, %message )
{
   %this.setText( %message );
   
   %sequenceNum = %this.sequenceNum + 1;
   %this.sequenceNum = %sequenceNum;
   
   %this.schedule( 4 * 1000, "clearMessage", %sequenceNum );
}

//---------------------------------------------------------------------------------------------

function BTEditorStatusBar::clearMessage( %this, %sequenceNum )
{
   // If we had no newer message in the meantime, clear
   // out the current text.
   
   %this.setText( "" );
}