@echo off
set /p fn="Filename: "

::write stuff into .h
echo /*%fn%.h generated on %date% at %time%*/>> %fn%.h
echo.>> %fn%.h
echo #include ^<SFML\Graphics\RenderWindow.hpp^>>> %fn%.h
echo.>> %fn%.h
echo extern sf::RenderWindow* g_WINDOW;>> %fn%.h
echo.>> %fn%.h
echo class %fn%>> %fn%.h
echo {>> %fn%.h
echo public:>> %fn%.h
echo   %fn%(){};>> %fn%.h
echo   ~%fn%(){};>> %fn%.h
echo.>> %fn%.h
echo public:>> %fn%.h
echo   void Update(float a_DeltaTime);>> %fn%.h
echo   void Draw();>> %fn%.h
echo.>> %fn%.h
echo };>> %fn%.h

::newline
echo.>> %fn%.h 

::write stuff into .cpp
echo #include "%fn%.h">> %fn%.cpp
echo. >> %fn%.cpp
echo %fn%::%fn%()>> %fn%.cpp
echo {>> %fn%.cpp
echo }>> %fn%.cpp
echo.>> %fn%.cpp
echo %fn%::~%fn%()>> %fn%.cpp
echo {>> %fn%.cpp
echo }>> %fn%.cpp
echo.>> %fn%.cpp
echo void %fn%::Update(float a_DeltaTime)>> %fn%.cpp
echo {>> %fn%.cpp
echo.   >> %fn%.cpp
echo }>> %fn%.cpp
echo.>> %fn%.cpp
echo void %fn%::Draw()>> %fn%.cpp
echo {>> %fn%.cpp
echo.   >> %fn%.cpp
echo }>> %fn%.cpp
echo.>> %fn%.cpp