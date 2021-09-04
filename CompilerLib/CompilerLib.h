#pragma once
class CompileObj{
public:
	void compile_file(const char* file_name);

	enum class keywords {
		comment_single_line,// #
		comment_open,		// <#
		comment_close,		// #>
		brace_open,			// {
		brace_close,		// }
		atomic,				// atomic
		bool_,				// bool
		break_,				// break
		case_,				// case:
		catch_,				// catch
		char_,				// char
		enum_,				// enum
		exit,				// exit program
		finally,			// 
		float_literal,		// float literal (e.g 12.34, -4.566 etc)
		fn,					// function
		foreach_,			// foreach (loop type)
		fp32,				// 32 bit floating point
		fp64,				// 64 bit floating point
		global,				// global
		if_,				// if
		in,					// in (used in foreach)
		int_literal,		// integer literal (e.g 1234, -4566 etc)
		int8,				// int - 8 bit
		int16,				// int - 16 bit
		int32,				// int - 32 bit
		int64,				// int - 64 bit
		let,				// let
		loop,				// loop
		match,				// match
		mut,				// mut
		Object,				// Object
		open,
		read,
		return_,			// return
		str,				// str (string)
		throw_,				// throw
		try_,				// try
		Type,				// Type
		uint8,				// unsigned int 8 bits
		uint16,				// unsigned int 16 bits
		uint32,				// unsigned int 32 bits
		uint64,				// unsigned int 64 bits
		write,
		error_				// serves as the extra option for errors in lexing
	};

private:
	struct cur_pos_and_lines_skipped {
		int cur_pos;
		int lines_skipped;
	};

	char* read_file_into_memory(const char* file_name, int* file_size);
	char determine_line_ending_character(char* human_code, int file_size, int& line_ending_extra_one);
	char* convert_code_human_to_binary(char *human_code, int file_size);
	char* word_match(const char* look_for_me, char* human_code, int cur_pos);
	int fast_forward_till_comment_end(char* human_code, int cur_pos, int file_size, int& cur_line, int& line_started_at_char_num, char line_ending_char, int line_ending_extra_one);
	int fast_forward_till_line_end(char* human_code, int cur_pos, int file_size);

	keywords match_number(char* human_code, char* binary_code, int *cur_pos, int cur_line, int file_size, int column);
	keywords match_type_literal(char* human_code, char* binary_code, int *cur_pos, int file_size);
	keywords match_keyword(char* human_code, char* binary_code, int *cur_pos, int file_size);
	keywords match_identifier(char* human_code, char* binary_code, int *cur_pos, int file_size);
	keywords check_letters(char* human_code, char* binary_code, int *cur_pos, int file_size);
	keywords match_string_literal(char* human_code, char* binary_code, int *cur_pos, int file_size);
	keywords match_symbol(char* human_code, char* binary_code, int *cur_pos, int file_size);

	std::string consume_digits(char* human_code, char* binary_code, int* cur_pos, int file_size);
};