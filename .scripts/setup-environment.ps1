# Windows Development Environment Setup Script
# This script installs Git, CMake, and Visual Studio Build Tools with MSVC

# Requires Administrator privileges
#Requires -RunAsAdministrator

Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host "  Windows Development Environment Setup" -ForegroundColor Cyan
Write-Host "  Installing: Git, CMake, and Visual Studio Build Tools (MSVC)" -ForegroundColor Cyan
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host ""

# Create temporary directory for downloads
$tempDir = "$env:TEMP\DevSetup"
New-Item -ItemType Directory -Force -Path $tempDir | Out-Null
Write-Host "Created temporary directory: $tempDir" -ForegroundColor Gray
Write-Host ""

# Function to check if a command exists
function Test-Command {
    param($Command)
    try {
        if (Get-Command $Command -ErrorAction Stop) {
            return $true
        }
    }
    catch {
        return $false
    }
}

# Function to download file with progress
function Download-File {
    param(
        [string]$Url,
        [string]$OutputPath,
        [string]$Name
    )
    
    Write-Host "Downloading $Name..." -ForegroundColor Yellow
    try {
        $ProgressPreference = 'SilentlyContinue'
        Invoke-WebRequest -Uri $Url -OutFile $OutputPath -UseBasicParsing
        $ProgressPreference = 'Continue'
        Write-Host "✓ Downloaded $Name" -ForegroundColor Green
        return $true
    }
    catch {
        Write-Host "✗ Failed to download $Name" -ForegroundColor Red
        Write-Host "  Error: $_" -ForegroundColor Red
        return $false
    }
}

# =============================================================================
# STEP 1: Install Git
# =============================================================================
Write-Host "[1/3] Installing Git" -ForegroundColor Cyan
Write-Host "---------------------" -ForegroundColor Cyan

if (Test-Command "git") {
    $gitVersion = git --version
    Write-Host "✓ Git is already installed: $gitVersion" -ForegroundColor Green
    $installGit = Read-Host "Do you want to reinstall/update Git? (y/N)"
    if ($installGit -ne "y" -and $installGit -ne "Y") {
        Write-Host "  Skipping Git installation" -ForegroundColor Yellow
        $skipGit = $true
    }
}

if (-not $skipGit) {
    # Download Git installer
    $gitUrl = "https://github.com/git-for-windows/git/releases/download/v2.43.0.windows.1/Git-2.43.0-64-bit.exe"
    $gitInstaller = Join-Path $tempDir "git-installer.exe"
    
    if (Download-File -Url $gitUrl -OutputPath $gitInstaller -Name "Git") {
        Write-Host "Installing Git (this may take a few minutes)..." -ForegroundColor Yellow
        
        # Silent installation with default options
        $gitArgs = @(
            "/VERYSILENT",
            "/NORESTART",
            "/NOCANCEL",
            "/SP-",
            "/CLOSEAPPLICATIONS",
            "/RESTARTAPPLICATIONS",
            "/COMPONENTS=icons,ext\reg\shellhere,assoc,assoc_sh",
            "/EditorOption=VisualStudioCode",
            "/PathOption=Cmd"
        )
        
        Start-Process -FilePath $gitInstaller -ArgumentList $gitArgs -Wait -NoNewWindow
        
        # Refresh environment variables
        $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
        
        if (Test-Command "git") {
            Write-Host "✓ Git installed successfully!" -ForegroundColor Green
        }
        else {
            Write-Host "✗ Git installation may have failed. Please restart your terminal." -ForegroundColor Red
        }
    }
}

Write-Host ""

# =============================================================================
# STEP 2: Install CMake
# =============================================================================
Write-Host "[2/3] Installing CMake" -ForegroundColor Cyan
Write-Host "----------------------" -ForegroundColor Cyan

if (Test-Command "cmake") {
    $cmakeVersion = cmake --version | Select-Object -First 1
    Write-Host "✓ CMake is already installed: $cmakeVersion" -ForegroundColor Green
    $installCMake = Read-Host "Do you want to reinstall/update CMake? (y/N)"
    if ($installCMake -ne "y" -and $installCMake -ne "Y") {
        Write-Host "  Skipping CMake installation" -ForegroundColor Yellow
        $skipCMake = $true
    }
}

if (-not $skipCMake) {
    # Download CMake installer
    $cmakeUrl = "https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-windows-x86_64.msi"
    $cmakeInstaller = Join-Path $tempDir "cmake-installer.msi"
    
    if (Download-File -Url $cmakeUrl -OutputPath $cmakeInstaller -Name "CMake") {
        Write-Host "Installing CMake..." -ForegroundColor Yellow
        
        # Silent installation with PATH option
        $cmakeArgs = @(
            "/i",
            $cmakeInstaller,
            "/quiet",
            "/norestart",
            "ADD_CMAKE_TO_PATH=System"
        )
        
        Start-Process "msiexec.exe" -ArgumentList $cmakeArgs -Wait -NoNewWindow
        
        # Refresh environment variables
        $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
        
        if (Test-Command "cmake") {
            Write-Host "✓ CMake installed successfully!" -ForegroundColor Green
        }
        else {
            Write-Host "✗ CMake installation may have failed. Please restart your terminal." -ForegroundColor Red
        }
    }
}

Write-Host ""

# =============================================================================
# STEP 3: Install Visual Studio Build Tools (MSVC)
# =============================================================================
Write-Host "[3/3] Installing Visual Studio Build Tools (MSVC)" -ForegroundColor Cyan
Write-Host "---------------------------------------------------" -ForegroundColor Cyan

# Check if Visual Studio or Build Tools is already installed
$vsInstalled = $false
$vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1"
$buildToolsPath = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1"

