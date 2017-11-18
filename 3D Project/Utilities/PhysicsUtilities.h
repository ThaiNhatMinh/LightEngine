#pragma once
#include <pch.h>

inline btVector3 ToBtVector3(const vec3& vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}

inline btQuaternion ToBtQuaternion(const quat& quaternion)
{
	return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

inline vec3 ToVector3(const btVector3& vector)
{
	return vec3(vector.x(), vector.y(), vector.z());
}

inline quat ToQuaternion(const btQuaternion& quaternion)
{
	return quat(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}
static btTransform Mat4x4_to_btTransform(mat4 const & mat)
{
	// convert from Mat4x4 (GameCode) to btTransform (Bullet)
	btMatrix3x3 bulletRotation;
	btVector3 bulletPosition;

	// copy rotation matrix
	for (int row = 0; row<3; ++row)
		for (int column = 0; column<3; ++column)
			bulletRotation[row][column] = mat[row][column]; // note the reversed indexing (row/column vs. column/row)
															//  this is because Mat4x4s are row-major matrices and
															//  btMatrix3x3 are column-major.  This reversed indexing
															//  implicitly transposes (flips along the diagonal) 
															//  the matrix when it is copied.

															// copy position
	for (int column = 0; column<3; ++column)
		bulletPosition[column] = mat[3][column];

	return btTransform(bulletRotation, bulletPosition);
}

static mat4 btTransform_to_Mat4x4(btTransform const & trans)
{
	mat4 returnValue;

	// convert from btTransform (Bullet) to Mat4x4 (GameCode)
	btMatrix3x3 const & bulletRotation = trans.getBasis();
	btVector3 const & bulletPosition = trans.getOrigin();

	// copy rotation matrix
	for (int row = 0; row<3; ++row)
		for (int column = 0; column<3; ++column)
			returnValue[row][column] = bulletRotation[row][column];
	// note the reversed indexing (row/column vs. column/row)
	//  this is because Mat4x4s are row-major matrices and
	//  btMatrix3x3 are column-major.  This reversed indexing
	//  implicitly transposes (flips along the diagonal) 
	//  the matrix when it is copied.

	// copy position
	for (int column = 0; column<3; ++column)
		returnValue[3][column] = bulletPosition[column];

	return returnValue;
}

