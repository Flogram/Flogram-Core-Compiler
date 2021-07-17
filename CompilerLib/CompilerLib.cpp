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
	return NULL;
}

int CompileObj::fast_forward_till_comment_end(const char* look_for_me, char* human_code, int cur_pos) {
	return 0;
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
				if (human_code[cur_pos + 1] == '[') {
					//comment_open		// #[
					//fast forward until you hit ]#, which closes the comment

				}
				else {
					//skip rest of line, fast forward until you hit a newline
				}

				break;
			case ']':
				if (human_code[cur_pos + 1] = '#') {
					//comment_close	// ]#
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
