set(SOURCES
    Weather.cpp
    main.cpp
)

serenity_app(Weather ICON app-screensaver)
target_link_libraries(Weather LibGUI LibCore LibGfx LibPthread)
