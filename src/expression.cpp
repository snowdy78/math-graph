#include "math_graph/expression.hpp"
#include "math_graph/call_function_action.hpp"
#include "math_graph/binary_operator_action.hpp"
#include "math_graph/unary_operator_action.hpp"
#include "math_graph/unpack_variable_action.hpp"
#include <boost/regex.hpp>
#include <iostream>

namespace mg
{
	expression::value_type expression::parse(const string_type &str, const function_dependencies &deps)
	{
		// find expressions (func, bracket expression or vars)
		constexpr static const char *expression_search_pattern
			= R"((?:([a-zA-Z](?:_[0-9]+)?)(\(((?:[^()]*|(?2))*)\)))|(\(((?:[^()]*|(?4))*)\))|([a-zA-Z][._0-9]*)+|([0-9.]+))";
		static const string_type operator_search_pattern = get_pattern(
			R"((^${unary})|(${binary}))",
			{
				{ "${unary}",  unary_operation::open_pattern()  },
				{ "${binary}", binary_operation::open_pattern() }
		  }
		);
		constexpr static const char *function_split_arguments_pattern = R"([^,]+)";
		constexpr static const char *binary_operator_operands_pattern = R"(\{(\d+)\}\s*[\${__op}]\s*\{(\d+)\})";
		constexpr static const char *unary_operator_operand_pattern	  = R"([\${__op}]\s*\{(\d+)\})";
		boost::regex split_args_rgx(function_split_arguments_pattern);
		boost::regex search_exprs(expression_search_pattern);
		boost::regex search_operators(operator_search_pattern);
		std::unordered_map<string_type, expression> objects;
		auto begin			   = boost::sregex_iterator(str.begin(), str.end(), search_exprs);
		auto end			   = boost::sregex_iterator();
		string_type simplified = str;
		size_t index		   = 0;
		for (auto i = begin; i != end; ++i, ++index)
		{
			const boost::smatch &match	= *i;
			const string_type matching	= match.str();
			const string_type str_index = std::to_string(index);
			if (match[1].matched && match[2].matched && match[3].matched)
			{ // function match
				auto args_begin = boost::sregex_iterator(match[3].str().begin(), match[3].str().end(), split_args_rgx);
				auto args_end	= boost::sregex_iterator();
				std::vector<expression> function_args;
				for (auto arg = args_begin; arg != args_end; ++arg)
				{
					function_args.emplace_back(arg->str());
				}
				objects.emplace(
					str_index, std::make_unique<call_function_action>(deps.at(match[2].str()), std::move(function_args))
				);
			}
			if (match[4].matched && match[5].matched)
			{ // bracket expression (inner expression)
				objects.emplace(str_index, match[5].str());
			}
			if (match[6].matched)
			{ // variable matched
				string_type group = match[6].str();
				if (group != matching)
				{
					throw std::runtime_error("unable to parse variable '" + matching + "'");
				}
				objects.emplace(str_index, std::make_unique<unpack_variable_action>(group));
			}
			if (match[7].matched)
			{ // number matched
				objects.emplace(str_index, number{ match[7].str() });
			}
			// replace matching to "{i}" for easy operation finding

			size_t pos = simplified.find(matching);
			sstream_type ss;
			ss << "{" << index << "}";
			simplified.replace(pos, matching.size(), ss.str());
		}
		// search all operations in this expression scope
		auto op_begin = boost::sregex_iterator(simplified.begin(), simplified.end(), search_operators);
		auto op_end	  = boost::sregex_iterator();
		std::vector<const prioritized *> operations;
		for (auto op = op_begin; op != op_end; ++op)
		{
			auto &match = *op;
			try
			{
				auto operator_char = match.str()[0];
				if (match[1].matched)
				{
					operations.push_back(&unary_operation::get_by_name(operator_char));
				}
				if (match[2].matched)
				{
					operations.push_back(&binary_operation::get_by_name(operator_char));
				}
			}
			catch (std::out_of_range &e)
			{
				throw std::runtime_error("unable to parse operation '" + match.str() + "'");
			}
		}
		// sort operations by their priorities
		std::sort(operations.begin(), operations.end(), [](auto a, auto b) {
			return a->priority() < b->priority();
		});
		// merge operations
		for (auto op = operations.begin(); op != operations.end(); op = operations.erase(op))
		{
			boost::smatch match;
			string_type str_index;
			if (auto binary = dynamic_cast<const binary_operation *>(*op))
			{
				std::cout << "b" << binary->get() << " ";
				boost::regex rgx(get_pattern(
					binary_operator_operands_pattern,
					{
						{ "${__op}", string_type(1, binary->get()) }
				 }
				));
				if (!boost::regex_search(simplified, match, rgx))
				{
					throw std::runtime_error("no operation found in string '" + simplified + "'");
				}
				if (!(match[1].matched && match[2].matched))
				{
					throw std::runtime_error((match[1].matched ? string_type("right") : "left") + " operand not found");
				}
				str_index			  = match[1].str();
				objects.at(str_index) = expression(
					std::make_unique<binary_operator_action>(objects.at(match[1]), *binary, objects.at(match[2]))
				);
				objects.erase(objects.find(match[2].str()));
			}
			else if (auto unary = dynamic_cast<const unary_operation *>(*op))
			{
				std::cout << "u" << unary->get() << " ";
				boost::regex rgx(get_pattern(
					unary_operator_operand_pattern,
					{
						{ "${__op}", string_type(1, unary->get()) }
				}
				));
				if (!boost::regex_search(simplified, match, rgx))
				{
					throw std::runtime_error("no operation found in string '" + simplified + "'");
				}
				if (!match[1].matched)
				{
					throw std::runtime_error("operand not found");
				}
				str_index = match[1].str();
				objects.at(str_index)
					= expression(std::make_unique<unary_operator_action>(*unary, objects.at(str_index)));
			}
			auto matching = match.str();
			size_t pos	  = simplified.find(matching);
			simplified.replace(pos, matching.size(), "{" + str_index + "}");
		}
		if (objects.size() != 1)
		{
			throw std::runtime_error("unable to parse expression '" + str + "'");
		}
		return copy_value(objects.begin()->second);
	}
} // namespace mg
