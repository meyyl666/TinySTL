#pragma once




namespace mystl{

	// 定义二元函数的参数型别的返回值型别
	template <class Arg1, class Arg2, class Result>
	struct binary_function
	{
		typedef Arg1      first_argument_type;
		typedef Arg2      second_argument_type;
		typedef Result    result_type;
	};



	template <class T>
	struct less :binary_function<T, T, bool>
	{
		bool operator()(const T& x, , const T& y)const { return x < y; }
	};
}