                                                                                                     /*
/##################################################################################################\
# Description : #                                                                                  #
#################                                                                                  #
#                                                                                                  #
#  Ce fichier est le point d'entr�e du moteur ( le "main()" ).                                     #
#                                                                                                  #
#                                                                                                  #
\##################################################################################################/
                                                                                                     */

#include "window.h"
#include "input.h"
#include "timer.h"
#include "ogl.h"
#include "load.h"

#include "GameControl.h"


/****************************************************************************\
*                                                                            *
*                            Variables EXTERNES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                            Variables GLOBALES                              *
*                                                                            *
\****************************************************************************/

WINDOW    *win = NULL;
MY_INPUT     *inp = NULL;
TIMER     *tim = NULL;

GameControl *game = NULL;

/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/





/********************************************************************\
*                                                                    *
*  D�marre l'application (avant la main_loop). Renvoie false si      *
*  il y a une erreur.                                                *
*                                                                    *
\********************************************************************/
bool start()
{
	// initialisations de base
	// -----------------------
	win = new WINDOW();									// pr�pare la fen�tre
	win->create( 1366, 768, 16 , 60 , false );			// cr�e la fen�tre

	tim = new TIMER();									// cr�e un timer

	inp = new MY_INPUT(win);								// initialise la gestion clavier souris
	create_context(*win);								// cr�e le contexte OpenGL sur la fen�tre
	init_font(*win, "Courier");							// initialise la gestion de texte avec OpenGL

  
	// initialisations d'OpenGL
	// ------------------------
	glClearColor(0.0f,0.0f,0.0f,1.0f);					// set clear color for color buffer (RGBA, black)
	glViewport(0,0,win->Xres,win->Yres);				// zone de rendu (tout l'�cran)
 	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// mapping quality = best
	glFrontFace(GL_CCW);								// front of face is defined counter clock wise
	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK,  GL_FILL);					// back of a face is made of lines
	glCullFace(GL_BACK);								// cull back face only
	glDisable(GL_CULL_FACE);						    // disable back face culling

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 0.0, 0.0, 10.0, 0.0 };
	GLfloat zeroes[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat halves[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat ones[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat attenuation[] = { 0.01f };
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position);
	glLightfv(GL_LIGHT4, GL_POSITION, light_position);
	glLightfv(GL_LIGHT5, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, halves);
	glLightfv(GL_LIGHT1, GL_AMBIENT, zeroes);
	glLightfv(GL_LIGHT2, GL_AMBIENT, zeroes);
	glLightfv(GL_LIGHT3, GL_AMBIENT, zeroes);
	glLightfv(GL_LIGHT4, GL_AMBIENT, zeroes);
	glLightfv(GL_LIGHT5, GL_AMBIENT, zeroes);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, halves);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, zeroes);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, zeroes);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, zeroes);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, zeroes);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, zeroes);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zeroes);
	glLightfv(GL_LIGHT1, GL_SPECULAR, zeroes);
	glLightfv(GL_LIGHT2, GL_SPECULAR, zeroes);
	glLightfv(GL_LIGHT3, GL_SPECULAR, zeroes);
	glLightfv(GL_LIGHT4, GL_SPECULAR, zeroes);
	glLightfv(GL_LIGHT5, GL_SPECULAR, zeroes);
	glLightfv(GL_LIGHT2, GL_LINEAR_ATTENUATION, attenuation);
	glLightfv(GL_LIGHT3, GL_LINEAR_ATTENUATION, attenuation);
	glLightfv(GL_LIGHT4, GL_LINEAR_ATTENUATION, attenuation);
	glLightfv(GL_LIGHT5, GL_LINEAR_ATTENUATION, attenuation);

	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	//glEnable(GL_DEPTH_TEST);


	win->set_title("Pacman: the movie: the game: the musical");

	game = new GameControl();
	
	return true;
}

/********************************************************************\
*                                                                    *
*  les formes                                               *
*                                                                    *
\********************************************************************/

