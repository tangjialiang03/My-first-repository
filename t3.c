//������ Gobang   
//2006 �Ƽ��� 2020K8009907032
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define MAXLINE 1000//������󳤶�
#define SIZE 15//���̴�СΪ15*15
#define CHARSIZE 2//����ʹ�õ���GB2312���룬ÿһ�������ַ�ռ��2���ֽڡ�
#define MAXLEN 100//�������󳤶ȣ�һ���ò���
#define EMPTY 0//���̴˴�û������
#define ALLWAY 13
//��������Ҫ��
#define WIN5 0//����
#define ALIVE4 1//����
#define DIE4 2//����
#define LOWDIE4 3//������
#define ALIVE3 4//����
#define TIAO3 5//����
#define DIE3 6//����
#define ALIVE2 7//���
#define LOWALIVE2 8//�λ��
#define DIE2 9//�߶�
#define SAFE 10//��Χ�������γ����ӣ�û���κ���в
#define LONGER 11//����
#define SPECIAL 12//�������
//���ֺ���
//Alphago�����ֵȼ���ϸ
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
//���������ֵȼ���ϸ
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
//�������
#define Unaccepted 0 //���λ�����ӣ������£�������ͷ�

//ԭ������ 
char board0[SIZE][SIZE*CHARSIZE+1] = 
{
		"���өөөөөөөөөөөөө�",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"���۩۩۩۩۩۩۩۩۩۩۩۩۩�"
};

//��������
int board1[SIZE][SIZE];//���������ڼ�¼���̸��,�����е����̣���1��0��¼�����Ϣ 
char board2[SIZE][SIZE*CHARSIZE+1];//�����������������̣������ձ���ӡ���ն˵�����

//����
char black[]="��";//������
char blacknow[]="��"; //�������һ������λ��
char white[]="��";//������
char whitenow[]="��"; //�������һ������λ��

//������Ҫ����
struct Point{
	int x;
	int y;
};//��¼�����ӵ�����(x,y)
struct Situation {
	int longer;//����
	int special;//�������
	int win5;//����
	int alive4;//��4
    int die4;//��4
    int lowdie4;//�ͼ���4
	int alive3;//��3
	int tiao3;//��3
	int die3;//��3
	int alive2;//��2
	int lowalive2;//�ͼ���2
	int die2;//��2
	int safe;//��ȫ
};//��ǰλ�õ����ƣ���ָ���
int result = 0;//result=0������Ľ����У�result=1�������ֺ������ʤ����result=2������ְ���ʤ����result=3����ƽ�֣�result=4����������
int mode = 0;//��Ϸģʽѡ��1Ϊ���˴�ս��2Ϊ�˻���ս�������֣���3Ϊ���˴�ս�������֣�
int order = 1;//��¼����˳��1�����ֵ����壨���1����2�����ֵ����壨���2��
int counter=0;//��¼���岽������0��ʼ������ʵ��������1
char player1[MAXLEN];//���1�����壩�ǳ�
char player2[MAXLEN];//���2�����壩�ǳ�
char ai_name[]="Alphago";//�����������������ƣ�
struct Point next={0,0};//��¼������һ��������λ��
struct Point mybestpoint[SIZE*SIZE]={0};//��¼�����ϴ��һЩ��
int hisresult[SIZE*SIZE]={0};//��������һ�㣬��¼���ַ����ϴ��һЩ��
int tempboard1[15][15] = {0};//��¼��ʱ���̸�֣���board1�������ڰѵ�ǰ������ͳһ��
int tempboard2[15][15] = {0};//��¼�Լ�����֮�����̸��

//���ú���
void start(void);
void iniboard(void);//����������������ɵ��ܳ�ʼ��������ʼ������1����������2��������2��ӡ����
void iniboard1(void);//���������̳�ʼ��
void recordtoboard2(void);//���������̶�����ʵ����
void displayboard2(void);//����ʵ������ʾ���ն�
void PVP(void);//mode of person versus person
void PVC(void);//mode of person versus computer
void CVP(void);//mode of computer versus person
void inimidtemp(struct Point mybestpoint[],int hisresult[],int tempboard1[15][15],int tempboard2[15][15]);
void competing(void);
void playchess1(int order);//ģʽһ:��������˳�����塣�����������λ�ã�����λ�ö���board1
void playchess2(int order);//ģʽ������ʵ��
void playchess3(int order);//ģʽ������ʵ��
void read(char input[],int order);//�������롣��������������ʽ������board1
void currentchess();//����������ʾ����ǰ���ӣ�һ������ʧ
int judge();//�ж��Ƿ���һ����ʤ
int win5(int a,int b);//�ж�board1[a][b]�Ƿ�����ֹ�������ʤ��
int type_judge(int chess[9]);//�ж������λ�õĵ�ǰ�����������
int judgeSection(int x,int a,int b);//judge whether x is in the section [a,b]
int judgeSection_plus(int x,int a,int b);//judge whether x is in the section [a,b)
int score_cal(int a,int b);
void getchess(int chess[9],const int state[15][15],struct Point point,int color,int direction);//��õ�ǰ�����������
int score_judge(const int state[15][15],struct Point point,int color);//�ۺ��ĸ��������е�ǰλ�������ʽ�ķ���
int giveScore(struct Situation situation,int color);//�������Ƹ���
int larger(int x,int y);
struct Point bestpoint(const int myscore[15][15],const int hisscore[15][15],int color,struct Point mybestpoint[]);//�����ҺͶ��ֵķ�����ѡȡ��������λ��
struct Point nextpoint(const int boardnow[15][15],int color,int tempboard[15][15]);//���ݵ�ǰ���ƣ�������һ�����λ��
int ban_judgement(const int board1[15][15],int x, int y, int color); //�����ж�

