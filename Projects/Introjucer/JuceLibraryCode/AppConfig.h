/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    There's a section below where you can add your own custom code safely, and the
    Projucer will preserve the contents of that block, but the best way to change
    any of these definitions is by using the Projucer's project settings.

    Any commented-out settings will assume their default values.

*/

#ifndef __JUCE_APPCONFIG_NTTT2W__
#define __JUCE_APPCONFIG_NTTT2W__

//==============================================================================
// [BEGIN_USER_CODE_SECTION]

// (You can add your own code in this section, and the Projucer will not overwrite it)

// [END_USER_CODE_SECTION]

//==============================================================================
#define JUCE_MODULE_AVAILABLE_juce_core                 1
#define JUCE_MODULE_AVAILABLE_juce_cryptography         1
#define JUCE_MODULE_AVAILABLE_juce_data_structures      1
#define JUCE_MODULE_AVAILABLE_juce_events               1

//==============================================================================
#ifndef    JUCE_STANDALONE_APPLICATION
 #ifdef JucePlugin_Build_Standalone
  #define  JUCE_STANDALONE_APPLICATION JucePlugin_Build_Standalone
 #else
  #define  JUCE_STANDALONE_APPLICATION 1
 #endif
#endif

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1

//==============================================================================
// juce_core flags:

#ifndef    JUCE_FORCE_DEBUG
 //#define JUCE_FORCE_DEBUG
#endif

#ifndef    JUCE_LOG_ASSERTIONS
 //#define JUCE_LOG_ASSERTIONS
#endif

#ifndef    JUCE_CHECK_MEMORY_LEAKS
 //#define JUCE_CHECK_MEMORY_LEAKS
#endif

#ifndef    JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
 //#define JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
#endif

#ifndef    JUCE_INCLUDE_ZLIB_CODE
 //#define JUCE_INCLUDE_ZLIB_CODE
#endif

#ifndef    JUCE_USE_CURL
 //#define JUCE_USE_CURL
#endif


#endif  // __JUCE_APPCONFIG_NTTT2W__
