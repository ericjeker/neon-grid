# GameContentSync.ps1 - PowerShell script for synchronizing game content using rclone

[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)]
    [string]$SourcePath,

    [Parameter(Mandatory=$true)]
    [string]$DestinationPath,

    [string]$RclonePath = "rclone",
    [switch]$DryRun,
    [switch]$ShowProgress,
    [switch]$DeleteExtraneous,
    [switch]$Compress,
    [string[]]$ExcludePatterns = @(),
    [string]$LogFile = "",
    [string]$ConfigFile = "",
    [int]$Transfers = 4,
    [int]$Checkers = 8,
    [string]$BandwidthLimit = "",
    [string]$SyncMode = "sync"  # sync, copy, or move
)

# Function to check if rclone is available
function Test-RcloneAvailable {
    try {
        $null = & $RclonePath version 2>$null
        return $true
    }
    catch {
        return $false
    }
}

# Function to build rclone arguments
function Build-RcloneArgs {
    param(
        [string]$Mode,
        [string]$Source,
        [string]$Destination,
        [bool]$IsDryRun,
        [bool]$IsVerbose,
        [bool]$ShouldShowProgress,
        [bool]$ShouldCompress,
        [string[]]$Excludes,
        [string]$LogPath,
        [string]$ConfigPath,
        [int]$TransferCount,
        [int]$CheckerCount,
        [string]$Bandwidth
    )

    
    $arguments = @($Mode)

    # Add source and destination
    
    $arguments += $Source
    
    $arguments += $Destination

    # Add dry-run flag if specified
    if ($IsDryRun) {
        
        $arguments += "--dry-run"
    }

    # Add verbose flag
    if ($IsVerbose) {
        
        $arguments += "-v"
    }

    # Add progress flag
    if ($ShouldShowProgress) {
        
        $arguments += "--progress"
    }

    # Add compression if specified
    if ($ShouldCompress) {
        
        $arguments += "--compress"
    }

    # Add exclude patterns
    foreach ($pattern in $Excludes) {
        
        $arguments += "--exclude"
        
        $arguments += $pattern
    }

    # Add log file if specified
    if ($LogPath -and $LogPath -ne "") {
        
        $arguments += "--log-file"
        
        $arguments += $LogPath
    }

    # Add config file if specified
    if ($ConfigPath -and $ConfigPath -ne "") {
        
        $arguments += "--config"
        
        $arguments += $ConfigPath
    }

    # Add transfers and checkers
    
    $arguments += "--transfers"
    
    $arguments += $TransferCount.ToString()
    
    $arguments += "--checkers"
    
    $arguments += $CheckerCount.ToString()

    # Add bandwidth limit if specified
    if ($Bandwidth -and $Bandwidth -ne "") {
        
        $arguments += "--bwlimit"
        
        $arguments += $Bandwidth
    }

    # Additional useful flags
    
    $arguments += "--checksum"  # Use checksums for verification
    
    $arguments += "--stats"     # Show transfer stats
    
    $arguments += "1s"          # Update stats every second

    return $arguments
}

# Main script execution
Write-Host ""
Write-Host "Game Content Synchronization Script (rclone)" -ForegroundColor Green
Write-Host "=============================================" -ForegroundColor Green
Write-Host ""

# Check if rclone is available
if (-not (Test-RcloneAvailable)) {
    Write-Error "rclone is not available or not found in PATH. Please install rclone or specify the correct path with -RclonePath parameter."
    Write-Host "You can install rclone from: https://rclone.org/downloads/"
    Write-Host "Or use chocolatey: choco install rclone"
    Write-Host "Or use scoop: scoop install rclone"
    exit 1
}

# Validate source path (for local sources)
if (-not $SourcePath.Contains(":") -and -not (Test-Path $SourcePath)) {
    Write-Error "Local source path does not exist: $SourcePath"
    exit 1
}

# Validate sync mode
$validModes = @("sync", "copy", "move")
if ($SyncMode -notin $validModes) {
    Write-Error "Invalid sync mode. Valid modes are: $($validModes -join ', ')"
    exit 1
}