//������
int main(){
	start();
	if(mode==1){//���˶�ս
	    PVP();
	}
	else if(mode==2){//�˻���ս
	    PVC();
	}
	else if(mode==3){//���˶�ս
	    CVP();
	}
    return 0;
}

void start(void){
	//�����սģʽ��1.���˶�ս 2.�˻���ս 3.���˶�ս
	printf("Welcome to Gobang Game!\nNow...please choose your desirable battle mode:\n1.People versus People  \n2.People versus AI  \n3.AI versus People\n");
	scanf("%d",&mode);
	//����Ƿ���սģʽ���룬��������ֱ������Ϸ�ģʽΪֹ
	while(mode!=1 && mode!=2 && mode!=3){
		printf("Illigal input! Please input your desirable mode again: ");
		scanf("%d",&mode);
	}
}

void PVP(void){//person versus person
	//����˫���ǳ�
	printf("Okey,so you'd better leave names of both sides here!\n");
    printf("player1: ");
	scanf("%s",player1);
	printf("player2: ");
	scanf("%s",player2);
	//�ܳ�ʼ��
	iniboard();
	//��ʼ��ս
    while(result==0){//��ս��
		playchess1(order);
        competing();	
	}
    switch(result){//���ն�������
	case 1:printf("%s wins!\n",player1);break;
	case 2:printf("%s wins!\n",player2);break;
	case 3:printf("draw��\n");break;
	case 4:printf("banned chess��\n");break;
	}
}

void PVC(void){//person versus computer
    //����˫���ǳ�
	printf("Please input Man's name: ");
	scanf("%s",player1);
	//�ܳ�ʼ��
	iniboard();
    //��ʼ��ս
	while(result==0){//��ս��
		playchess2(order);
        competing();
		inimidtemp(mybestpoint,hisresult,tempboard1,tempboard2);
	}
    switch(result){//�ն�������
	case 1:printf("%s wins!\n",player1);break;
	case 2:printf("%s wins!\n",ai_name);break;
	case 3:printf("draw!\n");break;
	case 4:printf("banned chess��\n");break;
	}
}

void CVP(void){//computer versus person
    //����˫���ǳ�
	printf("Please input Man's name: ");
	scanf("%s",player2);
	//�ܳ�ʼ��
	iniboard();
    //��ʼ��ս
	while(result==0){//��ս��
		playchess3(order);
        competing();
		inimidtemp(mybestpoint,hisresult,tempboard1,tempboard2);
	}
    switch(result){//�ն�������
	case 1:printf("%s wins!\n",ai_name);break;
	case 2:printf("%s wins!\n",player2);break;
	case 3:printf("draw!\n");break;
	case 4:printf("banned chess��\n");break;
	}
}

//���̳�ʼ��(����board1��board2�����������ն�)
void iniboard(void){
    iniboard1();
	recordtoboard2();
	displayboard2();
}

//��ʼ��board1����
void iniboard1(void){
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++)
		    board1[i][j]=0;
	}//��board1��0
}

//��board1�м�¼������λ�ã�ת����board2��
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

//�ն���ʾ����board2
void displayboard2(void){
	int i;
	//��һ��������
	system("clear");//����  
    printf("         Gobang:Work of �Ƽ���\n");
	//�ڶ�������board2�������Ļ��
	for(i=0;i<SIZE;i++){
		printf("%3d %s\n",SIZE-i,board2[i]);
	}
	//������������������һ����ĸA B .... 
	printf("   ");
	for(i=0;i<SIZE;i++)
	printf("%2c",'A'+i);
	printf("\n");
	if(mode==2 && order==1)
	    printf("\n%s: %d %c\n",ai_name,15-next.x,next.y+'A');
	if(mode==3 && order==2)
	    printf("\n%s: %d %c\n",ai_name,15-next.x,next.y+'A');
} 

