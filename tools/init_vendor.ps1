[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"

$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
Push-Location $repositoryRoot

try {
    & git submodule update --init -- libKCD2 vendor/Address-Library-For-KCSE
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to initialize the libKCD2 or Address Library submodule."
    }

    & git -C libKCD2 config `
        submodule.Projects/KCSE.url `
        https://github.com/F02K/KCSE-for-kcd2.git
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to configure the KCSE HTTPS submodule URL."
    }

    & git -C libKCD2 submodule update --init -- Projects/KCSE
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to initialize libKCD2's KCSE submodule."
    }

    Write-Host "libKCD2, KCSE, and Address-Library-For-KCSE are initialized at their pinned commits."
}
finally {
    Pop-Location
}
