#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include "BaseLayer.h"
#include "TypeInfoDef.h"

class GameWorld;
class GameMapLogic;
class GameMap: public BaseLayer<GameMap>
{
public:
	GameMap();
	virtual ~GameMap();

	virtual bool init();
	void initWithMap();
	void loadObject(experimental::TMXTiledMap *map, int height);
	virtual bool databind(void *data);

	void update(float dt);

	void startGame();

	void setGameWorld(GameWorld *gameWorld);
	GameWorld* getGameWorld() const { return m_pGameWorld; }

	NormalVector* getNormals() { return &m_allNormals; }

private:
	GameWorld		*m_pGameWorld;
	GameMapLogic	*m_pGameMapLogic;
	MapVector			m_allMaps;

	NormalVector		m_allNormals;
};

#endif