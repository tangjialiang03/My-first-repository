//五子棋 Gobang   
//2006 唐嘉良 2020K8009907032
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define MAXLINE 1000//数组最大长度
#define SIZE 15//棋盘大小为15*15
#define CHARSIZE 2//棋盘使用的是GB2312编码，每一个中文字符占用2个字节。
#define MAXLEN 100//输入的最大长度，一般用不到
#define EMPTY 0//棋盘此处没有棋子
#define ALLWAY 13
//基本子力要素
#define WIN5 0//成五
#define ALIVE4 1//活四
#define DIE4 2//死四
#define LOWDIE4 3//次死四
#define ALIVE3 4//活三
#define TIAO3 5//跳三
#define DIE3 6//眠三
#define ALIVE2 7//活二
#define LOWALIVE2 8//次活二
#define DIE2 9//眠二
#define SAFE 10//周围不可能形成五子，没有任何威胁
#define LONGER 11//长连
#define SPECIAL 12//特殊禁手
//赋分函数
//Alphago下棋打分等级明细
#define score_0 5000
#define score_1 25000
#define score_2 5000
#define score_3 2000
#define score_4 1000
#define score_5 900
#define score_6 800
#define score_7 310
#define score_8 310
#define score_9 140
#define score_10 80
#define score_11 60
#define score_12 90
#define score_13 40
#define score_14 20
//人类下棋打分等级明细
#define BAN -5
#define Hscore_0 4000
#define Hscore_1 20000
#define Hscore_2 4000
#define Hscore_3 1600
#define Hscore_4 800
#define Hscore_5 560
#define Hscore_6 500
#define Hscore_7 250
#define Hscore_8 250
#define Hscore_9 80
#define Hscore_10 40
#define Hscore_11 25
#define Hscore_12 50
#define Hscore_13 15
#define Hscore_14 10
//特殊分数
#define Unaccepted 0 //这个位置有子，不能下，赋予最低分

//原初棋盘 
char board0[SIZE][SIZE*CHARSIZE+1] = 
{
		"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};

//工作棋盘
int board1[SIZE][SIZE];//此数组用于记录棋盘格局,即心中的棋盘，以1与0记录棋局信息 
char board2[SIZE][SIZE*CHARSIZE+1];//此数组用于显现棋盘，即最终被打印至终端的棋盘

//棋子
char black[]="●";//黑棋子
char blacknow[]="▲"; //黑棋最近一次下棋位置
char white[]="◎";//白棋子
char whitenow[]="△"; //白棋最近一次下棋位置

//其它重要参量
struct Point{
	int x;
	int y;
};//记录所落子的坐标(x,y)
struct Situation {
	int longer;//长连
	int special;//特殊禁手
	int win5;//成五
	int alive4;//活4
    int die4;//死4
    int lowdie4;//低级死4
	int alive3;//活3
	int tiao3;//跳3
	int die3;//死3
	int alive2;//活2
	int lowalive2;//低级活2
	int die2;//死2
	int safe;//安全
};//当前位置的形势，打分根据
int result = 0;//result=0代表对弈进行中，result=1代表先手黑子玩家胜利，result=2代表后手白子胜利，result=3代表平局，result=4代表触碰禁手
int mode = 0;//游戏模式选择，1为人人大战，2为人机大战（人先手），3为机人大战（机先手）
int order = 1;//记录对弈顺序，1代表轮到黑棋（玩家1），2代表轮到白棋（玩家2）
int counter=0;//记录下棋步数，从0开始，比真实步数少了1
char player1[MAXLEN];//玩家1（黑棋）昵称
char player2[MAXLEN];//玩家2（白棋）昵称
char ai_name[]="Alphago";//阿尔法狗（电脑名称）
struct Point next={0,0};//记录机器下一步的落子位置
struct Point mybestpoint[SIZE*SIZE]={0};//记录分数较大的一些点
int hisresult[SIZE*SIZE]={0};//往后搜索一层，记录对手分数较大的一些点
int tempboard1[15][15] = {0};//记录此时棋盘格局，与board1区别在于把当前步的数统一了
int tempboard2[15][15] = {0};//记录自己下完之后棋盘格局

//所用函数
void start(void);
void iniboard(void);//即下面三个函数组成的总初始函数：初始化棋盘1，填入棋盘2并将棋盘2打印出来
void iniboard1(void);//将心中棋盘初始化
void recordtoboard2(void);//将心中棋盘读入现实棋盘
void displayboard2(void);//将现实棋盘显示在终端
void PVP(void);//mode of person versus person
void PVC(void);//mode of person versus computer
void CVP(void);//mode of computer versus person
void inimidtemp(struct Point mybestpoint[],int hisresult[],int tempboard1[15][15],int tempboard2[15][15]);
void competing(void);
void playchess1(int order);//模式一:根据下棋顺序下棋。玩家输入落子位置，将该位置读入board1
void playchess2(int order);//模式二类似实现
void playchess3(int order);//模式三类似实现
void read(char input[],int order);//处理输入。接受四种输入形式，读入board1
void currentchess();//在棋盘上显示出当前落子，一步后消失
int judge();//判断是否有一方获胜
int win5(int a,int b);//判断board1[a][b]是否让棋局构成五子胜利
int type_judge(int chess[9]);//判断下这个位置的当前方向成子类型
int judgeSection(int x,int a,int b);//judge whether x is in the section [a,b]
int judgeSection_plus(int x,int a,int b);//judge whether x is in the section [a,b)
int score_cal(int a,int b);
void getchess(int chess[9],const int state[15][15],struct Point point,int color,int direction);//获得当前方向棋局数组
int score_judge(const int state[15][15],struct Point point,int color);//综合四个方向评判当前位置棋局形式的分数
int giveScore(struct Situation situation,int color);//根据形势给分
int larger(int x,int y);
struct Point bestpoint(const int myscore[15][15],const int hisscore[15][15],int color,struct Point mybestpoint[]);//根据我和对手的分数，选取最大利益的位置
struct Point nextpoint(const int boardnow[15][15],int color,int tempboard[15][15]);//根据当前形势，计算下一步棋的位置
int ban_judgement(const int board1[15][15],int x, int y, int color); //禁手判断

//主函数
int main(){
	start();
	if(mode==1){//人人对战
	    PVP();
	}
	else if(mode==2){//人机对战
	    PVC();
	}
	else if(mode==3){//机人对战
	    CVP();
	}
    return 0;
}

void start(void){
	//输入对战模式：1.人人对战 2.人机对战 3.机人对战
	printf("Welcome to Gobang Game!\nNow...please choose your desirable battle mode:\n1.People versus People  \n2.People versus AI  \n3.AI versus People\n");
	scanf("%d",&mode);
	//处理非法对战模式输入，重新输入直到输入合法模式为止
	while(mode!=1 && mode!=2 && mode!=3){
		printf("Illigal input! Please input your desirable mode again: ");
		scanf("%d",&mode);
	}
}

void PVP(void){//person versus person
	//输入双方昵称
	printf("Okey,so you'd better leave names of both sides here!\n");
    printf("player1: ");
	scanf("%s",player1);
	printf("player2: ");
	scanf("%s",player2);
	//总初始化
	iniboard();
	//开始对战
    while(result==0){//对战中
		playchess1(order);
        competing();	
	}
    switch(result){//在终端输出结果
	case 1:printf("%s wins!\n",player1);break;
	case 2:printf("%s wins!\n",player2);break;
	case 3:printf("draw！\n");break;
	case 4:printf("banned chess！\n");break;
	}
}

void PVC(void){//person versus computer
    //输入双方昵称
	printf("Please input Man's name: ");
	scanf("%s",player1);
	//总初始化
	iniboard();
    //开始对战
	while(result==0){//对战中
		playchess2(order);
        competing();
		inimidtemp(mybestpoint,hisresult,tempboard1,tempboard2);
	}
    switch(result){//终端输出结果
	case 1:printf("%s wins!\n",player1);break;
	case 2:printf("%s wins!\n",ai_name);break;
	case 3:printf("draw!\n");break;
	case 4:printf("banned chess！\n");break;
	}
}

void CVP(void){//computer versus person
    //输入双方昵称
	printf("Please input Man's name: ");
	scanf("%s",player2);
	//总初始化
	iniboard();
    //开始对战
	while(result==0){//对战中
		playchess3(order);
        competing();
		inimidtemp(mybestpoint,hisresult,tempboard1,tempboard2);
	}
    switch(result){//终端输出结果
	case 1:printf("%s wins!\n",ai_name);break;
	case 2:printf("%s wins!\n",player2);break;
	case 3:printf("draw!\n");break;
	case 4:printf("banned chess！\n");break;
	}
}

//棋盘初始化(包括board1和board2，并显现在终端)
void iniboard(void){
    iniboard1();
	recordtoboard2();
	displayboard2();
}

//初始化board1棋盘
void iniboard1(void){
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++)
		    board1[i][j]=0;
	}//将board1清0
}

