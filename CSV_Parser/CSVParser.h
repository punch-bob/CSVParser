#pragma once
#include <string>
#include <istream>
#include <sstream>
#include "General.h"
#include "Exception.h"

template<typename... Types>
class CSVParser
{
public:
	class Iterator
	{
	public:
		typedef std::tuple<Types...>& reference;
		typedef std::tuple<Types...>* pointer;

		Iterator(CSVParser* parser) : parser(parser)
		{
			if (parser != nullptr)
			{
				ParsingStr();
				position = parser->stream->tellg();
				count_lines = parser->skip_str;
			}
		};

		~Iterator() = default;

		Iterator& operator++()
		{
			if (parser == nullptr)
			{
				return *this;
			}
			if (!parser->valid())
			{
				parser = nullptr;
				return *this;
			}
			ParsingStr();
			position = parser->stream->tellg();
			count_lines++;
			return *this;
		}

		bool operator==(Iterator& b)
		{
			return parser == b.parser;
		}

		bool operator!=(Iterator& b)
		{
			return !(*this == b);
		}

		reference operator*()
		{
			return current_tuple;
		}

		pointer operator->()
		{
			return &current_tuple;
		}

		void ParsingStr()
		{
			parser->stream->seekg(position);
			size_t readed_elements = ReadTuple(*parser->stream, current_tuple, parser->str_delim, parser->type_delim);
			if (readed_elements  < sizeof...(Types))
			{
				
				throw Exceptions("Bad file format in " + std::to_string(count_lines + 1) + " row, " + std::to_string(readed_elements + 1) + " column");
			}
		}
	private:
		CSVParser<Types...>* parser;
		std::tuple<Types...> current_tuple;
		size_t position = 0;
		size_t count_lines = 0;
	};

	CSVParser(std::istream& in, const size_t& skip_str)
	{
		std::string tmp;
		for (size_t i = 0; i < skip_str; ++i)
		{
			getline(in, tmp, str_delim);
		}
		CSVParser::stream = &in;
		CSVParser::skip_str = skip_str;
		CSVParser::skip_simb = in.tellg();
	}

	Iterator begin()
	{
		return Iterator(this);
	}

	Iterator end()
	{
		return Iterator(nullptr);
	}

	bool valid()
	{
		return stream->peek() != EOF;
	}

	template<typename... Types>
	friend bool ReadTuple(std::istream& in, std::tuple<Types...>& t, char str_delim, char type_delim);
private:
	size_t skip_simb = 0;
	size_t skip_str = 0;
	char str_delim = '\n';
	char type_delim = ',';
	std::istream* stream;
};