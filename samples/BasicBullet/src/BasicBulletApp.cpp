#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
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
	
private:
	MayaCamUI mCam;
//	PolyLine<Vec3f> mPath;
	vector< Vec3f > mPath;
};

void BasicBulletApp::setup()
{
	CameraPersp cam;
	cam.setPerspective( 60.0f, getWindowAspectRatio(), 2.0f, 2000.0f );
	cam.lookAt( Vec3f( 5.0f, 25.0f, -60.0f ), Vec3f( 0, -28, 0 ), Vec3f::yAxis() );
	
	mCam.setCurrentCam(cam);
	registerMouseDown( &mCam, &MayaCamUI::mouseDown );
	registerMouseDrag( &mCam, &MayaCamUI::mouseDrag );
	
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
	
	for( int i = 0; i < 100; i++ )
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
		
		startTransform.setOrigin( btVector3( 2, 10+i, 0 ) );
		
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
	mPath.clear();
	
	for ( int j=mWorld->getNumCollisionObjects()-1; j >= 0; j-- )
	{
		btCollisionObject* obj = mWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if( body && body->getMotionState() )
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);
			mPath.push_back( fromBullet( trans.getOrigin() ) );
		}
	}
}

void BasicBulletApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::enableAlphaBlending();
	
	gl::setMatrices( mCam.getCamera() );
	
	gl::color( ColorA( 1.0, 1.0, 1.0, 0.5 ) );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, &( mPath[0] ) );
	glDrawArrays( GL_LINE_STRIP, 0, mPath.size() );
	glDisableClientState( GL_VERTEX_ARRAY );
	
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