# Common game content exclusions
$defaultExcludes = @(
    # Exclude folders
    ".git/**",
    ".idea/**",
    ".vs/**",
    "Binaries/**",
    "Build/**",
    "Config/**",
    "DerivedDataCache/**",
    "Intermediate/**",
    "Saved/**",
    "Scripts/**",
    "Source/**",
    # Content folders specific exclusions
    "__ExternalActors__/**/*.tmp",
    "__ExternalObjects__/**/*.tmp"
    # Exclude files
    "*.tmp",
    "*.log",
    "*.cache",
    "Thumbs.db",
    ".DS_Store",
    "*.pdb",
    "*.ilk",
    "*.sln.DotSettings.user",
    "*.uasset.tmp",
    "*.umap.tmp"
)

# Merge default excludes with user-specified excludes
$allExcludes = $defaultExcludes + $ExcludePatterns

# Display sync information
Write-Host ""
Write-Host "Sync Configuration:" -ForegroundColor Cyan
Write-Host "  Mode:          $SyncMode"
Write-Host "  Source:        $SourcePath"
Write-Host "  Destination:   $DestinationPath"
Write-Host "  Dry Run:       $DryRun"
Write-Host "  Compress:      $Compress"
Write-Host "  Show Progress: $ShowProgress"
Write-Host "  Verbose:       $($VerbosePreference -ne 'SilentlyContinue')"
Write-Host "  Transfers:     $Transfers"
Write-Host "  Checkers:      $Checkers"

if ($BandwidthLimit) {
    Write-Host "  Bandwidth:     $BandwidthLimit"
}

if ($ConfigFile) {
    Write-Host "  Config File:   $ConfigFile"
}

if ($allExcludes.Count -gt 0) {
    Write-Host "  Excludes:      $($allExcludes[0..2] -join ', ')$(if ($allExcludes.Count -gt 3) { "... ($($allExcludes.Count) total)" })"
}

if ($LogFile) {
    Write-Host "  Log File:      $LogFile"
}

Write-Host ""

# Build rclone arguments
$isVerboseEnabled = $VerbosePreference -ne 'SilentlyContinue'
$rcloneArgs = Build-RcloneArgs -Mode $SyncMode -Source $SourcePath -Destination $DestinationPath `
    -IsDryRun $DryRun -IsVerbose $isVerboseEnabled -ShouldShowProgress $ShowProgress `
    -ShouldCompress $Compress -Excludes $allExcludes -LogPath $LogFile `
    -ConfigPath $ConfigFile -TransferCount $Transfers -CheckerCount $Checkers `
    -Bandwidth $BandwidthLimit

# Display the command that will be executed
Write-Verbose "Executing: $RclonePath $($rcloneArgs -join ' ')"
Write-Host "Starting $SyncMode operation..." -ForegroundColor Yellow
Write-Host ""

# Execute rclone
try {
    $startTime = Get-Date

    if ($DryRun) {
        Write-Host "=== DRY RUN MODE - No files will be modified ===" -ForegroundColor Magenta
        Write-Host ""
    }

    & $RclonePath @rcloneArgs

    $endTime = Get-Date
    $duration = $endTime - $startTime

    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "$SyncMode operation completed successfully!" -ForegroundColor Green
        Write-Host "Duration: $($duration.ToString('hh\:mm\:ss'))" -ForegroundColor Green

        if ($DryRun) {
            Write-Host ""
            Write-Host "Note: This was a dry run. No files were actually synchronized." -ForegroundColor Magenta
        }
    } else {
        Write-Host ""
        Write-Error "rclone completed with exit code: $LASTEXITCODE"
        exit $LASTEXITCODE
    }
}
catch {
    Write-Host ""
    Write-Error "Failed to execute rclone: $($_.Exception.Message)"
    exit 1
}

# Optional: Display sync statistics if log file was used
if ($LogFile -and (Test-Path $LogFile)) {
    Write-Host ""
    Write-Host "Sync log saved to: $LogFile" -ForegroundColor Cyan
    Write-Host ""
}

# Show rclone remotes if this was the first run
if (-not $DryRun -and $LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "Tip: Use 'rclone listremotes' to see configured remotes" -ForegroundColor Gray
    Write-Host "Tip: Use 'rclone config' to configure new remotes" -ForegroundColor Gray
    Write-Host ""
}