//将board1中记录的棋子位置，转化到board2中
void recordtoboard2(void){
    for(int i=0;i<SIZE;i++)
		for(int j=0;j<SIZE*CHARSIZE+1;j++)
		board2[i][j]=board0[i][j];
    for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
		if (board1[i][j]==1)
		{board2[i][j*CHARSIZE]=black[0];
		board2[i][j*CHARSIZE+1]=black[1];}
		if (board1[i][j]==2)
		{board2[i][j*CHARSIZE]=white[0];
		board2[i][j*CHARSIZE+1]=white[1];}
		}
	}
}

//终端显示棋盘board2
void displayboard2(void){
	int i;
	//第一步：清屏
	system("clear");//清屏  
    printf("         Gobang:Work of 唐嘉良\n");
	//第二步：将board2输出到屏幕上
	for(i=0;i<SIZE;i++){
		printf("%3d %s\n",SIZE-i,board2[i]);
	}
	//第三步：输出最下面的一行字母A B .... 
	printf("   ");
	for(i=0;i<SIZE;i++)
	printf("%2c",'A'+i);
	printf("\n");
	if(mode==2 && order==1)
	    printf("\n%s: %d %c\n",ai_name,15-next.x,next.y+'A');
	if(mode==3 && order==2)
	    printf("\n%s: %d %c\n",ai_name,15-next.x,next.y+'A');
} 

//将所用的中间棋盘清零回到初始状态，每次下完都执行一次
void inimidtemp(struct Point mybestpoint[],int hisresult[],int tempboard1[15][15],int tempboard2[15][15]){
	int comboard[SIZE][SIZE]={0};//给出零元
	for(int i=0;i<255;i++){//将mybestpoint和hisresult清零
		if((mybestpoint[i].x)!=0)
		    mybestpoint[i].x=0;
		else{

		}
		if((mybestpoint[i].y)!=0)
		    mybestpoint[i].y=0;
		else{

		}
		if(hisresult[i]!=0)
		    hisresult[i]=0;
		else{

		}
	}
	for(int i=0;i<SIZE;i++)
		for(int j=0;j<SIZE;j++){//将tempboard清零
			tempboard1[i][j]=comboard[i][j];
			tempboard2[i][j]=comboard[i][j];
		}
}

void competing(){//It is compteting now
	recordtoboard2();//将board1记录入board2
	result=judge();//判断结果
	order=(order==1)?2:1;//改变顺序
	currentchess();//独特地显示最后一步的棋子
	displayboard2();//打印棋盘
	counter+=1;//步数加一
}

//PVP模式，玩家输入落子位置，将该位置读入board1
void playchess1(int order){
    char input[4];
	if(order==1){//玩家1落子
		printf("player1 input: ");
        scanf("%s",input);//考虑接受四种形式输入：形如h6、3h、14H、H10,这里需要*专门判断输入形式，并按照各自规则读如board1*
        read(input,order);
	}
	if(order==2){//玩家2落子
		printf("player2 input: ");
        scanf("%s",input);
		read(input,order);
	}
}

//PVC模式的输入
void playchess2(int order){
	char input[4];
	if(order==2){//电脑落子
		next = nextpoint(board1,order,tempboard1);//获取下一步棋位置
	    board1[next.x][next.y]=22;
	}else if(order==1){//玩家落子
		printf("%s:please input \n",player1);
		scanf("%s",input);
		read(input,order);
	}
}

//CVP模式的输入
void playchess3(int order){
	char input[4];
	if(order==1){//电脑落子
		next = nextpoint(board1,order,tempboard1);//获取下一步棋位置
	    board1[next.x][next.y]=11;
	}else if(order==2){//玩家落子
		printf("%s:please input \n",player2);
		scanf("%s",input);
		read(input,order);
	}
}

