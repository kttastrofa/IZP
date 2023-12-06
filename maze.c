/**
 * @name xmeciak00
 * @file maze.c
 * @date 1.12.2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//direction
#define L   1 //left
#define R   2 //right
#define D   3 //down
#define U   4 //up

//borders
#define LB  1 //left
#define RB  2 //right
#define DB  4 //down
#define UB  4 //up

//triangle shape
#define upsidedown      ((r+c)%2==0)
#define downsidedown    ((r+c)%2!=0)

//from [triangle_side]
#define f_left          (pc<c) 
#define f_right         (pc>c) 
#define f_down          (pr>r)
#define f_up            (pr<r)

//to [triangle_side]
#define to_left         (c--)
#define to_right        (c++)
#define to_up           (r--)
#define to_down         (r++)

#define cell_index      ((r-1)*(map->cols)+(c-1))


/// @brief struct of maze
typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

/**
 * @brief here is "the window of function's store", here you are provided all of the used functions of this code
 * @details you can find details of particural functions on rows which numbers are listed next to the clarifications of them, on the right side (ex.: //123)
*/
void    initMap(Map *map, int rows, int cols);                                                          //143
int     readFILE(char *argv, Map *map);                                                                 //162
void    freeMap(Map *map);                                                                              //195
int     getCell(int r, int c, Map *map);                                                                //212
bool    isborder(Map *map, int r, int c, int border);                                                   //225
bool    bdrOK(int r, int c, Map *map);                                                                  //253
bool    fileISvalid(Map *map);                                                                          //309
void    printHelp();                                                                                    //327                         
int     start_border(Map *map, int r, int c, int leftright, char *previous);                            //348
int     clear_path(Map *map);                                                                           //486
int     on_my_way(Map *map, int r, int c, int leftright, char *previous, int direction);                //529
int     savepath(Map *map, int r, int c, int leftright, char *previous, int direction, char *path);     //571
void    shortest(char *lp, char *rp);                                                                   //610





///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////                                         ///////////////////////////////////
///////////////////////////////////              MAIN FUNCTION              /////////////////////////////////// 
///////////////////////////////////                                         ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    Map *map = malloc(sizeof(Map));
    initMap(map, 10, 10);
    char previous[]="0,0";

    
        if (argc<2 || argc>5)                               printf("Call for '--help'\n");
        else if (strcmp(argv[1], "--help")==0)              printHelp();
        else if (strcmp(argv[1], "--test")==0)
        {
            readFILE(argv[2], map);
            if (fileISvalid(map) == true)                   printf("Valid\n");
            else                                            printf("Invalid\n");
        }
        

        else if (strcmp(argv[1], "--help")!=0 || strcmp(argv[1], "--test")!=0)
        {
            int r = atoi(argv[2]), c = atoi(argv[3]);
            readFILE(argv[4], map);
            if (r<1 || r>map->rows || c<1 || c>map->cols)   printf("Call for '--help'\n");//needs to be a number, number usable in maze

            if (strcmp(argv[1], "--shortest")==0)
            {
                clear_path(map);
                printf("ano\n");

                char left_path[]="0,0";
                char right_path[]="0,0";
                
                savepath(map, r, c, L, previous, start_border(map, r, c, L, previous), left_path);
                savepath(map, r, c, R, previous, start_border(map, r, c, R, previous), right_path);
                shortest(left_path, right_path);
            }
            else if (strcmp(argv[1], "--lpath")==0)
            {
                on_my_way(map, r, c, L, previous, start_border(map, r, c, L, previous));
            }
            else if (strcmp(argv[1], "--rpath")==0)
            {
                on_my_way(map, r, c, R, previous, start_border(map, r, c, R, previous));  
            }
            else                                            printf("Call for '--help'\n");
        }
        
    
    freeMap(map);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////                                         ///////////////////////////////////
///////////////////////////////////            FUNCTION'S STORE             /////////////////////////////////// 
///////////////////////////////////                                         ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief initialisation of structures components (all of them will be reallocated during reading the map from a file)
/// @param map got temporary values
/// @param rows got temporary values
/// @param cols got space in memory of size of rows*cols*hhn
void initMap(Map *map, int rows, int cols) 
{
  map->rows = rows;
  map->cols = cols;
  map->cells = (unsigned char *)malloc((rows * cols + 1) * sizeof(unsigned char));

  if (map->cells == NULL) 
  {
    fprintf(stderr, "Error\n");
    //return 1;
  }
}



