find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES fr)
qt_add_translations(AutoKeys)
target_sources(AutoKeys PRIVATE icon.rc)
qt_add_resources(AutoKeys imageresources
    PREFIX "/images"
    FILES icon.png icon-active.png
)