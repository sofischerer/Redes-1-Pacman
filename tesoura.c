#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SEGMENT 31


long file_size(char *name)
{
    FILE *fp = fopen(name, "rb");

    long size=-1;
    if(fp)
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp)+1;
        fclose(fp);
    }
    return size;
}

int main()
{
    int segments, i, accum;
    FILE *fp1, *fp2;
    char largeFileName[]={"../garchomp.jpg"};//change to your path
/*    long sizeFile = file_size("largeFileName");*/
    segments = 2 /*sizeFile/SEGMENT + 1*/;//ensure end of file
    char filename[260]={"../smallFileName.txt"};//base name for small files.
    char line[1080];

    fp1 = fopen(largeFileName, "r");
    if(fp1)
    {
        for(i=0;i<segments;i++)
        {
            accum = 0;
            fp2 = fopen(filename, "w");
            if(fp2)
            {
                while(fgets(line, 31, fp1) && accum <= SEGMENT)
                {
                    accum += strlen(line);//track size of growing file
                    fputs(line, fp2);
                }
                fclose(fp2);
            }
        }
        fclose(fp1);
    }
    return 0;
}
