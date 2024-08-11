function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/AutoKeys.exe", "@StartMenuDir@/AutoKeys.lnk",
            "workingDirectory=@TargetDir@");
    }
}