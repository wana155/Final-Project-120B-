// #include  <cstdlib>

//==================TETRIS GAME FUNCTIONS==============///
const uint8_t Xlength=7;
const uint8_t Ylength=6;
enum ShapeType{T,RevL,Z,RevZ,L,I,Square}Shpe;

unsigned char Tetris_min(unsigned char a,unsigned char b)
{
	return (a<b)? a:b;
}

struct Shape
{   
	uint8_t type;
	uint8_t face;
	int8_t Row;
	uint8_t Colunm;
	uint8_t colums_used; 
	unsigned char shapeA[4][4];
};
struct Shape current;
unsigned char vertical[8][12];
unsigned char RCount;
unsigned char speedLevel=30; 

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
unsigned char score;
unsigned char level;

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
void Tetris_makeShape(uint8_t param)      
{
	uint8_t RandomNum = rand() % param; 
	 while (prev==RandomNum||RandomNum==5)
	 { RandomNum=rand()%param; }
	prev=RandomNum;  
	current.type=RandomNum;
	current.face=1;
	current.Row=-3;
	{current.Colunm=3;}
	changeFace(current.type,current.face);
	
}

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


unsigned char used(unsigned char c){  
	for(int i=3;i>=0;i--)
	{if (current.shapeA[c][i])
		{return (i+1);
		break;}
	}
	return 0;
}

void Tetris_Draw_A_Sqaure(uint8_t x, uint8_t y)
{
	for (int i=x+1;i<x+6+1;i++)
	for (int j=y;j<y+5;j++)
	{{nokia_lcd_set_pixel(i,j,1);}}
}
void Tetris_screen_saver()
{ /*
	Tetris_makeShape(5);
	if (current.shapeA==Z||current.type==RevZ)
	{changeFace(2);}
	current.Colunm=0;
	current.Row=0;
	for (int i=0;i<current.colums_used;i++)
	{ for (int j=0;j<4;j++)
	{if (current.shapeA[i][j])
	{vertical[i+current.Colunm][j+current.Row]=1;}}}
	Tetris_makeShape(5);
	if (current.shapeA==Z||current.type==RevZ)
	{changeFace(2);}
	current.Colunm=3;
	current.Row=0;
	for (int i=0;i<current.colums_used;i++)
	{ for (int j=0;j<4;j++)
	{if (current.shapeA[i][j])
	{vertical[i+current.Colunm][j+current.Row]=1;}}}
	Tetris_makeShape(5);
	if (current.shapeA==Z||current.type==RevZ)
	{changeFace(2);}
	current.Colunm=6;
	current.Row=0;
	for (int i=0;i<current.colums_used;i++)
	{ for (int j=0;j<4;j++)
	{if (current.shapeA[i][j])
	{vertical[i+current.Colunm][j+current.Row]=1;}}}
	*/
	
		Tetris_erase_shape();

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

void Tetris_erase_shape()
{
		for (int i=0;i<4;i++)
		{ for (int j=0;j<4;j++)
			{ current.shapeA[i][j]=0;}}
}
void changeFace(uint8_t type,uint8_t face)
{   

	//Erase the current shape//

	
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
			{/*if (!Tetris_safe_L_Move(current.Colunm,current.Row)||Tetris_safe_R_Move(current.Colunm-3,current.Row) && current.Colunm>3)
			{current.Colunm-=3;
		
			if (current.Colunm>4){current.Colunm=4;}
			current.colums_used=4;
			Tetris_erase_shape();
			current.shapeA[0][0]=1;
			current.shapeA[1][0]=1;
			current.shapeA[2][0]=1;
			current.shapeA[3][0]=1;}
			else if (!Tetris_safe_L_Move(current.Colunm+2,current.Row)||Tetris_safe_R_Move(current.Colunm-1,current.Row) && current.Colunm>1)
			{current.Colunm-=2;
				
				if (current.Colunm>4){current.Colunm=4;}
				current.colums_used=4;
				Tetris_erase_shape();
				current.shapeA[0][0]=1;
				current.shapeA[1][0]=1;
				current.shapeA[2][0]=1;
			current.shapeA[3][0]=1;}
			else if (!Tetris_safe_L_Move(current.Colunm+1,current.Row)||Tetris_safe_R_Move(current.Colunm-2,current.Row) && current.Colunm>2)
			{current.Colunm-=3;
				
				if (current.Colunm>4){current.Colunm=4;}
				current.colums_used=4;
				Tetris_erase_shape();
				current.shapeA[0][0]=1;
				current.shapeA[1][0]=1;
				current.shapeA[2][0]=1;
			current.shapeA[3][0]=1;}
			else if (!Tetris_safe_L_Move(current.Colunm+3,current.Row)||Tetris_safe_R_Move(current.Colunm,current.Row) && current.Colunm>0)
			{current.Colunm-=1;
				
				if (current.Colunm>4){current.Colunm=4;}
				current.colums_used=4;
				Tetris_erase_shape();
				current.shapeA[0][0]=1;
				current.shapeA[1][0]=1;
				current.shapeA[2][0]=1;
			current.shapeA[3][0]=1;}
			
			
			else{*/
				current.face=1;//}
			}
			
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
void Tetris_Set_Shape(uint8_t x, uint8_t y, uint8_t Type, uint8_t Side)
{

	switch(Type){
		case RevL:
		if (Side==1){
		Tetris_Draw_A_Sqaure(x,y);// main
		Tetris_Draw_A_Sqaure(x+Xlength,y);
		Tetris_Draw_A_Sqaure(x+(2*Xlength),y);
		Tetris_Draw_A_Sqaure(x+(2*Xlength),y+Ylength);}
		if (Side==2){
		Tetris_Draw_A_Sqaure(x,y);// main
		Tetris_Draw_A_Sqaure(x,y+Ylength);
		Tetris_Draw_A_Sqaure(x,y+Ylength*2);
		Tetris_Draw_A_Sqaure(x-Xlength,y+Ylength*2);}
		if (Side==3){
		Tetris_Draw_A_Sqaure(x,y);// main
		Tetris_Draw_A_Sqaure(x,y+Ylength);
		Tetris_Draw_A_Sqaure(x+Xlength,y+Ylength);
		Tetris_Draw_A_Sqaure(x+Xlength*2,y+Ylength);}
		if (Side==4){
		Tetris_Draw_A_Sqaure(x,y);// main
		Tetris_Draw_A_Sqaure(x,y+Ylength);
		Tetris_Draw_A_Sqaure(x,y+Ylength*2);
		Tetris_Draw_A_Sqaure(x+Xlength,y);			
		}
		break;
		
		
	}
	
}

//