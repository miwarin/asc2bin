#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

enum ST_ANALYZE
{
  ST_ANALYZE_HIGH,
  ST_ANALYZE_LOW,
  ST_ANALYZE_DELIM,
};

char hex(char c)
{
  char h = 0;

  if('0' <= c && c <= '9')
  {
    h = c - '0';
  }
  else if('a' <= c && c <= 'f')
  {
    h = (c - 'a') + 10;
  }
  else if('A' <= c && c <= 'F')
  {
    h = (c - 'A') + 10;
  }
  return h;
}

void conv(char* in_file_path, char* out_file_path)
{
  struct stat st;
  int result;
  int in_file_size;
  char* in_file_buf = NULL;
  char* out_file_buf = NULL;
  FILE* in_fp = NULL;
  FILE* out_fp = NULL;
  int read_bytes;
  int write_bytes;
  char hi;
  char lo;
  int i;
  int in_pos;
  int out_pos;
  int analy_st;

  result = stat(in_file_path, &st);
  if(result != 0)
  {
    perror("stat");
    goto ERROR;
  }

  in_file_size = st.st_size;

  in_file_buf = (char*)malloc(in_file_size);
  if(in_file_buf == NULL)
  {
    perror("malloc");
    goto ERROR;
  }

  out_file_buf = (char*)malloc(in_file_size);
  if(out_file_buf == NULL)
  {
    perror("malloc");
    goto ERROR;
  }


  in_fp = fopen(in_file_path, "rb");
  if(in_fp == NULL)
  {
    perror("fopen");
    goto ERROR;
  }

  read_bytes = fread(in_file_buf, 1, in_file_size, in_fp);
  if(read_bytes != in_file_size)
  {
    perror("fread");
    goto ERROR;
  }

  analy_st = ST_ANALYZE_HIGH;

  for(out_pos = 0, in_pos = 0; in_pos < in_file_size; in_pos++)
  {
    char c = in_file_buf[in_pos];

    switch(analy_st)
    {
    case ST_ANALYZE_HIGH:
      hi = hex(c);
      analy_st = ST_ANALYZE_LOW;
      break;
    case ST_ANALYZE_LOW:
      lo = hex(c);
      out_file_buf[out_pos] = (hi << 4) + lo;
      out_pos++;
      analy_st = ST_ANALYZE_DELIM;
      break;
    case ST_ANALYZE_DELIM:
      if(c == ' ' || c == '\n')
      {
        analy_st = ST_ANALYZE_HIGH;
      }
      break;
    default:
      break;
    }

  }

  out_fp = fopen(out_file_path, "wb");
  if(out_fp == NULL)
  {
    perror("fopen");
    goto ERROR;
  }

  write_bytes = fwrite(out_file_buf, 1, out_pos, out_fp);
  if(write_bytes != out_pos)
  {
    perror("fread");
    goto ERROR;
  }

ERROR:
  if(in_file_buf != NULL)
  {
    free(in_file_buf);
  }

  if(out_file_buf != NULL)
  {
    free(out_file_buf);
  }

  if(in_fp != NULL)
  {
    fclose(in_fp);
  }

  if(out_fp != NULL)
  {
    fclose(out_fp);
  }
}


int main(int ac, char** av)
{
  char* in_file_path;
  char* out_file_path;

  in_file_path = "asc2bin.txt";
  out_file_path = "asc2bin.bin";

  conv(in_file_path, out_file_path);  

  return 0;
}
