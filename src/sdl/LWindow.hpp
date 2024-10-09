#ifndef LWINDOW_HPP
#define LWINDOW_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include <stdio.h>
#include <string>

/* SDL Init object */
class LWindow
{
public:
  /* The window renderer */
  SDL_Renderer *gRenderer = NULL;

  /* Globally used font */
  TTF_Font *gFont = NULL;

  /* Handles window events */
  void handleEvent(SDL_Event &e);

  /* Window width in pixels */
  int getWidth();
  /* Window height in pixels */
  int getHeight();

  /* The X render scale of the window */  
  float getScaleX();
  /* The Y render scale of the window */  
  float getScaleY();

  /* Window focii */
  bool hasMouseFocus();
  bool hasKeyboardFocus();
  bool isMinimized();

  /* toggles fullscreen */
  bool toggleFullscreen();

  /* Initialises variables */
  LWindow(int width = 1280, int height = 720, std::string name = "Window");

  /* deallocates memory */
  ~LWindow();

private:
  /* The window being rendered to */
  SDL_Window *gWindow = NULL;

  /* Window dimensions */
  int wWidth;
  int wHeight;

  /* Scaling factor */
  float scaleX = 1.0f;
  float scaleY = 1.0f;

  /* Window focus */
  bool wMouseFocus;
  bool wKeyboardFocus;
  bool wFullScreen;
  bool wMinimized;

  /* The name of the window */
  std::string windowName= "";
};

#endif