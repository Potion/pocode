#include "poMath.h"


bool almostEqual(float a, float b, float tolerance)
{
	return fabs(a - b) < tolerance;
}

float dcos( float degree )
{
	return cos( degree*3.14159f/180.0 );
}

float dsin( float degree )
{
	return sin( degree*3.14159f/180.0f );
}

float dtan( float degree)
{
	return tan( degree*3.14159f/180.0f );
}

float datan2( float y, float x )
{
	return atan2( y,x )*180.0/3.14159f;
}

float poRand()
{
	return (float)rand() / (float)RAND_MAX;
}

float poRand(float max)
{
	return poRand()*max;
}

float poRand(float min, float max)
{
	return min + (max-min)*poRand();
}

bool pointInTriangle( poPoint &P, poPoint &A, poPoint &B, poPoint &C )
{
    poPoint AB = A - B;
    poPoint BC = B - C;
    poPoint PA = P - A;

    if ( BC.x*AB.y - BC.y*AB.x > 0)    // test winding
    {
        if ( PA.x*AB.y - PA.y*AB.x > 0 )
            return false;
        poPoint PB = P - B;
        if ( PB.x*BC.y - PB.y*BC.x > 0 )
            return false;
        poPoint CA = C - A;
        poPoint PC = P - C;
        if ( PC.x*CA.y - PC.y*CA.x > 0 )
            return false;
    }
    else
    {
        if ( PA.x*AB.y - PA.y*AB.x < 0 )
            return false;
        poPoint PB = P - B;
        if ( PB.x*BC.y - PB.y*BC.x < 0 )
            return false;
        poPoint CA = C - A;
        poPoint PC = P - C;
        if ( PC.x*CA.y - PC.y*CA.x < 0 )
            return false;
    }
    
    return true;
}

void set_identity(float* mat, int n)
{
	memset(mat, 0, sizeof(float)*n*n);
	for(int i=0; i<n; i++)
		mat[i*n+i] = 1;
}

/**
 * use gaussian elimination to solve a system of linear equations
 * A is n,n matrix
 * b is n vector
 * on completion solution will be in b
 * DESTRUCTIVE of both A and b
 **/
bool gaussian_solve(float* A, float* b, int n)
{
	for(int pivot=0; pivot<n; pivot++)
	{
		int row, col;
		
		int maxrow = pivot;
		float maxele = fabs(A[pivot*n+pivot]);
		for(row=pivot+1; row<n; row++)
		{
			float ele = fabs(A[row*n+pivot]);
			if(ele > maxele)
			{
				maxrow = row;
				maxele = ele;
			}
		}

		if(maxele == 0.f)
		{
			printf("WARNING: gaussian_elimination: singular matrix\n");
			return false;
		}
		
		if(maxrow != pivot)
		{
			float tmp;
			for(col=0; col<n; col++)
			{
				tmp = A[maxrow*n+col];
				A[maxrow*n+col] = A[pivot*n+col];
				A[pivot*n+col] = tmp;
			}
			tmp = b[maxrow];
			b[maxrow] = b[pivot];
			b[pivot] = tmp;
		}
		
		float invPivot = 1.0f / A[pivot*n+pivot];
		for(col=0; col<n; col++)
			A[pivot*n+col] *= invPivot;
		b[pivot] *= invPivot;
		
		for(row=pivot+1; row<n; row++)
		{
			float factor = A[row*n+pivot];
			for(col=0; col<n; col++)
				A[row*n+col] -= factor*A[pivot*n+col];
			b[row] -= factor*b[pivot];
		}
	}

	int p = n-1;
	do
	{
		p--;
		for(int col=p+1; col<n; col++)
			b[p] -= A[p*n+col]*b[col];
	}
	while(p > 0);

	return true;
}

/**
 * use gaussian elimination to calculate the determinant of a matrix
 * A is n,n matrix
 * returns determinant
 * NON-DESTRUCTIVE
 **/
