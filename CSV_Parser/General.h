#pragma once
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

template<typename Type, size_t N, size_t Last>
struct tuple_printer
{
	static void print(std::ostream& out, const Type& value)
	{
		out << std::get<N>(value) << ", ";
		tuple_printer<Type, N + 1, Last>::print(out, value);
	}
};

template<typename Type, size_t N>
struct tuple_printer<Type, N, N>
{
	static void print(std::ostream& out, const Type& value)
	{
		out << std::get<N>(value);
	}
};


template<typename... Types>
std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& value)
{
	out << "(";
	tuple_printer<std::tuple<Types...>, 0, sizeof...(Types) - 1>::print(out, value);
	out << ")";
	return out;
}

template<class T>
bool convert(const std::string& str, T& t)
{
	std::istringstream stream(str);
	stream >> t;
	return true;
}

template<size_t Iter, typename... Types>
struct tuple_reader
{
	static void read(std::vector<std::string>& vector_str, std::tuple<Types...>& t)
	{
		tuple_reader<Iter - 1, Types...>::read(vector_str, t);
		convert(vector_str[0], std::get<Iter>(t));
		vector_str.erase(vector_str.begin());
	}
};

template<typename... Types>
struct tuple_reader<0, Types...>
{
	static void read(std::vector<std::string>& vector_str, std::tuple<Types...>& t)
	{
		convert(vector_str[0], std::get<0>(t));			
		vector_str.erase(vector_str.begin());
	}
};

template<typename... Types>
size_t ReadTuple(std::istream& in, std::tuple<Types...>& t, const char str_delim, const char type_delim)
{
	std::string str, tmp;
	getline(in, str, str_delim);
	std::istringstream stream(str);
	std::vector<std::string> vector_str;
	size_t push_counter = 0;
	while (stream.peek() != EOF)
	{
		std::getline(stream, tmp, type_delim);
		vector_str.push_back(tmp);
		push_counter++;
	}
	if (push_counter < sizeof...(Types))
	{
		return push_counter;
	}
	tuple_reader<sizeof...(Types) - 1, Types...>::read(vector_str, t);
	return push_counter;
}