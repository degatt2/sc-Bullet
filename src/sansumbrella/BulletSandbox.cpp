/*
 *  BulletSandbox.cpp
 *  WaterCycle
 *
 *  Created by David Wicks on 1/6/11.
 *  Copyright 2011 David Wicks. All rights reserved.
 *
 */

#include "BulletSandbox.h"
#include "cinder/app/App.h"

using namespace sansumbrella;
using namespace cinder;
using namespace std;

BulletSandbox::BulletSandbox()
{
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher( mCollisionConfiguration );
	
	mOverlappingPairCache = new btDbvtBroadphase();
	
	mSolver = new btSequentialImpulseConstraintSolver;
	
	mWorld = new btDiscreteDynamicsWorld( mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration );
	mWorld->setGravity( btVector3( 0, -10, 0 ) );
	
	mDebugDrawer = new DebugDrawer();
	mWorld->setDebugDrawer( mDebugDrawer );
	
}

BulletSandbox::~BulletSandbox()
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



void BulletSandbox::setDebugMode( int debugMode )
{
	mWorld->getDebugDrawer()->setDebugMode( debugMode );
}

void BulletSandbox::debugDraw()
{
	mWorld->debugDrawWorld();
}

void BulletSandbox::debugPrint()
{
	for ( int j=mWorld->getNumCollisionObjects()-1; j >= 0; j-- )
	{
		btCollisionObject* obj = mWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if( body && body->getMotionState() )
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);
			app::console() << "Position: " << fromBullet( trans.getOrigin() ) << endl;
		}
	}	
}

void BulletSandbox::stepSimulation( btScalar timeStep,int maxSubSteps, btScalar fixedTimeStep )
{
	mWorld->stepSimulation( timeStep, maxSubSteps, fixedTimeStep );
}











