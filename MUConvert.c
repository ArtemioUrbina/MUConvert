#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

#define UNDEFINED	0
#define PC88		1
#define MUC 		2

void PC88writeline(char *buffer, int len, int comment, int *line, FILE *out)
{
	fprintf(out, "%d '", *line);
	fwrite(buffer, sizeof(char), len, out);
	fprintf(out, "\r\n");
	*line += 10;
}

void MUCwriteline(char *buffer, int len, FILE *out)
{
	fwrite(buffer, sizeof(char), len, out);
	fprintf(out, "\r\n");
}

int MUCtoPC88(FILE *file, FILE *out, int removecomments)
{
	char buffer[BUFFER_LEN];
	int read = 0, comment = 0, line = 10, 
		pos = 0, lastlen = 0;

	while (!feof(file))
	{
		read = fgetc(file);
		if(read != EOF)
		{
			if(read == '\t')
				read = ' ';
			switch(read)
			{
				case 0x0D: // Ignore
					break;
				case 0x0A: // Enter
					if(pos || !pos && lastlen)  // eat up multipe empty lines
						PC88writeline(buffer, pos, comment, &line, out);
					lastlen = pos;
					pos = 0;
					comment = 0;
					break;
				case ';': //commented line, ignore
					if(removecomments)
						comment = 1;
					break;
				default:
					if(!comment)
					{
						if(pos < BUFFER_LEN)
							buffer[pos++] = read;
						else
						{
							PC88writeline(buffer, pos, comment, &line, out);
							pos = 0;
							buffer[pos++] = read;
						}
					}
					break;
			}
		}
	}
	printf("Done. Use dskdrv02a with M88 to insert basic file into D88 disk image");
	return 1;
}

int PC88toMUC(FILE *file, FILE *out)
{
	char buffer[BUFFER_LEN];
	int read = 0, linenum = 0, pos = 0, header = 0;

	fprintf(out, "#mucom88 1.x\r\n#title MUC file\r\n#composer Unknown\r\n");
	fprintf(out, "#author Unknown\r\n#voice EDITME\r\n#pcm EDITME\r\n");
	fprintf(out, "#comment Exported from PC8801 with MUConverter\r\n\r\n");

	while (!feof(file))
	{
		read = fgetc(file);
		if(read != EOF)
		{
			switch(read)
			{
				case 0x0D: // Ignore
					break;
				case 0x0A: // Enter
					if(!header)
						MUCwriteline(buffer, pos, out);
					header = 0;
					pos = 0;
					break;
				case '\'':	// reset counter and copy data until enter
					linenum = atol(buffer);
					if(!linenum)
					{
						printf("Invalid PC-8801 BASIC format, aborting\n");
						printf("Line data: %s\n", buffer);
						return 0;
					}
					if(linenum == 1)
						header = 1;
					linenum = 0;
					pos = 0;
					break;
				default:
					if(pos < BUFFER_LEN)
						buffer[pos++] = read;
					else
					{
						printf("Invalid format or buffer length exceeded\nAborting");
						return 0;
					}
					break;
			}
		}
	}
	printf("Done. Don\'t forget to edit the #voice and #pcm lines to the file\n");
	return 1;
}

void ShowUsage()
{
	printf("Usage:\n\tMUConvert -pc88 <source> <target> [-withcomments]\n");
	printf("\tMUConvert -muc <source> <target>\n");
	printf("\t-pc88: Adds BASIC line numbers, removes Tabs and Comments by default\n");
	printf("\t-muc: Removes BASIC line numbers and file header from exported BASIC file\n");
	printf("\t-withcomments: In PC88 mode leaves all comments intact\n");
}

int main(int argc, char **argv)
{
	FILE *file, *out;
	int type = UNDEFINED, removecomments = 1;
	
	printf("MUC Windows <-> PC-88 MUCOM88 converter\n");
	printf("\tArtemio Urbina 2018 under GPL\n\tThanks to Yuzo Koshiro\n");

	if(argc >= 4)
	{
		if(!strcmp(argv[1], "-muc"))
			type = MUC;
		if(!strcmp(argv[1], "-pc88"))
			type = PC88;
	}
	if(argc == 5 && !strcmp(argv[4], "-withcomments"))
		removecomments = 0;

	if(type == UNDEFINED || argc < 4)
	{
		ShowUsage();
		return 0;
	}

	file = fopen(argv[2], "rb");
	if(!file)
	{
		printf("Could not open file %s\n", argv[2]);
		return 0;
	}
	out = fopen(argv[3], "wb");
	if(!out)
	{
		printf("Could not open file %s for writing\n", argv[3]);
		return 0;
	}

	printf("Converting %s to %s\n", argv[2], argv[3]);

	if(type == PC88)
		MUCtoPC88(file, out, removecomments);

	if(type == MUC)
		PC88toMUC(file, out);

	fclose(file);
	fclose(out);
	
	return 0;
}
