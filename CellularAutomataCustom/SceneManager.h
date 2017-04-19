/*SceneManager.h generated on Thu 02/16/2017 at 19:38:46.26*/
#pragma once

#include "TileScene2D.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

public:
	void PreUpdate(double a_DeltaTime);
	void Update(double a_DeltaTime);
	void Draw();

private:
	TileScene2D* m_TileScene2d;

};

