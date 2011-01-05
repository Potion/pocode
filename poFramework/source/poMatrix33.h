#include "poCommon.h"

#ifndef __poMatrix33_h__
#define __poMatrix33_h__

class poMatrix33
{
public:
	poMatrix33();
	poMatrix33(float values[9]);
	~poMatrix33();
	
	float& operator()(int row, int col) { return mat[row*4+col]; }
	
	void multiply(const poMatrix33& rhs);
	void multiply(const float rhs[9]);
	poPoint transform(const poPoint& p);

	void rotate(float angle);
	void scale(float sx, float sy);
	void shear(float kx, float ky);
	void translate(float tx, float ty);


	float mat[9];
};

typedef std::list<poMatrix33> poMatrix33List;

#endif