//处理输入。接受四种输入形式，读入board1
void read(char input[],int order){
	extern int board1[SIZE][SIZE];//将外部的board1拉入read函数中
	int midboard[SIZE][SIZE];
	////开始翻译输入。将输入的input，转换为对应的board1的坐标coordinate
	int row=0,line=0;
	//如果输入input以字母开头
	if(isalpha(input[0])){
        int i=1;
		//确定纵坐标
		if(input[0]<'a')//input[0]是A~Z
		    line = input[0]-'A';
        if(input[0]>'Z')//input[0]是a~z
		    line = input[0]-'a';
		//确定横坐标
		for(i=1;isdigit(input[i]);i++)
            row = row*10 + (input[i]-'0');
	}
	//如果输入input以数字开头
	if(isdigit(input[0])){
        int j=0;
		//确定横坐标
		for(j=0;isdigit(input[j]);j++)
            row = row*10 + (input[j]-'0');
		//确定纵坐标
		if(input[j]<'a')//input[0]是A~Z
		    line = input[j]-'A';
        if(input[j]>'Z')//input[0]是a~z
		    line = input[j]-'a';
	}////结束翻译输入
	//判断落子位置是否合法
	//1.判断落子是否越出棋盘
    if(row>15 || line>=15 || row==0){  //A～O对应line 为0～14，而输入的数字对应的row就是它本身，即1～15
		printf("Where do you want to go?\n");
		printf("Now! You should input again: ");
		scanf("%s",input);
		read(input,order);
	}
	//2.判断落子位置是否已经存在棋子
    else if(board1[SIZE-row][line]!=0){
        printf("Wrong!\n");
		printf("Now! You should input again: ");
		scanf("%s",input);
		read(input,order);
	}
	//3.判断是否为禁手，只针对模式1与模式2（先手执黑子）
	else{
	    if((mode==2 || mode==1) && order==1 && (ban_judgement(board1,SIZE-row,line,1)==1)){
            board1[SIZE-row][line]=3;
        }else{
            if(order==1){
	            for(int i=0;i<SIZE;i++)
				    for(int j=0;j<SIZE;j++)
					    midboard[i][j]=board1[i][j];
					midboard[SIZE-row][line]=11;
				for(int i=0;i<SIZE;i++)
				    for(int j=0;j<SIZE;j++)
				        board1[i][j]=midboard[i][j];
			}
	        else if(order==2){
	            for(int i=0;i<SIZE;i++)
				    for(int j=0;j<SIZE;j++)
					    midboard[i][j]=board1[i][j];
					midboard[SIZE-row][line]=22;
				for(int i=0;i<SIZE;i++)
				    for(int j=0;j<SIZE;j++)
				        board1[i][j]=midboard[i][j];//先用11与22记录当前所下位置，等到currentchess()函数执行的时候方便判断，到时候再改回1和2
			}
	    }
	}
}

//判断当前局势,改变state
int judge(){
	extern int board1[SIZE][SIZE];
	extern int order;
	int a,b=0;
	int ban,empty=0;

	//如果board1里有3，则黑子触犯禁手
	for(int m=0;m<15;m++){
		for(int n=0;n<15;n++){
			if(board1[m][n]==3){
			   ban=1;
		   }
		}
	}
	if(ban==1)
	    return 4;//禁手

    //如果棋盘被填满，即board1没有0了，则平局
	for(int m=0;m<15;m++){
		for(int n=0;n<15;n++){
			if(board1[m][n]==0){
				empty=1;
			}  
		}
	}  
	if(empty==0)
	    return 3;//平局

	//因为在下这一步棋之前state是0，所以获胜的必要条件是新下的这一步棋在五子中。所以只要考虑新子周围能不能和它成五即可。
    if(order==1){
        for(int m=0;m<SIZE;m++){
		    for(int n=0;n<SIZE;n++){
			    if(board1[m][n]==11){
					a=m;
					b=n;//传递新子的数组坐标
				}
		    }
	    }
		if(win5(a,b))             
		    {return 1;}//判断成五，胜利
		else{
			return 0;//继续比赛
		}
	}
	if(order==2){
        for(int m=0;m<SIZE;m++){
		    for(int n=0;n<SIZE;n++){
			    if(board1[m][n]==22){
					a=m;
					b=n;//传递新子的数组坐标
				}
		    }
	    }
		if(win5(a,b))
		    {return 2;}//判断成五，胜利
		else{
			return 0;//继续比赛
		}
	}
}

//判断board1[a][b]是否让棋局构成成五 (大于等于五个子)
int win5(int a,int b){//注意：a,b是数组坐标
    extern int board1[SIZE][SIZE];
	extern int order;
	int x,y;//坐标辅助循环量
	int left_num=0,right_num=0,up_num=0,down_num=0,left_up_num=0,left_down_num=0,right_up_num=0,right_down_num=0;//统计八个方向有多少个连续相同棋子
	int east=0,north=0,en=0,es=0;//四个可能的五子方向：横向、纵向、东北向、东南向；它们为1则连成五子，为0则连不成五子
	//黑子判断
	if(order==1){
        //横向判断
		for(y=b;y>0 && board1[a][y-1]==1 ;y--)                 
		    left_num+=1;
		for(y=b;y<SIZE-1 && board1[a][y+1]==1;y++)
		    right_num+=1;                               
		if(left_num + right_num + 1>=5)
		    east=1;
		//纵向判断
		for(x=a;x<SIZE-1 && board1[x+1][b]==1;x++)
		    down_num+=1;
		for(x=a;x>0 && board1[x-1][b]==1;x--)
		    up_num+=1;
		if(down_num + up_num + 1>=5)
		    north=1;
		//东北向判断
		for(x=a,y=b;x>0 && y<SIZE-1 && board1[x-1][y+1]==1;x--,y++)
		    right_up_num +=1;
		for(x=a,y=b;x<SIZE-1 && y>0 && board1[x+1][y-1]==1;x++,y--)
		    left_down_num +=1;
		if(right_up_num + left_down_num + 1>=5)
		    en=1;
		//东南向判断
		for(x=a,y=b;x<SIZE-1 && y<SIZE-1 && board1[x+1][y+1]==1;x++,y++)
		    right_down_num +=1;
		for(x=a,y=b;x>0 && y>0 && board1[x-1][y-1]==1;x--,y--)
		    left_up_num +=1;
		if(right_down_num + left_up_num + 1>=5)
		    es=1;
		//成五总判断
		if(east || north || en || es)
		    return 1;
		return 0;
	}
	
	//白子判断
	if(order==2){
        //横向判断
		for(y=b;y>0 && board1[a][y-1]==2 ;y--)                 
		    left_num+=1;
		for(y=b;y<SIZE-1 && board1[a][y+1]==2;y++)
		    right_num+=1;                               
		if(left_num + right_num + 1>=5)
		    east=1;
		//纵向判断
		for(x=a;x<SIZE-1 && board1[x+1][b]==2;x++)
		    down_num+=1;
		for(x=a;x>0 && board1[x-1][b]==2;x--)
		    up_num+=1;
		if(down_num + up_num + 1>=5)
		    north=1;
		//东北向判断
		for(x=a,y=b;x>0 && y<SIZE-1 && board1[x-1][y+1]==2;x--,y++)
		    right_up_num +=1;
		for(x=a,y=b;x<SIZE-1 && y>0 && board1[x+1][y-1]==2;x++,y--)
		    left_down_num +=1;
		if(right_up_num + left_down_num + 1>=5)
		    en=1;
		//东南向判断
		for(x=a,y=b;x<SIZE-1 && y<SIZE-1 && board1[x+1][y+1]==2;x++,y++)
		    right_down_num +=1;
		for(x=a,y=b;x>0 && y>0 && board1[x-1][y-1]==2;x--,y--)
		    left_up_num +=1;
		if(right_down_num + left_up_num + 1>=5)
		    es=1;
		//成五总判断
		if(east || north || en || es)
		    return 1;
		return 0;
	}
}

