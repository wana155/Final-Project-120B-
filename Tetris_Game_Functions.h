
//==================TETRIS GAME FUNCTIONS==============///
const uint8_t Xlength=7;                        // Lenght of a single square
const uint8_t Ylength=6;                        // Width of the single square
enum ShapeType{T,RevL,Z,RevZ,L,I,Square}Shpe;   // Type definition for different shapes

unsigned char Tetris_min(unsigned char a,unsigned char b)  // Calculates the minimun of two numbbers
{return (a<b)? a:b;}

struct Shape        // Definition of a shape
{
    uint8_t type;   // Stores the type
	uint8_t face;   // Stores the current face
	int8_t Row;     // Stores the current row
	uint8_t Colunm; // Stores current colunm
	uint8_t colums_used; // Colunms used in the 4x4 array by the current shape
	unsigned char shapeA[4][4]; // Array that stores the squares in the shape
};
struct Shape current;
unsigned char vertical[8][12];
unsigned char RCount;
unsigned char speedLevel=30; 

/*  This function checks whether is safe to move a shape to the
 right: if there is enough space available along the shape's squares,
 returns 1, else return 0.
 */
uint8_t Tetris_safe_R_Move(uint8_t col, uint8_t row)
{ uint8_t safe=1;
	for (int j=0; j<4 ;j++)
	{ if (current.shapeA[0][j]==0 && current.shapeA[1][j]==1)
		{ if (vertical[col][j+row]==1)
			 {safe=0;}
		}
	  if(current.shapeA[0][j]==1)
	  {  if (vertical[col-1][j+row]==1)
		     {safe=0;}
	  }
	}
	return safe;
}
/*
 This function checks whether is safe to move a shape to the left: if
 there is enough space available along the shape's squares, returns 1,
 else return 0.
 */
uint8_t Tetris_safe_L_Move(uint8_t col, uint8_t row)
{ uint8_t safe=1;
	for (int j=0; j<4 ;j++)
	{ if (current.shapeA[current.colums_used-1][j]==0 && current.shapeA[current.colums_used-2][j]==1)
		{ if (vertical[col+current.colums_used-1][j+row]==1)
			{safe=0;}
		}
		if(current.shapeA[current.colums_used-1][j]==1)
		{  if (vertical[col+current.colums_used][j+row]==1)
			{safe=0;}
		}
	}
	return safe;
}
/*  This function is called whenever a shape has been placed to check if
 any row (line from left to right in the game screen) has been completed.
 If so, the function calls EraseRow to delete the row.
 */
void Tetris_check(){
	RCount=0;
		for (int j=11;j>=2;j--)
		{ for (int i=0;i<8;i++)
			{ if (vertical[i][j])
				{RCount++;}
			}
			if (RCount==8)
			{ EraseRow(j);
				j=j+1;
			}
			RCount=0;
		}
}
unsigned char score;            // Keeps the score of the game
unsigned char level;            // Keeps the level of the game

/*  EraseRow function deletes a row that is completed during game play.
 It also increases the level and updates the drop-down speed of the game.
 */
void EraseRow(unsigned char R)
{
	for (int i=0;i<8;i++)
	{ for (int k=R;k>=2;k--)
		{vertical[i][k]=vertical[i][k-1];}
	}	
	level++;
	score=score+5;
	
	if (level>6)
	{speedLevel=speedLevel-1;}
	else{speedLevel=speedLevel-4;}
		
}
uint8_t prev=0;
/*  This function creates a new shape and stores its values in current.
 It uses a randomizer function to select a new shape. It then calls changeFace
 function to fill in the shape array with it correponding values.
 */
void Tetris_makeShape(uint8_t param)      
{
	uint8_t RandomNum = rand() % param; 
	 while (prev==RandomNum)
	 { RandomNum=rand()%param; }
	prev=RandomNum;  
	current.type=RandomNum;
	current.face=1;
	current.Row=-3;
	{current.Colunm=3;}
	changeFace(current.type,current.face);
	
}
/*  This function counts the number of occupied squares for a specific column c.
 */
