// CompilerLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include <stdio.h>
#include <stdlib.h>

#include "CompilerLib.h"

errno_t err;

char *CompileObj::read_file_into_memory(const char* file_name, int* file_size) {
	FILE* fp;
	if ((err = fopen_s(&fp, file_name, "rb")) != 0) {
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

}


/// <summary>
/// Fast forward until you reach the end of the comment
/// Treat comments as nested
/// </summary>
/// <param name="human_code">Pointer to beginning of this block of code</param>
/// <param name="cur_pos">Start fast forwarding here</param>
/// <returns></returns>
int CompileObj::fast_forward_till_comment_end(char* human_code, int cur_pos) {
	//allow nested comments   <#  #>
	int num_open_comments = 1;

	while (true) {
		if (human_code[cur_pos + 1] == '#' && human_code[cur_pos + 1] == '>') {
			if (num_open_comments == 1) {
				break;
			}
			num_open_comments -= 1;
		}
		else if (human_code[cur_pos + 1] == '<' && human_code[cur_pos+1] == '#') {
			num_open_comments += 1;
		}
		cur_pos++;
	}
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
	while (true && cur_pos < file_size) {
		if (human_code[cur_pos] == '\r') {
			if (human_code[cur_pos + 1] == '\n') {
				return cur_pos + 2;
			}
			return cur_pos + 1;
		}
		else if (human_code[cur_pos] == '\n') {
			return cur_pos + 1;
		}
		cur_pos++;
	}
	return cur_pos;
}

/// <summary>
/// Step through non-code characters until you get to a letter or { or } or # or < which indicates the beginning of a variable
/// Fast forward through whitespace <tab> <newline> <space>
/// </summary>
/// <param name="look_for_me"></param>
/// <param name="human_code">pointer to beginning of block of code</param>
/// <param name="cur_pos">Start fast forwarding here</param>
/// <returns>new character position</returns>
int CompileObj::fast_forward_through_whitespace(char* human_code, int cur_pos) {

	while(true){
		char cur_char = human_code[cur_pos];
		if (cur_char != ' ' && cur_char != '/t' && cur_char != '/n' && cur_char != '/r') {
			return cur_pos;
		}

		cur_pos++;
	}
}

char* CompileObj::convert_code_human_to_binary(char* human_code, int file_size) {
	//reserve as much space as we need for the human readable file, binary file will be smaller
	//so after writing, trim down to the amount of memory that was actually used, temporarily using a little extra
	char* binary_code = (char *)malloc(file_size);

	for (int cur_pos = 0; cur_pos < file_size; cur_pos++) {
		//read from human_code one character at a time
		char next_char = human_code[cur_pos];

		switch (next_char) {
			//comment_single_line		// #
			case '#':
				//skip rest of line, fast forward until you hit a newline


				break;
			case '<':  // <# multi-line comment #>
				if (human_code[cur_pos + 1] = '#') {
					//comment_close	// #>
					cur_pos = fast_forward_till_comment_end(human_code, cur_pos + 2);
				}
				break;
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
