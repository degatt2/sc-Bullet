#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "btBulletDynamicsCommon.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicBulletApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	btDiscreteDynamicsWorld* world;
};

void BasicBulletApp::setup()
{
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher( collisionConfiguration );
	
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	
	world = new btDiscreteDynamicsWorld( dispatcher, overlappingPairCache, solver, collisionConfiguration );
	world->setGravity( btVector3( 0, -10, 0 ) );
	
	// create the ground body
	
	btCollisionShape* groundShape = new btBoxShape( btVector3( btScalar(50.), btScalar(50.), btScalar(50.) ) );
	
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	
	collisionShapes.push_back(groundShape);
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin( btVector3( 0, -56, 0 ) );
	
	{
		btScalar mass(0.);
		bool isDynamic = ( mass != 0.f );
		
		btVector3 localInertia( 0, 0, 0 );
		if( isDynamic )
			groundShape->calculateLocalInertia(mass, localInertia);
		
		btDefaultMotionState* groundMotionState = new btDefaultMotionState( groundTransform );
		btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, groundMotionState, groundShape, localInertia );
		btRigidBody* body = new btRigidBody( rbInfo );
		
		world->addRigidBody( body );
	}
	
	{
		// create a dynamic rigidbody
		btCollisionShape* colShape = new btSphereShape( btScalar(1.) );
		collisionShapes.push_back( colShape );
		
		// create dynamic objects
		btTransform startTransform;
		startTransform.setIdentity();
			
		btScalar	mass( 1.0f );
		bool	isDynamic = ( mass != 0.0f );
		
		btVector3 localInertia( 0, 0, 0 );
		
		if( isDynamic )
			colShape->calculateLocalInertia(mass, localInertia);
		
		startTransform.setOrigin( btVector3( 2, 10, 0 ) );
		
		btDefaultMotionState* motionStation = new btDefaultMotionState( startTransform );
		btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, motionStation, colShape, localInertia );
		btRigidBody* body = new btRigidBody( rbInfo );
		
		world->addRigidBody( body );
	}
	
	// simulate some stuff
	
	for( int t=0; t != 100; ++t )
	{
		world->stepSimulation( 1.0f/60.0f , 10 );
		
		for ( int j=world->getNumCollisionObjects()-1; j >= 0; j-- )
		{
			btCollisionObject* obj = world->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			if( body && body->getMotionState() )
			{
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);
				console() << "World pos = " << trans.getOrigin().getX() << ", " << trans.getOrigin().getY() << ", "
					<< trans.getOrigin().getZ() << endl;
			}
		}
	}
	
	// delete rigidbodies
	for( int i = world->getNumCollisionObjects()-1; i >= 0; i-- )
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		
		if( body && body->getMotionState() )
		{
			delete body->getMotionState();
		}
		world->removeCollisionObject( obj );
		delete obj;
	}
	
	// delete collision shapes
	for( int j=0; j < collisionShapes.size(); j++ )
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}
	
	delete world;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
	
	quit();
}

void BasicBulletApp::mouseDown( MouseEvent event )
{
}

void BasicBulletApp::update()
{
}

void BasicBulletApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}


CINDER_APP_BASIC( BasicBulletApp, RendererGl )