/********************************************************************\
*                                                                    *
*  Boucle principale, appel�e pour construire une image, g�rer les   *
*  entr�es.                                                          *
*                                                                    *
\********************************************************************/
void main_loop() 
{

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//				gestion des touches	et du temps													//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	inp->refresh();
	tim->update_horloge();


	if (inp->keys[KEY_CODE_ESCAPE]) 
	{	  
		PostMessage(win->handle,WM_CLOSE,0,0);	// Stoppe la "pompe � message" en y envoyant le message "QUIT"
	}





	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						�a commence ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	game->Update(tim->get_dt());
	game->ProcessInput(inp);

	// votre code OpenGL ici
	glMatrixMode(GL_PROJECTION);  //la matrice de projection sera celle selectionnee
								  //composition d'une matrice de projection
	glLoadIdentity(); //on choisit la matrice identit�
	gluPerspective(60, (double)win->Xres / (double)win->Yres, 0.01, 30000);   //mise en place d une proj angle de vue 60 deg near 10 far 30000
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(14, 18, -25,		// position
	//	14, 18, 1,		// point cible
	//	0, -1, 0);		// vecteur up
	game->SetCamera(false);
	game->Draw();

	//game->DrawMap(win, -102.0f, -60.0f, 60, 60);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						�a finit ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	swap_buffer(win);	// affiche l'image compos�e � l'�cran
}


/********************************************************************\
*                                                                    *
* Arr�te l'application                                               *
*                                                                    *
\********************************************************************/
void stop()
{
	delete inp;
	delete tim;

	delete game;

	if (win)
	{
		kill_font();
		kill_context(*win);
		delete win;
	}
}


/********************************************************************\
*                                                                    *
* Point d'entr�e de notre programme pour Windows ("WIN32")           *
*                                                                    *
\********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR szCmdLine,int iCmdShow)
{
	MSG  msg;

	win	= NULL;
	inp	= NULL;
	tim = NULL;

	if (start() == false)								// initialise la fen�tre et OpenGL
	{
		debug("start() : �chec !");
		stop();
		return 1;
	}

	// main loop //
	// --------- //
	//   __
	//  /  \_
	//  |  \/ 
	//  \__/

	while (true)
	{
		if (PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))	// s'il y a un message, appelle WndProc() pour le traiter
		{
			if (!GetMessage(&msg,NULL,0,0))				// "pompe � message"
				break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		else 
		{
			main_loop();								// sinon, appelle main_loop()
		}
	}


	stop();												// arr�te OpenGL et ferme la fen�tre

	return 0;
}



/********************************************************************\
*                                                                    *
*  Boucle des messages                                               *
*                                                                    *
\********************************************************************/
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch (msg)
    {

      case WM_MOVE:        win->Xpos = (dword) LOWORD(lParam);
                           win->Ypos = (dword) HIWORD(lParam);
                           return 0;

      case WM_CLOSE:       PostQuitMessage(0); // dit � GetMessage() de renvoyer 0
                           return 0;

      case WM_SYSCOMMAND:  // �vite l'�conomiseur d'�cran
                           switch (wParam)
                             {
                               case SC_SCREENSAVE:
                               case SC_MONITORPOWER:
                               return 0;
                             }
                           break;
/*
      case WM_CHAR:        la touche est traduite dans ce msg 
                           return 0;
*/

      case WM_KEYDOWN:     inp->set_key_down ((dword)wParam);
                           return 0;

      case WM_KEYUP:       inp->set_key_up   ((dword)wParam);
                           return 0;

      case WM_LBUTTONDOWN: inp->set_mouse_left_down();
                           return 0;

      case WM_LBUTTONUP:   inp->set_mouse_left_up();
                           return 0;

      case WM_RBUTTONDOWN: inp->set_mouse_right_down();
                           return 0;

      case WM_RBUTTONUP:   inp->set_mouse_right_up();
                           return 0;

      case WM_MBUTTONDOWN: inp->set_mouse_middle_down();
                           return 0;

      case WM_MBUTTONUP:   inp->set_mouse_middle_up();
                           return 0;

   }

  return DefWindowProc(hwnd,msg,wParam,lParam);
}

