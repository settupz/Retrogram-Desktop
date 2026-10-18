'''
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
'''
import sys, os, re

sys.dont_write_bytecode = True
scriptPath = os.path.dirname(os.path.realpath(__file__))
sys.path.append(scriptPath + '/../cmake')
import run_cmake
sys.path.append(scriptPath + '/build')
import qt_version

executePath = os.getcwd()
def finish(code):
    global executePath
    os.chdir(executePath)
    sys.exit(code)

def error(message):
    print('[ERROR] ' + message)
    finish(1)

if sys.platform == 'win32' and 'COMSPEC' not in os.environ:
    error('COMSPEC environment variable is not set.')

scriptName = os.path.basename(scriptPath)

arguments = sys.argv[1:]

officialTarget = ''
officialTargetFile = scriptPath + '/build/target'
if os.path.isfile(officialTargetFile):
    with open(officialTargetFile, 'r') as f:
        for line in f:
            officialTarget = line.strip()

arch = ''
if officialTarget in ['win', 'uwp']:
    arch = 'x86'
elif officialTarget in ['win64', 'uwp64']:
    arch = 'x64'
elif officialTarget in ['winarm', 'uwparm']:
    arch = 'arm'
if not qt_version.resolve(arch):
    error('Unsupported platform.')

if 'qt6' in arguments:
    arguments.remove('qt6')

if officialTarget != '':
    officialApiIdFile = scriptPath + '/../../DesktopPrivate/custom_api_id.h'
    if not os.path.isfile(officialApiIdFile):
        error('DesktopPrivate/custom_api_id.h not found.')
    with open(officialApiIdFile, 'r') as f:
        for line in f:
            apiIdMatch = re.search(r'ApiId\s+=\s+(\d+)', line)
            apiHashMatch = re.search(r'ApiHash\s+=\s+"([a-fA-F\d]+)"', line)
            if apiIdMatch:
                arguments.append('-DTDESKTOP_API_ID=' + apiIdMatch.group(1))
            elif apiHashMatch:
                arguments.append('-DTDESKTOP_API_HASH=' + apiHashMatch.group(1))
    if arch != '':
        arguments.append(arch)

import shutil
for base_dir in [
    os.path.abspath(os.path.join(scriptPath, '../../Libraries/win64/tdesktop_rust')),
    os.path.abspath(os.path.join(scriptPath, '../../Libraries/tdesktop_rust')),
]:
    src_cpp = os.path.join(base_dir, 'out', 'src', 'wallet_engine', 'wallet_engine.cpp')
    inc_cpp = os.path.join(base_dir, 'out', 'include', 'wallet_engine', 'wallet_engine.cpp')
    if not os.path.exists(src_cpp):
        os.makedirs(os.path.dirname(src_cpp), exist_ok=True)
        if os.path.exists(inc_cpp):
            shutil.copyfile(inc_cpp, src_cpp)
        else:
            # Re-generate with cargo bindgen if dll exists
            dll = os.path.join(base_dir, 'target', 'x86_64-pc-windows-msvc', 'release', 'tdesktop_rust.dll')
            manifest = os.path.join(base_dir, '..', 'wallet-engine', 'bindgen', 'cpp', 'bindgen', 'Cargo.toml')
            cargo = os.path.abspath(os.path.join(scriptPath, '../../ThirdParty/rust/cargo/bin/cargo.exe'))
            if not os.path.exists(cargo):
                cargo = 'cargo'
            if os.path.exists(manifest) and os.path.exists(dll):
                import subprocess
                subprocess.run([cargo, 'run', '--manifest-path', manifest, '--locked', '--', '--library', '--out-dir', os.path.join(base_dir, 'out', 'include', 'wallet_engine'), dll], cwd=base_dir)
                if os.path.exists(inc_cpp):
                    shutil.copyfile(inc_cpp, src_cpp)

filtered_args = []
skip_next = False
for i, arg in enumerate(arguments):
    if skip_next:
        skip_next = False
        continue
    if arg == '-D' and i + 1 < len(arguments):
        nxt = arguments[i + 1]
        if nxt.startswith('TDESKTOP_API_TEST=') or nxt.startswith('CMAKE_COMPILE_WARNING_AS_ERROR=') or nxt.startswith('DESKTOP_APP_DISABLE_AUTOUPDATE='):
            skip_next = True
            continue
    if arg.startswith('-DTDESKTOP_API_TEST=') or arg.startswith('-DCMAKE_COMPILE_WARNING_AS_ERROR='):
        continue
    filtered_args.append(arg)

arguments = filtered_args + [
    '-DTDESKTOP_API_ID=611335',
    '-DTDESKTOP_API_HASH=d524b414d21f4d37f0864c1df41ac9c',
    '-DDESKTOP_APP_DISABLE_AUTOUPDATE=ON',
    '-DCMAKE_COMPILE_WARNING_AS_ERROR=OFF',
]

finish(run_cmake.run(scriptName, arguments))
