#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{

}

void cPlayer::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

GLfloat VelocityX = 0.0f;
GLfloat VelocityY = 0.0f;

void cPlayer::update(float elapsedTime)
{
	//Swap Cameras
	if (m_InputMgr->isKeyDown(VK_NUMPAD1)){
		camMode++;
		if (camMode == 2){
			camMode = 0;
		}
	}
	
	//Swap Cameras
	if (m_InputMgr->isKeyDown(VK_NUMPAD2)){
		musicMode++;
		if (musicMode == 0){
			mute = false;
		}
		if (musicMode == 1){
			mute = true;
		}
		if (musicMode == 2){
			mute = false;
			musicMode = 0;
		}
	}

	//Player Movement
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		rotationAngle -= 90.0f;
		translationZ -= 3.0f;
	}
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		rotationAngle -= 90.0f;
		translationZ += 3.0f;
	}
	if (m_InputMgr->isKeyDown(VK_UP))
	{
		rotationAngle += 90.0f;
		translationZ -= 3.0f;
	}
	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		rotationAngle += 90.0f;
		translationZ += 3.0f;
	}


	if (m_InputMgr->isKeyDown("w"||"W"))
	{
		translationX += 5.0f;
	}
	if (m_InputMgr->isKeyDown("s" || "S"))
	{
		translationX -= 5.0f;
	}
	if (m_InputMgr->isKeyDown("d" || "D"))
	{
		translationY += 5.0f;
	}
	if (m_InputMgr->isKeyDown("a" || "A"))
	{
		translationY -= 5.0f;
	}

/*
	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		glm::vec3 mdlLaserDirection;
		mdlLaserDirection.x = -(float)glm::sin(glm::radians(this->getRotation()));
		mdlLaserDirection.y = 0.0f;
		mdlLaserDirection.z = (float)glm::cos(glm::radians(this->getRotation()));
		mdlLaserDirection *= -1;

		// Add new bullet sprite to the vector array
		theTardisLasers.push_back(new cLaser);
		int numLasers = theTardisLasers.size() - 1;
		theTardisLasers[numLasers]->setDirection(mdlLaserDirection);
		theTardisLasers[numLasers]->setRotation(0.0f);
		theTardisLasers[numLasers]->setScale(glm::vec3(1, 1, 1));
		theTardisLasers[numLasers]->setSpeed(5.0f);
		theTardisLasers[numLasers]->setPosition(this->getPosition() + mdlLaserDirection);
		theTardisLasers[numLasers]->setIsActive(true);
		//theTardisLasers[numLasers]->setMdlDimensions(theLaser.getModelDimensions());
		theTardisLasers[numLasers]->update(elapsedTime);
		// play the firing sound
		m_SoundMgr->getSnd("Shot")->playAudio(AL_TRUE);
	}
*/
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
/*
	for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
	{
		(*laserIterartor)->update(elapsedTime);
		for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->SphereSphereCollision((*laserIterartor)->getPosition(), (*laserIterartor)->getMdlRadius()))
			{
				// if a collision set the bullet and spaceship to false
				(*enemyIterator)->setIsActive(false);
				(*laserIterartor)->setIsActive(false);
				// play the explosion sound.
				m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
			}
		}
	}

	vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin();
	while (laserIterartor != theTardisLasers.end())
	{
		if ((*laserIterartor)->isActive() == false)
		{
			laserIterartor = theTardisLasers.erase(laserIterartor);
		}
		else
		{
			++laserIterartor;
		}
	}

	vector<cEnemy*>::iterator enemyIterartor = theEnemy.begin();
	while (enemyIterartor != theEnemy.end())
	{
		if ((*enemyIterartor)->isActive() == false)
		{
			enemyIterartor = theEnemy.erase(enemyIterartor);
		}
		else
		{
			++enemyIterartor;
		}
	}
*/
	// Find out what direction we should be thrusting, using rotation.
	m_mdlRotation -= rotationAngle;
	if (m_mdlRotation > 90){ m_mdlRotation = 90;}
	if (m_mdlRotation < 0){ m_mdlRotation = 0;}
	
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = -(float)glm::sin(glm::radians(m_mdlRotation));  // Remember to adjust for radians
	mdlVelocityAdd.y = 0.0f;
	mdlVelocityAdd.z = -(float)glm::cos(glm::radians(m_mdlRotation));

	//mdlVelocityAdd *= translationX;
	//mdlVelocityAdd *= translationY;
	mdlVelocityAdd *= translationZ;
	m_mdlDirection += mdlVelocityAdd;

	VelocityX -= (translationX * elapsedTime);
	VelocityY -= (translationY * elapsedTime);

	m_mdlPosition += m_mdlDirection * m_mdlSpeed *elapsedTime;
	m_mdlDirection *= 0.95f;

	rotationAngle = 0;
	translationX = 0;
	translationY = 0;
	translationZ = 0;
}

cPlayer::~cPlayer()
{

}