//计算下一步棋子的位置
struct Point nextpoint(const int boardnow[15][15],int color,int tempboard[15][15]){
	int hiscolor=(color==1)?2:1;
	int myscore[15][15]={0};
	int hisscore[15][15]={0};
	//是否首次下棋
	int ini_play=0;
	int k=0,h=0;
	for(k=0;k<15;k++){
		for(h=0;h<15;h++){
			if(boardnow[k][h]>0){
				ini_play = 1;
			}
		}
	}
	if(ini_play==0){//是
        struct Point point={7,7};//下在天元位置
		return point;
	}
	//将心中棋盘复制到暂时棋盘上，改当前步标志
	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			switch(boardnow[i][j]){
				case 22:tempboard[i][j]=2;break;
				case 11:tempboard[i][j]=1;break;
				default:tempboard[i][j]=boardnow[i][j];break;
			}
		}
	}
	//打分
	for(int i=0;i<15;i++)
	    for(int j=0;j<15;j++){
			struct Point point={i,j};
			//我的分数
			myscore[i][j]=score_judge(tempboard,point,color);
			//对手分数
			hisscore[i][j]=score_judge(tempboard,point,hiscolor);
		}
	//根据分数，给出落子位置
	return bestpoint(myscore,hisscore,color,mybestpoint);
}

//综合四个方向评判当前棋局形势的分数
int score_judge(const int state[15][15],struct Point point,int color){
	struct Situation situation={0};//记录当前形势变量
    if(state[point.x][point.y]!=0)
	    return Unaccepted;//该位置有棋子，不能下，零分
	else if((ban_judgement(state,point.x,point.y,color)==1) ){
		if(mode==3)
		    return Unaccepted;//禁手位置不能下，主要针对模式3，机器先手
		else {
			
		}
	}else{
		for(int direction=0;direction<4;direction++){//0东，1北，2东南，3东北
            int type;
	        int chess[9]={0};
	        getchess(chess,state,point,color,direction);
	        type=type_judge(chess);//取得类型
		    if(type==WIN5)
			    situation.win5+=1;
			if(type==ALIVE4)
			    situation.alive4+=1;
			if(type==DIE4)
			    situation.die4+=1;
			if(type==LOWDIE4)
			    situation.lowdie4+=1;
            if(type==ALIVE3)
			    situation.alive3+=1;
			if(type==TIAO3)
			    situation.tiao3+=1;
			if(type==DIE3)
			    situation.die3+=1;
			if(type==ALIVE2)
			    situation.alive2+=1;
			if(type==LOWALIVE2)
			    situation.lowalive2+=1;
			if(type==DIE2)
			    situation.die2+=1;
			if(type==SAFE)
			    situation.safe+=1;
			if(type==LONGER)
			    situation.longer+=1;
			if(type==SPECIAL)
			    situation.special+=1;
		}
		return giveScore(situation,color);//得出分数
	}
}

//给出分数
int giveScore(struct Situation situation,int color){
	int situa[ALLWAY]={0};
	//数组转存
    situa[0]=situation.longer;
	situa[1]=situation.special;
	situa[2]=situation.win5;
	situa[3]=situation.alive4;
	situa[4]=situation.die4;
	situa[5]=situation.lowdie4;
	situa[6]=situation.alive3;
	situa[7]=situation.tiao3;
	situa[8]=situation.die3;
	situa[9]=situation.alive2;
	situa[10]=situation.lowalive2;
	situa[11]=situation.die2;
	situa[12]=situation.safe;
	int grade;//分数
	if(mode==2 && color==2){//人机对战，机器后手
		grade=score_14;//初始底分
		//以下是各种情况的赋分规则
		if(situa[0]>=1)
		    grade+=score_0;//白棋没有禁手

		if(situa[3]+situa[4]+situa[5]>1 || situa[3]>1 || situa[4]+situa[5]>=2 || (situa[4]+situa[5]>=1 && situa[6]>=1) || (situa[3]>=1 && situa[6]>=1))
		    grade+=score_0;

		if(situa[2]>=1 || situa[0]>=1)
		    grade+=score_1;

		if(situa[3]==1)
		    grade+=score_2;

		if(situa[6]>=2)
		    grade+=score_3;

		if((situa[8]>=1 && situa[6]>=1))
		    grade+=score_4;

		if(situa[4]>=1 && situa[4]!=2)
		    grade+=((situa[4])*score_5);

		if(situa[4]+situa[5]>=1 && situa[9]>=1)
		    grade+=score_4;

		if(situa[5]>=1 && situa[5]!=2)
		    grade+=((situa[5])*score_6);

		if(situa[8]>=2)
		    grade+=score_6;

		if(situa[6]>=1)
		    grade+=score_7;

		if(situa[7]>=1)
            grade+=score_8;

		if(situa[9]+situa[10]>=2)
		    grade+=score_9;

		if(situa[9]>=1)
		    grade+=score_10;

		if(situa[10]>=1)
		    grade+=score_11;

		if(situa[8]>=1)
		    grade+=score_12;

		if(situa[11]>=1)
		    grade+=score_13;

		return grade;
	}

	if(mode==2 && color==1){//人机对战，人先手
		grade=Hscore_14;//初始底分
		//以下是各种情况的赋分规则
		if(situa[1]>=1)
		    return BAN;//禁手

		if(situa[3]+situa[4]+situa[5]>1 || situa[3]>1 || situa[4]+situa[5]>=2 || situa[0]>=1)
		    return BAN;//禁手

		if((situa[4]+situa[5]>=1 && situa[6]+situa[7]>=1) || (situa[3]>=1 && situa[6]+situa[7]>=1))
		    return Hscore_0;

		if(situa[2]>=1)
		    grade+=Hscore_1;

		if(situa[3]==1)
		    grade+=Hscore_2;

		if(situa[0]>=1)
		    return BAN;//禁手

		if(situa[6]+situa[7]>=2)
		    return BAN;//禁手

		if(situa[8]>=1 && situa[6]>=1)
		    grade+=Hscore_4;

		if(situa[4]+situa[5]>=1 && situa[9]+situa[10]>=1)
		    grade+=Hscore_4;

		if(situa[4]>=1 && situa[4]!=2)
		    grade+=((situa[4])*Hscore_5);

		if(situa[5]>=1 && situa[5]!=2)
		    grade+=((situa[5])*Hscore_6);

		if(situa[8]>=2)
            grade+=Hscore_6;

		if(situa[6]>=1)
		    grade+=Hscore_7;

		if(situa[7]>=1)
		    grade+=Hscore_8;

		if(situa[9]+situa[10]>=2)
		    grade+=Hscore_9;

		if(situa[9]>=1)
		    grade+=Hscore_10;

		if(situa[10]>=1)
		    grade+=Hscore_11;

		if(situa[8]>=1)
		    grade+=Hscore_12;

		if(situa[11]>=1)
		    grade+=Hscore_13;

		return grade;
	}
	if(mode==3 && color==1){//机人对战，机器先手
		grade=score_14;//初始底分
		//以下是各种情况的赋分规则
		if((situa[4]+situa[5]>=1 && situa[6]+situa[7]>=1) || (situa[3]>=1 && situa[6]+situa[7]>=1))
		    grade+=score_0;

		if(situa[2]>=1)
		    grade+=score_1;

		if(situa[3]==1)
		    grade+=score_2;

        if(situa[6]+situa[7]>=2)
		    grade+=score_3;

		if(situa[8]>=1 && situa[6]>=1)
		    grade+=score_4;

		if(situa[4]>=1 && situa[9]+situa[10]>=1)
		    grade+=score_4;

		if(situa[4]>=1 && situa[4]!=2)
		    grade+=((situa[4])*score_5);

		if(situa[5]>=1 && situa[5]!=2)
		    grade+=((situa[5])*score_6);

		if(situa[8]>=2)
		    grade+=score_6;

		if(situa[6]>=1)
		    grade+=score_7;

		if(situa[7]>=1)
		    grade+=score_8;

		if(situa[9]+situa[10]>=2)
		    grade+=score_9;

		if(situa[9]>=1)
            grade+=score_10;

		if(situa[10]>=1)
		    grade+=score_11;

		if(situa[8]>=1)
		    grade+=score_12;

		if(situa[11]>=1)
		    grade+=score_13;

		return grade;
	}
	if(mode==3 && color==2){//机人对战，人后手
		grade=Hscore_14;//初始底分
        ///以下是各种情况的赋分规则
		if(situa[1]>=1)
		    grade+=Hscore_0;//后手，没有禁手

		if(situa[3]+situa[4]+situa[5]>1 || situa[4]>1 || situa[4]+situa[5]>=2 || (situa[4]+situa[5]>=1 && situa[6]+situa[7]>=1) || (situa[3]>=1 && situa[6]+situa[7]>=1))
		    grade+=Hscore_0;

		if(situa[2]>=1 || situa[0]>=1)
		    grade+=Hscore_1;

		if(situa[3]==1)
		    grade+=Hscore_2;

		if(situa[6]+situa[7]>=2)
		    grade+=Hscore_3;

		if((situa[8]>=1 && situa[6]>=1))
		    grade+=Hscore_4;

		if(situa[4]>=1 && situa[9]+situa[10]>=1)
		    grade+=Hscore_4;

		if(situa[4]>=1 && situa[4]!=2)
		    grade+=((situa[4])*Hscore_5);

		if(situa[5]>=1 && situa[5]!=2)
		    grade+=((situation.lowdie4)*Hscore_6);

		if(situa[8]>=2)
		    grade+=Hscore_6;

		if(situa[6]>=1)
		    grade+=Hscore_7;

		if(situa[7]>=1)
            grade+=Hscore_8;

		if(situa[9]+situa[10]>=2)
		    grade+=Hscore_9;

		if(situa[9]>=1)
		    grade+=Hscore_10;

		if(situa[10]>=1)
		    grade+=Hscore_11;

		if(situa[8]>=1)
		    grade+=Hscore_12;

		if(situa[11]>=1)
		    grade+=Hscore_13;

		return grade;
	}
}


