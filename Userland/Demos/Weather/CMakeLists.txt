set(SOURCES
    Matrix.cpp
    main.cpp
)

serenity_app(Matrix ICON app-screensaver)
target_link_libraries(Matrix LibGUI LibCore LibGfx LibPthread)
