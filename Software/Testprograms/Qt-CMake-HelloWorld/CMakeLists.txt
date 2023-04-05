cmake_minimum_required(VERSION 3.8.2)

project(helloworld)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# There may be a way to tell up front if Qt5 is going to be found, but I haven't found
# a foolproof way to do it yet, so settle for the default error message for now.
if(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)
   message(WARNING "CMAKE_PREFIX_PATH is not defined, so find_package may not work. Set the CMAKE_PREFIX_PATH "
           "environment variable to the install prefix of Qt 5, either on the command line as "
           "-DCMAKE_PREFIX_PATH=\"path/to/Qt5/lib/cmake\" or with set(CMAKE_PREFIX_PATH path/to/Qt5/lib/cmake)")
endif(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

SET(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake")
set(DCMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake")

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# Find the QtWidgets library
# set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/include")
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Tell CMake to create the helloworld executable
add_executable(helloworld main.cpp mainwindow.cpp mainwindow.ui resources.qrc)

# Add the Qt5 Widgets for linking
target_link_libraries(helloworld Qt5::Widgets Qt::Core)






# cmake_minimum_required(VERSION 3.5)

# #---------------------------------------------------------------------
# # I ADD THIS ONE TO COMPILE IN VSCODE AS ASKED IN THE DOCUMENTATION
# #---------------------------------------------------------------------
# set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\Qt\\5.15.2\\mingw81_64")
# #---------------------------------------------------------------------

# project(helloworld LANGUAGES CXX)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

# set(TS_FILES mainwindow.ts)

# add_executable(helloworld main.cpp mainwindow.cpp mainwindow.ui resources.qrc)

# target_link_libraries(helloworld PRIVATE Qt5::Widgets)

# qt5_create_translation(QM_FILES ${TS_FILES})

# #---------------------------------------------------------------------
# # ADD THE FOLLOWING COMMANDS HERE
# #---------------------------------------------------------------------
# set(Qt_qmakeExecutable "%{Qt:qmakeExecutable}")
# set(Qt_QT_INSTALL_PREFIX "%{Qt:QT_INSTALL_PREFIX}")
# set(Compiler_C "%{Compiler:Executable:C}")
# set(Compiler_Cxx "%{Compiler:Executable:Cxx}")

# add_definitions(
#     -DQT_QMAKE_EXECUTABLE:FILEPATH=${Qt_qmakeExecutable}
#     -DCMAKE_PREFIX_PATH:PATH=${Qt_QT_INSTALL_PREFIX}
#     -DCMAKE_C_COMPILER:FILEPATH=${Compiler_C}
#     -DCMAKE_CXX_COMPILER:FILEPATH=${Compiler_Cxx}
# )
# #---------------------------------------------------------------------