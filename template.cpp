#include <iostream>
#include <chrono>

template <int N>
struct Int
{
	static const int result = N;
};

struct Null
{
	static const int result = 0;
};

template <typename H, typename T = Null>
struct List
{
	typedef H Head;
	typedef T Tail;
};

template <typename List>
struct Length
{
	static const int result = 1 + Length<typename List::Tail>::result;
};

template <>
struct Length <Null>
{
	static const int result = 0;
};

template <typename List, int Idx>
struct ElemAt
{
	typedef typename ElemAt<typename List::Tail, Idx - 1>::result result;
};

template <typename List>
struct ElemAt <List, 0>
{
	typedef typename List::Head result;
};

template <int N>
struct ElemAt <Null, N>
{
	typedef Null result;
};

template <>
struct ElemAt<Null, 0>
{
	typedef Null result;
};

template <typename X, typename Y>
struct Equal
{
	static const bool result = false;
};

template <typename X>
struct Equal <X, X>
{
	static const bool result = true;
};

template <typename list, typename Elem>
struct Prepend
{
	typedef List<Elem, list> result;
};

template <typename list, typename Elem>
struct Append
{
	typedef List<typename list::Head, typename Append<typename list::Tail, Elem>::result> result;
};

template <typename Elem>
struct Append <Null, Elem>
{
	typedef List<Elem> result;
};

template <typename List, typename Element>
struct Position
{
	static const int result = Equal<Element, typename List::Head>::result ? 0 : 1 + Position<typename List::Tail, Element>::result;
};

template <typename Element>
struct Position <Null, Element>
{
	static const int result = 0;
};

template <typename list>
struct Swap
{
	typedef List<typename list::Tail::Head, List<typename list::Head, typename list::Tail::Tail> > result;
};

template <typename List, int num = INT_MAX>
struct Lowest
{
	static const int result = (num < List::Head::result) ? Lowest<typename List::Tail, num>::result : Lowest<typename List::Tail, List::Head::result>::result;
};

template <int num>
struct Lowest <Null, num>
{
	static const int result = num;
};

template <typename list, int pos>
struct SetFront
{
	typedef typename Swap<List<typename list::Head, typename SetFront<typename list::Tail, pos - 1>::result> >::result result;
};

template <typename List>
struct SetFront <List, 1>
{
	typedef typename Swap<List>::result result;
};

template <typename List>
struct SetFront <List, 0>
{
	typedef List result;
};

template <typename list>
struct Sort
{
	typedef typename SetFront<list, Position<list, Int<Lowest<list>::result> >::result >::result newList;
	typedef List<typename newList::Head, typename Sort<typename newList::Tail>::result> result;
};

template <>
struct Sort <Null>
{
	typedef Null result;
};

int main()
{
	// format the list with:
	// perl -pe '1 while s/^(.*?)(?<!<)(?<![0-9])([0-9]+)(.*?)$/$1List<Int<$2>, $3> /g;s/(List<Int<[0-9]+>),( >)/$1$2/g'
	// The numbers must be on the same line and seperated by whitespaces
	auto begin = std::chrono::high_resolution_clock::now();
	typedef List<Int<35>,  List<Int<98>,  List<Int<33>,  List<Int<27>,  List<Int<46>,  List<Int<77>,  List<Int<54>,  List<Int<92>,  List<Int<35>,  List<Int<32> > > > > > > > > > > originalList;
	typedef Sort<originalList>::result newList;
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Original: ";
	std::cout << ElemAt<originalList, 0>::result::result << ' ' << ElemAt<originalList, 1>::result::result << ' '
			  << ElemAt<originalList, 2>::result::result << ' ' << ElemAt<originalList, 3>::result::result << ' '
			  << ElemAt<originalList, 4>::result::result << ' ' << ElemAt<originalList, 5>::result::result << ' '
			  << ElemAt<originalList, 6>::result::result << ' ' << ElemAt<originalList, 7>::result::result << ' '
			  << ElemAt<originalList, 8>::result::result << ' ' << ElemAt<originalList, 9>::result::result << std::endl;
	std::cout << "Template sorting: ";
	std::cout << ElemAt<newList, 0>::result::result << ' ' << ElemAt<newList, 1>::result::result << ' '
			  << ElemAt<newList, 2>::result::result << ' ' << ElemAt<newList, 3>::result::result << ' '
			  << ElemAt<newList, 4>::result::result << ' ' << ElemAt<newList, 5>::result::result << ' '
			  << ElemAt<newList, 6>::result::result << ' ' << ElemAt<newList, 7>::result::result << ' '
			  << ElemAt<newList, 8>::result::result << ' ' << ElemAt<newList, 9>::result::result << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::duration<float, std::chrono::milliseconds::period> >(end - begin).count() << "ms" << std::endl;
	begin = std::chrono::high_resolution_clock::now();
	int nums[10] = {35, 98, 33, 27, 46, 77, 54, 92, 35, 32};
	std::sort(std::begin(nums), std::end(nums));
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Normal sorting:   ";
	for (auto& i : nums)
		std::cout << i << " ";
	std::cout << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::duration<float, std::chrono::milliseconds::period> >(end - begin).count() << "ms" << std::endl;
}
