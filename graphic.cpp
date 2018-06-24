#include <irrlicht.h>
#include "sailshade.hpp"

//=================================
// scene node custom (sailshade)
namespace irr
{
namespace scene
{
class CSailshadeSceneNode : public irr::scene::ISceneNode
{

private :

    irr::core::aabbox3d<irr::f32> m_box;           
    irr::video::SMaterial m_material;
    irr::video::S3DVertex m_vertices[6];

public:

    CSailshadeSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, SailShade::Triangle sail, bool shadow, irr::s32 id=0):
    	irr::scene::ISceneNode(parent, sceneManager, id)
    {
	    m_material.Wireframe = false;
	    m_material.Lighting = false;

	    irr::video::SColor color;
	    if(shadow)
	    	color = irr::video::SColor(100,0,0,0);
	    else
	    	color = irr::video::SColor(255,40,170,170);

	    m_vertices[0] = irr::video::S3DVertex(sail.p1.x, sail.p1.z + 0.05f, sail.p1.y,  0,0,0, color, 0,0);
		m_vertices[1] = irr::video::S3DVertex(sail.p1.x, sail.p1.z + 0.06f, sail.p1.y,  0,0,0, color, 0,0);
		m_vertices[2] = irr::video::S3DVertex(sail.p2.x, sail.p2.z + 0.05f, sail.p2.y,  0,0,0, color, 0,0);
		m_vertices[3] = irr::video::S3DVertex(sail.p2.x, sail.p2.z + 0.06f, sail.p2.y,  0,0,0, color, 0,0);
		m_vertices[4] = irr::video::S3DVertex(sail.p3.x, sail.p3.z + 0.05f, sail.p3.y,  0,0,0, color, 0,0);
		m_vertices[5] = irr::video::S3DVertex(sail.p3.x, sail.p3.z + 0.06f, sail.p3.y,  0,0,0, color, 0,0);

		if(!shadow)
		{
			irr::scene::IMeshSceneNode* pilier1 = sceneManager->addCubeSceneNode (0.05f, 0, -1, irr::core::vector3df(sail.p1.x, sail.p1.z/2, sail.p1.y), irr::core::vector3df(0.0f,0.0f,0.0f), irr::core::vector3df(1.0f, sail.p1.z/0.05,1.0f));
			pilier1->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
			//pilier1->setMaterialTexture(0, sceneManager->getVideoDriver()->getTexture("media/pilar.jpg"));
			pilier1->addShadowVolumeSceneNode();

			irr::scene::IMeshSceneNode* pilier2 = sceneManager->addCubeSceneNode (0.05f, 0, -1, irr::core::vector3df(sail.p2.x, sail.p2.z/2, sail.p2.y), irr::core::vector3df(0.0f,0.0f,0.0f), irr::core::vector3df(1.0f, sail.p2.z/0.05, 1.0f));
			pilier2->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
			//pilier2->setMaterialTexture(0, sceneManager->getVideoDriver()->getTexture("media/pilar.jpg"));
			pilier2->addShadowVolumeSceneNode();

			irr::scene::IMeshSceneNode* pilier3 = sceneManager->addCubeSceneNode (0.05f, 0, -1, irr::core::vector3df(sail.p3.x, sail.p3.z/2, sail.p3.y), irr::core::vector3df(0.0f,0.0f,0.0f), irr::core::vector3df(1.0f, sail.p3.z/0.05, 1.0f));
			pilier3->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
			//pilier3->setMaterialTexture(0, sceneManager->getVideoDriver()->getTexture("media/pilar.jpg"));
			pilier3->addShadowVolumeSceneNode();
		}

	    m_box = irr::core::aabbox3d<irr::f32>(-20.0f, -20.0f, -20.0f, 20.0f, 20.0f, 20.0f);

	    m_material.BackfaceCulling = false;
	}

    virtual void OnRegisterSceneNode()
    {
	    if (IsVisible)
	        SceneManager->registerNodeForRendering(this);

	    ISceneNode::OnRegisterSceneNode();
	}

    virtual void render()
    {
    	irr::u16 indices[] = {5,3,1, 1,3,0, 2,0,3, 3,5,2, 4,2,5, 5,1,4, 0,4,1, 0,2,4};
    	irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

	    driver->setMaterial(m_material);
	    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);

