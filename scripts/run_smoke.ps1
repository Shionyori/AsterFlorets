$ErrorActionPreference = 'Stop'

Write-Host "[Smoke] Start AsterFlorets smoke checks..."

$root = Split-Path -Parent $PSScriptRoot
$bin = Join-Path $root "build/windows-msvc/bin/Debug"
$testsExe = Join-Path $bin "AsterFloretsSmokeTests.exe"
$galleryExe = Join-Path $bin "ComponentGallery.exe"
$layoutExe = Join-Path $bin "LayoutDemo.exe"

$required = @($testsExe, $galleryExe, $layoutExe)
foreach ($file in $required) {
    if (-not (Test-Path $file)) {
        throw "[Smoke] Missing binary: $file"
    }
}

$env:QT_QPA_PLATFORM = "offscreen"

Write-Host "[Smoke] Running AsterFloretsSmokeTests.exe ..."
& $testsExe
if ($LASTEXITCODE -ne 0) {
    throw "[Smoke] Test executable failed with exit code $LASTEXITCODE"
}

Write-Host "[Smoke] OK - binaries exist and smoke tests passed."