unsigned char Tetris_Bottom_up(unsigned char c){

	for(int i=0;i<12;i++)
	{if (vertical[c][i])
		{ if (i<=current.Row)
			{  if ((current.type==L&&current.face==1)||(current.type==RevL&&current.face==1)) 
					{i=current.Row+1;}
				else{i=current.Row;}
				while (vertical[c][i]==1)
				{i++;}
			   while(vertical[c][i]==0 && i<12)
			   {i++;}
				return (12-i);
				break;
			}
		else{return (12-i);}
		break;}
	}
	return 0;
}
/*  This function counts the number of occupied squares in the shape array.
 */
unsigned char used(unsigned char c){  
	for(int i=3;i>=0;i--)
	{if (current.shapeA[c][i])
		{return (i+1);
		break;}
	}
	return 0;
}
/*  This function draws a customized square for the game shapes
 */
void Tetris_Draw_A_Sqaure(uint8_t x, uint8_t y)
{
	for (int i=x+1;i<x+6+1;i++)
	for (int j=y;j<y+5;j++)
	{{nokia_lcd_set_pixel(i,j,1);}}
}
/* This function draws a customized screen saver for the idle times
 on the nokia screen. For instance when the player losses the game
 or is about to begin a new game.
 */
void Tetris_screen_saver()
{
		Tetris_erase_shape();
   //Draw chess-board like pattern at the top of the screen//
	for (int i=0;i<8;i++)
	{for(int j=0;j<4;j++)
		{ vertical[i][j]=0;
			if ((i%2)==0)
			{if ((j%2)==0)
				{vertical[i][j]=1;}
			}
			else{if (((j+1)%2)==0)
				{vertical[i][j]=1;}
			}}}
    //Draw tree random shapes at the middle  of the screen//
	Tetris_makeShape(5);
	if (current.type==Z||current.type==RevZ)
	{changeFace(current.type,2);}
	if (current.type==L)
	{changeFace(current.type,3);}
	current.Colunm=0;
	current.Row=5;
	for (int i=0;i<current.colums_used;i++)
	{ for (int j=0;j<4;j++)
	{if (current.shapeA[i][j])
	{vertical[i+current.Colunm][j+current.Row]=1;}}}
	Tetris_makeShape(5);
	if (current.type==Z||current.type==RevZ)
	{changeFace(current.type,2);}
	if (current.type==L)
	{changeFace(current.type,3);}
	current.Colunm=3;
	current.Row=5;
	for (int i=0;i<current.colums_used;i++)
	{ for (int j=0;j<4;j++)
	{if (current.shapeA[i][j])
	{vertical[i+current.Colunm][j+current.Row]=1;}}}
	Tetris_makeShape(5);
	if (current.type==Z||current.type==RevZ)
	{changeFace(current.type,2);}
	current.Colunm=6;
	current.Row=5;
	for (int i=0;i<current.colums_used;i++)
	{ for (int j=0;j<4;j++)
	{if (current.shapeA[i][j])
	{vertical[i+current.Colunm][j+current.Row]=1;}}}
	//Draw chess-board like pattern at the bottom of the screen//
	for (int i=0;i<8;i++)
		{for(int j=9;j<12;j++)
			{vertical[i][j]=0;
				 if ((i%2)==0)
				{if ((j%2)==0)
					{vertical[i][j]=1;}
				}
				else{if (((j+1)%2)==0)
					{vertical[i][j]=1;}
					}}}						
}
/* This function deletes the current shape on the array
 */
void Tetris_erase_shape()
{
		for (int i=0;i<4;i++)
		{ for (int j=0;j<4;j++)
			{ current.shapeA[i][j]=0;}}
}
/* This function replaces the current shape in the array for
 a rotated version.
 */
