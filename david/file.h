/**
 * File module.
 * Consiste of helper function for reading files.
 * David Gonzalez
 * 15.10.2016
 */

#include <stdbool.h>

#define FILE_READ_FULL_LINE_MAX 256

/**
 * Data structure to store entire file content
 */
typedef struct file_content
{
    char ** lines;      // Data: one line per element
    size_t array_size;  // Number of lines in the array
} file_content;

/**
 * Read an entire text file and put it in a newly allocated array.
 * @param path Path to the file.
 * @return Newly allocated space for array (one line per element) or NULL if any error. Must be freed.
 */
file_content * file_read_full(const char * path);

/**
 * Free the content loaded by file_read_full().
 * @param content The structure returned by file_read_full().
 */
void file_content_free(file_content * content);

/**
 * Get number of lines in a file (already opened).
 * @param fp Opened file.
 * @param do_rewind If TRUE rewind the file pointer to the beginning.
 * @return result Number of lines.
 */
size_t file_get_lines_number(FILE * fp, bool do_rewind);
