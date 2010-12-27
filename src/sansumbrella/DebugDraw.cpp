/*
 *  DebugDraw.cpp
 *  BasicBullet
 *
 *  Created by David Wicks on 12/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */

#include "sansumbrella/DebugDraw.h"
#include "cinder/app/App.h"

using namespace cinder;
using namespace sansumbrella;

DebugDrawer::DebugDrawer():
mDebugMode(0)
{
}

void DebugDrawer::drawLine( const btVector3 &from, const btVector3 &to, const btVector3 &color )
{
	gl::color( Color( color.getX(), color.getY(), color.getZ() ) );
	gl::drawLine( fromBullet(from), fromBullet(to) );
}

void DebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
{
	gl::color( Color( fromColor.getX(), fromColor.getY(), fromColor.getZ() ) );
	gl::drawVector( fromBullet(from), fromBullet(to) );
}

void DebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
{
	gl::color( Color( color.getX(), color.getY(), color.getZ() ) );
	gl::drawSphere( fromBullet(p), radius );
}

void DebugDrawer::drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
{
	btVector3 halfExtent = (boxMax - boxMin) * btScalar(0.5f);
	btVector3 center = (boxMax + boxMin) * btScalar(0.5f);
	
	gl::color( Color( color.getX(), color.getY(), color.getZ() ) );
	gl::drawCube( fromBullet(center), fromBullet(halfExtent) );	//this might be 50% the size we need, so we'll check...
}

void DebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
{
	gl::color( ColorA( color.getX(), color.getY(), color.getZ(), alpha ) );
	gl::drawLine( fromBullet(a), fromBullet(b) );
	gl::drawLine( fromBullet(b), fromBullet(c) );
	gl::drawLine( fromBullet(c), fromBullet(a) );
}



void DebugDrawer::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	gl::color( Color( color.getX(), color.getY(), color.getZ() ) );
	gl::drawSphere( fromBullet(PointOnB), distance, 4 );
}

void DebugDrawer::reportErrorWarning( const char *warningString )
{
	app::console() << warningString << std::endl;
}

void DebugDrawer::draw3dText( const btVector3 &location, const char *textString )
{
	app::console() << "not currently drawing 3d text: " << textString << std::endl;
}

void DebugDrawer::setDebugMode( int debugMode )
{
	mDebugMode = debugMode;
}
