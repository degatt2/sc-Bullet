/*
 *  BulletSandbox.h
 *  WaterCycle
 *
 *  Created by David Wicks on 1/6/11.
 *  Copyright 2011 David Wicks. All rights reserved.
 *
 */

#pragma once

#include "btBulletDynamicsCommon.h"
#include "sansumbrella/DebugDraw.h"
#include "sansumbrella/BulletConversions.h"

namespace sansumbrella
{
	class BulletSandbox
	{
	public:
		BulletSandbox();
		~BulletSandbox();
		
		void setDebugMode( int debugMode=btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE );
		void debugDraw();
		void debugPrint();
		
		void manageCollisionShape( btCollisionShape* shape ){ mCollisionShapes.push_back( shape ); }
		void addRigidBody( btRigidBody* body ){ mWorld->addRigidBody(body); }
		
		void stepSimulation( btScalar timeStep,int maxSubSteps=1, btScalar fixedTimeStep=btScalar(1.)/btScalar(60.) );
		
		btDiscreteDynamicsWorld* getWorld(){ return mWorld; }
		btAlignedObjectArray<btCollisionShape*>& getCollisionShapes(){ return mCollisionShapes; }
		
	private:
		btDiscreteDynamicsWorld* mWorld;
		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btBroadphaseInterface* mOverlappingPairCache;
		btSequentialImpulseConstraintSolver* mSolver;
		DebugDrawer* mDebugDrawer;
		btAlignedObjectArray<btCollisionShape*> mCollisionShapes;
	};
}