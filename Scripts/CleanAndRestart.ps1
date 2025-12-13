# UE5 Clean Rebuild Script
# Place this script in your project root directory
# Run with: .\CleanAndRestart.ps1 -ProjectName {projectName} -UEVersion {version}
# Both parameters are optional

param(
    [string]$ProjectName = "",
    [string]$UEVersion = "5.6"  # Adjust to your UE version
)

# Get the script directory (project root)
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

# Find .uproject file if ProjectName not specified
if ($ProjectName -eq "") {
    $UProjectFiles = Get-ChildItem -Path $ProjectRoot -Filter "*.uproject"
    if ($UProjectFiles.Count -eq 0) {
        Write-Error "No .uproject file found in current directory!"
        exit 1
    }
    if ($UProjectFiles.Count -gt 1) {
        Write-Error "Multiple .uproject files found. Please specify project name with -ProjectName parameter."
        exit 1
    }
    $ProjectName = $UProjectFiles[0].BaseName
}

$UProjectFile = Join-Path $ProjectRoot "$ProjectName.uproject"
if (!(Test-Path $UProjectFile)) {
    Write-Error "Project file $UProjectFile not found!"
    exit 1
}

# Set up Unreal tool paths
$UnrealVersionSelector = "C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"

# Check if tools exist
if (!(Test-Path $UnrealVersionSelector)) {
    Write-Error "UnrealVersionSelector not found at: $UnrealVersionSelector"
    Write-Host "Please update the UE version or installation path in the script"
    exit 1
}

Write-Host "Cleaning project: $ProjectName" -ForegroundColor Green

# 1. Delete .vs folder
$VSFolder = Join-Path $ProjectRoot ".vs"
if (Test-Path $VSFolder) {
    Write-Host "Deleting .vs folder..." -ForegroundColor Yellow
    Remove-Item $VSFolder -Recurse -Force
}

# 2. Delete Binaries folder
$BinariesFolder = Join-Path $ProjectRoot "Binaries"
if (Test-Path $BinariesFolder) {
    Write-Host "Deleting Binaries folder..." -ForegroundColor Yellow
    Remove-Item $BinariesFolder -Recurse -Force
}

# 3. Delete Intermediate folder
$IntermediateFolder = Join-Path $ProjectRoot "Intermediate"
if (Test-Path $IntermediateFolder) {
    Write-Host "Deleting Intermediate folder..." -ForegroundColor Yellow
    Remove-Item $IntermediateFolder -Recurse -Force
}

# 4. Generate Visual Studio project files
Write-Host "Generating Visual Studio project files..." -ForegroundColor Yellow
$process = Start-Process -FilePath $UnrealVersionSelector -ArgumentList "/projectfiles", $UProjectFile -Wait -PassThru -NoNewWindow
if ($process.ExitCode -ne 0) {
    Write-Error "Failed to generate project files. Exit code: $($process.ExitCode)"
    exit 1
}

# 5. Launch the .uproject file
Write-Host "Launching Unreal Editor..." -ForegroundColor Green
Start-Process $UProjectFile

# 6. Open solution (optional)
$SolutionFile = Join-Path $ProjectRoot "$ProjectName.sln"
if (Test-Path $SolutionFile) {
    Write-Host "Opening solution..." -ForegroundColor Green
    Start-Process $SolutionFile
}

Write-Host "Clean rebuild process completed!" -ForegroundColor Green