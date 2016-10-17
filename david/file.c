/**
 * File module.
 * Consiste of helper function for reading files.
 * David Gonzalez
 * 15.10.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

/**
 * Read an entire text file and put it in a newly allocated array.
 * @param path Path to the file.
 * @return Newly allocated space for array (one line per element) or NULL if any error. Must be freed.
 */
file_content * file_read_full(const char * path)
{
    FILE * fp = fopen(path, "r");
    file_content * content = NULL;

    if (fp != NULL)
    {
        content = malloc(sizeof(file_content));
        content->array_size = file_get_lines_number(fp, true);
        content->lines = calloc(content->array_size, sizeof(char*));

        // Read each lines
        char current_line[FILE_READ_FULL_LINE_MAX] = "";
        for (size_t i = 0; i < content->array_size; ++i)
        {
            if (fgets(current_line, FILE_READ_FULL_LINE_MAX, fp) != NULL)
            {
                // Remove trailing linefeed
                current_line[strcspn(current_line, "\r\n")] = '\0';
                size_t len = strlen(current_line) + 1;

                // Copy
                content->lines[i] = calloc(len, sizeof(char));
                strncpy(content->lines[i], current_line, len);
            }
        }

        fclose(fp);
        fp = NULL;
    }
    else
    {
        perror(path);
    }

    return content;
}

/**
 * Free the content loaded by file_read_full().
 * @param content The structure returned by file_read_full().
 */
void file_content_free(file_content * content)
{
    if (content != NULL)
    {
        for (size_t i = 0; i < content->array_size; ++i)
        {
            free(content->lines[i]);
        }
        free(content->lines);
        free(content);
    }
}

/**
 * Get number of lines in a file (already opened).
 * @param fp Opened file.
 * @param do_rewind If TRUE rewind the file pointer to the beginning.
 * @return result Number of lines.
 */
size_t file_get_lines_number(FILE * fp, bool do_rewind)
{
    size_t result = 0;

    while (!feof(fp))
    {
        char ch = fgetc(fp);
        if (ch == '\n')
        {
            result++;
        }
    }

    if (do_rewind == true)
    {
        rewind(fp);
    }

    return result;
}
