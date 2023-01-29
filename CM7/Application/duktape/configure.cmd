@ECHO OFF

@GOTO run

:help

ECHO Usage: configure.py [options]
ECHO.
ECHO Prepare Duktape source files and a duk_config.h configuration header for
ECHO compilation. Source files can be combined (amalgamated) or kept separate. See
ECHO http://wiki.duktape.org/Configuring.html for examples.
ECHO.
ECHO Options:
ECHO   -h, --help            show this help message and exit
ECHO   --source-directory=SOURCE_DIRECTORY
ECHO                         Directory with raw input sources (defaulted based on
ECHO                         configure.py script path)
ECHO   --output-directory=OUTPUT_DIRECTORY
ECHO                         Directory for output files (created automatically if
ECHO                         it doesn't exist, reused if safe)
ECHO   --license-file=LICENSE_FILE
ECHO                         Source for LICENSE.txt (defaulted based on
ECHO                         configure.py script path)
ECHO   --authors-file=AUTHORS_FILE
ECHO                         Source for AUTHORS.rst (defaulted based on
ECHO                         configure.py script path)
ECHO   --git-commit=GIT_COMMIT
ECHO                         Force git commit hash
ECHO   --git-describe=GIT_DESCRIBE
ECHO                         Force git describe
ECHO   --git-branch=GIT_BRANCH
ECHO                         Force git branch name
ECHO   --duk-dist-meta=DUK_DIST_META
ECHO                         duk_dist_meta.json to read git commit etc info from
ECHO   --separate-sources    Output separate sources instead of combined source
ECHO                         (default is combined)
ECHO   --line-directives     Output #line directives in combined source (default is
ECHO                         false)
ECHO   --rom-support         Add support for ROM strings/objects (increases
ECHO                         duktape.c size considerably)
ECHO   --rom-auto-lightfunc  Convert ROM built-in function properties into
ECHO                         lightfuncs automatically whenever possible
ECHO   --builtin-file=FILENAME
ECHO                         Built-in string/object YAML metadata to be applied
ECHO                         over default built-ins (multiple files may be given,
ECHO                         applied in sequence)
ECHO   --unicode-data=UNICODE_DATA
ECHO                         Provide custom UnicodeData.txt
ECHO   --special-casing=SPECIAL_CASING
ECHO                         Provide custom SpecialCasing.txt
ECHO   --config-metadata=CONFIG_METADATA
ECHO                         metadata directory (defaulted based on configure.py
ECHO                         script path)
ECHO   --platform=PLATFORM   platform (default is autodetect)
ECHO   --compiler=COMPILER   compiler (default is autodetect)
ECHO   --architecture=ARCHITECTURE
ECHO                         architecture (default is autodetec)
ECHO   --c99-types-only      assume C99 types, no legacy type detection
ECHO   --dll                 dll build of Duktape, affects symbol visibility macros
ECHO                         especially on Windows
ECHO   --emit-legacy-feature-check
ECHO                         emit preprocessor checks to reject legacy feature
ECHO                         options (DUK_OPT_xxx)
ECHO   --emit-config-sanity-check
ECHO                         emit preprocessor checks for config option consistency
ECHO                         (DUK_USE_xxx)
ECHO   --omit-removed-config-options
ECHO                         omit removed config options from generated headers
ECHO   --omit-deprecated-config-options
ECHO                         omit deprecated config options from generated headers
ECHO   --omit-unused-config-options
ECHO                         omit unused config options from generated headers
ECHO   --add-active-defines-macro
ECHO                         add DUK_ACTIVE_DEFINES macro, for development only
ECHO   --define=OPTION       force #define option using a C compiler like syntax,
ECHO                         e.g. "--define DUK_USE_DEEP_C_STACK" or "--define
ECHO                         DUK_USE_TRACEBACK_DEPTH=10"
ECHO   -D OPTION             synonym for --define, e.g. "-DDUK_USE_DEEP_C_STACK" or
ECHO                         "-DDUK_USE_TRACEBACK_DEPTH=10"
ECHO   --undefine=OPTION     force #undef option using a C compiler like syntax,
ECHO                         e.g. "--undefine DUK_USE_DEEP_C_STACK"
ECHO   -U OPTION             synonym for --undefine, e.g. "-UDUK_USE_DEEP_C_STACK"
ECHO   --option-yaml=YAML    force option(s) using inline YAML (e.g. --option-yaml
ECHO                         "DUK_USE_DEEP_C_STACK: true")
ECHO   --option-file=FILENAME
ECHO                         YAML file(s) providing config option overrides
ECHO   --fixup-file=FILENAME
ECHO                         C header snippet file(s) to be appended to generated
ECHO                         header, useful for manual option fixups
ECHO   --fixup-line=LINE     C header fixup line to be appended to generated header
ECHO                         (e.g. --fixup-line "#define DUK_USE_FASTINT")
ECHO   --sanity-warning      emit a warning instead of #error for option sanity
ECHO                         check issues
ECHO   --use-cpp-warning     emit a (non-portable) #warning when appropriate
ECHO   --quiet               Suppress info messages (show warnings)
ECHO   --verbose             Show verbose debug messages
ECHO. 
GOTO quit

:run

SET dukpath=C:\Projects\duktape-2.7.0
SET scriptpath=%~dp0

SET python=C:\Python\27-64\python.exe
SET output=%TEMP%\PTC05_Duktype
SET prjdest=%scriptpath%

SET options=--output-directory=%output%
SET options=%options% --option-file=%scriptpath%\stm32.yaml
SET options=%options% --git-commit=0000270
SET options=%options% --git-describe=MIP_270
SET options=%options% --git-branch=MIP
SET options=%options% --rom-support
SET options=%options% --rom-auto-lightfunc
SET options=%options% --platform=generic
SET options=%options% --architecture=arm32
SET options=%options% --compiler=gcc
SET options=%options% --fixup-file=%scriptpath%\duk_custom.in

PUSHD %scriptpath%
CD %dukpath%
REM %python% tools\configure.py --help > %scriptpath%\config.txt
%python% tools\configure.py %options%

COPY /Y %output%\duk_config.h %prjdest%
COPY /Y %output%\duktape.c %prjdest%
COPY /Y %output%\duktape.h %prjdest%

DEL /Q /S %output%

POPD
GOTO quit

:quit

PAUSE
