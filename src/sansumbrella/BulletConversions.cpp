/*
 *  BulletConversions.cpp
 *  BasicBullet
 *
 *  Created by David Wicks on 12/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */


#include "sansumbrella/BulletConversions.h"

namespace sansumbrella
{
	cinder::Vec3f fromBullet( const btVector3& in )
	{
		return cinder::Vec3f( in.getX(), in.getY(), in.getZ() );
	}
	
	btVector3 toBullet( const cinder::Vec3f& in )
	{
		return btVector3( in.x, in.y, in.z );
	}
}