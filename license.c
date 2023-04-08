#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
if (argc != 2)
{
printf("Usage: ./read infile\n");
return 1;
}

char buffer[7];

char *plates[8];

FILE *infile = fopen(argv[1], "r");

if (infile == NULL)
{
    printf("Error opening file.\n");
    return 1;
}

int idx = 0;

while (fgets(buffer, sizeof(buffer), infile) != NULL && idx < 8)
{
    buffer[strcspn(buffer, "\n")] = '\0';

    plates[idx] = strdup(buffer);
    idx++;
}

fclose(infile);

for (int i = 0; i < idx; i++)
{
    printf("%s\n", plates[i]);
    free(plates[i]);
}

return 0;
}