//�����õ��м���������ص���ʼ״̬��ÿ�����궼ִ��һ��
void inimidtemp(struct Point mybestpoint[],int hisresult[],int tempboard1[15][15],int tempboard2[15][15]){
	int comboard[SIZE][SIZE]={0};//������Ԫ
	for(int i=0;i<255;i++){//��mybestpoint��hisresult����
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
		for(int j=0;j<SIZE;j++){//��tempboard����
			tempboard1[i][j]=comboard[i][j];
			tempboard2[i][j]=comboard[i][j];
		}
}

void competing(){//It is compteting now
	recordtoboard2();//��board1��¼��board2
	result=judge();//�жϽ��
	order=(order==1)?2:1;//�ı�˳��
	currentchess();//���ص���ʾ���һ��������
	displayboard2();//��ӡ����
	counter+=1;//������һ
}

//PVPģʽ�������������λ�ã�����λ�ö���board1
void playchess1(int order){
    char input[4];
	if(order==1){//���1����
		printf("player1 input: ");
        scanf("%s",input);//���ǽ���������ʽ���룺����h6��3h��14H��H10,������Ҫ*ר���ж�������ʽ�������ո��Թ������board1*
        read(input,order);
	}
	if(order==2){//���2����
		printf("player2 input: ");
        scanf("%s",input);
		read(input,order);
	}
}

//PVCģʽ������
void playchess2(int order){
	char input[4];
	if(order==2){//��������
		next = nextpoint(board1,order,tempboard1);//��ȡ��һ����λ��
	    board1[next.x][next.y]=22;
	}else if(order==1){//�������
		printf("%s:please input \n",player1);
		scanf("%s",input);
		read(input,order);
	}
}

//CVPģʽ������
void playchess3(int order){
	char input[4];
	if(order==1){//��������
		next = nextpoint(board1,order,tempboard1);//��ȡ��һ����λ��
	    board1[next.x][next.y]=11;
	}else if(order==2){//�������
		printf("%s:please input \n",player2);
		scanf("%s",input);
		read(input,order);
	}
}

//�������롣��������������ʽ������board1
void read(char input[],int order){
	extern int board1[SIZE][SIZE];//���ⲿ��board1����read������
	int midboard[SIZE][SIZE];
	////��ʼ�������롣�������input��ת��Ϊ��Ӧ��board1������coordinate
	int row=0,line=0;
	//�������input����ĸ��ͷ
	if(isalpha(input[0])){
        int i=1;
		//ȷ��������
		if(input[0]<'a')//input[0]��A~Z
		    line = input[0]-'A';
        if(input[0]>'Z')//input[0]��a~z
		    line = input[0]-'a';
		//ȷ��������
		for(i=1;isdigit(input[i]);i++)
            row = row*10 + (input[i]-'0');
	}
	//�������input�����ֿ�ͷ
	if(isdigit(input[0])){
        int j=0;
		//ȷ��������
		for(j=0;isdigit(input[j]);j++)
            row = row*10 + (input[j]-'0');
		//ȷ��������
		if(input[j]<'a')//input[0]��A~Z
		    line = input[j]-'A';
        if(input[j]>'Z')//input[0]��a~z
		    line = input[j]-'a';
	}////������������
	//�ж�����λ���Ƿ�Ϸ�
	//1.�ж������Ƿ�Խ������
    if(row>15 || line>=15 || row==0){  //A��O��Ӧline Ϊ0��14������������ֶ�Ӧ��row������������1��15
		printf("Where do you want to go?\n");
		printf("Now! You should input again: ");
		scanf("%s",input);
		read(input,order);
	}
	//2.�ж�����λ���Ƿ��Ѿ���������
    else if(board1[SIZE-row][line]!=0){
        printf("Wrong!\n");
		printf("Now! You should input again: ");
		scanf("%s",input);
		read(input,order);
	}
	//3.�ж��Ƿ�Ϊ���֣�ֻ���ģʽ1��ģʽ2������ִ���ӣ�
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
				        board1[i][j]=midboard[i][j];//����11��22��¼��ǰ����λ�ã��ȵ�currentchess()����ִ�е�ʱ�򷽱��жϣ���ʱ���ٸĻ�1��2
			}
	    }
	}
}

//�жϵ�ǰ����,�ı�state
int judge(){
	extern int board1[SIZE][SIZE];
	extern int order;
	int a,b=0;
	int ban,empty=0;

	//���board1����3������Ӵ�������
	for(int m=0;m<15;m++){
		for(int n=0;n<15;n++){
			if(board1[m][n]==3){
			   ban=1;
		   }
		}
	}
	if(ban==1)
	    return 4;//����

    //������̱���������board1û��0�ˣ���ƽ��
	for(int m=0;m<15;m++){
		for(int n=0;n<15;n++){
			if(board1[m][n]==0){
				empty=1;
			}  
		}
	}  
	if(empty==0)
	    return 3;//ƽ��

	//��Ϊ������һ����֮ǰstate��0�����Ի�ʤ�ı�Ҫ���������µ���һ�����������С�����ֻҪ����������Χ�ܲ��ܺ������弴�ɡ�
    if(order==1){
        for(int m=0;m<SIZE;m++){
		    for(int n=0;n<SIZE;n++){
			    if(board1[m][n]==11){
					a=m;
					b=n;//�������ӵ���������
				}
		    }
	    }
		if(win5(a,b))             
		    {return 1;}//�жϳ��壬ʤ��
		else{
			return 0;//��������
		}
	}
	if(order==2){
        for(int m=0;m<SIZE;m++){
		    for(int n=0;n<SIZE;n++){
			    if(board1[m][n]==22){
					a=m;
					b=n;//�������ӵ���������
				}
		    }
	    }
		if(win5(a,b))
		    {return 2;}//�жϳ��壬ʤ��
		else{
			return 0;//��������
		}
	}
}