//获取当前方向棋局数组chess
void getchess(int chess[9],const int state[15][15],struct Point point,int color,int direction){
	int hiscolor;//边界出界填充使用
	hiscolor=(color==1)?2:1;//标记对方棋子颜色
	chess[4]=color;//给形势数组中心填上该点颜色
	if(direction==0){//横向形势
		int i=point.x,j=1;
		while(j<=4){
			int row=point.y - j;
			if(row<0){
				for(;j<=4;j++)
				    chess[4-j]=hiscolor;//界外则填上对方颜色
			}
			chess[4-j]=state[i][row];
			j++;
		}
		i=point.x;
		j=1;
		while(j<=4){
			int row = point.y+j;
			if(row>14){
				for(;j<=4;j++)
				    chess[4+j]=hiscolor;//界外则填上对方颜色
			}
			chess[4+j]=state[i][row];
			j++;
		}
		/*for(int i=point.x,j=1;j<=4;j++){
			int row=point.y - j;
			if(row<0){
				for(;j<=4;j++)
				    chess[4-j]=hiscolor;
			}
			chess[4-j]=state[i][row];
		}
		for(int i=point.x,j=1;j<=4;j++){
			int row = point.y+j;
			if(row>14){
				for(;j<=4;j++)
				    chess[4+j]=hiscolor;
			}
			chess[4+j]=state[i][row];
		}*/
	}
	else if(direction==1){//纵向形势
		int i=1,j=point.y;
		while(i<=4){
			int line=point.x - i;
			if(line<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;//界外则填上对方颜色
			}
			chess[4-i]=state[line][j];
			i++;
		}
		i=1;
		j=point.y;
		while(i<=4){
			int line = point.x+i;
			if(line>14){
				for(;i<=4;i++)
				    chess[4+i]=hiscolor;//界外则填上对方颜色
			}
			chess[4+i]=state[line][j];
			i++;
		}
        /*for(int j=point.y,i=1;i<=4;i++){
			int line=point.x-i;
			if(line<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;
			}
			chess[4-i]=state[line][j];
		}
		for(int i=1,j=point.y;i<=4;i++){
			int line=point.x+i;
			if(line>14){
				for(;i<=4;i++)
				    chess[4+i]=hiscolor;
			}
			chess[4+i]=state[line][j];
		}*/
	}
	else if(direction==2){//斜向形势——1
		int i=1,j=1;
		while(i<=4){
			int line=point.x - i;
			int row=point.y - j;
			if(line<0||row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;//界外则填上对方颜色
			}
			chess[4-i]=state[line][row];
			i++;
			j++;
		}
		i=1;
		j=1;
		while(i<=4){
			int line=point.x+i;
			int row = point.y+j;
			if(line>14||row>14){
				for(;i<=4;i++)
				    chess[4+i]=hiscolor;//界外则填上对方颜色
			}
			chess[4+i]=state[line][row];
			i++;
			j++;
		}
		/*for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x-i;
			int row=point.y-j;
			if(line<0 || row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;
			}
			chess[4-i]=state[line][row];
		}
		for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x+i;
			int row=point.y+j;
			if(line>14 || row>14){
				for(;i<=4;i++)
				    chess[4+i]=hiscolor;
			}
			chess[4+i]=state[line][row];
		}*/
	}
	else if(direction==3){//斜向形势——2
		int i=1,j=1;
		while(i<=4){
			int line=point.x + i;
			int row=point.y - j;
			if(line>14||row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;//界外则填上对方颜色
			}
			chess[4-i]=state[line][row];
			i++;
			j++;
		}
		i=1;
		j=1;
		while(i<=4){
			int line=point.x-i;
			int row = point.y+j;
			if(line<0||row>14){
				for(;i<=4;i++)
				    chess[4+i]=hiscolor;//界外则填上对方颜色
			}
			chess[4+i]=state[line][row];
			i++;
			j++;
		}
		/*for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x+i;
			int row=point.y-j;
			if(line>14 || row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;
			}
			chess[4-i]=state[line][row];
		}
		for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x -i;
			int row=point.y +j;
			if(line<0 || row>14){
                for(;i<=4;i++)
				    chess[4+i]=hiscolor;
			}
			chess[4+i]=state[line][row];
		}*/
	}
	else{

	}

	/*switch(direction){
	case 0:
	    for(int i=point.x,j=1;j<=4;j++){
			int row=point.y - j;
			if(row<0){
				for(;j<=4;j++)
				    chess[4-j]=hiscolor;
				break;
			}
			chess[4-j]=state[i][row];
		}
		for(int i=point.x,j=1;j<=4;j++){
			int row = point.y+j;
			if(row>14){
				for(;j<=4;j++)
				    chess[4+j]=hiscolor;
				break;
			}
			chess[4+j]=state[i][row];
		}
		break;
	case 1:
	    for(int j=point.y,i=1;i<=4;i++){
			int line=point.x-i;
			if(line<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;
				break;
			}
			chess[4-i]=state[line][j];
		}
		for(int i=1,j=point.y;i<=4;i++){
			int line=point.x+i;
			if(line>14){
				for(;i<=4;i++)
				    chess[4+i]=hiscolor;
				break;
			}
			chess[4+i]=state[line][j];
		}
		break;
	case 2:
	    for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x-i;
			int row=point.y-j;
			if(line<0 || row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;
				break;
			}
			chess[4-i]=state[line][row];
		}
		for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x+i;
			int row=point.y+j;
			if(line>14 || row>14){
				for(;i<=4;i++)
				    chess[4+i]=hiscolor;
				break;
			}
			chess[4+i]=state[line][row];
		}
		break;
	case 3:
	    for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x+i;
			int row=point.y-j;
			if(line>14 || row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;
				break;
			}
			chess[4-i]=state[line][row];
		}
		for(int i=1,j=1;i<=4;i++,j++){
			int line=point.x -i;
			int row=point.y +j;
			if(line<0 || row>14){
                for(;i<=4;i++)
				    chess[4+i]=hiscolor;
				break;
			}
			chess[4+i]=state[line][row];
		}
		break;
	default:
	    break;
	}*/
}

