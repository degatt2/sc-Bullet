#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "btBulletDynamicsCommon.h"
#include "sansumbrella/DebugDraw.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace sansumbrella;

class BasicBulletApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void shutdown();
	
	btDiscreteDynamicsWorld* mWorld;
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher* mDispatcher;
	btBroadphaseInterface* mOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mSolver;
	DebugDrawer* mDebugDrawer;
	btAlignedObjectArray<btCollisionShape*> mCollisionShapes;
};

void BasicBulletApp::setup()
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher( mCollisionConfiguration );
	
	mOverlappingPairCache = new btDbvtBroadphase();
	
	mSolver = new btSequentialImpulseConstraintSolver;
	
	mWorld = new btDiscreteDynamicsWorld( mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration );
	mWorld->setGravity( btVector3( 0, -10, 0 ) );
	
	mDebugDrawer = new DebugDrawer();
	mWorld->setDebugDrawer( mDebugDrawer );
	mWorld->getDebugDrawer()->setDebugMode( btIDebugDraw::DBG_DrawWireframe );
	
	// create the ground body
	
	btCollisionShape* groundShape = new btBoxShape( btVector3( btScalar(50.), btScalar(50.), btScalar(50.) ) );
	
	mCollisionShapes.push_back(groundShape);
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
		
		mWorld->addRigidBody( body );
	}
	
	{
		// create a dynamic rigidbody
		btCollisionShape* colShape = new btSphereShape( btScalar(1.) );
		mCollisionShapes.push_back( colShape );
		
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
		
		mWorld->addRigidBody( body );
	}
}

void BasicBulletApp::mouseDown( MouseEvent event )
{
}

void BasicBulletApp::update()
{
	mWorld->stepSimulation( 1.0f/60.0f , 10 );
	
	for ( int j=mWorld->getNumCollisionObjects()-1; j >= 0; j-- )
	{
		btCollisionObject* obj = mWorld->getCollisionObjectArray()[j];
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

void BasicBulletApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	mWorld->debugDrawWorld();
}

void BasicBulletApp::shutdown()
{
	// delete rigidbodies
	for( int i = mWorld->getNumCollisionObjects()-1; i >= 0; i-- )
	{
		btCollisionObject* obj = mWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		
		if( body && body->getMotionState() )
		{
			delete body->getMotionState();
		}
		mWorld->removeCollisionObject( obj );
		delete obj;
	}
	
	// delete collision shapes
	for( int j=0; j < mCollisionShapes.size(); j++ )
	{
		btCollisionShape* shape = mCollisionShapes[j];
		mCollisionShapes[j] = 0;
		delete shape;
	}
	
	delete mWorld;
	delete mSolver;
	delete mOverlappingPairCache;
	delete mDispatcher;
	delete mCollisionConfiguration;
	delete mDebugDrawer;
}


CINDER_APP_BASIC( BasicBulletApp, RendererGl )
