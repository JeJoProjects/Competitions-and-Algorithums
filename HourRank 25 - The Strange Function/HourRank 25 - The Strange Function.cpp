#include <std::vector>
#include <algorithm>
#include <iostream>
#include <iterator>

//#define lli long long int

//******************************************************************************
long long int GCD(long long int a, long long int b)
{
   if( (a!=0) && (b!=0) )
   {
      a < 0 ? a*=-1 : a;
      b < 0 ? b*=-1 : b;
      //std::cout<<"a"<<a<<"b"<<b<<std::endl;
      while(a)
      {
         long long int t = b % a;
         b = a;
         a = t;
      }
      //std::cout<<"a"<<a<<"b"<<b<<std::endl;
      return b;
   }
   else
      return ( b < 0 ?  b*=-1: b);
}
//******************************************************************************
long long int FindGCD(std::vector<long long int> vec, unsigned int n)
{
    if(n==1) return GCD(0, vec[0] );
    else
    {
       if(n==2)
         return GCD(vec[0], vec[1]);
       else
       {
          long long int g=vec[0];
          for (auto i = 1; i < n; i++)
            g = GCD(g, vec[i]);
          return g;
       }
    }
}
//*************************************************************
class SegmentTree
{
private:
   long long int n;
	std::vector<long long int> data;
public:
	SegmentTree(long long int count)         // array initialization constructor.
	{
		this->n    = count;                 // the size of the array.
		this->data = std::vector<long long int>(2 * n, 0); // initialize the std::vector with 2n.
		//data.assign(2*n, 0);
		//data.resize(2 * n);
	}
                                          // std::vector initialization constructor.
	SegmentTree(std::vector<long long int> const &values)
	{
		this->n = values.size();
		this->data = std::vector<long long int>(2 * n);
		copy(values.begin(), values.end(), &data[0] + n);
		for (auto idx = n - 1; idx > 0; idx--)
			data[idx] = min(data[idx * 2], data[idx * 2 + 1]);
	}

	void updateST(unsigned int idx, long long int value)      // to input/ update the values.
	{
		idx += n;                           // index=index+size of the array.
		data[idx] = value;                  // assign the value to respective index.

		while (idx > 1)            // Updating all other minimum values in the tree.
      {
			idx >>= 1;
			long long int new_min = max(data[idx<<1], data[idx<<1|1]);
         if (new_min == data[idx]) break;
         data[idx] = new_min;

		}
	}

	long long int Range_Max(unsigned int left, unsigned int right)          // interval [left, right).
	{
		//int result = numeric_limits<int>::max();   // std::set minimum to infinity.
		left += n;                                   // both left & right index+1.
		right += n;

		long long int result = data[left];
		while (left < right)
      {
			if (left & 1)  result = max(result, data[left++]);
			if (right & 1) result = max(result, data[--right]);
			left >>= 1;
			right >>= 1;
		}
		return result;
	}
};
//******************************************************************************
class SegmentTreeSUM
{
private:
   long long int n;
	std::vector<long long int> data;
public:
	SegmentTreeSUM(long long int count)    // array initialization constructor.
	{
		this->n    = count;                 // the size of the array.
		this->data = std::vector<long long int>(2 * n, 0); // initialize the std::vector with 2n.
	}
                                          // std::vector initialization constructor.
	SegmentTreeSUM(std::vector<long long int> const &values)
	{
		this->n = values.size();
		this->data = std::vector<long long int>(2 * n);
		copy(values.begin(), values.end(), &data[0] + n);
		for (auto idx = n - 1; idx > 0; idx--)
			data[idx] = min(data[idx * 2], data[idx * 2 + 1]);
	}
	void updateST(unsigned int idx, long long int value)
	{
		idx += n;
		data[idx] = value;

		while (idx > 1)
      {
			idx /= 2;
			data[idx] = data[2 * idx] + data[2 * idx + 1];
		}
	}

	long long int Range_Sum(unsigned int left, unsigned int right)
	{ // interval [left, right)
		long long int ret = 0;
		left += n;
		right += n;

		while (left < right) {
			if (left & 1) ret += data[left++];
			if (right & 1) ret += data[--right];
			left >>= 1;
			right >>= 1;
		}
		return ret;
	}
};

//***************************************************************************
int main()
{
    unsigned int n;
    std::cin >> n;
    if(1<=n && n<=500000)
    {
       std::vector<long long int> a(n);
       SegmentTree st_max(n);
       SegmentTreeSUM st_sum(n);
       unsigned int i=0, _size=a.size();
       for(std::vector<long long int>::iterator itr=a.begin(); itr!=a.end(); ++itr)
       {
          std::cin>>*itr;
          st_max.updateST(i,*itr);
          st_sum.updateST(i,*itr);
          ++i;
       }

       long long int Maximum=0;
       for(unsigned int l=0; l<_size; ++l)
       {
          for(unsigned int r=l; r<_size; ++r)
          {
             std::vector<long long int> vec;
             vec.assign(a.begin()+l, a.end()-_size+r+1);
             //copy(vec.begin(), vec.end(), ostream_iterator<long long int>(std::cout," "));
             //std::cout<<std::endl;
             long long int gcd=FindGCD(vec, vec.size()), temp=0;
             /*std::cout <<l+1<<" "<<r+1
                  <<" Range GCD :"<<gcd
                  <<" Range Sum :"<<st_sum.Range_Sum(l, r+1)
                  <<" Range Max :"<<st_max.Range_Max(l, r+1) << std::endl<<std::endl;*/
             temp=gcd*(st_sum.Range_Sum(l, r+1) - st_max.Range_Max(l, r+1));
             Maximum=max(temp, Maximum);
          }
       }
       std::cout<<Maximum<<std::endl;
    }
    return 0;
}
