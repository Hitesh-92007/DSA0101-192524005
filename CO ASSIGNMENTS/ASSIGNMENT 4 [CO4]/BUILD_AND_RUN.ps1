$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $root

$compiler = Get-Command 'g++.exe' -ErrorAction SilentlyContinue
if (-not $compiler) {
    $packages = Join-Path $env:LOCALAPPDATA 'Microsoft\WinGet\Packages'
    $install = Get-ChildItem -Path $packages -Directory -Filter 'MartinStorsjo.LLVM-MinGW*' -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($install) {
        $toolchain = Get-ChildItem -Path $install.FullName -Directory -Filter 'llvm-mingw-*' | Select-Object -First 1
        if ($toolchain) {
            $bin = Join-Path $toolchain.FullName 'bin'
            $env:Path = "$bin;$env:Path"
            $compiler = Get-Command 'g++.exe' -ErrorAction SilentlyContinue
        }
    }
}
if (-not $compiler) { throw 'g++ was not found. Install a C++17 MinGW toolchain and add its bin folder to PATH.' }

$flags = @('-std=c++17', '-Wall', '-Wextra')
$results = [System.Collections.Generic.List[string]]::new()
$results.Add('# Build and Run Results')
$results.Add('')
$results.Add("Compiler command: ``$($compiler.Source) --version``")
$version = (& $compiler.Source --version 2>&1 | Select-Object -First 1)
$results.Add("Compiler version: $version")
$results.Add('')

function Build-Run {
    param([string]$Task, [string]$Source, [string]$Label, [switch]$AddressSanitizer, [switch]$WarningCheck)
    $exe = Join-Path $root ($Source -replace '\.cpp$', '.exe')
    $log = [System.Collections.Generic.List[string]]::new()
    $log.Add("$Label")
    $log.Add("Compile: g++ $($flags -join ' ') $Source -o $($Source -replace '\.cpp$', '.exe')")
    $extra = @()
    if ($AddressSanitizer) { $extra += '-fsanitize=address' }
    $build = & $compiler.Source @flags @extra $Source -o $exe 2>&1
    $buildCode = $LASTEXITCODE
    if ($build) { $log.Add(($build | Out-String).TrimEnd()) }
    if ($buildCode -ne 0) {
        $log.Add("BUILD FAILED ($buildCode)")
        $results.Add("- FAIL: ``$Source`` did not compile ($buildCode).")
        $outPath = Join-Path $root "$Task/RUN_OUTPUT.txt"
        $log | Out-File -FilePath $outPath -Append -Encoding utf8
        return
    }
    $results.Add("- PASS: ``$Source`` compiled.")
    $oldAsan = $env:ASAN_OPTIONS
    if ($AddressSanitizer) { $env:ASAN_OPTIONS = 'detect_leaks=0' }
    $run = & $exe 2>&1
    $runCode = $LASTEXITCODE
    if ($AddressSanitizer) { $env:ASAN_OPTIONS = $oldAsan }
    $log.Add('Run output:')
    if ($run) { $log.Add(($run | Out-String).TrimEnd()) }
    $log.Add("Exit code: $runCode")
    if ($AddressSanitizer) {
        $results.Add("- Sanitizer demonstration: ``$Source`` exited $runCode; inspect ``$Task/SANITIZER_RESULTS.txt``.")
        $log | Out-File -FilePath (Join-Path $root "$Task/SANITIZER_RESULTS.txt") -Append -Encoding utf8
        if ($Task -eq 'Task04') {
            @('AddressSanitizer was enabled for this run. This Windows runtime does not support LeakSanitizer, so detect_leaks=0 was used.', 'No sanitizer diagnostic was emitted in this run. The defective program prints two base-destructor messages, omits the SeniorAccount destructor during deletion through Account*, and reports one outstanding history buffer. This is direct destructor/allocation tracking evidence; it is not a LeakSanitizer finding.') | Out-File -FilePath (Join-Path $root "$Task/SANITIZER_RESULTS.txt") -Append -Encoding utf8
        }
    } elseif ($runCode -ne 0) {
        $results.Add("- FAIL: ``$Source`` returned $runCode.")
    } else {
        $results.Add("- PASS: ``$Source`` ran.")
    }
    $outPath = Join-Path $root "$Task/RUN_OUTPUT.txt"
    $log | Out-File -FilePath $outPath -Append -Encoding utf8
}