float guassian_determinant(float* A, int n)
{
	float* copy = new float[n*n];
	memcpy(copy, A, sizeof(float)*n*n);
	
	float det = 1.0f;
	
	for(int pivot=0; pivot<n; pivot++)
	{
		int row, col;
		
		int maxrow = pivot;
		float maxele = fabs(copy[pivot*n+pivot]);
		for(row=pivot+1; row<n; row++)
		{
			float ele = fabs(copy[row*n+pivot]);
			if(ele > maxele)
			{
				maxrow = row;
				maxele = ele;
			}
		}
		
		if(maxele == 0.f)
		{
			printf("WARNING: gaussian_elimination: singular matrix\n");
			delete [] copy;
			return 0.0f;
		}
		
		if(maxrow != pivot)
		{
			float tmp;
			for(col=0; col<n; col++)
			{
				tmp = copy[maxrow*n+col];
				copy[maxrow*n+col] = copy[pivot*n+col];
				copy[pivot*n+col] = tmp;
			}
			det = -det;
		}
		
		float invPivot = 1.0f / copy[pivot*n+pivot];
		for(col=0; col<n; col++)
			copy[pivot*n+col] *= invPivot;
		det *= invPivot;
		
		for(row=pivot+1; row<n; row++)
		{
			float factor = copy[row*n+pivot];
			for(col=0; col<n; col++)
				copy[row*n+col] -= factor*copy[pivot*n+col];
		}
	}
	
	delete [] copy;
	
	return 1.0f/det;
}

/**
 * use gaussian elimination to find inverse of square matrix
 * A is n,n matrix
 * on completion, inverse with be in A
 * DESTRUCTIVE
 */
bool gaussian_invert(float* A, int n)
{
	int* swap = new int[n];
	
	for(int pivot=0; pivot<n; pivot++)
	{
		int row, col;
		
		int maxrow = pivot;
		float maxele = fabs(A[pivot*n+pivot]);
		for(row=pivot+1; row<n; row++)
		{
			float ele = fabs(A[row*n+pivot]);
			if(ele > maxele)
			{
				maxrow = row;
				maxele = ele;
			}
		}

		if(maxele == 0.f)
		{
			printf("WARNING: gaussian_elimination: singular matrix\n");
			delete [] swap;
			return false;
		}
		
		swap[pivot] = maxrow;
		if(maxrow != pivot)
		{
			float tmp;
			for(col=0; col<n; col++)
			{
				tmp = A[maxrow*n+col];
				A[maxrow*n+col] = A[pivot*n+col];
				A[pivot*n+col] = tmp;
			}
		}
		
		float invPivot = 1.0f / A[pivot*n+pivot];
		for(col=0; col<n; col++)
			A[pivot*n+col] *= invPivot;

		A[pivot*n+pivot] = invPivot;
		
		for(row=0; row<n; row++)
		{
			if(row == pivot)
				continue;
			float factor = A[row*n+pivot];
			A[row*n+pivot] = 0.0f;
			for(col=0; col<n; col++)
				A[row*n+col] -= factor*A[pivot*n+col];
		}
	}

	int p = n-1;
	do
	{
		p--;
		if(swap[p] != p)
		{
			float tmp;
			for(int row=0; row<n; row++)
			{
				tmp = A[row*n+swap[p]];
				A[row*n+swap[p]] = A[row*n+p];
				A[row*n+p] = tmp;
			}
		}
	}
	while(p > 0);
	
	delete [] swap;
	return true;
}

void perspective_matrix(float fov, float aspect, float near, float far, float m[16])
{
    float d = 1.0f/dtan(fov);
    float recip = 1.0f/(near-far);

	set_identity(m, 4);
    m[0] = d/aspect;
    m[5] = d;
    m[10] = (near+far)*recip;
    m[11] = 2*near*far*recip;
    m[14] = -1.0f;
    m[15] = 0.0f;
}

void transpose_matrix(float* m, int n)
{
	for(int i=0; i<n; i++)
		for(int j=i; j<n; j++)
		{
			float tmp = m[i*4+j];
			m[i*4+j] = m[j*4+i];
			m[j*4+i] = tmp;
		}
}