//根据四个方向上的形势数组，判断成子类型
int type_judge(int chess[9]){
	int mycolor=chess[4];//读取自己颜色
	int hiscolor,i;
	int left,right;
	int count=1;
	int same_a=1;//记录相同信息
	int same_b=1;//记录相同信息
    //define SOME of the ban-situations，部分禁手情况
    int chess1[9]={EMPTY,mycolor,EMPTY,mycolor,mycolor,mycolor,EMPTY,mycolor,EMPTY};
	int chess2[9]={EMPTY,mycolor,mycolor,EMPTY,mycolor,mycolor,EMPTY,mycolor,mycolor};
	int chess3[9]={mycolor,mycolor,EMPTY,mycolor,mycolor,EMPTY,mycolor,mycolor,EMPTY};
	int chess4[9]={mycolor,mycolor,mycolor,EMPTY,mycolor,EMPTY,mycolor,mycolor,mycolor};
	//decide the color，确定自己和对手颜色
	if(mycolor==1)
	    hiscolor=2;
	else if(mycolor==2)
        hiscolor=1;
	//count the number of my chess(the mid only)，判断从中心开始，往左右两边数分别有多少自己的棋子，只记录连续棋子数量
	for(int i=1;i<=4;i++){//往左数
		if(chess[4-i]==mycolor)
		    count++;
		else{
			left=4-i;
			break;
		}
	}
	for(int i=1;i<=4;i++){//往右数
		if(chess[4+i]==mycolor)
		    count++;
		else{
			right=4+i;
			break;
		}
	}
    //judge different situations
	//先判断特殊禁手
	if(count==3){//中心有三个连续棋子
		for(i=1;i<8;i++){
			if(chess[i]==chess1[i]){
			;
			}
			else{
				same_a=0;
				break;
			}
		}
		if(same_a==1){
            return SPECIAL;//特殊禁手
		}
		same_a=1;   
	}

	if(count==2){//中心有两个连续棋子
        for(i=1;i<9;i++){
			if(chess[i]==chess2[i]){
			;
			}
			else{
				same_a=0;
				break;
			}
		}
		for(i=0;i<8;i++){
			if(chess[i]==chess3[i]){
			;
			}
			else{
				same_b=0;
				break;
			}
		}
		if(same_a==1 || same_b==1){
            return SPECIAL;//特殊禁手
		}
		same_a=same_b=1;
	}

	if(count==1){//中心有一个连续棋子
		for(i=0;i<9;i++){
			if(chess[i]==chess4[i]){
			;
			}
			else{
				same_a=0;
				break;
			}
		}
		if(same_a==1){
			return SPECIAL;//特殊禁手
		}
		same_a=1;
	}
    //再判断正常情况
	if(count>=5)//中心有五个连续棋子
	    if(count>5)
		    return LONGER;//长连
		else
		    return WIN5;//成五

	if(count==4){//中心有四个连续棋子
		if(chess[left]==EMPTY && chess[right]==EMPTY)
			return ALIVE4;//活四
		else if(chess[left]==hiscolor && chess[right]==hiscolor)
		    return SAFE;//安全
		else if(chess[left]==EMPTY || chess[right]==EMPTY)
		    return DIE4;//死四
	}

	if(count==3){//中心有三个连续棋子
	    //下面是一些遍历性的判断
		if(chess[left]==EMPTY && chess[right]==EMPTY){
			if(chess[left-1]==hiscolor && chess[right+1]==hiscolor)
			    return DIE3;//死三
			else if(chess[left-1]==mycolor && chess[right+1]==mycolor)
			    return WIN5;//成五
			else if(chess[left-1]==mycolor || chess[right+1]==mycolor)
			    return LOWDIE4;//次死四
			else if(chess[left-1]==EMPTY || chess[right+1]==EMPTY)
			    return ALIVE3;//活三
		}
		else if(chess[left]==hiscolor && chess[right]==hiscolor){
			return SAFE;//安全
		}
		else if(chess[left]==EMPTY || chess[right]==EMPTY){
			if(chess[left]==hiscolor){
				if(chess[right+1]==hiscolor)
				    return SAFE;//安全
				if(chess[right+1]==EMPTY)
				    return DIE3;//死三
				if(chess[right+1]==mycolor)
				    return LOWDIE4;//次死四
			}
			if(chess[right]==hiscolor){
				if(chess[left-1]==hiscolor)
				    return SAFE;//安全
				if(chess[left-1]==EMPTY)
				    return DIE3;//死三
				if(chess[left-1]==mycolor)
				    return LOWDIE4;//次死四
			}
		}
	}

	if(count==2){//中心有两个连续棋子
		if(chess[left]==EMPTY && chess[right]==EMPTY){
			if((chess[right+1]==EMPTY && chess[right+2]==mycolor)||
			    (chess[left-1]==EMPTY && chess[left+2]==mycolor))
				return DIE3;//死三
			else if(chess[left-1]==EMPTY && chess[right+1]==EMPTY)
			    return ALIVE2;//活二

			if((chess[right+1]==mycolor && chess[right+2]==hiscolor)||
			    (chess[left-1]==mycolor && chess[left-2]==hiscolor))
				return DIE3;//死三

			if((chess[right+1]==mycolor && chess[right+2]==mycolor)||
			    (chess[left-1]==mycolor && chess[left-2]==mycolor))
				return LOWDIE4;//次死四
			
			if((chess[right+1]==mycolor && chess[right+2]==EMPTY)||
			    (chess[left-1]==mycolor && chess[left-2]==EMPTY))
				return TIAO3;//跳三
		}
		else if(chess[left]==hiscolor && chess[right]==hiscolor){
			return SAFE;//安全
		}
		else if(chess[left]==EMPTY || chess[right]==EMPTY){
			if(chess[left]==hiscolor){
				if(chess[right+1]==hiscolor || chess[right+2]==hiscolor){
					return SAFE;//安全
				}
				else if(chess[right+1]==EMPTY && chess[right+2]==EMPTY){
					return DIE2;//死二
				}
				else if(chess[right+1]==mycolor && chess[right+2]==mycolor){
					return LOWDIE4;//次死四
				}
				else if(chess[right+1]==mycolor || chess[right+2]==mycolor){
					return DIE3;//死三
				}
			}
			if(chess[right]==hiscolor){
				if(chess[left-1]==hiscolor || chess[left-2]==hiscolor){
					return SAFE;//安全
				}
				else if(chess[left-1]==EMPTY && chess[left-2]==EMPTY){
					return DIE2;//死二
				}
				else if(chess[left-1]==mycolor && chess[left-2]==mycolor){
					return LOWDIE4;//次死四
				}
				else if(chess[left-1]==mycolor || chess[left-2]==mycolor){
					return DIE3;//死三
				}
			}
		}
	}

	if(count==1){//中心有一个连续棋子
		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==mycolor && chess[left-3]==mycolor)
		    return LOWDIE4;//次死四
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==mycolor && chess[right+3]==mycolor)
		    return LOWDIE4;//次死四
		
		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==mycolor && chess[left-3]==EMPTY && chess[right]==EMPTY)
		    return TIAO3;//跳三
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==mycolor && chess[right+3]==EMPTY && chess[left]==EMPTY)
		    return TIAO3;//跳三

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==mycolor && chess[left-3]==hiscolor && chess[right]==EMPTY)
		    return DIE3;//死三
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==mycolor && chess[right+3]==hiscolor && chess[left]==EMPTY)
		    return DIE3;//死三

		if(chess[left]==EMPTY && chess[left-1]==EMPTY && chess[left-2]==mycolor && chess[left-3]==mycolor)
		    return DIE3;//死三
		if(chess[right]==EMPTY && chess[right+1]==EMPTY && chess[right+2]==mycolor && chess[right+3]==mycolor)
		    return DIE3;//死三

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==EMPTY && chess[left-3]==mycolor)
		    return DIE3;//死三
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==EMPTY && chess[right+3]==mycolor)
		    return DIE3;//死三
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[left]==EMPTY && chess[left-1]==mycolor)
		    return DIE3;//死三

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==EMPTY && chess[left-3]==EMPTY && chess[right]==EMPTY)
		    return LOWALIVE2;//次活二
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==EMPTY && chess[right+3]==EMPTY && chess[left]==EMPTY)
		    return LOWALIVE2;//次活二

		if(chess[left]==EMPTY && chess[left-1]==EMPTY && chess[left-2]==mycolor && chess[left-3]==EMPTY && chess[right]==EMPTY)
		    return LOWALIVE2;//次活二
		if(chess[right]==EMPTY && chess[right+1]==EMPTY && chess[right+2]==mycolor && chess[right+3]==EMPTY && chess[left]==EMPTY)
		    return LOWALIVE2;//次活二

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==EMPTY && chess[left-3]==EMPTY && chess[right]==hiscolor)
		    return DIE2;//死二
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==EMPTY && chess[right+3]==EMPTY && chess[left]==hiscolor)
		    return DIE2;//死二

		if(chess[left]==EMPTY && chess[left-1]==EMPTY && chess[left-2]==mycolor && chess[left-3]==EMPTY && chess[right]==hiscolor)
		    return DIE2;//死二
		if(chess[right]==EMPTY && chess[right+1]==EMPTY && chess[right+2]==mycolor && chess[right+3]==EMPTY && chess[left]==hiscolor)
		    return DIE2;//死二
	}
	return SAFE;//安全
}

