#include <stdbool.h> // true, false, ...
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

typedef struct {
  // input buffer structure
  char *buffer;
  size_t buffer_length;
  // size_t is guaranteed to be large enough to
  //       represent   all possible indices into the array.
  ssize_t input_length;

} InputBuffer;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECONGNISED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECONGISED_STATEMENT } PrepareResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;
typedef struct {
  StatementType type;
} Statement;

InputBuffer *new_input_buffer() {
  // allocate the momeory
  InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
  // assign initial values to buffer
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;
  return input_buffer;
}

void print_prompt() { printf("db > "); }
void read_input(InputBuffer *input_buffer) {
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

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  }

  else {
    return META_COMMAND_UNRECONGNISED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer *input_buffer,
                                Statement *statement) {
  // our SQL compiler sits here
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strncmp(input_buffer->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECONGISED_STATEMENT;
}

void execute_statement(Statement *statement) {
  // our virtual machine
  switch (statement->type) {
  case (STATEMENT_INSERT):
    printf("this is where we would do an insert. \n");
    break;
  case (STATEMENT_SELECT):
    printf("this is where we would do a select. \n");
    break;
  }
}

int main(int argc, char *argv[]) {
  // create a new input buffer
  InputBuffer *input_buffer = new_input_buffer();

  while (true) {
    // print prompt
    print_prompt();
    // read the input
    read_input(input_buffer);

    // handle meta commands
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
      case (META_COMMAND_SUCCESS):
        continue;

      case (META_COMMAND_UNRECONGNISED_COMMAND):
        printf("unknown meta command '%s'\n.", input_buffer->buffer);
        continue;
      }
    }
    // handle statements;
    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
    case (PREPARE_SUCCESS):
      break;
    case (PREPARE_UNRECONGISED_STATEMENT):
      printf("unknown keywork at start of '%s'\n.", input_buffer->buffer);
      continue;
    }

    execute_statement(&statement);
    printf("executed.\n");
  }
}
