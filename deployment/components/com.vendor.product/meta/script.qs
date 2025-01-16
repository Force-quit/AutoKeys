function Component()
{
    
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    component.addOperation("CreateShortcut", "@TargetDir@/AutoKeys.exe", "@StartMenuDir@/AutoKeys.lnk", "workingDirectory=@TargetDir@");
}