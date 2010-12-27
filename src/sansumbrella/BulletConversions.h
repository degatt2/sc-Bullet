/*
 *  BulletConversions.h
 *  BasicBullet
 *
 *  Created by David Wicks on 12/27/10.
 *  Copyright 2010 David Wicks. All rights reserved.
 *
 */

#ifndef SU_BULLET_CONVERSIONS_H
#define SU_BULLET_CONVERSIONS_H

#include "LinearMath/btVector3.h"
#include "cinder/Vector.h"

namespace sansumbrella
{
	cinder::Vec3f	fromBullet( const btVector3& in );
	btVector3		toBullet( const cinder::Vec3f& in );
}

#endif