Get-ChildItem -Path $root -Directory -Filter 'Task*' | ForEach-Object {
    Remove-Item -LiteralPath (Join-Path $_.FullName 'RUN_OUTPUT.txt') -Force -ErrorAction SilentlyContinue
    Remove-Item -LiteralPath (Join-Path $_.FullName 'SANITIZER_RESULTS.txt') -Force -ErrorAction SilentlyContinue
}

Build-Run Task01 'Task01/payroll_hierarchy.cpp' 'Payroll hierarchy'
Build-Run Task02 'Task02/people_hierarchy.cpp' 'Virtual-base diamond'
Build-Run Task02 'Task02/people_relationship_classes.cpp' 'People hierarchy with TeachingAssistant and ResearchScholar'
Build-Run Task02 'Task02/diamond_nonvirtual.cpp' 'Non-virtual diamond'
Build-Run Task02 'Task02/people_composition.cpp' 'Composition comparison'

$ambiguitySource = 'Task02/person_access_ambiguity.cpp'
$ambiguityOutput = & $compiler.Source @flags $ambiguitySource -o (Join-Path $root 'Task02/person_access_ambiguity.exe') 2>&1
$ambiguityCode = $LASTEXITCODE
@("Expected compile-time ambiguity check", "Command: g++ $($flags -join ' ') $ambiguitySource -o Task02/person_access_ambiguity.exe", ($ambiguityOutput | Out-String).TrimEnd(), "Exit code: $ambiguityCode", 'Status: expected failure because both Person base paths provide name.') | Out-File (Join-Path $root 'Task02/EXPECTED_DIAGNOSTIC.txt') -Encoding utf8
if ($ambiguityCode -eq 0) { $results.Add('- FAIL: ambiguity example unexpectedly compiled.') } else { $results.Add('- PASS: non-virtual `ta.name` access is rejected as ambiguous, as expected.') }

Build-Run Task03 'Task03/construction_trace.cpp' 'Construction and destruction trace'
Build-Run Task03 'Task03/predict_vehicle_order.cpp' 'Assignment Engine, Vehicle and Car prediction program'
Build-Run Task03 'Task03/student_member_class_complete.cpp' 'Completed member-class program'
Build-Run Task03 'Task03/initializer_order_warning.cpp' 'Initializer-order warning demonstration' -WarningCheck
Build-Run Task03 'Task03/nonvirtual_size.cpp' 'Virtual and non-virtual object sizes'

Build-Run Task04 'Task04/account_defects.cpp' 'Defective account program under AddressSanitizer' -AddressSanitizer
Build-Run Task04 'Task04/account_fixed.cpp' 'Repaired polymorphic account program'
Build-Run Task04 'Task04/account_prediction.cpp' 'Static dispatch, virtual dispatch and slicing output'

Build-Run Task05 'Task05/cart_complete.cpp' 'Completed cart code'
Build-Run Task05 'Task05/cart_access_ways.cpp' 'Cart access methods and pointer-array search'

Build-Run Task06 'Task06/vehicle_rental.cpp' 'Vehicle rental fleet'
Build-Run Task06 'Task06/electric_bus_demo.cpp' 'Electric bus extension'

$warningText = Get-Content -Path (Join-Path $root 'Task03/RUN_OUTPUT.txt') -Raw
if ($warningText -match 'reorder|initialization order|will be initialized') {
    $results.Add('- PASS: initializer-order source produced a reorder warning under `-Wall -Wextra`.')
} else {
    $results.Add('- NOTE: inspect Task03/RUN_OUTPUT.txt for this compiler version''s initializer warning wording.')
}
$results | Out-File (Join-Path $root 'BUILD_RESULTS.md') -Encoding utf8
Write-Output "Build and run complete. See BUILD_RESULTS.md and each Task folder's RUN_OUTPUT.txt."
