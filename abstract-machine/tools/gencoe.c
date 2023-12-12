#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <inttypes.h>

static char *img_file = NULL;
static char *elf_file = NULL;
static char *tar_file_0 = NULL;
static char *tar_file_1 = NULL;
static char *tar_file_2 = NULL;
static char *tar_file_3 = NULL;
int mode;

static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"elf"      , required_argument, NULL, 'e'},
    {"target_0" , required_argument, NULL, 't'},
    {"target_1" , required_argument, NULL, 'y'},
    {"target_2" , required_argument, NULL, 'u'},
    {"target_3" , required_argument, NULL, 'i'},
    {"hex_0"    , required_argument, NULL, 'a'},
    {"hex_1"    , required_argument, NULL, 's'},
    {"hex_2"    , required_argument, NULL, 'd'},
    {"hex_3"    , required_argument, NULL, 'f'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-he:t:y:u:i:a:s:d:f:", table, NULL)) != -1) {
    switch (o) {
      case 'e': elf_file = optarg; break;
      case 't': mode = 0; tar_file_0 = optarg; break;
      case 's':
      case 'y': tar_file_1 = optarg; break;
      case 'd':
      case 'u': tar_file_2 = optarg; break;
      case 'f':
      case 'i': tar_file_3 = optarg; break;
      case 'a': tar_file_0 = optarg; mode = 1; break;
      case 1: img_file = optarg; return 0;
      default:
        printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
        printf("\t-e,--elf=ELF_FILE       load elf\n");
        printf("\n");
        exit(0);
    }
  }
  return 0;
}

char init_0[] = "memory_initialization_radix=16;\n";
char init_1[] = "memory_initialization_vector=";

#define MAP(c, f) c(f)
#define TAR(f) f(0) f(1) f(2) f(3)
#define INIT(t) fwrite(init_0, 1, sizeof(init_0) - 1, tar##t); fwrite(init_1, 1, sizeof(init_1) - 1, tar##t);
#define CREATE(t) FILE *tar##t = fopen(tar_file_##t, "w");
#define WRITE(t) fwrite(bufhex+(t * 4), 1, 4, tar##t);

#define COE_LEN 65536

void gencoe() {
  MAP(TAR, CREATE)
  printf("Open target files.\n");
  FILE *img = fopen(img_file, "rb");
  if (img == NULL) printf("fail to open img.\n");
  else printf("Open image file.\n");
  
  fseek(img, 0, SEEK_END);
  long size = ftell(img);
  if (size > (COE_LEN << 2)) {
    printf("This programm is too big for cpu.\n");
    exit(0);
  }
  // printf("size = %d\n", size);

  MAP(TAR, INIT)
  long offset = 0;
  uint8_t buf[4];
  char bufhex[20];
  while (offset < size) {
    fseek(img, offset, SEEK_SET);
    fread(buf, 1, sizeof(buf), img);
    switch (mode) {
      case 0: sprintf(bufhex, "%02x, %02x, %02x, %02x, ", buf[0], buf[1], buf[2], buf[3]); break;
      case 1: sprintf(bufhex, "%02x \n%02x \n%02x \n%02x\n", buf[0], buf[1], buf[2], buf[3]);
    }
    MAP(TAR, WRITE)
    offset += sizeof(buf);
  }
  printf("counter = %d\n", offset >> 2);
  sprintf(bufhex, "00, 00, 00, 00, ");
  while (offset < (COE_LEN << 2)) {
    MAP(TAR, WRITE)
    offset += 4;
  }
  printf("counter = %d\n", offset >> 2);
  printf("Done.\n");
}

int main(int argc, char *argv[]) {
  parse_args(argc, argv);
  gencoe();
  
  return 0;
}