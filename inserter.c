//
// insertr v1.0
//
// by Jose Peral
//

#include <stdio.h>
#include <ctype.h>         // upper lower case converition functions
#include <string.h>
#include <stdlib.h>

/* Private Local Function prototypes */
void PrintHelp(void);

//----------------------------------------------------------
// Global variables
//

// input file
char input_file[256]={0};
char output_file[256]={0};
char find[256]={0};
char insert[256]={0};
unsigned char error=0;

int main(int argc, char*argv[]){

  system("cls");
  printf("[Inserter v1.0, by Jose Peral (2020-05-20)]\r\n\r\n");

  if (argc<=1){
      PrintHelp();
      return(-1);
  }

  // Parameters Parsing
  unsigned char i;
  for(i=1;i<argc;i++){

    if       (strcmp(argv[i], "-in")==0){
      if ( strlen(argv[i+1]) < sizeof(input_file) ){
        strcpy(input_file,argv[i+1]);
      }else{
        printf("Parameter is too long.\r\n");
        return(-1);
      }
      i++;
    }else if (strcmp(argv[i], "-out")==0){
      if ( strlen(argv[i+1]) < sizeof(output_file) ){
        strcpy(output_file,argv[i+1]);
      }else{
        printf("Parameter is too long.\r\n");
        return(-1);
      }
      i++;
    }else if (strcmp(argv[i], "-find")==0){
      if ( strlen(argv[i+1]) < sizeof(find) ){
        strcpy(find,argv[i+1]);
      }else{
        printf("Find parameter is too long.\r\n");
        return(-1);
      }
      i++;
    }else if (strcmp(argv[i], "-insert")==0){
      if ( strlen(argv[i+1]) < sizeof(insert) ){
        strcpy(insert,argv[i+1]);
      }else{
        printf("Insert parameter is too long.\r\n");
        return(-1);
      }
      i++;
    }else{
      PrintHelp();
      return(-1);
    }

  } // end of for argc parsing

  printf("Parameters:\n  InputFile: %s\n OutputFile: %s\n       Find: \"%s\"\n     Insert: \"%s\"\n\n", input_file, output_file, find, insert);

  //  Check input file
  FILE* fp=fopen(input_file,"rb");

  // Get file size.
  unsigned long file_size;
  if(fp!=0){
    printf("Opening input file: \"%s\" ",input_file);
    fseek(fp,0,SEEK_END);
    file_size = ftell(fp);
    rewind(fp);
    printf("File size=%d bytes", file_size);
  }else{
    printf("Error opening file %s\r\n");
	  return (-1);
  }

  // Allocate RAM
  unsigned char *pRamFile_start;
  pRamFile_start = malloc(file_size);
  if (pRamFile_start==0){
    printf("Error allocating memory\r\n");
    return(-1);
  }

  // Load file into RAM memory.
  fread(pRamFile_start, 1, file_size, fp);
  fclose(fp); // Close the .csv file

  // Print first 64 bytes (just for fun)
  printf("\r\nDumping the first 64 bytes just for fun...");
  unsigned long j;
  unsigned char *pRamFile;
  pRamFile = pRamFile_start;
  printf("\r\nFile dump:");
  for(j=0; j<256; j++){
    if(j%32==0) printf("\r\n");
    printf("%02x ",*pRamFile);
    pRamFile++;
  }

  // Search for the pattern start address
  printf("\n\nSearching for \"%s\"...\r\n", find);
  pRamFile = pRamFile_start;
  unsigned char *pRamFile_match;
  unsigned long address=0;
  for(j=0; j<file_size; j++){                      // cycle all bytes of ramfile
    if (memcmp(pRamFile, find, strlen(find))==0){  // looking for the find pattern
      pRamFile_match = pRamFile;                   // if a match happens, then save the pointer and address.
      address = j;
      printf("Found pattern \"%s\" at 0x%08x \n", find, address);
    }
    pRamFile++;
  }
  if (address==0){
    printf("Pattern not found.\n");
    return(-1);
  }

  // Print the match area before patching
  printf("\r\nDumping @ last matching address before insertion...");
  pRamFile = pRamFile_match;
  printf("\r\nFile dump:");
  for(j=0; j<256; j++){
    if(j%32==0) printf("\r\n");
    printf("%02x ",*pRamFile);
    pRamFile++;
  }

  // insert find with pattern.
  printf("\r\n\r\nInserting \"%s\" at address 0x%08x\r\n", insert,address);
  memcpy(pRamFile_match, insert, strlen(insert));

  // Print the match area after patching
  printf("\r\nDumping @ last matching address after insertion...");
  pRamFile = pRamFile_match;
  printf("\r\nFile dump:");
  for(j=0; j<256; j++){
    if(j%32==0) printf("\r\n");
    printf("%02x ",*pRamFile);
    pRamFile++;
  }

  // Export output file
  printf("\r\n\r\nSaving output file: %s\r\n", output_file);
  fp=fopen(output_file,"wb");
  fwrite(pRamFile_start, 1 , file_size, fp);
  fclose(fp);

  // Destroy dinamically allocated memory.
  free(pRamFile_start);

	return(0);
} // end of main()

//-----------------------------------------
//
// Local Private functions
//
void PrintHelp(void){
  printf("parameter not supported.\r\n");
  printf("-----------------------------------------------------------------------------------------------------\r\n");
  printf("This program will scan the input binary file specified after -in,\r\n");
  printf("looking in there for the ASCII string specified after -find (%d bytes max),\r\n", sizeof(find));
  printf("and replacing it with the ASCII string specified after -insert (%d bytes max)\r\n",sizeof(insert));
  printf("Note that -insert string will overwrite -find pattern at the matching address.\r\n");
  printf("Usage:\r\n");
  printf("   inserter.exe -in <FILENAME> -out <FILENAME> -find <FINDPATTERN> -insert <INSERTPATTERN>\r\n");
  printf("-----------------------------------------------------------------------------------------------------\r\n");
}