    	driver->drawIndexedTriangleList(&m_vertices[0], 6, &indices[0], 8);
	}                      


    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const
    {
    	return m_box;
    }
    virtual irr::u32 getMaterialCount() const
    {
    	return 1;
    }
    virtual irr::video::SMaterial& getMaterial(irr::u32 i)
    {
    	i=i; // remove useless warning
    	return m_material;
    }             
};

class CFloorSceneNode : public irr::scene::ISceneNode
{

private :

    irr::core::aabbox3d<irr::f32> m_box;           
    irr::video::SMaterial m_material;
    irr::video::S3DVertex m_vertices[8];

public:

    CFloorSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* sceneManager, SailShade::Rectangle floor, irr::s32 id=0):
    	irr::scene::ISceneNode(parent, sceneManager, id)
    {
	    m_material.Wireframe = false;
	    m_material.Lighting = false;

	    irr::video::SColor color = irr::video::SColor(255,255,255,255);

	    m_vertices[0] = irr::video::S3DVertex(floor.p1.x, floor.p1.z,       floor.p1.y,  0,0,0, color, 0,0);
		m_vertices[1] = irr::video::S3DVertex(floor.p1.x, floor.p1.z + 0.01, floor.p1.y,  0,0,0, color, 0,0);
		m_vertices[2] = irr::video::S3DVertex(floor.p2.x, floor.p2.z,       floor.p2.y,  0,0,0, color, 0,0);
		m_vertices[3] = irr::video::S3DVertex(floor.p2.x, floor.p2.z + 0.01, floor.p2.y,  0,0,0, color, 0,0);
		m_vertices[4] = irr::video::S3DVertex(floor.p3.x, floor.p3.z,       floor.p3.y,  0,0,0, color, 0,0);
		m_vertices[5] = irr::video::S3DVertex(floor.p3.x, floor.p3.z + 0.01, floor.p3.y,  0,0,0, color, 0,0);
		m_vertices[6] = irr::video::S3DVertex(floor.p4.x, floor.p4.z,       floor.p4.y,  0,0,0, color, 0,0);
		m_vertices[7] = irr::video::S3DVertex(floor.p4.x, floor.p4.z + 0.01, floor.p4.y,  0,0,0, color, 0,0);

	    m_box = irr::core::aabbox3d<irr::f32>(-20.0f, -20.0f, -20.0f, 20.0f, 20.0f, 20.0f);

	    m_material.BackfaceCulling = false;
	}

    virtual void OnRegisterSceneNode()
    {
	    if (IsVisible)
	        SceneManager->registerNodeForRendering(this);

	    ISceneNode::OnRegisterSceneNode();
	}

    virtual void render()
    {
    	irr::u16 indices[] = {4,2,6, 6,2,0, 0,2,1, 1,2,3, 3,2,5, 5,2,4, 4,6,5, 5,6,7, 7,6,1, 1,6,0, 1,3,7, 7,3,5};
    	irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

	    driver->setMaterial(m_material);
	    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);

    	driver->drawIndexedTriangleList(&m_vertices[0], 8, &indices[0], 12);
	}                      


    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const
    {
    	return m_box;
    }
    virtual irr::u32 getMaterialCount() const
    {
    	return 1;
    }
    virtual irr::video::SMaterial& getMaterial(irr::u32 i)
    {
    	i=i; // remove useless warning
    	return m_material;
    }             
};
}
}
//=======================================

