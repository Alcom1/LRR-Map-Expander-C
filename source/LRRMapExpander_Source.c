//Program by Alcom Isst.

//This program is intended to read the 9 different .map files used in level maps for LEGO Rock Raiders, and increase the size of the map borders by that amount.
//This program opperates under the following steps:
//  1:  Request user input for size increase.
//  2:  Request user input for use of HeightMatch.
//  3:  Check for and read Dugg.map header.
//  4:  Write new Dugg.map header with increased width and height.
//  5:  Read Dugg.map terrain, and write the increased size version into Dugg.map.
//  6:  Repeat Steps 4 and 5 for Cror, Emrg, Erod, Fall, High, Path, Surf, and Tuto.map files. Header for additional maps is copied from Dugg.map.

//This program has two purposes:
//  1:  To enable level developers to add larger borders to levels for better camera movement at map edges.
//  2:  To enable level developers to expand their map for additions to the map.

//Notes:
//  1:  Accommodated. All .map files except High.map and Surf.map have int 0 or char {0,0} as their default contents.
//      High.map has int 2048 or char {8,0} as the default. Surf.map has int 256 or char {1,0} as the default.
//  2:  Accommodated. All .map files are one integer wider and taller than are shown ingame.
//      The right most column and the bottom row do not appear ingame.
//      These columns are still needed by the method that LEGO Rock Raiders reads .map files.

#include <stdio.h>
#include <math.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Structure of .map files.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct MAP          // The structure of .map file headers.
{
    char MAP[4];            // 'MAP ' indicating that this is a .map file,
    char tag[2];            // Tag unique to each batch of .map files.
                            // (REMOVED) Block of void characters.
    char x;                 // width of map.
                            // (REMOVED) Block of void characters.
    char y;                 // Height of map.
                            // (REMOVED) Block of void characters.
                            // NOTE: Left-most column and bottom row of .map files is ignored by the game.
} complex;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Call functions for additional .map files and to check map files for legitamacy.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Check_MAP(char check[]);

int coor_convert(char num[], int l, int i);

int Read_Cror(int x, int y, int i, complex str);
int Read_Emrg(int x, int y, int i, complex str);
int Read_Erod(int x, int y, int i, complex str);
int Read_Fall(int x, int y, int i, complex str);
int Read_High(int x, int y, int i, complex str);
int Read_High_match(int x, int y, int i, complex str);
int Read_Path(int x, int y, int i, complex str);
int Read_Surf(int x, int y, int i, complex str);
int Read_Tuto(int x, int y, int i, complex str);

