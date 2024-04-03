#include "command.h"

#include <string.h>

void HDPSCommand::append(char *data) {
  char *query = nullptr;
  char *result = nullptr;

  query = strtok(data, "\r\n");
  result = strtok(NULL, "\r\n");
  if (!result) {
    // the unit will either reply with the result of the query on one line
    // and then the prompt on the next line, or it will just reply with the
    // prompt in case of a void command.
    //
    // if it's a void command, set result to point to the first split.
    result = query;
  }

  if (query) {
    this->data_ += query;
  }

  if (result) {
  }
}
