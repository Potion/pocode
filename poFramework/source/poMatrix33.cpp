#include "poMatrix33.h"

poMatrix33::poMatrix33()
{
	set_identity(mat, 3);
}

poMatrix33::poMatrix33(float values[9])
{
	memcpy(mat, values, sizeof(float)*9);
}

poMatrix33::~poMatrix33()
{}

void poMatrix33::multiply(const poMatrix33& rhs)
{
	multiply(rhs.mat);
}

void poMatrix33::multiply(const float rhs[9])
{
	float result[9];
	
	result[0] = mat[0]*rhs[0] + mat[1]*rhs[3] + mat[2]*rhs[6];
	result[1] = mat[0]*rhs[1] + mat[1]*rhs[4] + mat[2]*rhs[7];
	result[2] = mat[0]*rhs[2] + mat[1]*rhs[5] + mat[2]*rhs[8];

	result[3] = mat[3]*rhs[0] + mat[4]*rhs[3] + mat[5]*rhs[6];
	result[4] = mat[3]*rhs[1] + mat[4]*rhs[4] + mat[5]*rhs[7];
	result[5] = mat[3]*rhs[2] + mat[4]*rhs[5] + mat[5]*rhs[8];

	result[6] = mat[6]*rhs[0] + mat[7]*rhs[3] + mat[8]*rhs[6];
	result[7] = mat[6]*rhs[1] + mat[7]*rhs[4] + mat[8]*rhs[7];
	result[8] = mat[6]*rhs[2] + mat[7]*rhs[5] + mat[8]*rhs[8];
	
	memcpy(mat, result, sizeof(float)*9);
//	printf("-=: mat :=-\n%f %f %f\n%f %f %f\n%f %f %f\n\n", mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8]);
}

poPoint poMatrix33::transform(const poPoint& p)
{
	poPoint result = p;
	result.x = mat[0]*p.x + mat[1]*p.y + mat[2];
	result.y = mat[3]*p.x + mat[4]*p.y + mat[5];
	result.z = mat[6]*p.x + mat[7]*p.y + mat[8];
	return result;
}

void poMatrix33::rotate(float angle)
{
	float cosTheta = dcos(angle);
	float sinTheta = dsin(angle);

	float rot[9];
	set_identity(rot, 3);
	rot[0] = cosTheta;
	rot[1] = -sinTheta;
	rot[3] = sinTheta;
	rot[4] = cosTheta;
	rot[8] = 1;
	
	multiply(rot);
}

void poMatrix33::scale(float sx, float sy)
{
	float scl[9];
	set_identity(scl, 3);
	scl[0] = sx;
	scl[4] = sy;
	scl[8] = 1;
	
	multiply(scl);
}

void poMatrix33::shear(float kx, float ky)
{
	float shr[9];
	set_identity(shr, 3);
	shr[0] = dtan(kx);
	shr[4] = dtan(ky);
	shr[8] = 1;
	
	multiply(shr);
}

void poMatrix33::translate(float tx, float ty)
{
	float trn[9];
	set_identity(trn, 3);
	trn[0] = 1;
	trn[2] = tx;
	trn[4] = 1;
	trn[5] = ty;
	trn[8] = 1;
	
	multiply(trn);
}