if (Test-Path $vsPath) {
    Write-Host "✓ Visual Studio 2022 Community is already installed" -ForegroundColor Green
    $vsInstalled = $true
}
elseif (Test-Path $buildToolsPath) {
    Write-Host "✓ Visual Studio Build Tools 2022 is already installed" -ForegroundColor Green
    $vsInstalled = $true
}

if ($vsInstalled) {
    Write-Host ""
    Write-Host "Visual Studio/Build Tools is already installed." -ForegroundColor Yellow
    Write-Host "If you want to modify the installation, run Visual Studio Installer manually." -ForegroundColor Yellow
    $installVS = Read-Host "Do you want to download and run the installer anyway? (y/N)"
    if ($installVS -ne "y" -and $installVS -ne "Y") {
        Write-Host "  Skipping Visual Studio installation" -ForegroundColor Yellow
        $skipVS = $true
    }
}

if (-not $skipVS) {
    Write-Host ""
    Write-Host "Choose your installation option:" -ForegroundColor Yellow
    Write-Host "  1. Build Tools only (lighter, ~3-4 GB, no IDE)" -ForegroundColor White
    Write-Host "  2. Visual Studio Community (full IDE, ~7-10 GB, includes everything)" -ForegroundColor White
    $vsChoice = Read-Host "Enter choice (1 or 2)"
    
    if ($vsChoice -eq "1") {
        # Build Tools only
        $vsUrl = "https://aka.ms/vs/17/release/vs_buildtools.exe"
        $vsName = "Visual Studio Build Tools 2022"
        $vsInstaller = Join-Path $tempDir "vs_buildtools.exe"
    }
    else {
        # Full Visual Studio Community
        $vsUrl = "https://aka.ms/vs/17/release/vs_community.exe"
        $vsName = "Visual Studio Community 2022"
        $vsInstaller = Join-Path $tempDir "vs_community.exe"
    }
    
    if (Download-File -Url $vsUrl -OutputPath $vsInstaller -Name $vsName) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Yellow
        Write-Host "IMPORTANT: Visual Studio Setup" -ForegroundColor Yellow
        Write-Host "========================================" -ForegroundColor Yellow
        Write-Host "The Visual Studio installer will now open." -ForegroundColor White
        Write-Host ""
        Write-Host "Please select the following workload:" -ForegroundColor Cyan
        Write-Host "  ✓ Desktop development with C++" -ForegroundColor Green
        Write-Host ""
        Write-Host "This will install:" -ForegroundColor White
        Write-Host "  - MSVC compiler" -ForegroundColor Gray
        Write-Host "  - MSBuild" -ForegroundColor Gray
        Write-Host "  - Windows SDK" -ForegroundColor Gray
        Write-Host "  - CMake tools" -ForegroundColor Gray
        Write-Host ""
        Write-Host "Press any key to launch the installer..." -ForegroundColor Yellow
        $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
        
        # Launch installer (user will interact with GUI)
        Start-Process -FilePath $vsInstaller -Wait
        
        Write-Host ""
        Write-Host "Visual Studio installer completed." -ForegroundColor Green
        Write-Host "If you installed Visual Studio, it should now be available." -ForegroundColor Green
    }
}

Write-Host ""

# =============================================================================
# Cleanup
# =============================================================================
Write-Host "Cleaning up temporary files..." -ForegroundColor Gray
Remove-Item -Path $tempDir -Recurse -Force -ErrorAction SilentlyContinue

# =============================================================================
# Verification
# =============================================================================
Write-Host ""
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host "  Installation Complete - Verifying..." -ForegroundColor Cyan
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host ""

# Refresh PATH one more time
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# Check Git
Write-Host "Checking Git..." -ForegroundColor Cyan
if (Test-Command "git") {
    $gitVer = git --version
    Write-Host "  ✓ $gitVer" -ForegroundColor Green
}
else {
    Write-Host "  ✗ Git not found in PATH" -ForegroundColor Red
    Write-Host "    Please restart your terminal or computer" -ForegroundColor Yellow
}

# Check CMake
Write-Host "Checking CMake..." -ForegroundColor Cyan
if (Test-Command "cmake") {
    $cmakeVer = cmake --version | Select-Object -First 1
    Write-Host "  ✓ $cmakeVer" -ForegroundColor Green
}
else {
    Write-Host "  ✗ CMake not found in PATH" -ForegroundColor Red
    Write-Host "    Please restart your terminal or computer" -ForegroundColor Yellow
}

# Check Visual Studio
Write-Host "Checking Visual Studio..." -ForegroundColor Cyan
if (Test-Path $vsPath) {
    Write-Host "  ✓ Visual Studio 2022 Community found" -ForegroundColor Green
    Write-Host "    Location: C:\Program Files\Microsoft Visual Studio\2022\Community" -ForegroundColor Gray
}
elseif (Test-Path $buildToolsPath) {
    Write-Host "  ✓ Visual Studio Build Tools 2022 found" -ForegroundColor Green
    Write-Host "    Location: C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools" -ForegroundColor Gray
}
else {
    Write-Host "  ✗ Visual Studio not found" -ForegroundColor Red
    Write-Host "    Make sure you completed the Visual Studio installation" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host "  Next Steps" -ForegroundColor Cyan
Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. Close and reopen your terminal (or restart computer)" -ForegroundColor Yellow
Write-Host "2. Verify installations by running:" -ForegroundColor Yellow
Write-Host "     git --version" -ForegroundColor White
Write-Host "     cmake --version" -ForegroundColor White
Write-Host "3. To use MSVC, open 'Developer PowerShell for VS 2022' from Start menu" -ForegroundColor Yellow
Write-Host ""
Write-Host "Setup complete! 🎉" -ForegroundColor Green
Write-Host ""
