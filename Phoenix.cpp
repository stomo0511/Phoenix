/*
 * Phoenix.cpp
 *
 *  Created on: 2018/01/17
 *      Author: stomo
 */

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

#define EPS 0.000001  // 停止判定
#define MAXIT 30      // 最大反復回数
#define N 5           // 多項式の次数

// Zrs: ゼロ点ベクトル
std::vector<std::complex<double>> Zrs {
	std::complex<double> (  0.0,  1.0 ),
	std::complex<double> (  1.0,  2.0 ),
	std::complex<double> ( -1.0,  2.0 ),
	std::complex<double> (  3.0, -3.0 ),
	std::complex<double> ( -3.0, -3.0 )
};

// Apollonius比 Alp[P][Zero]
double Alp[5][5] = {
		{ 0.22537, 0.091394, 0.091394, 0.011544, 0.011544 },  // P=2
		{ 0.47341, 0.345393, 0.345393, 0.185013, 0.185013 },  // P=4
		{ 0.66136, 0.572516, 0.572516, 0.435258, 0.435258 },  // P=8
		{ 0.79171, 0.737833, 0.737833, 0.646822, 0.646822 },  // P=16
		{ 0.87622, 0.845898, 0.845898, 0.792585, 0.792585 }   // P=32
};

// Apollonius弧の startArg & endArg
// P, base(i), oppo(j), start, end

// Apollonius 円クラス
class ApCircle
{
protected:
	std::complex<double> c_; // center
	double r_;  // radius

public:
	ApCircle( const double alp, const std::complex<double> base, const std::complex<double> oppo )
	{
		assert( alp > 0.0 );

		c_ = (base - alp*alp*oppo) / (1 - alp*alp);
		r_ = alp*abs(base - oppo) / (1 - alp*alp);
	}

	// Getters
	std::complex<double> center() const { return c_; }
	double radius() const { return r_; }

	// Operator
	ApCircle &operator=( const ApCircle& ac )
	{
		c_ = ac.c_;
		r_ = ac.r_;

		return *this;
	}
};

class ApArc
{
protected:
	std::complex<double> c_; // 中心
	double r_;    // 半径
	double stA_;  // 始点の偏角
	double edA_;  // 終点の偏角

public:
	// Constructor
	ApArc( const int p, const int base, const int oppo,
			const double stA, const double edA )
	{
		assert( base != oppo );

		c_ = (Zrs[base] - Alp[p][base]*Alp[p][base]*Zrs[oppo]) / (1 - Alp[p][base]*Alp[p][base]);
		r_ = Alp[p][base]*abs(Zrs[base] - Zrs[oppo]) / (1 - Alp[p][base]*Alp[p][base]);
		stA_ = 0.0;
		edA_ = 2.0*M_PI;
	}

	// Getters
	std::complex<double> center() const { return c_; }
	double radius() const { return r_; }
	double startArg() const { return stA_; }
	double endArg() const { return edA_; }

	std::complex<double> startPt() const
	{
		std::complex<double> tmp( cos( stA_ ), sin( stA_ ) );
		return c_ + tmp;
	}

	std::complex<double> endPt() const
	{
		std::complex<double> tmp( cos( edA_ ), sin( edA_ ) );
		return c_ + tmp;
	}
};

int main(int argc, char *argv[])
{
	if (argc <2)
	{
		std::cerr << "Usage: a.out [order of Nourein's method]\n";
		exit (EXIT_FAILURE);
	}
	const int P = atoi(argv[1]);    // Order of Nourein method
	assert( (P==2) || (P==4) || (P==8) || (P==16) || (P==32) );
	int p;
	if (P==2) p=0;
	else if (P==4)  p=1;
	else if (P==8)  p=2;
	else if (P==16) p=3;
	else if (P==32) p=4;

	for (int i=0; i<N; i++)
	{
		std::cout << i << ":\n";

		std::complex<double> base = Zrs[i];
		double alp = Alp[p][i];
		for (int j=0; j<N; j++)
		{
			if (i != j)
			{
				ApArc tmp( p, i, j, 0.0, 2.0*M_PI );
				std::cout << j << ": " << tmp.center() << ", " << tmp.radius() << std::endl;
			}
		}
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}
