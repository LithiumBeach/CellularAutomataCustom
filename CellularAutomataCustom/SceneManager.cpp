#include "SceneManager.h"
 
SceneManager::SceneManager()
{
	m_TileScene2d = new TileScene2D();
}

SceneManager::~SceneManager()
{
	delete m_TileScene2d;
	m_TileScene2d = nullptr;
}

void SceneManager::PreUpdate(double a_DeltaTime)
{
	m_TileScene2d->PreUpdate((float)a_DeltaTime);
}

void SceneManager::Update(double a_DeltaTime)
{
	//m_TileScene2d->Update((float)a_DeltaTime);
}

void SceneManager::Draw()
{
	m_TileScene2d->Draw();
}
