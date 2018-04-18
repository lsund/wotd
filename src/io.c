/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#include "io.h"

wchar_t *wtext;
Uint textlen;

// The table of filehandles
static Filehandle *filehandle = NULL;

// Number of allocated filehandles
static Uint allocatedFilehandle = 0;

// Number of open files
static Uint currentopen = 0;


// The following three tables store important information to
// generate meaningfull error messages.
static Uint filedesc(
        char *file,
        Uint line,
        bool existing,
        FILE *fp
    )
{

    Filedesctype fd;
    fd = fileno(fp);
    if (fd == -1) {
        fprintf(stderr,"cannot find filedescriptor: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (existing) {
        if (allocatedFilehandle <= (Uint) fd) {
            fprintf(stderr,"file %s, line %lu: cannot open file: fd=%lu, "
                    "not enough file handles available\n",
                    file,
                    (Ulong) line,
                    (Ulong) fd);
            exit(EXIT_FAILURE);
        } else {
            if (filehandle[fd].createfile == NULL)
            {
                fprintf(stderr,"file %s, line %lu: cannot open file: fd=%lu, "
                        "file handle not occurpied\n",
                        file,
                        (Ulong) line,
                        (Ulong) fd);
                exit(EXIT_FAILURE);
            }
        }
    }
    return (Uint) fd;
}

static void setinfo(char *file, Uint line, Uint fd, char *path, char *mode)
{
  while (fd >= allocatedFilehandle) {
    Uint i;

    ALLOC(
        filehandle,
        filehandle,
        Filehandle,
        allocatedFilehandle + INCFILEHANDLES
    );

    for (i=allocatedFilehandle; i<allocatedFilehandle+INCFILEHANDLES; i++) {
      filehandle[i].createfile = NULL;
    }

    allocatedFilehandle += INCFILEHANDLES;

  }

  NOTSUPPOSEDTOBENULL(filehandle);

  if (filehandle[fd].createfile != NULL) {
    fprintf(stderr,"file %s, line %lu: open file \"%s\" with mode \"%s\": "
                   "handle %lu already occupied\n",
           file,(Ulong) line,path,mode,(Ulong) fd);
    exit(EXIT_FAILURE);
  }
  strcpy(filehandle[fd].createmode,mode);
  if (strlen(path) > PATH_MAX) {
    fprintf(stderr,"file %s, line %lu: cannot open file \"%s\": "
                   "path is too long\n",file,(Ulong) line,path);
    exit(EXIT_FAILURE);
  }
  strcpy(filehandle[fd].path,path);

  filehandle[fd].createfile = file;
  filehandle[fd].createline = line;

  currentopen++;
}


// Create a filehandle for the relevant file information
FILE *createfilehandle(char *file, Uint line, char *path, char *mode)
{
    FILE *fp;

    fp = fopen(path, mode);

    if (fp == NULL) {
        ERROR2("cannot open file \"%s\": %s", path, strerror(errno));
        return NULL;
    }

    return fp;
}


// Open file in readmode, return file descriptor. The length of the file is
// stored in `textlen`. If `writefile` is true if the file should also be
// opened for triting.
static int fileOpen(char *name, Uint *textlen, bool writefile)
{
    int fd;
    struct stat buf;

    if((fd = open(name,(writefile) ? O_RDWR : O_RDONLY)) == -1) {
        ERROR1("fileOpen: Cannot open \"%s\"",name);
        return -1;
    }
    if(fstat(fd,&buf) == -1) {
        ERROR2("file \"%s\": fstat(fd = %d) failed",name,fd);
        return -2;
    }
    *textlen = (Uint) buf.st_size;
    return fd;
}

// Allocates `len` bytes starting at offset in the file `fd` into memory.
caddr_t fileParts(int fd, Uint offset, Uint len, bool writemap)
{
    caddr_t addr;

    addr = (caddr_t) mmap(
                        (caddr_t) 0,
                        (size_t) len,
                        writemap ? (PROT_READ | PROT_WRITE) : PROT_READ,
                        MAP_PRIVATE,fd,(off_t) offset
                    );
    if (addr == MAP_FAILED) {
        ERROR4("fileParts(fd = %d, left = %ld, len = %ld, %s) failed",
                fd,
                (long) offset,
                (long) len,
                writemap ? "writable map" : "readable map");
        return NULL;
    }
    return addr;
}


// Frees the text specified
void freetextspace()
{
  (void) munmap((caddr_t) wtext,(size_t) textlen);
}


// Return the pointer to the contents of a file, as a string.
caddr_t file2String(char *name, Uint *textlen)
{
    int fd;

    fd = fileOpen(name, textlen, false);
    if(fd < 0) {
        return NULL;
    }
    return fileParts(fd, 0, *textlen, false);
}

Uint file2Array(char *name, Uint *textlen, int size, wchar_t ***wordsp)
{
    wchar_t **words = *wordsp;
    int fd = fileOpen(name, textlen, false);

    if (fd < 0) {
        return -1;
    }
    int max_line_len = 1001;

    FILE *fp = fopen(name, "r");
    if (fp == NULL) {
        fprintf(stderr,"Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    Uint i;
    for (i = 0; i < 999; i++)
    {
        Uint j;

        /* Allocate space for the next line */
        words[i] = (wchar_t *) malloc(max_line_len * sizeof(wchar_t));

        if (words[i] == NULL) {
            fprintf(stderr,"Out of memory (3).\n");
            exit(4);
        }

        wint_t c;
        j = 0;
        do  {
            c = fgetwc(fp);
            if (c == WEOF) {

                *wordsp = words;
                fclose(fp);
                return i;
            }
            words[i][j] = c;
            j++;
        } while (c != 10);

        words[i][j - 1] = 0;
    }
    *wordsp = words;
    fclose(fp);
    return i;
}


FILE *open_append(const char *path)
{
    fclose(fopen(path, WRITEMODE));
    return fopen(path, APPENDMODE);
}
