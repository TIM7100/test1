import os, sys

def try_convert(filepath):
    with open(filepath, 'rb') as f:
        raw = f.read()
    if len(raw) == 0:
        return 'skip-empty'
    try:
        raw.decode('utf-8')
        return 'skip-utf8'
    except UnicodeDecodeError:
        pass
    try:
        text = raw.decode('gbk')
    except UnicodeDecodeError:
        return 'fail'
    with open(filepath, 'w', encoding='utf-8', newline='') as f:
        f.write(text)
    return 'converted'

def main():
    root = r'E:\LILIN\NPG83_MainBoard'
    subdirs = [
        r'CodeFile\CommonFunction',
        r'CodeFile\MainSlave',
        r'CodeFile\ScreenOS',
        r'CodeFile\UserFunction',
        r'CodeFile\UserPeripheral',
        r'CodeFile\system\src',
        r'CodeFile\system\inc',
    ]
    skip_prefixes = ('stm32f10x_', 'core_cm3', 'misc', 'usmart')

    converted = []
    skipped = []
    failed = []

    for sd in subdirs:
        d = os.path.join(root, sd)
        for dirpath, _dirnames, filenames in os.walk(d):
            for fn in filenames:
                lfn = fn.lower()
                if not (lfn.endswith('.c') or lfn.endswith('.h') or lfn.endswith('.s')):
                    continue
                if any(fn.startswith(sp) for sp in skip_prefixes):
                    skipped.append(os.path.join(dirpath, fn) + ' [standard-lib-skip]')
                    continue
                fp = os.path.join(dirpath, fn)
                r = try_convert(fp)
                if r == 'converted':
                    converted.append(fp)
                elif r == 'fail':
                    failed.append(fp)
                else:
                    skipped.append(fp + ' [' + r + ']')

    startup = os.path.join(root, 'pro', 'startup_stm32f10x_xl.s')
    if os.path.exists(startup):
        r = try_convert(startup)
        if r == 'converted':
            converted.append(startup)
        elif r == 'fail':
            failed.append(startup)
        else:
            skipped.append(startup + ' [' + r + ']')

    print('=== Converted (%d) ===' % len(converted))
    for f in converted:
        print(f)
    print()
    print('=== Skipped (%d) ===' % len(skipped))
    for f in skipped:
        print(f)
    print()
    print('=== Failed (%d) ===' % len(failed))
    for f in failed:
        print(f)

if __name__ == '__main__':
    main()
