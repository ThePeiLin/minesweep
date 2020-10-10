#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<termios.h>
#define ARRY(X,Y) ((X)+((Y)*(rrow)))
typedef struct block
{
	char front;
	char back;
}Block;
void createMap();
void print();
void mainFolk();
void keyBoard();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();
void quit();
void openBlock();
void flag();
void gameOver();
int getch();
Block *pb;
int line,row,rline,rrow,x,y,bombNumber,realBombNumber,rrealBombNumber;
int yl,xl;
int ifGameOver;
int main(int argc,char **argv)
{
	line=0;row=0;bombNumber=0;
	if(argc>1)
		if(**(argv+1)=='-')
			switch(*(1+*(argv+1)))
			{
				case 'a':
				case 'A':line=9;row=9;bombNumber=9;break;
				case 'b':
				case 'B':line=18;row=18;bombNumber=18;break;
				case 'd':
				case 'D':
						printf("input line and row\n");
						scanf("%d %d",&line,&row);
						printf("How many landmines you want?\n");
						scanf("%d",&bombNumber);
						break;
				case '?':
				case 'h':
				case 'H':printf("-A or -H\n");exit(0);break;
				default:printf("Please type\" -h\" for help.\n");exit(0);break;
			}
	printf("\033[?1049h");
	while(1)
	{
		createMap();
		printf("\033[2J\033[?25l");
		mainFolk();
	}
	printf("\033[?1049l");
	return 0;
}
int getch()
{
	struct termios oldt,newt;
	int ch;
	tcgetattr(STDIN_FILENO,&oldt);
	newt=oldt;
	newt.c_lflag &=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	ch=getchar();
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	return ch;
}
void createMap()
{
	int yc,xc,i;
	if(line==0&&row==0&&bombNumber==0)
	{
		printf("input line and row\n");
		scanf("%d %d",&line,&row);
		printf("How many landmines you want?\n");
		scanf("%d",&bombNumber);
	}
	rline=line+2;
	rrow=row+2;
	pb=(Block*)malloc((rline*rrow)*sizeof(Block));
	for(i=0;i<(rrow*rline);i++)
	{
		pb[i].front=1;
		pb[i].back=0;
	}
	for(yc=0;yc<=(line+1);yc++)
	{	
		for(xc=0;xc<=(row+1);xc++)
		{
			if((yc==0)||(yc==line+1)||(xc==0)||(xc==(row+1)))
			{
				pb[ARRY(xc,yc)].front=4;
				pb[ARRY(xc,yc)].back=10;
			}
		}
	}
	srand((unsigned)time(0));
	for(i=0;i<bombNumber;)
	{
		int yr=1+rand()%line;
		int xr=1+rand()%row;
		pb[ARRY(xr,yr)].back=(pb[ARRY(xr,yr)].back!=9)?((9)*(++i)/i):0;
	}
	for(yc=1;yc<=line;yc++)
	{
		for(xc=1;xc<=row;xc++)
		{
			(pb[ARRY(xc,yc)].back!=9)?
			({
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc-1,yc-1)].back==9)?1:0;
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc,yc-1)].back==9)?1:0;
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc+1,yc-1)].back==9)?1:0;
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc-1,yc)].back==9)?1:0;
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc+1,yc)].back==9)?1:0;
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc-1,yc+1)].back==9)?1:0;
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc,yc+1)].back==9)?1:0;
				pb[ARRY(xc,yc)].back+=(pb[ARRY(xc+1,yc+1)].back==9)?1:0;
			}):0;
		}
	
	}
	xc=0;yc=0;
	realBombNumber=bombNumber;
	rrealBombNumber=bombNumber;
}
void mainFolk()
{
	ifGameOver=0;
	xl=(int)((rrow)/2)+1;
	yl=(int)((rline)/2)+1;
	print();
	while(1)
	{
		switch(getch())
		{
			case 'h':moveLeft();break;
			case 'k':moveUp();break;
			case 'j':moveDown();break;
			case 'l':moveRight();break;
			case 'a':flag();break;
			case 's':openBlock();break;
			case 'q':quit();break;
		}
		if(rrealBombNumber==0)gameOver(1);
		if(ifGameOver)break;
	}
}
void print()
{
	int light;
	printf("\033[1;1H");
	int xp,yp;
	for(yp=0;yp<=(line+1);yp++)
	{
		for(xp=0;xp<=(row+1);xp++)
		{
			light=(xp==(xl-1)&&yp==(yl-1))?46:0;
			switch(pb[ARRY(xp,yp)].front)
			{
				case 0:switch(pb[ARRY(xp,yp)].back)
				{
						case 0:printf("\033[0m\033[%dm.",light);break;
						case 1:printf("\033[0m\033[%dm1",light);break;
						case 2:printf("\033[0m\033[%dm2",light);break;
						case 3:printf("\033[0m\033[%dm3",light);break;
						case 4:printf("\033[0m\033[%dm4",light);break;
						case 5:printf("\033[0m\033[%dm5",light);break;
						case 6:printf("\033[0m\033[%dm6",light);break;
						case 7:printf("\033[0m\033[%dm7",light);break;
						case 8:printf("\033[0m\033[%dm8",light);break;
						case 9:printf("\033[0m\033[%dm*",light);break;
				}break;
				case 1:printf("\033[0m\033[%dm=",light);break;
				case 2:printf("\033[31m\033[%dm!",light);break;
				case 3:printf("\033[0m\033[%dm?",light);break;
				case 4:printf("\033[0m\033[44m#");break;
			}
		}
		printf("\n");
	}
	printf("\033[0m\033[%d;%dH",((int)(rline/2))+1,(rrow+2));
	printf("\033[K");
	printf("mines:%d",realBombNumber);
}
void moveUp()
{
	yl-=(yl>2)?1:0;
	print();
}
void moveDown()
{
	yl+=(yl<(line+1))?1:0;
	print();
}
void moveLeft()
{
	xl-=(xl>2)?1:0;
	print();
}
void moveRight()
{
	xl+=(xl<(row+1))?1:0;
	print();
}
void quit()
{
	printf("\033[2J\033[?25h");
	if(pb!=NULL)free(pb);
	pb=NULL;
	printf("\033[?1049l");
	exit(0);
}
void openBlock()
{
	x=xl-1;
	y=yl-1;
	if(pb[ARRY(x,y)].front!=0)
	{
		realBombNumber+=(pb[ARRY(x,y)].front==2)?1:0;
		(pb[ARRY(x,y)].back==0)?
		({
			int tier;
			int xt,yt;
			pb[ARRY(x,y)].front=0;
			for(tier=1;({
					int canOpenBlock=0;
					for(yt=tier;yt>(-tier-1)&&canOpenBlock==0;yt--)
						for(xt=tier;xt>(-tier-1)&&canOpenBlock==0;xt-=((yt==tier)||(yt==(-tier)))?1:(tier*2))
							canOpenBlock=(((y-yt>=1)&&(y-yt<=line)&&(x-xt>=1)&&(x-xt<=row))&&
								(((pb[ARRY(x-xt-1,y-yt)].front==1)&&(pb[ARRY(x-xt-1,y-yt)].back==0))
								||((pb[ARRY(x-xt+1,y-yt)].front==1)&&(pb[ARRY(x-xt+1,y-yt)].back==0))
								||((pb[ARRY(x-xt,y-yt-1)].front==1)&&(pb[ARRY(x-xt,y-yt-1)].back==0))
								||((pb[ARRY(x-xt,y-yt+1)].front==1)&&(pb[ARRY(x-xt,y-yt+1)].back==0)))
								&&((pb[ARRY(x-xt,y-yt)].front==1)&&(pb[ARRY(x-xt,y-yt)].back!=9)))?
								1
								:0;
					canOpenBlock;})
					;tier+=(
			{
				int haveUnopenedBlock=1;
				for(yt=tier;yt>(-tier-1)&&haveUnopenedBlock==0;yt--)
					for(xt=tier;xt>(-tier-1)&&haveUnopenedBlock==0;xt-=((yt==tier)||(yt==(-tier)))?1:(tier*2))
						haveUnopenedBlock=(((y-yt>=1)&&(y-yt<=line)&&(x-xt>=1)&&(x-xt<=row))&&
							(((pb[ARRY(x-xt-1,y-yt)].front!=4)&&(pb[ARRY(x-xt-1,y-yt)].back==0))
							||((pb[ARRY(x-xt+1,y-yt)].front!=4)&&(pb[ARRY(x-xt+1,y-yt)].back==0))
							||((pb[ARRY(x-xt,y-yt-1)].front!=4)&&(pb[ARRY(x-xt,y-yt-1)].back==0))
							||((pb[ARRY(x-xt,y-yt+1)].front!=4)&&(pb[ARRY(x-xt,y-yt+1)].back==0)))
							&&((pb[ARRY(x-xt,y-yt)].front==1)&&(pb[ARRY(x-xt,y-yt)].back!=9)))?
					1
					:0;
				haveUnopenedBlock;
			}))
				for(yt=tier;yt>(-tier-1);yt--)
					for(xt=tier;xt>(-tier-1);xt-=((yt==tier)||(yt==(-tier)))?1:(tier*2))
						pb[ARRY(x-xt,y-yt)].front=(((y-yt>=1)&&(y-yt<=line)&&(x-xt>=1)&&(x-xt<=row))&&
						(((pb[ARRY(x-xt-1,y-yt)].front!=4)&&(pb[ARRY(x-xt-1,y-yt)].back==0))
						||((pb[ARRY(x-xt+1,y-yt)].front!=4)&&(pb[ARRY(x-xt+1,y-yt)].back==0))
						||((pb[ARRY(x-xt,y-yt-1)].front!=4)&&(pb[ARRY(x-xt,y-yt-1)].back==0))
						||((pb[ARRY(x-xt,y-yt+1)].front!=4)&&(pb[ARRY(x-xt,y-yt+1)].back==0))))?
						0
						:(pb[ARRY(x-xt,y-yt)].front);
		})
		:((pb[ARRY(x,y)].back==9)?(gameOver(0)):(pb[ARRY(x,y)].front=0));
	}
	if(ifGameOver==0)print();
}
void flag()
{
	x=xl-1;
	y=yl-1;
	switch(pb[ARRY(x,y)].front)
	{
		case 1:pb[ARRY(x,y)].front++;realBombNumber--;rrealBombNumber-=(pb[ARRY(x,y)].back==9)?1:0;break;
		case 2:pb[ARRY(x,y)].front++;realBombNumber++;rrealBombNumber+=(pb[ARRY(x,y)].back==9)?1:0;break;
		case 3:pb[ARRY(x,y)].front=1;break;
	}
	print();
}
void gameOver(int mode)
{
	printf("\033[?25h");
	if(mode==0)
	{
		int i;
		for(i=0;i<(rline*rrow);i++)
			pb[i].front=(pb[i].front!=4)?0:4;
		print();
		printf("\033[%d;%dHYou failed.Again?(Y/n)",(int)(rline/2),rrow+2);
		free(pb);
		pb=NULL;
		((getchar()=='n')||(getchar()=='N'))?(quit()):(ifGameOver=1);
	}
	else if(mode==1)
	{
		int i;
		for(i=0;i<(rline*rrow);i++)
			pb[i].front=(pb[i].front!=4)?0:4;
		print();
		printf("\033[%d;%dHYou win!Again?(Y/n)",(int)(rline/2),rrow+2);
		free(pb);
		pb=NULL;
		((getchar()=='n')||(getchar()=='N'))?(quit()):(ifGameOver=1);
	}
}
