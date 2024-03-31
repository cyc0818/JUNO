#include <vector>

namespace NR
{
template<class T>
void sift_down(std::vector <T> &ra, const int l, const int r)
{
	int j, jold;
	T a;
	a = ra[l];
	jold = l;
	j = 2 * l + 1;
	while (j <= r)
	{
		if (j < r && ra[j] < ra[j + 1]) j++;
		if (a >= ra[j]) break;
		ra[jold] = ra[j];
		jold = j;
		j = 2 * j + 1;
	}
	ra[jold] = a;
}

template<class T>
void SWAP(T &a, T &b)
{
	T tmp;
	tmp = a;
	a = b;
	b = tmp;
}

template<class T>
void hpsort(std::vector<T> &ra)
{
	int i, n = ra.size();
	for(i = n / 2 - 1; i >= 0; i--)
	{
		sift_down(ra, i, n - 1);
	}
	for (i = n - 1; i > 0; i--) 
	{
		SWAP(ra[0], ra[i]);
		sift_down(ra,0,i-1);
	}
}
}
