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
		{ 0.22537048,  0.091394087, 0.091394087, 0.011544329, 0.011544329 },  // P=2
		{ 0.473412, 0.345393,  0.345393,  0.185013,  0.185013  },  // P=4
		{ 0.661355, 0.572516,  0.572516,  0.435258,  0.435258  },  // P=8
		{ 0.791709, 0.737833,  0.737833,  0.646822,  0.646822  },  // P=16
		{ 0.87622,  0.845898,  0.845898,  0.792585,  0.792585  }   // P=32
};

class ApArc
{
protected:
	std::complex<double> c_; // 中心
	double r_;    // 半径
	double stA_;  // 始点の偏角
	double edA_;  // 終点の偏角

public:
	// Default Constructor
	ApArc()
	{
	}

	// Constructor
	ApArc( const int p, const int base, const int oppo,
			const double stA = 0.0, const double edA = 2.0*M_PI )
	{
		assert( base != oppo );

		c_ = (Zrs[base] - Alp[p][base]*Alp[p][base]*Zrs[oppo]) / (1 - Alp[p][base]*Alp[p][base]);
		r_ = Alp[p][base]*abs(Zrs[base] - Zrs[oppo]) / (1 - Alp[p][base]*Alp[p][base]);
		stA_ = stA;
		edA_ = edA;
	}

	// Getters
	std::complex<double> center() const { return c_; }
	double radius() const { return r_; }
	double startArg() const { return stA_; }
	double endArg() const { return edA_; }

	// 偏角 stA_ の位置
	std::complex<double> startPt() const
	{
		std::complex<double> tmp( cos( stA_ ), sin( stA_ ) );
		return c_ + tmp;
	}

	// 偏角 edA_ の位置
	std::complex<double> endPt() const
	{
		std::complex<double> tmp( cos( edA_ ), sin( edA_ ) );
		return c_ + tmp;
	}

	// Operator
	ApArc &operator=( const ApArc& ac )
	{
		c_ = ac.c_;
		r_ = ac.r_;
		stA_ = ac.stA_;
		edA_ = ac.edA_;

		return *this;
	}
};

void P2arcs( const int p, std::vector< ApArc>& ApArcs )
{
	ApArc temp;
	temp = ApArc(p,0,1, 1.511, -1.511);
	ApArcs.push_back(temp);

	temp = ApArc(p,0,2, 4.653, 1.631);
	ApArcs.push_back(temp);

	temp = ApArc(p,1,0);
	ApArcs.push_back(temp);

	temp = ApArc(p,2,0);
	ApArcs.push_back(temp);

	temp = ApArc(p,3,0);
	ApArcs.push_back(temp);

	temp = ApArc(p,4,0);
	ApArcs.push_back(temp);
}

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

	std::vector< ApArc > ApArcs;

	P2arcs( p, ApArcs );

	for (auto itr = ApArcs.begin(); itr < ApArcs.end(); ++itr)
	{
		std::cout << (*itr).center() << std::endl;
		std::cout << (*itr).startArg() << ", " << (*itr).endArg() << std::endl;
		std::cout << (*itr).startPt() << ", " << (*itr).endPt() << std::endl << std::endl;
	}

	return EXIT_SUCCESS;
}