int Read_Object(int i);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Main function.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int main()
{

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// FILE objects.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    FILE* ROCK;
    FILE* ROCK2;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Variables.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    complex str;

    int c1, c2, x, y;
    //  c1: Counter dimension 1
    //  c2: Counter dimension 2
    //  x:  width of map. Actual map width is x-1 for some reason.
    //  y:  height of map. Actual map height is y-1 for some reason.
    int i;
    //  i:  (USER INPUT) width and height to be added to the map.
    //      I'm sorry for not making width and height separate.
    //      I will separate them once I stop stressing out over if IGM will accept me.
    char r;
    //  r:  Character used for reading and writing each char in files.
    char h;
    //  h:  (USER INPUT) boolean state for if HeightMatch is used.

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//  Open Dugg.map.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    ROCK = fopen("Input\\Dugg.map", "rb");          // Opens original .map file.

    if(ROCK)                                        // Checks if .map file exists.
    {
        printf("Dugg.map file found!\n");
    }
    else
    {
        printf("No Dugg.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Dugg.map", "wb");        // Opens new .map file.

    if(!ROCK2)                                      // Checks for output folder.
    {
        printf("No Output folder found!\n");
        return 0;
    }

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Handle member MAP.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    for(c1 = 0; c1 < 4; c1++)                       // Reads "MAP " section of file header.
    {
        str.MAP[c1] = fgetc(ROCK);
    }

    if(Check_MAP(str.MAP))                          // Checks if .map file is valid.
    {
        fputc(str.MAP[0], ROCK2);                   // NOTE: Chains of fputc are used instead of fputs.
        fputc(str.MAP[1], ROCK2);                   //       This was done to accommodate existing and lacking void characters.
        fputc(str.MAP[2], ROCK2);
        fputc(str.MAP[3], ROCK2);
    }
    else
    {
        printf("Dugg.map is not a map file!\n");
        return 0;
    }
    printf("\n");

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// User Input.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    printf("Enter integer to expand borders by (limit: 1 - 20): ");
    scanf("%d", &i);
    if(i < 1 || i > 20)
    {
        printf("\n");
        printf("Invalid entry! Setting integer to default of 4.");
        printf("\n");
        i = 4;
    }
    printf("\n");

    printf("Use HeightMatch? (y/n): ");
    scanf(" %c", &h);
    printf("\n");
    if(h == 'y' || h == 'Y' || h == '1')
    {
        printf("HeightMatch enabled!");
        h = 1;
    }
    else if(h == 'n' || h == 'N' || h == '0')
    {
        printf("HeightMatch disabled!");
        h = 0;
    }
    else
    {
        printf("Invalid entry! Disabling HeightMatch.");
        h = 0;
    }
    printf("\n");
    printf("\n");

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Handle members header, x, y, space1, and space2.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    for(c2 = 0; c2 < 3; c2++)
    {
        for(c1 = 0; c1 < 4; c1++)
        {
            r = fgetc(ROCK);
            if(c1 == 0)
            {
                if(c2 == 0)                 // Reads first char of header.
                {
                    str.tag[0] = r;
                }
                if(c2 == 1)                 // Reads width char and writes new width to str.
                {
                    x = r;
                    str.x = x + 2 * i;
                    r = str.x;
                }
                if(c2 == 2)                 // Reads height char and writes new height to str.
                {
                    y = r;
                    str.y = y + 2 * i;
                    r = str.y;
                }
            }
            if(c1 == 1)
            {
                if(c2 == 0)
                {
                    str.tag[1] = 4;         // Reads second char of header.
                }
            }
        }
    }
    printf("Old width:\t%d\n", x);          // Displays of .map file dimensions.
    printf("Old height:\t%d\n", y);
    printf("New width:\t%d\n", str.x);
    printf("New height:\t%d\n", str.y);
//  printf("Tag: [%d][%d]\n", str.tag[0], str.tag[1]); // Unused tag viewer. Works and is interesting.
    printf("\n");

    fputc(str.tag[0], ROCK2);               // Put full file header in output Dugg.map.
    fputc(str.tag[1], ROCK2);               // NOTE: Chains of fputc are used instead of fputs.
    fputc(0, ROCK2);                        //       This was done to accommodate existing and lacking void characters.
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Handles Terrain member characters. Doesn't actually use Terrain member.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(0, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Functions for other
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    Read_Cror(x, y, i, str);
    Read_Emrg(x, y, i, str);
    Read_Erod(x, y, i, str);
    Read_Fall(x, y, i, str);
    if (h)
    {
        Read_High_match(x, y, i, str);
    }
    else
    {
        Read_High(x, y, i, str);
    }
    Read_Path(x, y, i, str);
    Read_Surf(x, y, i, str);
    Read_Tuto(x, y, i, str);
    printf("\n");
    Read_Object(i);

    printf("\n");
    system("pause");
    return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Function for identifying files as legitimate .map files.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Check_MAP(char check[])
{

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Variables.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    int c1;
    char sMc[5] = {'M','A','P',' '};
    //  c1:     Counter
    //  sMc:    Comparison string.

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Checking Comparison.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    for(c1 = 0; c1 < 4; c1++)
    {
        if(check[c1] != sMc[c1])
        {
            printf("Not a map file.");
            return 0;
        }
    }

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Return true.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    return 1;

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Function for modifying coordinates in ObjectList.ol.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int coor_convert(char num[], int l, int i)
{
    int c1, n = 0;
    // n: Neumerical value of coordinate being modified.

    for(c1 = 0; c1 <= l; c1++)                                  // for loop converting coordinates into an integer.
    {
        n = n + (num[l - c1] - '0') * (int)round(pow(10,c1));
    }

    if(((float)(n + i) / 10 > 1 && (float)n / 10 < 1) || ((float)(n + i) / 100 > 1 && (float)n / 100 < 1))    // Incase number of digits in coordinates increases.
    {
        l = l + 1;
    }

    n = n + i;  // changes coordinate to move object.

    for(c1 = 0; c1 <= l; c1++)                                  // for loop converting integer back into coordinates.
    {
        num[c1] = (n - n % (int)round(pow(10,l - c1))) / (int)round(pow(10,l - c1));
        n = n - num[c1] * (int)round(pow(10,l - c1));
    }
    return l;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Cror.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Cror(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\Cror.map", "rb");
    if(ROCK)
    {
        printf("Cror.map file found!\n");
    }
    else
    {
        printf("No Cror.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Cror.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(0, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Emrg.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Emrg(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\Emrg.map", "rb");
    if(ROCK)
    {
        printf("Emrg.map file found!\n");
    }
    else
    {
        printf("No Emrg.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Emrg.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(0, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Erod.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Erod(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\Erod.map", "rb");
    if(ROCK)
    {
        printf("Erod.map file found!\n");
    }
    else
    {
        printf("No Erod.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Erod.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(0, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Fall.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Fall(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\Fall.map", "rb");
    if(ROCK)
    {
        printf("Fall.map file found!\n");
    }
    else
    {
        printf("No Fall.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Fall.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(0, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read High.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_High(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\High.map", "rb");
    if(ROCK)
    {
        printf("High.map file found!\n");
    }
    else
    {
        printf("No High.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\High.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(8, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(8, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(8, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(8, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(8, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read High.map file with HeightMatch.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_High_match(int x, int y, int i, complex str)
{

    int c1, c2, c3 = 0;
    char r, b1[str.x * str.y * 2], b2[str.x * 2][str.y];

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\High.map", "rb");
    if(ROCK)
    {
        printf("High.map file found!\n");
    }
    else
    {
        printf("No High.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\High.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            b1[c3] = 8;
            c3++;
            b1[c3] = 0;
            c3++;
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            b1[c3] = 8;
            c3++;
            b1[c3] = 0;
            c3++;
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            b1[c3] = r;
            c3++;
            r = fgetc(ROCK);
            b1[c3] = r;
            c3++;
        }
//                                      // Neglects right excess and adds void.
        b1[c3] = 8;
        c3++;
        r = fgetc(ROCK);
        b1[c3] = 0;
        c3++;
        r = fgetc(ROCK);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            b1[c3] = 8;
            c3++;
            b1[c3] = 0;
            c3++;
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            b1[c3] = 8;
            c3++;
            b1[c3] = 0;
            c3++;
        }
    }

    c3 = 0;
    for(c2 = 0; c2 < str.y; c2++)               // Stores array inside a proper map-sized matrix.
    {
        for(c1 = 0; c1 < str.x * 2; c1++)
        {
            b2[c1][c2] = b1[c3];
            c3++;
        }
    }

    for(c2 = 0; c2 < i; c2++)                   // HeightMatch top section.
    {
        for(c1 = 0; c1 < (x + i) * 2; c1++)
        {
            b2[c1][c2] = b2[c1][i];
        }
    }

    for(c2 = y - 1 + i; c2 < str.y - 1; c2++)   // HeightMatch bottom section.
    {
        for(c1 = i; c1 < (x + i) * 2; c1++)
        {
            b2[c1][c2] = b2[c1][y - 2 + i];
        }
    }

    for(c2 = 0; c2 < str.y - 1; c2++)           // HeightMatch left section.
    {
        for(c1 = 0; c1 < i * 2; c1++)
        {
            b2[c1][c2] = b2[i * 2][c2];
            c1++;
            b2[c1][c2] = b2[i * 2 + 1][c2];
        }
    }

    for(c2 = 0; c2 < str.y - 1; c2++)           // HeightMatch right section.
    {
        for(c1 = (x - 1 + i) * 2; c1 < (str.x - 1) * 2; c1++)
        {
            b2[c1][c2] = b2[(x - 2 + i) * 2][c2];
            c1++;
            b2[c1][c2] = b2[(x - 2 + i) * 2 + 1][c2];
        }
    }

    for(c2 = 0; c2 < str.y; c2++)               // Sends matrix to high.map file.
    {
        for(c1 = 0; c1 < str.x * 2; c1++)
        {
            fputc(b2[c1][c2], ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Path.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Path(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\Path.map", "rb");
    if(ROCK)
    {
        printf("Path.map file found!\n");
    }
    else
    {
        printf("No Path.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Path.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(0, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Surf.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Surf(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\Surf.map", "rb");
    if(ROCK)
    {
        printf("Surf.map file found!\n");
    }
    else
    {
        printf("No Surf.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Surf.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(1, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(1, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(1, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(1, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(1, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Tuto.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Tuto(int x, int y, int i, complex str)
{

    int c1, c2;
    char r;

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\Tuto.map", "rb");
    if(ROCK)
    {
        printf("Tuto.map file found!\n");
    }
    else
    {
        printf("No Tuto.map file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\Tuto.map", "wb");

    fputc(str.MAP[0], ROCK2);
    fputc(str.MAP[1], ROCK2);
    fputc(str.MAP[2], ROCK2);
    fputc(str.MAP[3], ROCK2);
    fputc(str.tag[0], ROCK2);
    fputc(str.tag[1], ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.x, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(str.y, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);
    fputc(0, ROCK2);

    for(c1 = 0; c1 < 16; c1++)
    {
        r = fgetc(ROCK);
    }

    for(c2 = 0; c2 < i; c2++)           // Adds upper block of void characters.
    {
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < y - 1; c2++)       // For loop, looping each row added.
    {
        for(c1 = 0; c1 < i; c1++)       // Adds left borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
        for(c1 = 0; c1 < x - 1; c1++)   // Copies and adds center content.
        {
            r = fgetc(ROCK);
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            fputc(r, ROCK2);
        }
//                                      // Neglects right excess and adds void.
        r = fgetc(ROCK);
        fputc(0, ROCK2);
        r = fgetc(ROCK);
        fputc(0, ROCK2);
//
        for(c1 = 0; c1 < i; c1++)       // Adds right borders.
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    for(c2 = 0; c2 < i + 1; c2++)       // Adds lower bock of void characters.
    {                                   // NOTE: i + 1 is used as bottom excess is neglected.
        for(c1 = 0; c1 < str.x; c1++)
        {
            fputc(0, ROCK2);
            fputc(0, ROCK2);
        }
    }

    fclose(ROCK);
    fclose(ROCK2);

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Read Tuto.map file.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int Read_Object(int i)
{
    int c1, c1s;
    //  c1s: Save for c1s. Serves the same role as c1 in coor_convert function.
    char r, num[3];
    //  num: Array containing coordinates of object to be manipulated.

    FILE* ROCK;
    FILE* ROCK2;

    ROCK = fopen("Input\\ObjectList.ol", "rb");
    if(ROCK)
    {
        printf("ObjectList.ol file found!\n");
    }
    else
    {
        printf("No ObjectList.ol file found!\n");
        return 0;
    }
    ROCK2 = fopen("Output\\ObjectList.ol", "wb");

    r = fgetc(ROCK);
    while (r != -1)
    {
        fputc(r, ROCK2);
        r = fgetc(ROCK);
        if(r == 'x' || r == 'y')            // If chain detecting coordinate entry.
        {
            fputc(r, ROCK2);
            r = fgetc(ROCK);
            if(r == 'P')
            {
                fputc(r, ROCK2);
                r = fgetc(ROCK);
                if(r == 'o')
                {
                    fputc(r, ROCK2);
                    r = fgetc(ROCK);
                    if(r == 's')
                    {
                        while (r != -1)     // Loop incase of errant white space.
                        {
                            fputc(r, ROCK2);
                            r = fgetc(ROCK);
                            if (r >= '0' && r <= '9')
                            {
                                break;
                            }
                        }
                        for(c1 = 0; c1 < 3; c1++)
                        {
                            num[c1] = r;
                            r = fgetc(ROCK);
                            if (!(r >= '0' && r <= '9'))
                            {
                                break;
                            }
                        }

                        c1s = coor_convert(num, c1, i);
                        for(c1 = 0; c1 <= c1s; c1++)
                        {
                            fputc(num[c1] + '0', ROCK2);
                        }
                    }
                }
            }
        }
    }

    fclose(ROCK);
    fclose(ROCK2);
}