void changeFace(uint8_t type,uint8_t face)
{
	if (type==Square)
	{ if(face==1||face==2||face==3||face==4)
		{   current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[0][1]=1;
			current.shapeA[1][1]=1;
			current.shapeA[1][0]=1;
		}
}
	if (type==I)
	{ if(face==1||face==3)
		{ 	current.colums_used=1;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[0][1]=1;
			current.shapeA[0][2]=1;
			current.shapeA[0][3]=1;
	}
	if (face==2||face==4)
	{
			if (!Tetris_safe_L_Move(current.Colunm+3,current.Row)||!Tetris_safe_L_Move(current.Colunm+2,current.Row)||!Tetris_safe_L_Move(current.Colunm,current.Row)||!Tetris_safe_L_Move(current.Colunm+1,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{current.face=1;}
			
			else{
				if (current.Colunm>4){current.Colunm=4;}
				current.colums_used=4;
				Tetris_erase_shape();
				current.shapeA[0][0]=1;
				current.shapeA[1][0]=1;
				current.shapeA[2][0]=1;
				current.shapeA[3][0]=1;
		}
	}
}	
	
	if (type==T)
	{ if(face==1)
		{   current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[0][1]=1;
			current.shapeA[0][2]=1;
			current.shapeA[1][1]=1;
		}
		if (face==2)
		{ 	if (!Tetris_safe_L_Move(current.Colunm,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				{current.Colunm-=1;
			if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][1]=1;
			current.shapeA[1][1]=1;
			current.shapeA[2][1]=1;
			current.shapeA[1][0]=1;}
				else{current.face=1;}
			}
			else{if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][1]=1;
			current.shapeA[1][1]=1;
			current.shapeA[2][1]=1;
			current.shapeA[1][0]=1;
		}
		}
		if (face==3)
		{	current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[0][1]=1;
			current.shapeA[1][0]=1;
			current.shapeA[1][1]=1;
			current.shapeA[1][2]=1;
		}
		if (face==4)
		{		if (!Tetris_safe_L_Move(current.Colunm,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				{current.Colunm-=1;
			if (current.Colunm==6){current.Colunm=5;}
				current.colums_used=3;
				Tetris_erase_shape();
				current.shapeA[0][0]=1;
				current.shapeA[1][0]=1;
				current.shapeA[2][0]=1;
				current.shapeA[1][1]=1;}
				else{current.face=3;}
			}
			else{if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[1][0]=1;
			current.shapeA[2][0]=1;
			current.shapeA[1][1]=1;
		}
		}
		
	}
	if (type==L)
	{ if(face==1)
		{   current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[1][0]=1;
			current.shapeA[1][1]=1;
			current.shapeA[1][2]=1;
			current.shapeA[0][2]=1;
		} 
		if (face==2)
		{		if (!Tetris_safe_L_Move(current.Colunm,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				{current.Colunm-=1;
					if (current.Colunm==6){current.Colunm=5;}
				current.colums_used=3;
				Tetris_erase_shape();
				current.shapeA[0][1]=1;
				current.shapeA[1][1]=1;
				current.shapeA[2][1]=1;
				current.shapeA[0][0]=1;}
				else{current.face=1;}
			}
			else{if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][1]=1;
			current.shapeA[1][1]=1;
			current.shapeA[2][1]=1;
			current.shapeA[0][0]=1;
		}
		}
		if (face==3)
		{	current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[1][0]=1;
			current.shapeA[0][0]=1;
			current.shapeA[0][1]=1;
			current.shapeA[0][2]=1;
		}
		if (face==4)
		{	if (!Tetris_safe_L_Move(current.Colunm,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				{current.Colunm-=1;
				if (current.Colunm==6){current.Colunm=5;}
				current.colums_used=3;
				Tetris_erase_shape();
				current.shapeA[0][0]=1;
				current.shapeA[1][0]=1;
				current.shapeA[2][0]=1;
				current.shapeA[2][1]=1;}
				else{current.face=3;}
			}
			else{if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[1][0]=1;
			current.shapeA[2][0]=1;
			current.shapeA[2][1]=1;
		}
		}
		
	}	
	if (type==RevL)
	{ if(face==1)
		{   current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[0][1]=1;
			current.shapeA[0][2]=1;
			current.shapeA[1][2]=1;
		} 
		if (face==2)
		{	if (!Tetris_safe_L_Move(current.Colunm,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				{current.Colunm-=1;
			if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][1]=1;
			current.shapeA[1][1]=1;
			current.shapeA[2][1]=1;
			current.shapeA[2][0]=1;}
				else{current.face=1;}
			}
			else{if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][1]=1;
			current.shapeA[1][1]=1;
			current.shapeA[2][1]=1;
			current.shapeA[2][0]=1;
		}
		}
		if (face==3)
		{	current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[1][0]=1;
			current.shapeA[1][1]=1;
			current.shapeA[1][2]=1;
		}
		if (face==4)
		{	if (!Tetris_safe_L_Move(current.Colunm,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				{current.Colunm-=1;
			if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[1][0]=1;
			current.shapeA[2][0]=1;
			current.shapeA[0][1]=1;}
				else{current.face=3;}
			}
			else{if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[1][0]=1;
			current.shapeA[2][0]=1;
			current.shapeA[0][1]=1;
		}
			
		}
		
	}
	if (type==Z)
	{ if(face==1||face==3)
		{ 
		  if (!Tetris_safe_L_Move(current.Colunm,current.Row) || current.Colunm+(current.colums_used-1)==7)
			{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				{current.Colunm-=1;
				if (current.Colunm==6){current.Colunm=5;}
				current.colums_used=3;
				Tetris_erase_shape();
				current.shapeA[1][0]=1;
				current.shapeA[1][1]=1;
				current.shapeA[0][1]=1;
				current.shapeA[2][0]=1;}
			else{current.face=2;}
				}
		else{if (current.Colunm==6){current.Colunm=5;}
				current.colums_used=3;
				Tetris_erase_shape();
				current.shapeA[1][0]=1;
				current.shapeA[1][1]=1;
				current.shapeA[0][1]=1;
				current.shapeA[2][0]=1;
				}	
		} 
		if (face==2||face==4)
		{	current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[0][1]=1;
			current.shapeA[1][1]=1;
			current.shapeA[1][2]=1;
		}
	}
	
		if (type==RevZ)
		{ if(face==1||face==3)
			{  
				if (!Tetris_safe_L_Move(current.Colunm,current.Row))
				{if (Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
				   {current.Colunm-=1;
					if (current.Colunm==6){current.Colunm=5;}
					current.colums_used=3;
					Tetris_erase_shape();
					current.shapeA[0][0]=1;
					current.shapeA[1][0]=1;
					current.shapeA[1][1]=1;
					current.shapeA[2][1]=1;}
					else{current.face=2;}
				}
				else{
			if (current.Colunm==6){current.Colunm=5;}
			current.colums_used=3;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[1][0]=1;
			current.shapeA[1][1]=1;
			current.shapeA[2][1]=1;}
		}
		if (face==2||face==4)
		{	current.colums_used=2;
			Tetris_erase_shape();
			current.shapeA[0][1]=1;
			current.shapeA[0][2]=1;
			current.shapeA[1][1]=1;
			current.shapeA[1][0]=1;
		}
	}
}
//Prints the score in the second screen
void Tetris_score(uint8_t position)
{
			if (score<10)
			{	LCD_Cursor(position);
			LCD_WriteData(score+'0');}
			else if (score<20)
			{	uint8_t Tscore=score-10;
				LCD_Cursor(position);
				LCD_WriteData(1+'0');
				LCD_Cursor(position+1);
			LCD_WriteData(Tscore+'0');	}
			else if (score<30)
			{	uint8_t Tscore=score-20;
				LCD_Cursor(position);
				LCD_WriteData(2+'0');
				LCD_Cursor(position+1);
			LCD_WriteData(Tscore+'0');}
			else if (score<40)
			{	uint8_t Tscore=score-30;
				LCD_Cursor(position);
				LCD_WriteData(3+'0');
				LCD_Cursor(position+1);
			LCD_WriteData(Tscore+'0');}
			else if (score<50)
			{	uint8_t Tscore=score-40;
				LCD_Cursor(position);
				LCD_WriteData(3+'0');
				LCD_Cursor(position+1);
			LCD_WriteData(Tscore+'0');}
			else if (score<60)
			{	uint8_t Tscore=score-50;
				LCD_Cursor(position);
				LCD_WriteData(3+'0');
				LCD_Cursor(position+1);
			LCD_WriteData(Tscore+'0');}
			else if (score<70)
			{	uint8_t Tscore=score-60;
				LCD_Cursor(position);
				LCD_WriteData(3+'0');
				LCD_Cursor(position+1);
			LCD_WriteData(Tscore+'0');}
	
}


//