void SailShade::graphic(struct Rectangle rect, Location loc, Time time_beg, Time time_end)
{
	Azimuth azmt(loc, time_beg, time_end);
	double* director_ray = azmt.calc_azimuths(2);
	long nb_azimuth = azmt.m_nb_azimuth;

	/*==========INITIALISATION==========*/

		//création du device
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1600,1200), 32, false, true, false, 0);
		//création du driver
	irr::video::IVideoDriver* driver = device->getVideoDriver();
		//création du scene manager
	irr::scene::ISceneManager* sceneManager = device->getSceneManager();
		//curseur invisible
	device->getCursorControl()->setVisible(false);

	/*==========MODELE==========*/

	irr::scene::IMeshSceneNode* grass = sceneManager->addCubeSceneNode (10000.0f, 0, -1, irr::core::vector3df(0.0f,-0.05f,0.0f), irr::core::vector3df(0.0f,0.0f,0.0f), irr::core::vector3df(1.0f,0.00001f,1.0f));

	struct Rectangle floor = rect;
	irr::scene::CFloorSceneNode* floorNode = new irr::scene::CFloorSceneNode(sceneManager->getRootSceneNode(), sceneManager, floor, -1);

	Triangle sail = m_best.triangle;
	double director[3];
	director[0] = director_ray[0];
	director[1] = director_ray[1];
	director[2] = director_ray[2];
	Triangle projection = getProjectTriangle(sail, director);
	irr::scene::CSailshadeSceneNode* sailshadeNode = new irr::scene::CSailshadeSceneNode(sceneManager->getRootSceneNode(), sceneManager, sail, false, -1);
	sailshadeNode->drop();
	irr::scene::CSailshadeSceneNode* shadow = new irr::scene::CSailshadeSceneNode(sceneManager->getRootSceneNode(), sceneManager, projection, true, -1);

	/*==========MATERIALS, TEXTURES, LIGHTS==========*/

		//materials
	grass->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
	floorNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
	sailshadeNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
	shadow->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_VERTEX_ALPHA);
		//textures
	grass->setMaterialTexture(0, driver->getTexture("media/grass.jpg"));
	grass->getMaterial(0).getTextureMatrix(0).setTextureScale(500.0f, 500.0f);
	floorNode->setMaterialTexture(0, driver->getTexture("media/zone.jpg"));
		//light
    irr::scene::ILightSceneNode* lumiere = sceneManager->addLightSceneNode(0, irr::core::vector3df(director[0]*10000000000.0f,-director[2]*10000000000.0f,director[1]*10000000000.0f), irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 800.0f);
  	lumiere->setRadius(10000000000.0f);


	/*==========CAMERA==========*/

		//création de la keymap
	irr::SKeyMap keyMap[5];						//re-assigne les commandes
	keyMap[0].Action = irr::EKA_MOVE_FORWARD;	//avancer
	keyMap[0].KeyCode = irr::KEY_KEY_Z;			//w
	keyMap[1].Action = irr::EKA_MOVE_BACKWARD;	//reculer
	keyMap[1].KeyCode = irr::KEY_KEY_S;			//s
	keyMap[2].Action = irr::EKA_STRAFE_LEFT;	//gauche
	keyMap[2].KeyCode = irr::KEY_KEY_Q;			//a
	keyMap[3].Action = irr::EKA_STRAFE_RIGHT;	//droite
	keyMap[3].KeyCode = irr::KEY_KEY_D;			//d
	keyMap[4].Action = irr::EKA_JUMP_UP;		//sauter
	keyMap[4].KeyCode = irr::KEY_SPACE;			//espace
		//ajout caméra FPS
	irr::scene::ICameraSceneNode* camera = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.1f, -1, keyMap, 5);
	camera->setPosition(irr::core::vector3df(-10.0f, 10.0f, 0.0f));

	/*==========RENDU==========*/

		//couleur du fond (A,R,G,B)
	irr::video::SColor color(128,119,181,254);

	int azimuth=0;
	int i=0;
		//boucle principale
	while(device->run())
	{
		//demarre le rendu
		driver->beginScene(true, true, color);
		//calcule le rendu
		sceneManager->drawAll();
		//affiche le rendu
		driver->endScene();

		//passage à la nouvelle ombre
		i++;
		if(i == 10)
		{
			i=0;
			azimuth++;
			if(azimuth == nb_azimuth)
				azimuth = 0;
			director[0] = director_ray[azimuth*3];
			director[1] = director_ray[azimuth*3+1];
			director[2] = director_ray[azimuth*3+2];

			//lumiere->setPosition(irr::core::vector3df(-director[0]*10000000000.0f,director[2]*10000000000.0f,-director[1]*10000000000.0f));

			projection = getProjectTriangle(sail, director);
			shadow->remove();
			shadow = new irr::scene::CSailshadeSceneNode(sceneManager->getRootSceneNode(), sceneManager, projection, true, -1);
			shadow->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_VERTEX_ALPHA);
		}
	}

	/*==========FIN==========*/

		//libération de la mémoire
	shadow->drop();
	device->drop();
}
