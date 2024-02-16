# Migration script
#  Invoke from CMD: powershell.exe \MigrateProject.ps1 -deviceName STM32H7B3LIHxQ -uvprojxPath D:\ProjectFolder\Blinky.uvprojx

param 
(
    $deviceName,        # Device name
    $uvprojxPath        # uvprojx path
)

    
Write-Output "**************** Migration Script ****************"

# Project is not compatible with current DFP        
Write-Output "Project was created with an older version of the STM32H7xx_DFP and is not compatible."

# Display the PopUp window to notify the user and decide whether to migrate to new DFP.
$wshell = New-Object -ComObject Wscript.Shell
$popUpMessage = 
@'
Project was created with an older version of the STM32H7xx_DFP and is not compatible.
Do you want to migrate the project to new STM32H7xx_DFP?
'@
$Output = $wshell.Popup($popUpMessage, 0, "Note", 4 + 32 + 4096 + 65536)

$ret = "ProjectMigrationFailed"
$path = Split-Path -Path $uvprojxPath

if ($Output -eq 6)
{
    #Migration confirmed by the user            
    Write-Output "Migrating to new DFP..."

    # Close all uVision project instances
    $ReopenUVision = 0
    [System.Diagnostics.Process[]]$uVisionProcess = Get-Process UV4 -ErrorAction SilentlyContinue | Where-Object {$_.MainWindowTitle -and $_.MainWindowTitle -like ("*" + $uvprojxPath + "*")}
    foreach ($uVision in $uVisionProcess)
    {
        Write-Output ("Close " + $uvprojxPath + ".")
        $uVision.CloseMainWindow()
        Sleep 3
        if (!$uVision.HasExited)
        {
            $uVision | Stop-Process -Force
        }
        $ReopenUVision = 1
    }

    $oldPath = $path + "\RTE\Device\" + $deviceName

    # Update FrameworkCubeMX.gpdsc
    Write-Output ("Update " + $oldPath + "\FrameworkCubeMX.gpdsc.")
    # Remove "<command>..." line 
    $Content = Get-Content ($oldPath + "\FrameworkCubeMX.gpdsc") | Where-Object {$_ -notlike ("*<command>*")}

    # Replace "<workingDir>.." line
    $LineNumber = $Content | Select-String "<workingDir>" | Select-Object -ExpandProperty 'LineNumber'
    $Content[$LineNumber-1] = 
@'
      <workingDir>$PSTM32CubeMX</workingDir>                      <!-- path is specified either absolute or relative to PDSC or GPDSC file -->
      <exe>
        <command>$SMDK/CubeMX/STM32CubeMxLauncher.exe</command>
        <argument>$D</argument>                                   <!-- D = Device (Dname/Dvariant as configured by environment) -->
        <argument>#P</argument>                                   <!-- Project path and project name (as configured by environment) -->
        <argument>$SMDK/CubeMX/SC</argument>                      <!-- absolute or relative to ftl templates. $S = Device Family Pack base folder -->
      </exe>
'@

    # Update generator component version
    $LineNumber = $Content | Select-String "<component\s+generator\s*=\s*`"STM32CubeMX`"" | Select-Object -ExpandProperty 'LineNumber'
    $Content[$LineNumber-1] = 
@'
    <component generator="STM32CubeMX" Cvendor="Keil" Cclass="Device" Cgroup="STM32Cube Framework" Csub="STM32CubeMX" Cversion="1.2.0" condition="STCubeMX">
'@
    
    # Save updated FrameworkCubeMX.gpdsc
    Set-Content ($oldPath + "\FrameworkCubeMX.gpdsc") -value $Content

    # Move STM32CubeMX things from "..RTE\Device\Device name\" to "..STM32CubeMX"
    Write-Output ("Move STM32CubeMX files to " + $path + "\STM32CubeMX.")
    if (!(Test-Path -Path ($path + "\STM32CubeMX")))
    {
        New-Item  -Path  $path -Name "STM32CubeMX" -ItemType "directory"
    }
    Move-Item -path ($oldPath + "\STCubeGenerated") -Destination ($path + "\STM32CubeMX\STCubeGenerated")
    Move-Item -path ($oldPath + "\FrameworkCubeMX.gpdsc") -Destination ($path + "\STM32CubeMX\FrameworkCubeMX.gpdsc")
    Move-Item -path ($oldPath + "\MX_Device.h") -Destination ($path + "\STM32CubeMX\MX_Device.h")

    # Modify_uVisionProject - uvprojx
    Write-Output ("Update " + $uvprojxPath + ".")
    #Copy-Item -Path $uvprojxPath -Destination ($uvprojxPath + "_old")
    $Content = (Get-Content $uvprojxPath -Raw) -replace "(?ms)^\s*<Group>\s*(\r|\n|\r\n|\n\r)\s*<GroupName>:STM32CubeMX:Common Sources</GroupName>.*?</Group>\s*(\r|\n|\r\n|\n\r)", ''
    $Content = $Content -replace "<gpdsc name=`"RTE.*?gpdsc`">", '<gpdsc name="STM32CubeMX\FrameworkCubeMX.gpdsc">'
    Set-Content $uvprojxPath -value $Content

    # Modify_uVisionProject - uvoptx
    $uvoptxPath = $uvprojxPath -replace ".uvprojx", ".uvoptx"
    Write-Output ("Update " + $uvoptxPath + ".")
    #Copy-Item -Path $uvoptxPath -Destination ($uvoptxPath + "_old")
    (Get-Content $uvoptxPath -Raw) -replace "(?ms)^\s*<Group>\s*(\r|\n|\r\n|\n\r)\s*<GroupName>:STM32CubeMX:Common Sources</GroupName>.*?</Group>\s*(\r|\n|\r\n|\n\r)", '' | Set-Content $uvoptxPath

    #Reopen uVision
    if ($ReopenUVision -eq 1)
    {
        Write-Output "Reopen $uvprojxPath."
        Start-Process -FilePath "$uvprojxPath"
    }
    
    $ret = "ProjectMigrationSucceeded"
}

# Save result in temp file
New-Item -Path $path -Name "MigrateProject_Result.txt" -ItemType File -Value $ret