//�ж�board1[a][b]�Ƿ�����ֹ��ɳ��� (���ڵ��������)
int win5(int a,int b){//ע�⣺a,b����������
    extern int board1[SIZE][SIZE];
	extern int order;
	int x,y;//���긨��ѭ����
	int left_num=0,right_num=0,up_num=0,down_num=0,left_up_num=0,left_down_num=0,right_up_num=0,right_down_num=0;//ͳ�ư˸������ж��ٸ�������ͬ����
	int east=0,north=0,en=0,es=0;//�ĸ����ܵ����ӷ��򣺺������򡢶����򡢶���������Ϊ1���������ӣ�Ϊ0������������
	//�����ж�
	if(order==1){
        //�����ж�
		for(y=b;y>0 && board1[a][y-1]==1 ;y--)                 
		    left_num+=1;
		for(y=b;y<SIZE-1 && board1[a][y+1]==1;y++)
		    right_num+=1;                               
		if(left_num + right_num + 1>=5)
		    east=1;
		//�����ж�
		for(x=a;x<SIZE-1 && board1[x+1][b]==1;x++)
		    down_num+=1;
		for(x=a;x>0 && board1[x-1][b]==1;x--)
		    up_num+=1;
		if(down_num + up_num + 1>=5)
		    north=1;
		//�������ж�
		for(x=a,y=b;x>0 && y<SIZE-1 && board1[x-1][y+1]==1;x--,y++)
		    right_up_num +=1;
		for(x=a,y=b;x<SIZE-1 && y>0 && board1[x+1][y-1]==1;x++,y--)
		    left_down_num +=1;
		if(right_up_num + left_down_num + 1>=5)
		    en=1;
		//�������ж�
		for(x=a,y=b;x<SIZE-1 && y<SIZE-1 && board1[x+1][y+1]==1;x++,y++)
		    right_down_num +=1;
		for(x=a,y=b;x>0 && y>0 && board1[x-1][y-1]==1;x--,y--)
		    left_up_num +=1;
		if(right_down_num + left_up_num + 1>=5)
		    es=1;
		//�������ж�
		if(east || north || en || es)
		    return 1;
		return 0;
	}
	
	//�����ж�
	if(order==2){
        //�����ж�
		for(y=b;y>0 && board1[a][y-1]==2 ;y--)                 
		    left_num+=1;
		for(y=b;y<SIZE-1 && board1[a][y+1]==2;y++)
		    right_num+=1;                               
		if(left_num + right_num + 1>=5)
		    east=1;
		//�����ж�
		for(x=a;x<SIZE-1 && board1[x+1][b]==2;x++)
		    down_num+=1;
		for(x=a;x>0 && board1[x-1][b]==2;x--)
		    up_num+=1;
		if(down_num + up_num + 1>=5)
		    north=1;
		//�������ж�
		for(x=a,y=b;x>0 && y<SIZE-1 && board1[x-1][y+1]==2;x--,y++)
		    right_up_num +=1;
		for(x=a,y=b;x<SIZE-1 && y>0 && board1[x+1][y-1]==2;x++,y--)
		    left_down_num +=1;
		if(right_up_num + left_down_num + 1>=5)
		    en=1;
		//�������ж�
		for(x=a,y=b;x<SIZE-1 && y<SIZE-1 && board1[x+1][y+1]==2;x++,y++)
		    right_down_num +=1;
		for(x=a,y=b;x>0 && y>0 && board1[x-1][y-1]==2;x--,y--)
		    left_up_num +=1;
		if(right_down_num + left_up_num + 1>=5)
		    es=1;
		//�������ж�
		if(east || north || en || es)
		    return 1;
		return 0;
	}
}

//������һ�����ӵ�λ��
struct Point nextpoint(const int boardnow[15][15],int color,int tempboard[15][15]){
	int hiscolor=(color==1)?2:1;
	int myscore[15][15]={0};
	int hisscore[15][15]={0};
	//�Ƿ��״�����
	int ini_play=0;
	int k=0,h=0;
	for(k=0;k<15;k++){
		for(h=0;h<15;h++){
			if(boardnow[k][h]>0){
				ini_play = 1;
			}
		}
	}
	if(ini_play==0){//��
        struct Point point={7,7};//������Ԫλ��
		return point;
	}
	//���������̸��Ƶ���ʱ�����ϣ��ĵ�ǰ����־
	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			switch(boardnow[i][j]){
				case 22:tempboard[i][j]=2;break;
				case 11:tempboard[i][j]=1;break;
				default:tempboard[i][j]=boardnow[i][j];break;
			}
		}
	}
	//���
	for(int i=0;i<15;i++)
	    for(int j=0;j<15;j++){
			struct Point point={i,j};
			//�ҵķ���
			myscore[i][j]=score_judge(tempboard,point,color);
			//���ַ���
			hisscore[i][j]=score_judge(tempboard,point,hiscolor);
		}
	//���ݷ�������������λ��
	return bestpoint(myscore,hisscore,color,mybestpoint);
}

