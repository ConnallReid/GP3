#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


//#include <windows.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"
#include "cPyramid.h"
#include "cCube.h"
#include "cSphere.h"
#include "cMaterial.h"
#include "cLight.h"
#include "cStarfield.h"
#include "cFontMgr.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cLaser.h"
#include "tardisWarsGame.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;

    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
	
	//The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);


    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	// Create Texture map
	cTexture tardisTexture;
	tardisTexture.createTexture("Models/tardis.png");
	cTexture spaceShipTexture;
	spaceShipTexture.createTexture("Models/SpaceShip/sh3.jpg");
	cTexture laserTexture;
	laserTexture.createTexture("Models/laser.tga");
	cTexture starTexture;
	starTexture.createTexture("Images/star.png");

	cTexture spaceShip9000Texture;
	spaceShip9000Texture.createTexture("Models/SpaceShip9000/Maps/vj2c.jpg");

	//cTexture rock1Texture;
	//rock1Texture.createTexture("Models/Rock1/Rock-Texture-Surface.jpg");

	//cTexture rock2Texture;
	//rock2Texture.createTexture("Models/Rock2/Download.jpg");

	// the starfield
	cStarfield theStarField(starTexture.getTexture(), glm::vec3(50.0f, 50.0f, 50.0f));

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);

	// Create Light
	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 20, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight lfLight(GL_LIGHT1, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight rfLight(GL_LIGHT2, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(-30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight cbLight(GL_LIGHT3, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, -100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	// Load Fonts
	LPCSTR gameFonts[4] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/doctor_who.ttf", "game_over.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 72);
	theFontMgr->addFont("Space", gameFonts[1], 24);
	theFontMgr->addFont("DrWho", gameFonts[2], 48);
	//theFontMgr->addFont("GameOver", gameFonts[3], 1);

	// Load Sound
	LPCSTR gameSounds[4] = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav", "Audio/9000Miles.mp3" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	theSoundMgr->add("GameMusic", gameSounds[3]);

	// Create Camera 1
	cCamera theCamera1;
	theCamera1.setTheCameraPos(glm::vec3(0.0f, 0.0f, 75.0f));
	theCamera1.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera1.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera1.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera1.setTheProjectionMatrix(45.0f, theCamera1.getTheCameraAspectRatio(), 0.1f, 300.0f);
	theCamera1.update();

	// Create Camera 2
	cCamera theCamera2;
	theCamera2.setTheCameraPos(glm::vec3(0.0f, 0.0f, 60.0f));
	theCamera2.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera2.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera2.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera2.setTheProjectionMatrix(45.0f, theCamera2.getTheCameraAspectRatio(), 0.1f, 300.0f);
	theCamera2.update();

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	// Model
	cModelLoader tardisMdl;
	tardisMdl.loadModel("Models/tardis1314.obj", tardisTexture); // Player

	cModelLoader spaceShipMdl;
	spaceShipMdl.loadModel("Models/SpaceShip/Sample_Ship.obj", spaceShipTexture); // Enemy
	
	cModelLoader theLaser;
	theLaser.loadModel("Models/laser.obj", laserTexture);

	cModelLoader spaceShip9000Model;
	spaceShip9000Model.loadModel("Models/SpaceShip9000/Wraith_Raider_Starship.obj", spaceShip9000Texture);

	//cModelLoader rock1Model;
	//rock1Model.loadModel("Models/Rock1/Rock1.obj", rock1Texture);

	//cModelLoader rock2Model;
	//rock2Model.loadModel("Models/Rock2/sculpt.obj", rock2Texture);

	for (int loop = 0; loop < 5; loop++)
	{
		theEnemy.push_back(new cEnemy);
		theEnemy[loop]->randomise();
		theEnemy[loop]->setMdlDimensions(spaceShipMdl.getModelDimensions());
		theEnemy[loop]->setScale(glm::vec3(5, 5, 5));
	}

	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, -60), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(tardisMdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);


	float tCount = 0.0f;
	string timeRemaining;
	string muteMesage;
	string gameOver;

	//theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);

	if (mute = false){
		theSoundMgr->getSnd("GameMusic")->playAudio(AL_LOOPING);
	}
	else{

	}
	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;

   //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();
		
		// Lab code goes here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth,windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (camMode == 0){
			glLoadMatrixf((GLfloat*)& theCamera1.getTheViewMatrix());
		}
		else if (camMode == 1){
			glLoadMatrixf((GLfloat*)& theCamera2.getTheViewMatrix());
		}

		theStarField.render(0.0f);
		sunMaterial.useMaterial();
		sunLight.lightOn();
		lfLight.lightOn();
		rfLight.lightOn();
		cbLight.lightOn();

		for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->isActive())
			{
				spaceShipMdl.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
				(*enemyIterator)->update(elapsedTime);
			}
		}

		tardisMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
		thePlayer.update(elapsedTime);
		
		for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
		{
			if ((*laserIterartor)->isActive())
			{
				theLaser.renderMdl((*laserIterartor)->getPosition(), (*laserIterartor)->getRotation(), (*laserIterartor)->getScale());
				(*laserIterartor)->update(elapsedTime);
			}
		}

		//Create Game Timer
		miliSeconds -= 1;
		if (miliSeconds == 0){
			seconds-=1;
			miliSeconds = 60;
			if (seconds < 0){
				seconds = 0;
			}
		}
		timeRemaining = to_string(seconds);
		
		//Game Over When Time Runs Out
		if (seconds ==0){
			gameOver = "Game Over";
		}
		else {
			gameOver = "";
		}

		// display message when game muted
		if (mute == true){
			muteMesage = "Mute";
		}
		if (mute == false){
			muteMesage = " ";
		}
		
		glPushMatrix();
		theOGLWnd.setOrtho2D(windowWidth, windowHeight);
		theFontMgr->getFont("Space")->printText("Time Remaining", FTPoint(10, 35, 0.0f), colour3f(0.0f,255.0f,0.0f));
		theFontMgr->getFont("Space")->printText(timeRemaining.c_str(), FTPoint(260, 35, 0.0f), colour3f(255.0f, 255.0f, 0.0f)); // Display Time Remaining
		theFontMgr->getFont("Space")->printText(muteMesage.c_str(), FTPoint(10, 55, 0.0f), colour3f(255.0f, 0.0f, 0.0f));      // Display Mute When Mute
		theFontMgr->getFont("SevenSeg")->printText(gameOver.c_str(), FTPoint(400, 400, 10.0f), colour3f(255.0f, 0.0f, 0.0f));
		glPopMatrix();

		pgmWNDMgr->swapBuffers();

		tCount += elapsedTime;

		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
