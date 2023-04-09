// CSCE 3600 - major 2 - group 6
// path.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    path built-in program source, allows user to change path env

#include "path.h"

// In order to PATH modifications in path.c to remain persistent in the shell
// parent process calling the built-in path program the path is written
// to a file which the shell will update from when path returns
// from execution successfully.

// Retrieve PATH env from file passed by filename
// Returns a char buffer on success, NULL on error.
char* get_pathenv(char* env_filename) {

  char* path;
  FILE* path_file;
  path = malloc(MAX_PATH_LENGTH);

  if ( path == NULL ) {
    perror("ERROR: Failed to malloc path in get_path ");
    return path;
  }

  if ( (path_file = fopen(env_filename, READ_ONLY)) == NULL ) {
    perror("ERROR: Failed to open path_file ");
    free(path);
    return NULL;
  }

  if ( fgets(path, MAX_PATH_LENGTH, path_file) == NULL ) {
    perror("ERROR: Failed to read path from path_file" );
    fclose(path_file);
    free(path);
    return NULL;
  }

  fclose(path_file);
  return path;
}

// Write passed PATH env to file passed by filename
// proper usage is to read the current PATH, modify the string based on user in
// and then call set_path to set the new path in the path_file file.
// returns: 0 on success, -1 on error
int set_pathenv(char* env_filename, char* new_path) {

  FILE* path_file;
  if ( (path_file = fopen(env_filename, TRUNC)) == NULL ) {
    perror("ERROR: Failed to open/create path_file ");
    return -1;
  }

  if ( fputs(new_path, path_file) != EOF ) {
    fclose(path_file);
    return 0;
  }
  else {
    perror("ERROR: unsuccessful write to path_file ");
    fclose(path_file);
    return -1;
  }
}

// append string to path
int append_to_path(char* dirstr) {

  return 0;
}

// remove string from path
int remove_from_path(char* dirstr) {

  return 0;
}