/// @brief reads map from a file, saves informations into structure
/// @param argv works with input arguments [name of a file we work with]
/// @param map pointer to structure we save informations into
/// @return 0 - nothing, could be void, indeed
int readFILE(char *argv, Map *map)
{
    FILE *file;
    file = fopen(argv, "r"); 

    int i=0;

    if (file == NULL)
	{
		fprintf(stderr, "Perror");
		                                                return 1;
	}

	fscanf(file, "%d", &map->rows);
	fscanf(file, "%d", &map->cols);

	int size = (map->rows* map->cols) +1;

	map->cells = malloc(size);

	while(fscanf(file, "%s", &map->cells[i]) == 1)
	{
		i++;
	}

	fclose(file);

                                                        return 0;
}


/// @brief frees the memory
/// @param map pointer to map structure we are freeing memory of
void freeMap(Map *map) 
{
  if (map->cells!=NULL)
  {
    free(map->cells);
    map->cells = NULL;
  }
  map->rows = 0;
  map->cols = 0;
}


/// @brief finds the value of specific cell [ex.: [5,2]=7 -> all borders; 7 is the value]
/// @param r row of the cell
/// @param c column of the cell
/// @param map pointer to struture
/// @return value of cell
int getCell(int r, int c, Map *map)
{
    int cell=map->cells[cell_index]-'0';
                                                        return cell;
}


/// @brief checks if the cell has a border the specific side
/// @param map pointer to structure of the map
/// @param r row of the cell
/// @param c column of the cell
/// @param border left, right or upsided or downsided
/// @return true if the border is there, false if there's none
bool isborder(Map *map, int r, int c, int border)
{
    int cell=getCell(r, c, map);

    if (border==LB && (cell==1 || cell==3 || cell==5 || cell==7))
    {
                                                        return true;
    }
    if (border==RB && (cell==2 || cell==3 || cell==6 || cell==7))
    {
                                                        return true;
    }
    if ((border==UB || border==DB) && (cell==4 || cell==5 || cell==6 || cell==7))
    {
                                                        return true;
    } 
    
                                                        return false;
    
}



/// @brief checks if the border of the cell fits with borders of cells it is surounded by
/// @param r row the cell
/// @param c column of the cell
/// @param map pointer to the structure fo the map
/// @return true if the border is OK, false if it is not
bool bdrOK(int r, int c, Map *map)
{
   if (c<map->cols && r>1 && r<map->rows)
   {
        if (upsidedown && (isborder(map, r, c, RB)!=isborder(map, r, c+1, LB) || isborder(map, r, c, UB)!=isborder(map, r-1, c, DB)))
        {
                                                        return false;
        }
        if (downsidedown && (isborder(map, r, c, RB)!=isborder(map, r, c+1, LB) || isborder(map, r, c, DB)!=isborder(map, r+1, c, UB)))
        {
                                                        return false;
        }
   }
   else if (c==map->cols && r<map->rows && r>1)
   {
        if (upsidedown && isborder(map, r, c, UB)!=isborder(map, r-1, c, DB))
        {
                                                        return false;
        }
        if (downsidedown && isborder(map, r, c, DB)!=isborder(map, r+1, c, UB))
        {
                                                        return false;
        }
   }
   else if (c<map->cols && r==1 && r<map->rows)
   {
        if (upsidedown && (isborder(map, r, c, RB)!=isborder(map, r, c+1, LB)))
        {
                                                        return false;
        }
        if (downsidedown && (isborder(map, r, c, RB)!=isborder(map, r, c+1, LB) || isborder(map, r, c, DB)!=isborder(map, r+1, c, UB)))
        {
                                                        return false;
        }
   }
   else if (c<map->cols && r>1 && r==map->rows)
   {
        if (upsidedown && (isborder(map, r, c, RB)!=isborder(map, r, c+1, LB) || isborder(map, r, c, UB)!=isborder(map, r-1, c, DB)))
        {
                                                        return false;
        }
        if (downsidedown && (isborder(map, r, c, RB)!=isborder(map, r, c+1, LB)))
        {
                                                        return false;
        }
   }
   
   
                                                        return true;
}



/// @brief checks if the file is valid, meaning if the borders fit (based on bdrOK) and if the rows and cols fit with the maze itself
/// @param map pointer to structure of the map
/// @return true if file is valid, false if it is not
bool fileISvalid(Map *map)
{
    if (map->rows==((int)strlen((const char*)map->cells)/map->cols) && map->cols==((int)strlen((const char*)map->cells)/map->rows))
    {
        for (int r=1; r<=map->rows; r++)
        {
            for (int c=1; c<=map->cols; c++)
            {
                if (bdrOK(r, c, map)==0)                return false;
            }
        }
                                                        return true;
    }
                                                        return false;
}


