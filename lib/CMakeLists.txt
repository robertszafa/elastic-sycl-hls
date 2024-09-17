set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# THE LIST OF PLUGINS AND THE CORRESPONDING SOURCE FILES
# ======================================================
set(OPT_PASSES_PLUGINS
    HoistConstantGepTransform
    PipeDeduplicationPass
    ElasticAnalysisPrinter
    ElasticTransform
    DDGDotPrinter
    DynamicLoopFusionAnalysisPrinter
    DynamicLoopFusionTransform
)

set(HoistConstantGepTransform_SOURCES 
  HoistConstantGepTransform.cpp
)

set(PipeDeduplicationPass_SOURCES
  PipeDeduplicationPass.cpp
)

set(ElasticAnalysisPrinter_SOURCES
  ElasticAnalysisPrinter.cpp
  DataHazardAnalysis.cpp
  CDG.cpp
  CDDDAnalysis.cpp
)

set(ElasticTransform_SOURCES
  ElasticTransform.cpp
)

set(DDGDotPrinter_SOURCES
  DDGDotPrinter.cpp
  CDG.cpp
)

set(DynamicLoopFusionAnalysisPrinter_SOURCES
  DynamicLoopFusionAnalysisPrinter.cpp
  DynamicLoopFusionAnalysis.cpp
)

set(DynamicLoopFusionTransform_SOURCES
  DynamicLoopFusionTransform.cpp
  DynamicLoopFusionAnalysis.cpp
  CDG.cpp
)

# CONFIGURE THE PLUGIN LIBRARIES
# ==============================
foreach( plugin ${OPT_PASSES_PLUGINS} )
    # Create a library corresponding to 'plugin'
    add_library(
      ${plugin}
      SHARED
      ${${plugin}_SOURCES}
      )

    # Configure include directories for 'plugin'
    target_include_directories(
      ${plugin}
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )

    # On Darwin (unlike on Linux), undefined symbols in shared objects are not
    # allowed at the end of the link-edit. The plugins defined here:
    #  - _are_ shared objects
    #  - reference symbols from LLVM shared libraries, i.e. symbols which are
    #    undefined until those shared objects are loaded in memory (and hence
    #    _undefined_ during static linking)
    # The build will fail with errors like this:
    #    "Undefined symbols for architecture x86_64"
    # with various LLVM symbols being undefined. Since those symbols are later
    # loaded and resolved at runtime, these errors are false positives.
    # This behaviour can be modified via the '-undefined' OS X linker flag as
    # follows.
    target_link_libraries(
      ${plugin}
      "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>"
      )
endforeach()