//判断最佳下棋位置，思路：在自己和对手分差较大时按自己最大分数的点下，选择一个区间，尽量挑出自己和对手分都很高的位置作为备选
//之后，从备选里中找出让对手最大分数最小的点，如果有多个则再搜一层找自己最大分的点作为最佳点
struct Point bestpoint(const int myscore[15][15],const int hisscore[15][15],int color,struct Point mybestpoint[]){
	//记录自己和对手最大的分数
	int mymaxscore=0;
	int hismaxscore=0;
	int maxscore=0;
	int bestnum=0;
	//一些中间辅助变量
	int count=0;
    int pointnum=SIZE*SIZE;
	int together[SIZE][SIZE]={0};
	//自己和对手的下棋点
	struct Point target={0,0};
	struct Point histarget={0,0};
	//记录优势点
	struct Point mypointarray[pointnum];
	struct Point hispointarray[pointnum];
	struct Point pointarray[pointnum];

	for(int k=0;k<pointnum;k++){//初始化优势点数组
	    mypointarray[k].x=0,mypointarray[k].y=0;
		hispointarray[k].x=0,hispointarray[k].y=0;
		pointarray[k].x=0,pointarray[k].y=0;
	}

	for(int i=0;i<SIZE;i++)
	    for(int j=0;j<SIZE;j++){//计算分数
		    together[i][j] = score_cal(myscore[i][j],hisscore[i][j]);
			if(hisscore[i][j]>hismaxscore){//找对手最大分
				hismaxscore=hisscore[i][j];
			}
			if(myscore[i][j]>mymaxscore){//找自己最大分
				mymaxscore=myscore[i][j];
			}
		}
	
