#include <stdbool.h>  // true, false, ...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    // input buffer structure
    char* buffer;
    size_t buffer_length;  // size_t is guaranteed to be large enough to
			   // represent all possible indices into the array.
    ssize_t input_length;

} InputBuffer;

InputBuffer* new_input_buffer()
{
    // allocate the momeory
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));

    // assign initial values to buffer
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt() { printf("db > "); }
void read_input(InputBuffer* input_buffer)
{
    // read the input line and update the struct
    ssize_t bytes_read =
	getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    // validate
    if (bytes_read <= 0) {
	printf("error reading input \n");
	exit(EXIT_FAILURE);
    }

    // ignore the new line input
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char* argv[])
{
    // create a new input buffer
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
	// print prompt
	print_prompt();

	// read the input
	read_input(input_buffer);

	// handle exit or unknown input
	if (strcmp(input_buffer->buffer, ".exit") == 0) {
	    close_input_buffer(input_buffer);
	    exit(EXIT_SUCCESS);
	}
	else {
	    printf("unknown command \"%s\". \n", input_buffer->buffer);
	}
    }
}
