// CompilerLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "CompilerLib.h"

errno_t err;

char *CompileObj::read_file_into_memory(const char* file_name, int* file_size) {
	FILE* fp;
	if ((err = fopen_s(&fp, file_name, "rb")) == 0) {
		fseek(fp, 0, SEEK_END);
		*file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = (char*)malloc(*file_size + 1);

		fread(buffer, 1, *file_size, fp);

		fclose(fp);

		return buffer;
	}
	return nullptr;
}

//check if the rest of the string matches character
char* CompileObj::word_match(const char* look_for_me, char* human_code, int cur_pos) {
	return NULL;
}


/// <summary>
/// Fast forward until you reach the end of the comment
/// Treat comments as nested
/// </summary>
/// <param name="human_code">Pointer to beginning of this block of code</param>
/// <param name="cur_pos">Start fast forwarding here</param>
/// <returns></returns>
int CompileObj::fast_forward_till_comment_end(char* human_code, int cur_pos, int file_size, int& cur_line, int& line_started_at_char_num, char line_ending_char, int line_ending_extra_one) {
	//allow nested comments   <#  #>
	int num_open_comments = 1;

	while (cur_pos<file_size) {
		char cur_char = human_code[cur_pos];
		char next_char = human_code[cur_pos+1];
		if (cur_char == '#' && next_char == '>') {
			if (num_open_comments == 1) {
				return cur_pos+1;  //only increment by 1 and upon returning the cur_pos for loop will take care of the next one
			}
			num_open_comments -= 1;
		}
		else if (cur_char == '<' && next_char == '#') {
			num_open_comments += 1;
		}
		else if (cur_char == line_ending_char) {
			cur_line++;
			cur_pos += line_ending_extra_one;
			line_started_at_char_num = cur_pos+1; //plus 1 because we haven't incremented cur_pos yet
		}
		cur_pos++;
	}
	return cur_pos;
}

/// <summary>
/// Step through code until you reach the beginning of the next line
/// </summary>
/// <param name="look_for_me"></param>
/// <param name="human_code">Pointer to beginning of this block of code</param>
/// <param name="cur_pos">Start fast forwarding here</param>
/// <returns>new character position</returns>
int CompileObj::fast_forward_till_line_end(char* human_code, int cur_pos, int file_size) {
	//line ending could be '\n', '\r', or "\r\n"
	while (cur_pos < file_size) {
		if (human_code[cur_pos] == '\r') {
			if (human_code[cur_pos + 1] == '\n') {
				return cur_pos + 1;
			}
			return cur_pos;
		}
		else if (human_code[cur_pos] == '\n') {
			return cur_pos;
		}
		cur_pos++;
	}
	return cur_pos;
}

/// <summary>
/// Step through the code until either '/r' or '/n' is encountered
/// We now know this is what is being used in this file to mark line endings
/// </summary>
/// <param name="human_code">source code</param>
/// <param name="file_length"></param>
/// <param name="num_chars">if line ending is "\r\n",return 2, otherwise 1</param>
/// <returns></returns>
char CompileObj::determine_line_ending_character(char* human_code, int file_size, int& line_ending_extra_one) {
	line_ending_extra_one = 0;
	for (int i = 0; i < file_size; i++) {
		switch (human_code[i]) {
			case '\r':
				if (human_code[i + 1] == '\n') {
					line_ending_extra_one = 1;
				}

				return '\r';
			case '\n':
				return '\n';
		}
	}
	//if we reach here, no line endings detected, so it doesn't really matter
	return 1;
}


char* CompileObj::convert_code_human_to_binary(char* human_code, int file_size) {
	//reserve as much space as we need for the human readable file, binary file will be smaller
	//so after writing, trim down to the amount of memory that was actually used, temporarily using a little extra
	char* binary_code = (char *)malloc(file_size);
	int cur_line = 1;  //track line number for error messages
	int line_started_at_char_num = 0; //set equal to cur_char whenever we get to a new line
	int add_extra_one = 0;

	char line_ending_char = determine_line_ending_character(human_code, file_size, add_extra_one);

	for (int cur_pos = 0; cur_pos < file_size; cur_pos++) {
		//read from human_code one character at a time
		char cur_char = human_code[cur_pos];

		// Flogram keywords are case insensitive
		if (cur_char >= 'A' && cur_char <= 'Z') {
			cur_char += 32;
		}

		switch (cur_char) {
			//comment_single_line		// #
			case '#':
				//skip rest of line, fast forward until you hit a newline
				cur_pos = fast_forward_till_line_end(human_code, cur_pos + 1, file_size);
				cur_line++;
				line_started_at_char_num = cur_pos+1;

				break;
			case '<':  // <# multi-line comment #>
				if (human_code[cur_pos + 1] == '#') {
					//comment_close	// #>
					cur_pos = fast_forward_till_comment_end(human_code, cur_pos + 2, file_size, cur_line, line_started_at_char_num, line_ending_char, add_extra_one);
				}
				break;

			case '\n':
			case '\r':
				cur_pos += add_extra_one;  //add extra one to position.. if we need it with line ending "\n\r"
				cur_line += 1;
				line_started_at_char_num = cur_pos + 1;
			case ' ':
			case '\t':
				// in the future, might want to fast forward through whitespace for a small extra performance boost
				// afterall whitespace tends to occur in bulk.. might not be worth it
				break;
			default:
				// unrecognized character, report error
				std::cout << "Error, unrecognized character: " << cur_char << " at line #" << cur_line << " character #" << line_started_at_char_num - cur_pos << "\n";
				_exit(-1);
		}
	}

	return binary_code;
}

// TODO: This is an example of a library function
void CompileObj::compile_file(const char* file_name)
{
	int file_size=0;
	char* mem_file = read_file_into_memory(file_name, &file_size);
	
	char* binary_code = convert_code_human_to_binary(mem_file, file_size);
	
}
