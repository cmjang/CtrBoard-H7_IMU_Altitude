# Verify a name of a project folder
#  Invoke from CMD: powershell.exe \VerifyFolderName.ps1 -uvprojxPath D:\ProjectFolder\Blinky.uvprojx -ftlPath ..STM32H7xx_DFP\3.0.0\MDK\CubeMX\DC 

param 
(
    $uvprojxPath,       # uvprojx path
    $ftlTemplate        # ftl template folder path
)
    
Write-Output "******* Script: Verify project folder name *******"

$ret = "FolderNameOk"

# Check if Dual-core device
#    For dual core devices ftl template folder is ..MDK\CubeMX\DC
$ftlFolder = split-path -Path $ftlTemplate -leaf
$projFolderPath = split-path -Path $uvprojxPath

if ($ftlFolder -eq "DC")
{
    # ftlTemplate folder name is "DC" -> dual core device

    # uVision project folder name must be "CM4" or "CM7"
    $projFolder = split-path -Path $projFolderPath -leaf
    if (($projFolder -ne "CM4") -and ($projFolder -ne "CM7"))
    {
        # Display the PopUp window to notify the user about wrong folder name
        $wshell = New-Object -ComObject Wscript.Shell
$popUpMessage = 
@'
The project folder names are predefined for STM32H7xx dual core devices.

Please ensure that the uVision project for Cortex-M4 is in a folder named CM4 and uVision project for Cortex-M7 is in a folder named CM7.

Note that both CM4 and CM7 project folders must be in the same parent folder.
'@
        $Output = $wshell.Popup($popUpMessage, 0, "Incorrect project folder name", 0 + 16 + 4096 + 65536)        

        $ret = "FolderNameError"    
    }
}

# Save result in temp file
$resultFile = "VerifyProjectFolderName_Result.txt"
if (Test-Path -Path ($projFolderPath + "\" + $resultFile))
{
    Remove-Item -Path ($projFolderPath + "\" + $resultFile) -Force
}
New-Item -Path $projFolderPath -Name $resultFile -ItemType File -Value $ret
