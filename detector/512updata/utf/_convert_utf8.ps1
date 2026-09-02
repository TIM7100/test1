$root = 'E:\LILIN\NPG83_MainBoard'
$skipPattern = '^(stm32f10x_|core_cm3|misc|usmart)'
$gbk = [System.Text.Encoding]::GetEncoding('gbk')
$utf8Strict = New-Object System.Text.UTF8Encoding $false, $true
$utf8 = [System.Text.Encoding]::UTF8

$files = Get-ChildItem -Path "$root\CodeFile" -Recurse -Include '*.c','*.h'
$files += Get-ChildItem -Path "$root\pro\startup_stm32f10x_xl.s" -ErrorAction SilentlyContinue

$converted = 0
$skipped = 0
$failed = 0

foreach ($file in $files) {
    if ($file.Name -match $skipPattern) {
        $skipped++
        continue
    }
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    if ($bytes.Length -eq 0) {
        $skipped++
        continue
    }
    # Check if already valid UTF-8 (strict mode throws on invalid bytes)
    $isUtf8 = $true
    try {
        $null = $utf8Strict.GetString($bytes)
    } catch {
        $isUtf8 = $false
    }
    if ($isUtf8) {
        $skipped++
        continue
    }
    # Try GBK decode and write back as UTF-8
    try {
        $content = $gbk.GetString($bytes)
        [System.IO.File]::WriteAllText($file.FullName, $content, $utf8)
        Write-Output "Converted: $($file.FullName)"
        $converted++
    } catch {
        Write-Output "Failed: $($file.FullName)"
        $failed++
    }
}

Write-Output ""
Write-Output "=== Summary ==="
Write-Output "Converted: $converted"
Write-Output "Skipped:   $skipped"
Write-Output "Failed:    $failed"
