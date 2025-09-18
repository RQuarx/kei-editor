project(
    'Kei Editor',
    'cpp',
    default_options: ['cpp_std=c++23', 'warning_level=3'],
    meson_version: '>=1.8.0',
    version: '0.0.1',
    license: 'GPL-3.0-or-later',
    license_files: ['COPYING'],
)

add_project_arguments(
    [
        '-DAPP_NAME="@0@"'.format(meson.project_name()),
        '-DAPP_VERSION="@0@"'.format(meson.project_version()),
        '-DAPP_ID="org.kei.editor"',
    ],
    language: 'cpp',
)

add_project_dependencies(
    [
        dependency('jsoncpp'),
        dependency('sdl3'),
        dependency('sdl3-ttf'),
        dependency('fontconfig'),
    ],
    language: 'cpp',
)

default_config = 'config.json'
header_file = 'DEFAULT_CONFIG.hh'

gen_json = custom_target(
    'embed_json',
    command: [files('.github/scripts/embed_json.bash'), '@INPUT@', '@OUTPUT@'],
    output: header_file,
    input: default_config,
    capture: false,
)

subdir('src')
executable(
    'keditor',
    [source_files, gen_json],
    include_directories: include_directories('include', '.'),
)