	if(1){
		for(int i=0;i<15;i++)
	        for(int j=0;j<15;j++){
			    if(larger(together[i][j],maxscore)){//处理最大分数
				    maxscore=together[i][j];
			    }
		    }
		//下面是选定一个分数区间，将区间内所有点取出来作为第一层的备选
		if(larger(maxscore,3000) || maxscore==3000){//区间1
		    for(int i=0;i<15;i++)
                for(int j=0;j<15;j++){
		            if(judgeSection(together[i][j],maxscore-800,maxscore)){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
		        }
        }
		else if(judgeSection(maxscore,1400,3000)){//区间2
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){
					if(judgeSection(together[i][j],maxscore-500,maxscore)){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
				}
		}
		else if(judgeSection(maxscore,500,1400)){//区间3
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){
					if(judgeSection(together[i][j],maxscore-200,maxscore)){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
				}
		}
		else{//区间4
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){
					if(together[i][j]==maxscore){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
				}
		}
		for(int i=0;i<count;i++){//填入mybestpoint数组，作为备选
			mybestpoint[bestnum]=pointarray[i];
			bestnum+=1;
		}
	}
	
	if(bestnum==1){//只有一个分比较高的点，其余分都很低
		return mybestpoint[0];
	}
	else if(larger(bestnum,1)){//有多个分数较高的点
		for(int m=0;m<bestnum;m++){
			int hiscolor=(color==1)?2:1;
			int hisscore[15][15]={0};
			tempboard1[mybestpoint[m].x][mybestpoint[m].y]=color;
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){//为对手判断分数
					struct Point point={i,j};
					int score;
					hisscore[i][j]=score_judge(tempboard1,point,hiscolor);
				}
			for(int i=0;i<15;i++)
	            for(int j=0;j<15;j++){//选定对手优势点，填入hisresult
		            if(hisscore[i][j]>maxscore){
				        hisresult[m]=hisscore[i][j];
			        }
		        }
		}
		//中间变量，处理第二层搜索
        int min=hisresult[0];
		int def=0;//下标
		int s[100]={0};
		int num_1=0;
		//取出对手最大分数最小的点
		for(int i=0;i<bestnum;i++){
			if(hisresult[i]<min){
				def=i;
				min=hisresult[i];
				num_1=0;
				for(int k=0;k<100;k++)
				    s[k]=0;
			}
			if(hisresult[i]==min){
				s[num_1]=i;
				num_1+=1;
			}
		}
		//如果这样的点不止一个
		if(num_1!=0){
			int max,i,def_2;
			i=0;
			max=0;
			//代入这些点到棋盘中，找出自己第二层分数最大的点，作为最终结果
			while(i<num_1){
				def_2=s[i];
				int alpha,beta;
				alpha=mybestpoint[def_2].x;
				beta=mybestpoint[def_2].y;
				if(together[alpha][beta]>max){
					max=myscore[alpha][beta];
					def=def_2;
				}
				i++;
			}
		}
		//用随机数处理某些特殊步骤
		if(mode==3){
			if(counter==2){//CVP模式下第三步可能性很多，随机选一个点下
                srand((unsigned)time(NULL));
			    def=rand()%(bestnum);    
			}
			else if(counter!=2){
                
			}
		}
		if(mode==2){
			if(counter==1){//PVC模式下第二步可能性很多，随机选一个点下
                srand((unsigned)time(NULL));
			    def=rand()%(bestnum);    
			}
			else if(counter!=1){
                
			}
		}
		return mybestpoint[def];//返回最佳点
	}
}

//判断大小函数
int larger(int x,int y){
	if(x>y)
	    return 1;
	else{
		return 0;
	}
}

//分差放大函数
//独特计算方法，高优势放大
int score_cal(int a,int b){
	int x;
	if(a-b<=-1000){
		x=14*b/11+8*a/11;
	}
	else if(a-b>-1000 && a-b<=-600){
		x=13*b/11+9*a/11;
	}
	else if(a-b>-600 && a-b<=-200){
		x=12*b/11+10*a/11;
	}
	else if(a-b>-200 && a-b<=200){
        x=b+a;
	}
	else if(a-b>200 && a-b<=600){
		x=10*b/11+12*a/11;
	}
	else if(a-b>600 && a-b<=1000){
		x=9*b/11+13*a/11;
	}
	else if(a-b>1000){
		x=8*b/11+14*a/11;
	}
	return x;
}

//区间判断，判断一个数x是否在区间[a,b]内
int judgeSection(int x,int a,int b){
	if(x>=a){
		if(x<=b)
		    return 1;
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

//区间判断之进阶版，判断一个数x是否在区间[a,b)内
int judgeSection_plus(int x,int a,int b){
	if(x>=a){
		if(x<b)
		    return 1;
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

//在棋盘上用特殊记号显示出上一步的落子，下一步就消失
void currentchess(){
	extern int board1[SIZE][SIZE];
	extern char board2[SIZE][SIZE*CHARSIZE+1];
	extern char blacknow[],whitenow[];
    for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
            if(board1[i][j]==11){//将特殊黑棋改回黑棋
			    board2[i][j*CHARSIZE]=blacknow[0];
			    board2[i][j*CHARSIZE+1]=blacknow[1];
			    board1[i][j]=1;
		    }
		    if(board1[i][j]==22){//将特殊白棋改回白棋
			    board2[i][j*CHARSIZE]=whitenow[0];
			    board2[i][j*CHARSIZE+1]=whitenow[1];
			    board1[i][j]=2;
		    }
		}
	}
}

//禁手判断
int ban_judgement(const int state[15][15],int x,int y,int color){
    if(color==1){//只有黑棋才有禁手
		struct Situation situation_ban = {0};
		int situa_ban[ALLWAY]={0};
		//转移坐标
		struct Point judgement;
		judgement.x=x;
		judgement.y=y;
		//四个方向判断禁手
		for(int direction = 0;direction < 4;direction++){//0:east,1:north,2:south-east,3:north-east
	        int typeban;
	        int chess[9]={0};
	        //获取方向上棋子分布
			getchess(chess,state,judgement,color,direction);
	        //判断棋局类型
			typeban=type_judge(chess);
			//判断是否出现禁手
			if(typeban==LONGER)
			    situa_ban[0]++;
			if(typeban==ALIVE4)
			    situa_ban[3]++;
			if(typeban==DIE4)
			    situa_ban[4]++;
			if(typeban==LOWDIE4)
			    situa_ban[5]++;
			if(typeban==ALIVE3)
			    situa_ban[6]++;
			if(typeban==TIAO3)
			    situa_ban[7]++;
			if(typeban==SPECIAL)
			    situa_ban[1]++;
	    }
		//如果有禁手出现，返回1，代表禁手！
		if(situa_ban[1]>=1)
		    return 1;
		if(situa_ban[0]>=1)
		    return 1;
		if(situa_ban[3]+situa_ban[4]+situa_ban[5]>=2 || situa_ban[3]>=2 || situa_ban[4]>=2 || situa_ban[5]>=2)
		    return 1;
		if(situa_ban[6]+situa_ban[7]>=2 || situa_ban[6]>=2 || situa_ban[7]>=2)
		    return 1;
	}
	//否则返回0，代表没有禁手
	return 0;
}