//�ۺ��ĸ��������е�ǰ������Ƶķ���
int score_judge(const int state[15][15],struct Point point,int color){
	struct Situation situation={0};//��¼��ǰ���Ʊ���
    if(state[point.x][point.y]!=0)
	    return Unaccepted;//��λ�������ӣ������£����
	else if((ban_judgement(state,point.x,point.y,color)==1) ){
		if(mode==3)
		    return Unaccepted;//����λ�ò����£���Ҫ���ģʽ3����������
		else {
			
		}
	}else{
		for(int direction=0;direction<4;direction++){//0����1����2���ϣ�3����
            int type;
	        int chess[9]={0};
	        getchess(chess,state,point,color,direction);
	        type=type_judge(chess);//ȡ������
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
		return giveScore(situation,color);//�ó�����
	}
}

//��������
int giveScore(struct Situation situation,int color){
	int situa[ALLWAY]={0};
	//����ת��
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
	int grade;//����
	if(mode==2 && color==2){//�˻���ս����������
		grade=score_14;//��ʼ�׷�
		//�����Ǹ�������ĸ��ֹ���
		if(situa[0]>=1)
		    grade+=score_0;//����û�н���

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

	if(mode==2 && color==1){//�˻���ս��������
		grade=Hscore_14;//��ʼ�׷�
		//�����Ǹ�������ĸ��ֹ���
		if(situa[1]>=1)
		    return BAN;//����

		if(situa[3]+situa[4]+situa[5]>1 || situa[3]>1 || situa[4]+situa[5]>=2 || situa[0]>=1)
		    return BAN;//����

		if((situa[4]+situa[5]>=1 && situa[6]+situa[7]>=1) || (situa[3]>=1 && situa[6]+situa[7]>=1))
		    return Hscore_0;

		if(situa[2]>=1)
		    grade+=Hscore_1;

		if(situa[3]==1)
		    grade+=Hscore_2;

		if(situa[0]>=1)
		    return BAN;//����

		if(situa[6]+situa[7]>=2)
		    return BAN;//����

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
	if(mode==3 && color==1){//���˶�ս����������
		grade=score_14;//��ʼ�׷�
		//�����Ǹ�������ĸ��ֹ���
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
	if(mode==3 && color==2){//���˶�ս���˺���
		grade=Hscore_14;//��ʼ�׷�
        ///�����Ǹ�������ĸ��ֹ���
		if(situa[1]>=1)
		    grade+=Hscore_0;//���֣�û�н���

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


//��ȡ��ǰ�����������chess
void getchess(int chess[9],const int state[15][15],struct Point point,int color,int direction){
	int hiscolor;//�߽�������ʹ��
	hiscolor=(color==1)?2:1;//��ǶԷ�������ɫ
	chess[4]=color;//�����������������ϸõ���ɫ
	if(direction==0){//��������
		int i=point.x,j=1;
		while(j<=4){
			int row=point.y - j;
			if(row<0){
				for(;j<=4;j++)
				    chess[4-j]=hiscolor;//���������϶Է���ɫ
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
				    chess[4+j]=hiscolor;//���������϶Է���ɫ
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
	else if(direction==1){//��������
		int i=1,j=point.y;
		while(i<=4){
			int line=point.x - i;
			if(line<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;//���������϶Է���ɫ
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
				    chess[4+i]=hiscolor;//���������϶Է���ɫ
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
	else if(direction==2){//б�����ơ���1
		int i=1,j=1;
		while(i<=4){
			int line=point.x - i;
			int row=point.y - j;
			if(line<0||row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;//���������϶Է���ɫ
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
				    chess[4+i]=hiscolor;//���������϶Է���ɫ
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
	else if(direction==3){//б�����ơ���2
		int i=1,j=1;
		while(i<=4){
			int line=point.x + i;
			int row=point.y - j;
			if(line>14||row<0){
				for(;i<=4;i++)
				    chess[4-i]=hiscolor;//���������϶Է���ɫ
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
				    chess[4+i]=hiscolor;//���������϶Է���ɫ
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

//�����ĸ������ϵ��������飬�жϳ�������
int type_judge(int chess[9]){
	int mycolor=chess[4];//��ȡ�Լ���ɫ
	int hiscolor,i;
	int left,right;
	int count=1;
	int same_a=1;//��¼��ͬ��Ϣ
	int same_b=1;//��¼��ͬ��Ϣ
    //define SOME of the ban-situations�����ֽ������
    int chess1[9]={EMPTY,mycolor,EMPTY,mycolor,mycolor,mycolor,EMPTY,mycolor,EMPTY};
	int chess2[9]={EMPTY,mycolor,mycolor,EMPTY,mycolor,mycolor,EMPTY,mycolor,mycolor};
	int chess3[9]={mycolor,mycolor,EMPTY,mycolor,mycolor,EMPTY,mycolor,mycolor,EMPTY};
	int chess4[9]={mycolor,mycolor,mycolor,EMPTY,mycolor,EMPTY,mycolor,mycolor,mycolor};
	//decide the color��ȷ���Լ��Ͷ�����ɫ
	if(mycolor==1)
	    hiscolor=2;
	else if(mycolor==2)
        hiscolor=1;
	//count the number of my chess(the mid only)���жϴ����Ŀ�ʼ���������������ֱ��ж����Լ������ӣ�ֻ��¼������������
	for(int i=1;i<=4;i++){//������
		if(chess[4-i]==mycolor)
		    count++;
		else{
			left=4-i;
			break;
		}
	}
	for(int i=1;i<=4;i++){//������
		if(chess[4+i]==mycolor)
		    count++;
		else{
			right=4+i;
			break;
		}
	}
    //judge different situations
	//���ж��������
	if(count==3){//������������������
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
            return SPECIAL;//�������
		}
		same_a=1;   
	}

	if(count==2){//������������������
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
            return SPECIAL;//�������
		}
		same_a=same_b=1;
	}

	if(count==1){//������һ����������
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
			return SPECIAL;//�������
		}
		same_a=1;
	}
    //���ж��������
	if(count>=5)//�����������������
	    if(count>5)
		    return LONGER;//����
		else
		    return WIN5;//����

	if(count==4){//�������ĸ���������
		if(chess[left]==EMPTY && chess[right]==EMPTY)
			return ALIVE4;//����
		else if(chess[left]==hiscolor && chess[right]==hiscolor)
		    return SAFE;//��ȫ
		else if(chess[left]==EMPTY || chess[right]==EMPTY)
		    return DIE4;//����
	}

	if(count==3){//������������������
	    //������һЩ�����Ե��ж�
		if(chess[left]==EMPTY && chess[right]==EMPTY){
			if(chess[left-1]==hiscolor && chess[right+1]==hiscolor)
			    return DIE3;//����
			else if(chess[left-1]==mycolor && chess[right+1]==mycolor)
			    return WIN5;//����
			else if(chess[left-1]==mycolor || chess[right+1]==mycolor)
			    return LOWDIE4;//������
			else if(chess[left-1]==EMPTY || chess[right+1]==EMPTY)
			    return ALIVE3;//����
		}
		else if(chess[left]==hiscolor && chess[right]==hiscolor){
			return SAFE;//��ȫ
		}
		else if(chess[left]==EMPTY || chess[right]==EMPTY){
			if(chess[left]==hiscolor){
				if(chess[right+1]==hiscolor)
				    return SAFE;//��ȫ
				if(chess[right+1]==EMPTY)
				    return DIE3;//����
				if(chess[right+1]==mycolor)
				    return LOWDIE4;//������
			}
			if(chess[right]==hiscolor){
				if(chess[left-1]==hiscolor)
				    return SAFE;//��ȫ
				if(chess[left-1]==EMPTY)
				    return DIE3;//����
				if(chess[left-1]==mycolor)
				    return LOWDIE4;//������
			}
		}
	}

	if(count==2){//������������������
		if(chess[left]==EMPTY && chess[right]==EMPTY){
			if((chess[right+1]==EMPTY && chess[right+2]==mycolor)||
			    (chess[left-1]==EMPTY && chess[left+2]==mycolor))
				return DIE3;//����
			else if(chess[left-1]==EMPTY && chess[right+1]==EMPTY)
			    return ALIVE2;//���

			if((chess[right+1]==mycolor && chess[right+2]==hiscolor)||
			    (chess[left-1]==mycolor && chess[left-2]==hiscolor))
				return DIE3;//����

			if((chess[right+1]==mycolor && chess[right+2]==mycolor)||
			    (chess[left-1]==mycolor && chess[left-2]==mycolor))
				return LOWDIE4;//������
			
			if((chess[right+1]==mycolor && chess[right+2]==EMPTY)||
			    (chess[left-1]==mycolor && chess[left-2]==EMPTY))
				return TIAO3;//����
		}
		else if(chess[left]==hiscolor && chess[right]==hiscolor){
			return SAFE;//��ȫ
		}
		else if(chess[left]==EMPTY || chess[right]==EMPTY){
			if(chess[left]==hiscolor){
				if(chess[right+1]==hiscolor || chess[right+2]==hiscolor){
					return SAFE;//��ȫ
				}
				else if(chess[right+1]==EMPTY && chess[right+2]==EMPTY){
					return DIE2;//����
				}
				else if(chess[right+1]==mycolor && chess[right+2]==mycolor){
					return LOWDIE4;//������
				}
				else if(chess[right+1]==mycolor || chess[right+2]==mycolor){
					return DIE3;//����
				}
			}
			if(chess[right]==hiscolor){
				if(chess[left-1]==hiscolor || chess[left-2]==hiscolor){
					return SAFE;//��ȫ
				}
				else if(chess[left-1]==EMPTY && chess[left-2]==EMPTY){
					return DIE2;//����
				}
				else if(chess[left-1]==mycolor && chess[left-2]==mycolor){
					return LOWDIE4;//������
				}
				else if(chess[left-1]==mycolor || chess[left-2]==mycolor){
					return DIE3;//����
				}
			}
		}
	}

	if(count==1){//������һ����������
		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==mycolor && chess[left-3]==mycolor)
		    return LOWDIE4;//������
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==mycolor && chess[right+3]==mycolor)
		    return LOWDIE4;//������
		
		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==mycolor && chess[left-3]==EMPTY && chess[right]==EMPTY)
		    return TIAO3;//����
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==mycolor && chess[right+3]==EMPTY && chess[left]==EMPTY)
		    return TIAO3;//����

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==mycolor && chess[left-3]==hiscolor && chess[right]==EMPTY)
		    return DIE3;//����
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==mycolor && chess[right+3]==hiscolor && chess[left]==EMPTY)
		    return DIE3;//����

		if(chess[left]==EMPTY && chess[left-1]==EMPTY && chess[left-2]==mycolor && chess[left-3]==mycolor)
		    return DIE3;//����
		if(chess[right]==EMPTY && chess[right+1]==EMPTY && chess[right+2]==mycolor && chess[right+3]==mycolor)
		    return DIE3;//����

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==EMPTY && chess[left-3]==mycolor)
		    return DIE3;//����
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==EMPTY && chess[right+3]==mycolor)
		    return DIE3;//����
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[left]==EMPTY && chess[left-1]==mycolor)
		    return DIE3;//����

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==EMPTY && chess[left-3]==EMPTY && chess[right]==EMPTY)
		    return LOWALIVE2;//�λ��
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==EMPTY && chess[right+3]==EMPTY && chess[left]==EMPTY)
		    return LOWALIVE2;//�λ��

		if(chess[left]==EMPTY && chess[left-1]==EMPTY && chess[left-2]==mycolor && chess[left-3]==EMPTY && chess[right]==EMPTY)
		    return LOWALIVE2;//�λ��
		if(chess[right]==EMPTY && chess[right+1]==EMPTY && chess[right+2]==mycolor && chess[right+3]==EMPTY && chess[left]==EMPTY)
		    return LOWALIVE2;//�λ��

		if(chess[left]==EMPTY && chess[left-1]==mycolor && chess[left-2]==EMPTY && chess[left-3]==EMPTY && chess[right]==hiscolor)
		    return DIE2;//����
		if(chess[right]==EMPTY && chess[right+1]==mycolor && chess[right+2]==EMPTY && chess[right+3]==EMPTY && chess[left]==hiscolor)
		    return DIE2;//����

		if(chess[left]==EMPTY && chess[left-1]==EMPTY && chess[left-2]==mycolor && chess[left-3]==EMPTY && chess[right]==hiscolor)
		    return DIE2;//����
		if(chess[right]==EMPTY && chess[right+1]==EMPTY && chess[right+2]==mycolor && chess[right+3]==EMPTY && chess[left]==hiscolor)
		    return DIE2;//����
	}
	return SAFE;//��ȫ
}

//�ж��������λ�ã�˼·�����Լ��Ͷ��ֲַ�ϴ�ʱ���Լ��������ĵ��£�ѡ��һ�����䣬���������Լ��Ͷ��ֶַ��ܸߵ�λ����Ϊ��ѡ
//֮�󣬴ӱ�ѡ�����ҳ��ö�����������С�ĵ㣬����ж��������һ�����Լ����ֵĵ���Ϊ��ѵ�
struct Point bestpoint(const int myscore[15][15],const int hisscore[15][15],int color,struct Point mybestpoint[]){
	//��¼�Լ��Ͷ������ķ���
	int mymaxscore=0;
	int hismaxscore=0;
	int maxscore=0;
	int bestnum=0;
	//һЩ�м丨������
	int count=0;
    int pointnum=SIZE*SIZE;
	int together[SIZE][SIZE]={0};
	//�Լ��Ͷ��ֵ������
	struct Point target={0,0};
	struct Point histarget={0,0};
	//��¼���Ƶ�
	struct Point mypointarray[pointnum];
	struct Point hispointarray[pointnum];
	struct Point pointarray[pointnum];

	for(int k=0;k<pointnum;k++){//��ʼ�����Ƶ�����
	    mypointarray[k].x=0,mypointarray[k].y=0;
		hispointarray[k].x=0,hispointarray[k].y=0;
		pointarray[k].x=0,pointarray[k].y=0;
	}

	for(int i=0;i<SIZE;i++)
	    for(int j=0;j<SIZE;j++){//�������
		    together[i][j] = score_cal(myscore[i][j],hisscore[i][j]);
			if(hisscore[i][j]>hismaxscore){//�Ҷ�������
				hismaxscore=hisscore[i][j];
			}
			if(myscore[i][j]>mymaxscore){//���Լ�����
				mymaxscore=myscore[i][j];
			}
		}
	
	if(1){
		for(int i=0;i<15;i++)
	        for(int j=0;j<15;j++){
			    if(larger(together[i][j],maxscore)){//����������
				    maxscore=together[i][j];
			    }
		    }
		//������ѡ��һ���������䣬�����������е�ȡ������Ϊ��һ��ı�ѡ
		if(larger(maxscore,3000) || maxscore==3000){//����1
		    for(int i=0;i<15;i++)
                for(int j=0;j<15;j++){
		            if(judgeSection(together[i][j],maxscore-800,maxscore)){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
		        }
        }
		else if(judgeSection(maxscore,1400,3000)){//����2
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){
					if(judgeSection(together[i][j],maxscore-500,maxscore)){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
				}
		}
		else if(judgeSection(maxscore,500,1400)){//����3
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){
					if(judgeSection(together[i][j],maxscore-200,maxscore)){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
				}
		}
		else{//����4
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){
					if(together[i][j]==maxscore){
						struct Point totarget={i,j};
			            pointarray[count]=totarget;
						count+=1;
		            }
				}
		}
		for(int i=0;i<count;i++){//����mybestpoint���飬��Ϊ��ѡ
			mybestpoint[bestnum]=pointarray[i];
			bestnum+=1;
		}
	}
	
	if(bestnum==1){//ֻ��һ���ֱȽϸߵĵ㣬����ֶ��ܵ�
		return mybestpoint[0];
	}
	else if(larger(bestnum,1)){//�ж�������ϸߵĵ�
		for(int m=0;m<bestnum;m++){
			int hiscolor=(color==1)?2:1;
			int hisscore[15][15]={0};
			tempboard1[mybestpoint[m].x][mybestpoint[m].y]=color;
			for(int i=0;i<15;i++)
			    for(int j=0;j<15;j++){//Ϊ�����жϷ���
					struct Point point={i,j};
					int score;
					hisscore[i][j]=score_judge(tempboard1,point,hiscolor);
				}
			for(int i=0;i<15;i++)
	            for(int j=0;j<15;j++){//ѡ���������Ƶ㣬����hisresult
		            if(hisscore[i][j]>maxscore){
				        hisresult[m]=hisscore[i][j];
			        }
		        }
		}
		//�м����������ڶ�������
        int min=hisresult[0];
		int def=0;//�±�
		int s[100]={0};
		int num_1=0;
		//ȡ��������������С�ĵ�
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
		//��������ĵ㲻ֹһ��
		if(num_1!=0){
			int max,i,def_2;
			i=0;
			max=0;
			//������Щ�㵽�����У��ҳ��Լ��ڶ���������ĵ㣬��Ϊ���ս��
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
		//�����������ĳЩ���ⲽ��
		if(mode==3){
			if(counter==2){//CVPģʽ�µ����������Ժܶ࣬���ѡһ������
                srand((unsigned)time(NULL));
			    def=rand()%(bestnum);    
			}
			else if(counter!=2){
                
			}
		}
		if(mode==2){
			if(counter==1){//PVCģʽ�µڶ��������Ժܶ࣬���ѡһ������
                srand((unsigned)time(NULL));
			    def=rand()%(bestnum);    
			}
			else if(counter!=1){
                
			}
		}
		return mybestpoint[def];//������ѵ�
	}
}

//�жϴ�С����
int larger(int x,int y){
	if(x>y)
	    return 1;
	else{
		return 0;
	}
}

//�ֲ�Ŵ���
//���ؼ��㷽���������ƷŴ�
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

//�����жϣ��ж�һ����x�Ƿ�������[a,b]��
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

//�����ж�֮���װ棬�ж�һ����x�Ƿ�������[a,b)��
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

//��������������Ǻ���ʾ����һ�������ӣ���һ������ʧ
void currentchess(){
	extern int board1[SIZE][SIZE];
	extern char board2[SIZE][SIZE*CHARSIZE+1];
	extern char blacknow[],whitenow[];
    for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
            if(board1[i][j]==11){//���������Ļغ���
			    board2[i][j*CHARSIZE]=blacknow[0];
			    board2[i][j*CHARSIZE+1]=blacknow[1];
			    board1[i][j]=1;
		    }
		    if(board1[i][j]==22){//���������Ļذ���
			    board2[i][j*CHARSIZE]=whitenow[0];
			    board2[i][j*CHARSIZE+1]=whitenow[1];
			    board1[i][j]=2;
		    }
		}
	}
}

//�����ж�
int ban_judgement(const int state[15][15],int x,int y,int color){
    if(color==1){//ֻ�к�����н���
		struct Situation situation_ban = {0};
		int situa_ban[ALLWAY]={0};
		//ת������
		struct Point judgement;
		judgement.x=x;
		judgement.y=y;
		//�ĸ������жϽ���
		for(int direction = 0;direction < 4;direction++){//0:east,1:north,2:south-east,3:north-east
	        int typeban;
	        int chess[9]={0};
	        //��ȡ���������ӷֲ�
			getchess(chess,state,judgement,color,direction);
	        //�ж��������
			typeban=type_judge(chess);
			//�ж��Ƿ���ֽ���
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
		//����н��ֳ��֣�����1��������֣�
		if(situa_ban[1]>=1)
		    return 1;
		if(situa_ban[0]>=1)
		    return 1;
		if(situa_ban[3]+situa_ban[4]+situa_ban[5]>=2 || situa_ban[3]>=2 || situa_ban[4]>=2 || situa_ban[5]>=2)
		    return 1;
		if(situa_ban[6]+situa_ban[7]>=2 || situa_ban[6]>=2 || situa_ban[7]>=2)
		    return 1;
	}
	//���򷵻�0������û�н���
	return 0;
}