/// @brief prints brief help instructions
void printHelp() 
{
        printf("\n\nHere are some arguments, you could use:\n\n\n"
        "\'./maze --help                             explains which arguments you can use\n\n"
        "\'./maze --test filename.txt                will test if your file is valid for this programm or not\n\n"
        "\'./maze --shortest R C filename.txt        find the shortest path trough your maze and write the coordinates of each cell in the path\n\n"
        "\'./maze --lpath R C filename.txt           find the path trough your maze by lefthand logic and write the coordinates of each cell in the path\n\n"
        "\'./maze --rpath R C filename.txt           find the path trough your maze by the righthand logic and write the coordinates of each cell in the path\n\n"
        "\n* R C stands for starting coordinates, R as row, C as column"
        "\n** filename.txt stands for your file with values of your maze\n\n");
}



/// @brief based on hand logic navigates with which border we should continue
/// @param map pointer to structure of maze
/// @param r row of the cell we start/are on
/// @param c column of the cell
/// @param leftright tells with which logic we work
/// @param previous tells where we were before
/// @return direction - side of the border we continue by
int start_border(Map *map, int r, int c, int leftright, char *previous)
{
    int pr=previous[0]-'0';
    int pc=previous[2]-'0';

    if (strcmp(previous, "0,0")==0)
    {
        if (c==1 && isborder(map, r, c, LB)==false)
        {
            previous[0]=r+'0';
        }
        else if (c==map->cols && isborder(map, r, c, RB)==false)
        {
            previous[2]=map->cols+1+'0';
            previous[0]=r+'0';
        }
        else if (r==1 && downsidedown && isborder(map, r, c, DB)==false)
        {
            previous[2]=c+'0';
        }
        else if (r==map->rows && upsidedown && isborder(map, r, c, UB)==false)
        {
            previous[0]=map->rows+1+'0';
            previous[2]=c+'0';
        }
        else 
        {
            printf("Invalid input, try call for '--help'\n");
        }

        pr=previous[0]-'0';
        pc=previous[2]-'0';
    }
    
    
    previous[0]= r+'0';
    previous[2]= c+'0';

    
    if (leftright==L)//lefthand logic
    {
        if (f_left && upsidedown)//from left side, upside down
        {
            //up
            if (isborder(map, r, c, UB)==false)         return U;
            else if (isborder(map, r, c, RB)==false)    return R;
            else                                        return L;
        }
        else if (f_left && downsidedown)//from left side, downside down
        {
            //to right
            if (isborder(map, r, c, RB)==false)         return R;
            else if (isborder(map, r, c, DB)==false)    return D;
            else                                        return L;
        }
        else if (f_right && upsidedown)//from right side, upside down
        {
            //to left
            if(isborder(map, r, c, LB)==false)          return L;
            else if (isborder(map, r, c, UB)==false)    return U;
            else                                        return R;
        }
        else if (f_right && downsidedown)//from right side, downside down
        {
            //down
            if (isborder(map, r, c, DB)==false)         return D;
            else if (isborder(map, r, c, LB)==false)    return L;
            else                                        return R;
        }
        else if (f_up && upsidedown)//from upside
        {
            //to right
            if (isborder(map, r, c, RB)==false)         return R;
            else if (isborder(map, r, c, LB)==false)    return L;   
            else                                        return U;
        }  
        else if (f_down && downsidedown)//from downside  
        {
            //to left
            if(isborder(map, r, c, LB)==false)          return L;
            else if (isborder(map, r, c, RB)==false)    return R;
            else                                        return D;
        }
    }
    else if (leftright==R)//righthand logic
    {
        if (f_left && upsidedown)//from left side, upside down
        {
            //to right
            if (isborder(map, r, c, RB)==false)         return R;
            else if (isborder(map, r, c, UB)==false)    return U;
            else                                        return L;
        }
        else if ((f_left && downsidedown))//from left side, downside down
        {
            //to left
            if (isborder(map, r, c, DB)==false)         return D;
            else if (isborder(map, r, c, RB)==false)    return R;
            else                                        return L;
        }
        else if (f_right && upsidedown)//from right side, upside down
        {
            //up
            if (isborder(map, r, c, UB)==false)         return U;
            else if (isborder(map, r, c, LB)==false)    return L;
            else                                        return R;
        }
        else if (f_right && downsidedown)//from right side, downside down
        {
            //to left
            if(isborder(map, r, c, LB)==false)          return L;
            else if (isborder(map, r, c, DB)==false)    return D;
            else                                        return R;
        }
        else if (f_up && upsidedown)//from upside
        {
            //to left
            if(isborder(map, r, c, LB)==false)          return L;
            else if (isborder(map, r, c, RB)==false)    return R;
            else                                        return U;
        }  
        else if (f_down && downsidedown)//from downside 
        {
            //to right
            if(isborder(map, r, c, RB)==false)          return R;
            else if (isborder(map, r, c, LB)==false)    return L;
            else                                        return D;
        }
    }

                                                        return 0;
}



/// @brief clears the path, meaning 
/// @param map pointer to the structure of map
/// @return should have checked if the for end where i want (beacuse it did not). before, it was void
int clear_path(Map *map)
{
    for (int r=1; r<=map->rows; r++)
    {
        for (int c=1; c<=map->cols; c++)
        {
            if (isborder(map, r, c, LB)==true && isborder(map, r, c, RB)==true) 
            {
                map->cells[cell_index]=7+48;
                if (upsidedown) r=r-1;
                else            r=r+1;
                if (isborder(map, r, c, UB)==false) map->cells[cell_index]=getCell(r, c, map)+48+UB;
            }
            if ((isborder(map, r, c, LB)==true && isborder(map, r, c, UB)==true) || (isborder(map, r, c, LB)==true && isborder(map, r, c, DB)==true))
            {
                map->cells[cell_index]=7+48;
                c=c+1;
                if (isborder(map, r, c, RB)==false) map->cells[cell_index]=getCell(r, c, map)+48+R;
            }
            if ((isborder(map, r, c, RB)==true && isborder(map, r, c, UB)==true) || (isborder(map, r, c, RB)==true && isborder(map, r, c, DB)==true))
            {
                map->cells[cell_index]=7+48;
                c=c-1;
                if (isborder(map, r, c, LB)==false) map->cells[cell_index]=getCell(r, c, map)+48+L;
            } 
        }
        if (r==map->rows) return 0;
    }
    return 1;
    
}




/// @brief prints the coordinates of the cell we are on, moves coordinates to the next cell based od start_border
/// @param map pointer to the structure of the map
/// @param r row of the cell
/// @param c column of the cell
/// @param leftright logic we use (lefthanded/righthanded)
/// @param previous cell we were on
/// @param direction border we move trough
/// @return 0 if it goes as it should, 1 if not
int on_my_way(Map *map, int r, int c, int leftright, char *previous, int direction)
{

    if ((r > map->rows) || (c > map->cols) || (r < 1) || (c < 1))           return 0;
    printf("%d,%d\n", r, c);


    if (direction == D)         
    {
        r++;
        on_my_way(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous));
    }
    if (direction == U)         
    {
        r--;
        on_my_way(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous));
    }
    if (direction == L)         
    {
        c--;
        on_my_way(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous));
    }
    if (direction == R)         
    {
        c++;
        on_my_way(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous));
    }
    
    return 1;
}



/// @brief used only by shortest - saves the path to char
/// @param map pointer to the structure of the map
/// @param r row of the cell we are on
/// @param c column of the cell we are on
/// @param leftright logic we use (lefthanded/righthanded)
/// @param previous cell we were on
/// @param direction border we move trough
/// @param path char we save the coordinates of the path into
/// @return 0 if it goes as it should, 1 if not
int savepath(Map *map, int r, int c, int leftright, char *previous, int direction, char *path)
{
    
    if ((r > map->rows) || (c > map->cols) || (r < 1) || (c < 1))           return 0;
    
    strcat(path, previous);
    path[strlen(path)+1] ='\n';
    path[strlen(path)+1] ='\0';


    if (direction == D)         
    {
        r++;
        savepath(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous), path);
    }
    if (direction == U)         
    {
        r--;
        savepath(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous), path);
    }
    if (direction == L)         
    {
        c--;
        savepath(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous), path);
    }
    if (direction == R)         
    {
        c++;
        savepath(map, r, c, leftright, previous, start_border(map, r, c, leftright, previous), path);
    }
    
    return 1;
}



/// @brief checks which path was shorter and prints the shortest one
/// @param lp path of lefthanded logic
/// @param rp path of righthanded logic
void shortest(char *lp, char *rp)
{
    if (sizeof(lp)<sizeof(rp))  printf("%s", lp);
    else                        printf("%s", rp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////                                         ///////////////////////////////////
///////////////////////////////////                 THE END                 /////////////////////////////////// 
///////////////////////////////////                                         ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////