/******************************
Write By XPF:
Time :2018.01.13
key:  1,2,3,...m
child:0,1,2,3..m
//
界面：重新布局，定位光标，隐藏与显示
页面跳转： 添加跳转页面动画
*******************************/
#include "Btree.h"
#include <time.h>
#include <conio.h>
#include <windows.h>
using namespace std;
typedef long long ll;
int p=0;//注册人数
int book_num=0;  //书库现存图书的数量
const int book_max=1e6+5; //书库的最大容量
typedef struct Time     //存储系统时间
{
    int year;
    int mon;
    int day;
    void show()
    {
        cout<<year<<"年"<<mon<<"月"<<day<<"日";
    }
    Time()
    {
        year=mon=day=0;
    }
    Time(int q,int w,int e):year(q),mon(w),day(e){}
};
typedef struct nnode        //用户所看的图书
{
    int id ;
    nnode *next;
}*user_read;
typedef struct user    //Write By XPF
{

    char user_id[20];
    char user_name[20];
    char user_password[20];
//    int max_book;//可以借阅的最多书籍
    int now_book;//现在已借阅的图书数量  //最多8本
    user_read read;//已读的图书
    user()
    {
        strcpy(user_id," ");
        strcpy(user_name," ");
        strcpy(user_password," ");
        now_book=0;
        read=NULL;
    }
    user (char q[],char w[],char e[]){
        strcpy(user_id,q);
        strcpy(user_name,w);
        strcpy(user_password,e);
        now_book=0;
        read=NULL;
    }
    void show()    ////////////////////////////////////////**************************************
    {
        cout<<"Your name is : "<<user_name<<endl;
        cout<<"Your Id is : "<<user_id<<endl;
        cout<<"YOur password is : "<<user_password<<endl;
    }
} User,user;
user  client[1000];// 注册用户
user admin;
//user a;//////********************************************************************临时定义
struct node // user ,借阅信息，预约信息        //***缺少时间处理函数 *************************** 以写（李艳欣）
{

   Time brorrow_time ;
   Time return_time;
   bool be_pre;
   Time pre_brorrow_time;
   Time  get_time;
   user userdata;
   user userdata_two;
    node *next;
    node ()
    {
      next=NULL;
      be_pre=0;
    }
};
typedef struct book     //WRite By XPF
{

    int   book_id;//书号           默认为 0
    char book_name[20];//书名      默认为 " "
    char book_author[20];//作者    默认为 " "
    char book_pulish[20];//出版社  默认为 " "
    char book_time[21];//出版时间  默认为 " "
    double book_values;//书的定价  默认为 0
    int  book_num_now;//现存量     默认为 0
    int book_num_all;//总存量      默认为 0
    int book_num_pre;//预约的图书  默认为0
    bool book_judge;//是否为添加图书  默认为false
    node *head;
    book()
    {
        book_id=0;
        strcpy(book_author," ");
        strcpy(book_name," ");
        strcpy(book_pulish," ");
        strcpy(book_time," ");
        book_values=0;
        book_num_all=book_num_now=book_num_pre=0;
        book_judge=false;
        head=NULL;
    }

    book(int q,char w[],char e[],char r[],char t[],int a,double b)
    {
        book_id=q;
        strcpy(book_name,w);
        strcpy(book_author,e);
        strcpy(book_pulish,r);
        strcpy(book_time,t);
        book_num_all=book_num_now=a;
        book_values=b;
        book_judge=1;
        book_num_pre=0;
        head=NULL;
    }
    void show ()// 改
    {
        cout<<"\t\t\t     "<<setiosflags(ios_base::right)<<setw(6)<<book_id
            <<resetiosflags(ios_base::right)<<setw(10)<<book_name
            <<setw(10)<<book_author<<setw(10)<<book_pulish<<setw(12)
            <<book_time<<setw(8)<<book_values<<setw(10)<<book_num_now
            <<setw(10)<<book_num_all<<endl;
    }

} Book,book;
book lib_book[book_max]; //书库信息
///////////////SYSTEM//////////////////
void SetColor(unsigned short TextColor );
void Welcome_Menu();
void Welcome(int color);
inline void Goto_Other_window();//前往其他页面
void Login_Admin(Btree &tree);      //管理员登录界面
void Login_User(Btree &tree,user &admin);     //借阅者登录页面
void Reg_Menu(Btree &tree,user &admin);        //注册页面
void Entry_Menu(Btree &tree,user &admin);      //选择页面
void User_Menu(Btree &tree ,user &admin);       //借阅者页面
void First_Menu();      //进入界面
void Out_Menu(Btree &tree);         //退出界面
void Admin_Menu(Btree &tree );        //管理员界面
void Head();              //标题
void gotoxy(short x,short y);//定位函数
bool cmp_time(Time &a,Time &b);// 比较两个时间的大小 若a>=b ,返回1 ，否则返回0
void HideCursor();//隐藏光标
int isLeap(int year);// 判断闰年
Time Count_time(Time a);//获取归还天数
inline void GetDate(tm &tim) ;//获取系统时间
inline void Give_time(Time &a);//传递时间给结构体 time
inline bool user_cmp(user &a,user &b);// 判断两个用户信息是否一样*************************************************
bool Linklist_Search(node *head);       //遍历链表的查询 *****************************************************
inline void Init_lib(); //存测试数据             ********后期可以改成读文件
inline void Print_book(book &b);//打印图书信息        //格式太丑 更换
inline void Info_book_styple();//打印图书的基本信息项
void Print_book_All_information(book &b);//打印图书的所有信息
void Btree_INorder(Btree &tree); //    中序遍历
void Print_Linklist(node *head);//打印链表节点信息
void Add_Book(Btree &tree);//图书采集主页面
void add_book(Btree &tree);//添加图书详情页
void Delete_book(Btree &tree);//删除图书主页面
void delte_book(Btree  &tree);//图书删除详情页
int author_book_id[book_max];//某位作者所有图书ID
int author_book_num;//某位作者图书的数量
void Author_Zone(Btree &tree);//作者专区
/******************************
可添加 借阅排行榜，强行用快排
******************************/
void Reader_Push();//读者推荐图书  /************************************************************
void Btree_Inorder_Search(Btree &tree,char name[]);//遍历数组查找
void Info_book(Btree &tree);//查询图书信息主界面
void info_book(Btree &tree);//图书信息详情页
void Btree_Inorder_Info(Btree &tree);//查询图书的所有信息
void Info_book_find(Btree &tree);//查询图书信息（单本）
void Brorrow_book(Btree &tree,user &admin);//图书借阅界面
void brorrow_book(Btree &tree,user &admin);//图书借阅详情页
void Return_book(Btree &tree,user &admin);//图书归还主页面
void return_book(Btree &tree,user &admin);//图书归还详情页
void Pre_Brorrow_book(Btree &tree,int id,user &admin);//图书预约主界面
void pre_brorrow_book(Btree &tree,int id,user &admin);//图书预约详情页
//需要判断闰年归还的具体时间
void test();//开发测试程序
void ShowCursor();//显示光标
int main ()
{
    system("title:图书管理");
    First_Menu();
    return 0;
}
void Welcome_Menu()
{
    int i;
    for (i=0;i<=10;i++)
    {
        Welcome(i%16);
        Sleep(200);
    }
}

void Welcome(int color) //欢迎界面具体内容
{
    int x=36,y=6;
    SetColor(color);
    gotoxy(x,y);
    printf("  ::∴★∵**☆．∴★∵**☆．	");
    gotoxy(x,y+1);
    printf("  █████    ．☆．∵★∵∴☆． ");
    gotoxy(x,y+2);
    printf("  █田█田█．*☆．∴★∵．");
    gotoxy(x,y+3);
    printf("  █田█田█．*☆．∴★∵．");
    gotoxy(x,y+4);
    printf("  █田█田█    ．★∵∴☆．★∵∴．");
    gotoxy(x,y+5);
    printf("  █田█田█．★∵∴☆．★..**．");
    gotoxy(x,y+6);
	printf("  █田█田█  ．★★∵∴☆．★*☆．");
	gotoxy(x,y+7);
	printf("  █████．");
	gotoxy(x,y+8);
	printf("◢██□██◣．~~~~~*^_^*");
}

void 	SetColor(unsigned short TextColor )// 设置字体和背景颜色
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon,TextColor);
}
inline void Goto_Other_window()     //转载界面动画
{
    int i;
    for (i = 0; i<11; i++)
    {
        Sleep(100);
        printf("█ ");
    }
}
void test() //测试阶段
{
    //    int i,j,k;
//    Init_lib();
//    cout<<endl;
////    Print_book_All_information(lib_book[1]);
////    Time a;
////    Give_time(a);
////    Time aa(2019,01,01);
////    cout<<cmp_time(aa,a)<<endl;
//
////    Give_time(a);
////    a.show();
////    Time b;
////    b=Count_time(a);
////    b.show();
//////    if (lib_book[1].head==NULL)
//////    {
//////        node *p;
//////        int x;
//////        lib_book[1].head=new node ;
//////        lib_book[1].head->next=NULL;
//////        for(i=0;i<10;i++)
//////        {
//////            p=new node ;
//////            cin>>p->data;
//////            p->next=lib_book[1].head->next;
//////            lib_book[1].head->next=p;
//////            cout<<"****"<<endl;
//////        }
//////        Print_Linklist(lib_book[1].head);
//////    }
//    Btree tree=NULL;
//    for (i=1; i<=10; i++)
//        Btree_Insert(tree,i);
////        Info_book(tree);
////        Author_Zone(tree);
//     user admin("123","456_","123456");
////     Delete_book(tree);
//    Brorrow_book(tree,admin);
////    Print_book_All_information(lib_book[1]);
////    Return_book(tree,admin);
////     Info_book(tree);
}
void Head() //标题
{
    int		x = 25,y = 2;
    tm		date;
    HideCursor();
    GetDate(date);
    gotoxy(x,y);
    printf("╔════════════════════════════════════════════════════════╗");
    gotoxy(x,y+1);
    printf("║                                                        ║");
    gotoxy(x,y+2);
    printf("║                                                        ║");
    gotoxy(x,y+3);
    printf("║                     图书管理系统                       ║");
    gotoxy(x,y+4);
    printf("║                                                        ║");
    gotoxy(x,y+5);
    printf("║                                                        ║");
    gotoxy(x,y+6);
    printf("║                                                        ║");
    gotoxy(x,y+7);
    printf("╚════════════════════════════════════════════════════════╝");
    while(!_kbhit())												// 当未检测到按键
    {
        gotoxy(x+17,y+6);
        HideCursor();
        GetDate(date);												// 获取时间
        printf("%d年%2d月%2d日 %02d:%02d:%02d ",date.tm_year,date.tm_mon,date.tm_mday,date.tm_hour,date.tm_min,date.tm_sec);
        Sleep(1000);													// 显示时间
    }
}
void First_Menu()         //进入界面            /////需要修改
{

    //预处理
    Btree tree=NULL;
    user admin;
    HideCursor();
    Welcome_Menu();
    gotoxy(36,16);
//    SetColor(16);
    printf("即将进入图书管理系统！");
    gotoxy(36,18);
    Goto_Other_window();
    system("cls");
    SetColor(14);
    Init_lib();
    for (int i=1;i<=10;i++)
        Btree_Insert(tree,i);
    Entry_Menu(tree,admin);
//    Entry_Menu(tree,admin);
}
void Login_Admin(Btree &tree)      //管理员登陆页面            //x 36  x+6 x+18    x+6 x+12 x+14     已改 ，******************** 未写完
{
    int x=36;
    char key[20],ch;
//    HideCursor();
    gotoxy(x,12);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x,13);
    printf("|                                   |");
    gotoxy(x,14);
    printf("|                                   |");
    gotoxy(x,15);

    printf("|                                   |");
    gotoxy(x,16);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x+6,14);
    printf("管理员密码：");
    Head();
    gotoxy(x+18,14);
    ShowCursor();
    scanf("%s",key);
    if(strcmp(key,"000000")==0)
    {
        gotoxy(x+6,20);
        printf("密码正确，即将前往管理员界面！   ");
        gotoxy(x+6,22);
        Goto_Other_window();
        system("cls");
        Admin_Menu(tree);
    }
    else
    {
        gotoxy(x+6,20);
        printf("密码错误，是否重新输入！      ");
        gotoxy(x+12,22);
        printf("Y or N  ");
        gotoxy(x+14,24);
        while (ch=getch())
        {
            if (ch=='Y'||ch=='y'||ch=='N'||ch=='n')
                break;
        }
        if(ch=='Y'||ch=='y')
            {
                system("cls");
                Login_Admin(tree);
            }
        else
        {
            gotoxy(x+6,20);
            printf("即将返回主操作界面！        ");
            gotoxy(x+4,22);
            Goto_Other_window();
            system("cls");
            Entry_Menu(tree,admin);
        }
    }
}
void Login_User(Btree &tree,user &admin)       //用户登录                   //************************************未写完
{
    char id[20],key[20],ch;
    int i;
    HideCursor();
    gotoxy(36,12);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(36,13);
    printf("|                                   |");
    gotoxy(36,14);
    printf("|                                   |");
    gotoxy(36,15);
    printf("|                                   |");
    gotoxy(36,16);
    printf("|                                   |");
    gotoxy(36,17);
    printf("|                                   |");
    gotoxy(36,18);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    int x=36;
    gotoxy(x+6,14);
    printf("用户ID : ");

    gotoxy(x+6,16);
    printf("  密码 : ");
     Head();
    gotoxy(x+16,14);
    ShowCursor();
    scanf("%s",id);
    gotoxy(x+16,16);
    scanf("%s",key);


    for(i=0; i<p; i++)
    {
        if(strcmp(id,client[i].user_id)==0)
        {
            if(strcmp(key,client[i].user_password)==0)
            {
                gotoxy(x+8,20);
                printf("即将返回用户主界面！      ");
                gotoxy(x+4,22);
                Goto_Other_window();
                system("cls");
                User_Menu(tree,admin);     //跳转用户
                break;
            }
            gotoxy(x+6,20);
            printf("密码错误，是否重新输入！");
            gotoxy(x+18,22);

            while (ch=getch())
            {
                if (ch=='Y'||ch=='N'||ch=='y'||ch=='n')
                    break;
            }
            if(ch=='Y'||ch=='y')
                {
                       system("cls");
                    Login_User(tree,admin);
                }
            else
            {
                    gotoxy(x+8,20);
                    printf("即将返回主操作界面！      ");
                    gotoxy(x+4,22);
                    Goto_Other_window();
                    system("cls");
                    Entry_Menu(tree,admin);
            }
            break;
        }
    }
    if(i==p)
    {
        gotoxy(x+4,20);
        printf("没有该用户，请问您是否要注册？                  ");
        gotoxy(x+14,22);
        printf("Y or N");
        gotoxy(x+16,24);
        while (ch=getch())
        {
            if (ch=='Y'||ch=='N'||ch=='y'||ch=='n')
                break;
        }
        if(ch=='Y'||ch=='y')
        {
                gotoxy(x+8,20);
                printf("即将返回用户注册界面！           ");
                gotoxy(x+4,22);
                Goto_Other_window();
                system("cls");
            Reg_Menu(tree,admin);
        }
        else
        {
             gotoxy(x+8,20);
             printf("即将返回主操作界面！          ");
             gotoxy(x+4,22);
             Goto_Other_window();
                system("cls");
                Entry_Menu(tree,admin);
        }
    }
}
void Reg_Menu(Btree &tree,user &admin)         //用户注册                   //已改 ，到底是有多懒，gotoxy都不用   新添 showcuror函数
{
    char k;
   int x =36;
    gotoxy(x,12);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x,13);
    printf("|                                   |");
    gotoxy(x,14);
    printf("|                                   |");
    gotoxy(x,15);
    printf("|                                   |");
    gotoxy(x,16);
    printf("|                                   |");
    gotoxy(x,17);
    printf("|                                   |");
    gotoxy(x,18);
    printf("|                                   |");
    gotoxy(x,19);
    printf("|                                   |");
    gotoxy(x,20);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x+6,14);
    printf("用户ID : ");
    gotoxy(x+6,16);
    printf("  姓名 : ");
    gotoxy(x+6,18);
    printf("  密码 : ");
    Head();
    gotoxy(x+18,14);
    ShowCursor();
    scanf("%s",client[p].user_id);
    gotoxy(x+18,16);
    ShowCursor();
    scanf("%s",client[p].user_name);
    gotoxy(x+18,18);
    ShowCursor();
    scanf("%s",client[p++].user_password);
    gotoxy(x+13,22);
    admin=client[p-1];
    printf("确认注册?\n");
    gotoxy(x+13,24);
    printf(" Y or N");
    user caption =client[p-1];
    gotoxy(x+16,26);
    ShowCursor();
    while (k=getchar())
    {
        if (k=='Y'||k=='N'||k=='y'||k=='n')
            break;
    }
    //    k=getchar();      /////////////////////////////////////////

    if(k=='Y'||k=='y')
    {
        gotoxy(x+10,26);
        HideCursor();
        printf("您已注册成功！");
        gotoxy(x+6,28);
        printf("即将前往用户登录页面！");
        gotoxy(x+4,30);
        Goto_Other_window();
        system("cls");
        Login_User(tree,admin);
    }
    else
    {
        gotoxy(x+6,26);
        printf("即将返回主界面！");
        gotoxy(x+4,28);
        Goto_Other_window();
        system("cls");
        Entry_Menu(tree,admin);
    }
}
void Entry_Menu(Btree &tree,user &admin)      //登录页面的选择                           //修改完成
{
    char  n;
//    HideCursor();
    //printf("\n\n\n\n\n\n\n");
    int x=36;
    gotoxy(x,12);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x,13);
    printf("|                                   |");
    gotoxy(x,14);
    printf("|                                   |");
    gotoxy(x,15);
    printf("|                                   |");
    gotoxy(x,16);
    printf("|                                   |");
    gotoxy(x,17);
    printf("|                                   |");
    gotoxy(x,18);
    printf("|                                   |");
     gotoxy(x,19);
    printf("|                                   |");
     gotoxy(x,20);
    printf("|                                   |");
     gotoxy(x,21);
    printf("|                                   |");
    gotoxy(x,22);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    x+=10;
    gotoxy(x,14);
    printf("1. 管理员登录\n");
    gotoxy(x,16);
    printf("2. 借阅者登录\n");
    gotoxy(x,18);
    printf("3. 新用户注册\n");
    gotoxy(x,20);
    printf("0. 退出系统\n");
    gotoxy(x,24);
    printf("请输入：");
    Head();
    gotoxy(x+10,24);
    x-=6;
    ShowCursor();
    while(1)
    {
        n=getchar();
        if(n=='1')
        {
            gotoxy(x,24);
            printf("即将前往管理员登录界面！          ");
            gotoxy(x,26);
            Goto_Other_window();
            system("CLS");
            Login_Admin(tree);
        }
        else if(n=='2')
        {
            gotoxy(x,24);
            printf("即将前往用户登录界面！            ");
            gotoxy(x,26);
            Goto_Other_window();
            system("CLS");
            Login_User(tree,admin);
        }
        else if(n=='3')
        {
            gotoxy(x,24);
            printf("即将前往用户注册界面！           ");
            gotoxy(x,26);
            Goto_Other_window();
            system("CLS");
            Reg_Menu(tree,admin);
        }
        else if(n=='0')
        {
            gotoxy(x,24);
            printf("即将退出本系统！                    ");
            gotoxy(x,26);
            Goto_Other_window();
            system("CLS");
//            Out_Menu(tree);
            Out_Menu(tree);
        }
        else
            continue ;
    }
}
void User_Menu(Btree &tree ,user &admin)      //借阅者页面                   //修改完成
{

    char n;             //定义用int??
    HideCursor();
    int x =36;
    gotoxy(x,12);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x,13);
    printf("|                                   |");
    gotoxy(x,14);
    printf("|                                   |");
    gotoxy(x,15);
    printf("|                                   |");
    gotoxy(x,16);
    printf("|                                   |");
    gotoxy(x,17);
    printf("|                                   |");
    gotoxy(x,18);
    printf("|                                   |");
    gotoxy(x,19);
    printf("|                                   |");
    gotoxy(x,20);
    printf("|                                   |");
    gotoxy(x,21);
    printf("|                                   |");
    gotoxy(x,22);
    printf("|                                   |");
    gotoxy(x,23);
    printf("|                                   |");
    gotoxy(x,24);
    printf("|                                   |");
    gotoxy(x,25);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x+12,14);
    printf("1. 借阅图书\n");
    gotoxy(x+12,16);
    printf("2. 归还图书\n");
    gotoxy(x+12,18);
    printf("3. 作者专区\n");
    gotoxy(x+12,20);
    printf("4. 图书信息\n");
    gotoxy(x+12,22);
    printf("5. 返回主界面\n");
    gotoxy(x+12,24);
    printf("0. 退出系统\n");
    gotoxy(x+10,27);
    printf("请输入：");
    Head();
    gotoxy(x+18,27);
    ShowCursor();
    x--;
    while(1)
    {
        gotoxy(x+18,27);
        n=getchar();
        if(n=='1')
        {
            gotoxy(x+10,28);
            printf("即将前往图书借阅界面！");
            gotoxy(x+6,30);
            Goto_Other_window();

            system("CLS");
            //图书借阅
            Brorrow_book(tree,admin);
        }
        else if(n=='2')
        {
            gotoxy(x+10,28);
            printf("即将前往图书归还界面！");
            gotoxy(x+6,30);
            Goto_Other_window();

            system("CLS");
            Return_book(tree,admin);
            //图书归还
        }
        else if(n=='3')
        {
            gotoxy(x+10,28);
            printf("即将前往作者专区界面！");
            gotoxy(x+6,30);
            Goto_Other_window();

            system("CLS");
            //作者专区
            Author_Zone(tree);
        }
        else if(n=='4')
        {
            gotoxy(x+10,28);
            printf("即将前往图书信息界面！");
            gotoxy(x+6,30);
            Goto_Other_window();

            system("CLS");
            //图书信息
            Info_book(tree);
        }
        if(n=='5')
        {
            gotoxy(x+10,28);
            printf("即将返回界面！   ");
            gotoxy(x+6,30);
            Goto_Other_window();

            system("CLS");
            Entry_Menu(tree,admin);//返回系统
        }
        if(n=='0')
        {
            gotoxy(x+10,28);
            printf("即将退出程序！    ");
            gotoxy(x+6,30);
            Goto_Other_window();

            system("CLS");
            Out_Menu(tree);//退出系统
        }
    }
}
void Admin_Menu(Btree &tree)      //管理员页面           //修改完成
{
    char  n;
    HideCursor();
    int x=36;
     gotoxy(x,12);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x,13);
    printf("|                                   |");
    gotoxy(x,14);
    printf("|                                   |");
    gotoxy(x,15);
    printf("|                                   |");
    gotoxy(x,16);
    printf("|                                   |");
    gotoxy(x,17);
    printf("|                                   |");
    gotoxy(x,18);
    printf("|                                   |");
    gotoxy(x,19);
    printf("|                                   |");
    gotoxy(x,20);
    printf("|                                   |");
    gotoxy(x,21);
    printf("|                                   |");
    gotoxy(x,22);
    printf("|                                   |");
    gotoxy(x,23);
    printf("|                                   |");
    gotoxy(x,24);
    printf("|                                   |");
    gotoxy(x,25);
    printf("|                                   |");
    gotoxy(x,26);
    printf(" +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ ");
    gotoxy(x+12,14);
    printf("1. 图书入库\n");
    gotoxy(x+12,16);
    printf("2. 清除库存\n");
    gotoxy(x+12,18);
    printf("3. 作者专区\n");
    gotoxy(x+12,20);
    printf("4. 图书信息\n");
    gotoxy(x+12,22);
    printf("5. 返回登录\n");
    gotoxy(x+12,24);
    printf("0. 退出系统\n");
    gotoxy(x+10,28);
    printf("请输入：");

    Head();
    gotoxy(x+18,28);
    ShowCursor();
    while(1)
    {
        n=getchar();
        if(n=='1')
        {
            gotoxy(x+6,28);
            printf("即将前往图书入库界面！");
            gotoxy(x+6,30);
            Goto_Other_window();
            system("CLS");
            //图书入库
            Add_Book(tree);
        }
        else if(n=='2')
        {

            gotoxy(x+6,28);
            printf("即将前往清除库存界面！");
            gotoxy(x+6,30);
            Goto_Other_window();
            system("CLS");
            Delete_book(tree);

            //清除库存
        }
        else if(n=='3')
        {

            gotoxy(x+6,28);
            printf("即将前往作者专区界面！");
            gotoxy(x+6,30);
            Goto_Other_window();
            system("CLS");
            Author_Zone(tree);
            //作者专区
        }
        else if(n=='4')
        {

            gotoxy(x+6,28);
            printf("即将前往图书信息界面！");
            gotoxy(x+6,30);
            Goto_Other_window();
            system("CLS");
            Info_book(tree);
            //图书信息
        }
        if(n=='5')
        {

            gotoxy(x+6,28);
            printf("即将返回系统主界面！");
            gotoxy(x+6,30);
            Goto_Other_window();
            system("CLS");
            Entry_Menu(tree,admin);//返回系统
        }
        if(n=='0')
        {

            gotoxy(x+6,28);
            printf("即将退出本系统！");
            gotoxy(x+6,30);
            Goto_Other_window();
            system("CLS");
            Out_Menu(tree);//退出系统
        }
    }
}
void Out_Menu(Btree &tree)        //退出系统
{
    HideCursor();
    int x=25,y=5;
    gotoxy(x,y);
    printf("╔════════════════════════════════════════════════════════╗");
    gotoxy(x,y+1);
    printf("║                                                        ║");
    gotoxy(x,y+2);
    printf("║                                                        ║");
    gotoxy(x,y+3);
    printf("║                                                        ║");
    gotoxy(x,y+4);
    printf("║                    谢谢您的使用！                      ║");
    gotoxy(x,y+5);
    printf("║                                                        ║");
    gotoxy(x,y+6);
    printf("║                                                        ║");
    gotoxy(x,y+7);
    printf("║                      GOODBYE!!!                        ║");
    gotoxy(x,y+8);
    printf("║                                                        ║");
    gotoxy(x,y+9);
    printf("║                                                        ║");
    gotoxy(x,y+10);
    printf("║                      图书管理                          ║");
    gotoxy(x,y+11);
    printf("║                                                        ║");
    gotoxy(x,y+12);
    printf("╚════════════════════════════════════════════════════════╝");
     delete tree;
     gotoxy(x,y+20);
    exit(0);
//    return ;
}
void gotoxy(short x,short y)        //定点函数
// 移动光标到（x，y）坐标，25>x>=0,80>y>=0
{
    COORD point = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}
void HideCursor()//隐藏光标
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = 0; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}
void ShowCursor()//显示光标
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = 1; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}
void Print_book_All_information(book &b) //打印图书的所有信息 / //图书信息专区
{
    cout<<"\t\t\t     ******************************此书的基本信息**********************************         "<<endl;
    cout<<endl;
    cout<<"\t\t\t     "<<setiosflags(ios_base::right)<<setw(6)<<"书号"<<resetiosflags(ios_base::right)<<setw(10)<<"书名"<<setw(10)<<"作者"
    <<setw(10)<<"出版社"<<setw(12)<<"出版时间"<<setw(8)<<"定价"
    <<setw(10)<<"现存量"<<setw(10)<<"总存量"<<endl;
    cout<<endl;
    b.show();
    node *p=b.head;
    if(p==NULL)
    {
        cout<<endl;
        cout<<"\t\t\t     Sorry, 本书尚未被借阅！"<<endl;
        cout<<endl;
        return ;
    }
    else
    {
        cout<<endl;
        cout<<"\t\t\t     ******************************此书的借阅信息**********************************         "<<endl;
        cout<<endl;
        cout<<"\t\t\t     "<<setiosflags(ios_base::right)<<setw(15)<<"User_id"<<resetiosflags(ios_base::right)<<setw(15)<<"User_name"<<setw(15)<<"借阅时间"
        <<setw(15)<<"应还时间"<<endl;
        node *q=p->next;
        while (q)
        {
            cout<<endl;
            cout<<setiosflags(ios_base::right)<<setw(15)<<q->userdata.user_id<<resetiosflags(ios_base::right)<<setw(15)<<q->userdata.user_name;

            cout<<"\t";
            q->brorrow_time.show();
            cout<<"\t";
            q->return_time.show();
            q=q->next;
        }
        cout<<endl;
         if (b.book_num_pre==0)
         {
             cout<<endl;
            cout<<"\t\t\t     ***************************本书尚未被预约！***********************************         "<<endl;
            return ;
         }
         else
           {
               cout<<endl;
               cout<<"\t\t\t     ******************************此书的预约信息**********************************         "<<endl;
               cout<<endl;
               cout<<"\t\t\t     "<<setiosflags(ios_base::right)<<setw(15)<<"User_id"<<resetiosflags(ios_base::right)<<setw(15)<<"User_name"<<setw(15)<<"预约时间"
        <<setw(15)<<"取书时间"<<endl;
                   q=p->next;
                 while (q)
                 {
                     if (q->be_pre==1)
                        {
                            cout<<endl;
                            cout<<setiosflags(ios_base::right)<<setw(15)<<q->userdata_two.user_id<<resetiosflags(ios_base::right)<<setw(15)<<q->userdata_two.user_name;
                            cout<<"\t";
                            q->pre_brorrow_time.show();
                            cout<<"\t";
                            q->get_time.show();
                        }
                        q=q->next;
                 }
           }
    }

}
inline bool user_cmp(user &a,user &b)    //a==b return 1; else return 0
{
    if (strcmp(a.user_id,b.user_id)==0&&strcmp(a.user_name,b.user_name)==0)
        return 1;
    else
        return 0;
}
void Return_book(Btree &tree,user &admin) //图书归还主界面*******************************************************************************************************
{

    cout<<"\n\n\n\t\t\t     欢迎来到图书归还主界面！"<<endl;
    cout<<endl;
    while (1)
    {
        return_book(tree,admin);
        cout<<"\t\t\t     请问您是否要继续还书？？"<<endl<<endl;
        cout<<"\t\t\t         1.是     2.否"<<endl<<endl;
        cout<<"\t\t\t     请输入：";
        char key;
        while (cin>>key)
        {
            if (key=='1'||key=='2')
                break;
            else
                continue ;
        };
        if (key=='2')
            break;
    }
    getchar();//*********************************************************************************
    cout<<"\t\t\t     GoodBye,即将返回用户主界面！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     ";
    Goto_Other_window();
    system("cls");
    User_Menu(tree,admin);
}
void return_book(Btree &tree,user &admin) //图书归还详情页
{
    cout<<"\t\t\t     请输入您要归还的书号：";
    int id;
    cin>>id;
    if (lib_book[id].book_num_all==lib_book[id].book_num_now)
    {
        cout<<endl;
        cout<<"\t\t\t     对不起，我们书库并未借出这种类型的书！"<<endl<<endl;
        return ;
    }
    cout<<"\t\t\t     您要归还的图书的基本信息是："<<endl;
    cout<<endl;

    Print_book(lib_book[id]);
    cout<<endl;
    cout<<"\t\t\t     请问是否确认？"<<endl;
    cout<<"\t\t\t      1.是  2.否"<<endl;
    char key ;
    cout<<"\t\t\t     请输入：";
    while (cin>>key&&key==' '||key=='\n');
    if (key=='2')
        return ;
    lib_book[id].book_num_now++;
    node *p=lib_book[id].head;
    int yes=0;
    node *q;
    q=p->next;
    while (q)
    {
        if (user_cmp(admin,q->userdata))     // 如果找到admin
        {
            yes=1;
            p->next=q->next;
            free(q);
            q=p->next;
            break;
        }
        else
        {
            p=q;
            q=q->next;
        }
    }
    if (yes)
    {
        cout<< "\t\t\t          归还成功！！！ "<<endl;
        cout<<endl;
    }
    else
    {
        cout<<"\t\t\t     Sorry,"<<admin.user_name<<"并没有在我们书库借过这本书。"<<endl;
        cout<<endl;
        return ;
    }

}
bool cmp_time(Time &a,Time &b) // 比较两个时间的大小 若a>b ,返回1 ，否则返回0
{
    if (a.year>b.year)
        return 1;
    else if (a.year==b.year)
    {
        if (a.mon>b.mon)
            return 1;
        else if (a.mon==b.mon)
        {
            if (a.day>b.day)
                return 1;
            else return 0;
        }
        return  0;
    }
    else
        return 0;
}
int isLeap(int year)  //判断闰年
{
    // 必须先判断是平年的情况 后判断闰年的情况
    if((year%100==0 && year%400!=0) || year%3200==0)
    {
        // 能被100整除并且不能被400整除的不是闰年
        // 能被3200整除的不是闰年
        return 0;
    }
    else if(year%4==0 && year%100!=0)
    {
        // 能被4整除并且不能被100整除的是闰年
        return 1;
    }
    else if(year%400==0)
    {
        // 能被400整除的是闰年
        return 1;
    }
    else
        return 0;
}
Time Count_time(Time a) //获取归还时间，或者取书时间
{
     int flag=0;
    flag=isLeap(a.year);
    if(flag==1)
    {
        int end_day;
        end_day=a.day+30;
        switch(a.mon)
        {
        case 1:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 2:
            if(end_day>29)
            {
                a.mon=a.mon+1;
                a.day=end_day-29;
            }
            else
                a.day=end_day;
            break;
        case 3:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 4:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 5:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 6:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 7:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 8:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 9:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 10:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 11:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 12:
            if(end_day>31)
            {
                a.mon=a.mon-11;
                a.day=end_day-31;
                a.year=a.year+1;
            }
            else
                a.day=end_day;
            break;
        }
    }
    else
    {
        int end_day;
        end_day=a.day+30;
        switch(a.mon)
        {
        case 1:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 2:
            if(end_day>28)
            {
                a.mon=a.mon+1;
                a.day=end_day-28;
            }
            else
                a.day=end_day;
            break;
        case 3:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 4:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 5:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 6:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 7:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 8:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            break;
        case 9:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 10:
            if(end_day>31)
            {
                a.mon=a.mon+1;
                a.day=end_day-31;
            }
            else
                a.day=end_day;
            break;
        case 11:
            if(end_day>30)
            {
                a.mon=a.mon+1;
                a.day=end_day-30;
            }
            else
                a.day=end_day;
            break;
        case 12:
            if(end_day>31)
            {
                a.mon=a.mon-11;
                a.day=end_day-31;
                a.year=a.year+1;
            }
            else
                a.day=end_day;
            break;
        }
    }

     Time b=a;
     return b;
}
inline void Give_time(Time &a) //传递时间
{

    tm date;
    GetDate(date);
    a.year=date.tm_year;
    a.mon=date.tm_mon;
    a.day=date.tm_mday;
}
inline void	GetDate(tm &tim)// 获取系统时间，赋予tm结构体变量tim
{
	time_t curtime=time(0);
	tim = *localtime(&curtime);
	tim.tm_year += 1900;		// tm 年份少1900年
	tim.tm_mon ++;				// tm month 从0-11，故加1
}
bool Linklist_Search(node *head);           //*******************************************************************************************************
void Pre_Brorrow_book(Btree &tree,int id,user &admin)   //图书预约主界面   *************************************************************************************改
{


    cout<<"\n\n\n\t\t\t     欢迎来到图书预约系统！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     您需要预约的图书信息如下："<<endl;
    cout<<endl;
    /////       **********************************************************************
    Print_book(lib_book[id]);//打印图书的基本信息
    cout<<endl;
    cout<<"\t\t\t     请问您是否确认？"<<endl<<endl;
    cout<<"\t\t\t       1.是   2.否"<<endl<<endl;
    cout<<"\t\t\t     请输入：";
    char key;
        while (cin>>key)
    {
        if (key=='1'||key=='2')
            break;
        else
            continue ;
    };
    if (key=='1')
    {
        cout<<"\t\t\t     ****************************"<<endl;

        pre_brorrow_book(tree,id,admin);//预约图书详情页*****************************************
        ///////////////////预约图书//////////////////////////****************
//        cout<<"预约图书成功"<<endl<<endl;
    }
//    getch();
    cout<<"即将返回图书借阅界面"<<endl;
    cout<<endl;
    cout<<"\t";//***************************************************************************************
    Goto_Other_window();
    system("cls");
    //////返回图书借阅界面
    return ;

}
void pre_brorrow_book(Btree &tree,int id ,user &admin)  //图书预约详情页
{
    cout<<"\t\t\t     ****************************"<<endl;
    if (lib_book[id].book_num_pre==lib_book[id].book_num_all)
    {
        cout<<"\t\t\t     十分抱歉，所有的图书已被预约完，请等待下一轮的借阅和预约！"<<endl;
        return ;
    }
    bool yes =0;
    Time cmp;
    node *p =lib_book[id].head->next;
    while (p)
    {
        if(p->be_pre==0)
        {
            yes=1;
            cmp=p->return_time;
            break;
        }
        p=p->next;
    }

     if (!yes)
        return ;
    node *q=p->next;
    int pos =0;
    int cnt =0;
    while (q)
    {
        cnt++;
        if (q->be_pre==0)
        {
            pos=cnt;
            if (cmp_time(cmp,q->return_time))  //cmp_time(a,b) a>b return 1,else  return 0
            cmp=q->return_time;
         }
         q=q->next;
    }
    if (!pos)
        q=p;
    else
    {
        q=p->next;
        cnt=0;
        while (q)
        {
            cnt++;
            if (cnt==pos)
                break;
         q=q->next;
        }
    }
    Give_time(q->pre_brorrow_time);
    q->get_time=cmp;
    q->be_pre=1;
    q->userdata_two=admin;
    lib_book[id].book_num_pre++;
    cout<<"\t\t\t          预约成功！！！"<<endl<<endl;
    cout<<"\t\t\t     您的预约时间是：";
    q->pre_brorrow_time.show();
    cout<<endl;
    cout<<"\t\t\t     您的取书时间是：";
    q->get_time.show();
    cout<<endl;
    //***********************************************************************************
//     if()
}
void Brorrow_book(Btree &tree,user &admin)//图书借阅界面      //*************************************************************************************
{
//    system("cls");
    cout<<endl;
    cout<<"\n\n\n\t\t\t     欢迎来到图书借阅专区!(借阅的默认天数为30天)"<<endl;
    cout<<endl;
    while (1)
    {
        brorrow_book(tree,admin);
        cout<<"\t\t\t     请问您是否要继续借阅？"<<endl;
        cout<<endl;
        cout<<"\t\t\t        1.是   2.否"<<endl;
        cout<<endl;
        cout<<"\t\t\t     请输入：";
        char key;
        while (cin>>key)
        {
            if (key=='1'||key=='2')
                break;
            else
                continue ;
        };
        if (key=='2')
            break;
        cout<<endl;
    }
    getch();
    cout<<"\t\t\t     期待您的下次借阅！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     即将返回用户主界面！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     ";//****************
    Goto_Other_window();
    system("cls");
    User_Menu(tree,admin);//******************************************************************************************
}
void brorrow_book(Btree &tree,user &admin)//图书借阅详情页  //书号借阅
{
    cout<<"\t\t\t     请输入您需要借阅的书号：";
    int id ;
    cin>>id;
    cout<<endl;
    Find_res ret=Btree_Search(tree,id);
    if (ret.yes==0)
    {
        cout<<"\t\t\t     Sorry, 我们并没有收录这本书！"<<endl;
        cout<<endl;
        return ;
    }
    else
    {
        cout<<"\t\t\t     您要借阅的书的基本信息是："<<endl;
        cout<<endl;
        Print_book(lib_book[id]);
        cout<<endl;
        cout<<"\t\t\t     请问是否确认？"<<endl<<endl;
        cout<<"\t\t\t      1.是   2.否"<<endl<<endl;
        cout<<"\t\t\t     请输入：";
        char kkey;
        while (cin>>kkey&&kkey==' '||kkey=='\n');
        if (kkey=='1')
        {
            if (lib_book[id].book_num_now) //图书借阅
            {
                if (lib_book[id].head==NULL)
                {
                    lib_book[id].head=new node ;
                    lib_book[id].head->next=NULL;
                }
                //缺少重复借阅一本书的情况  //暴力判断 ，或者改变user 的 ID值类型
                //缺少判断借阅者借阅图书的上限情况
                //********************************************************************************************
                if (admin.now_book==3)      //借阅已达上限
                {
                    cout<<"\t\t\t     对不起，您的借书已达上限，请先看完您的借阅书籍！"<<endl;
                    cout<<endl;
                    return ;
                }
                node *p;
                p=new node ;
                //登记借阅者信息
                p->userdata=admin;//传入数据**********************************************************************
                Give_time(p->brorrow_time);
                p->return_time=Count_time(p->brorrow_time);
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////
                cout<<endl;
                cout<<"\t\t\t     您的个人信息如下："<<endl;
                cout<<endl;
                cout<<"\t\t\t     借阅证号：";
                cout<<p->userdata.user_id<<endl<<"\t\t\t     您的姓名："<<p->userdata.user_name<<endl;
                cout<<"\t\t\t     您的借阅时间是：";
                p->brorrow_time.show();
                cout<<endl;
                cout<<"\t\t\t     您的归还时间是：";
                p->return_time.show();
                cout<<endl;
                p->next=lib_book[id].head->next;
                lib_book[id].head->next=p;
                lib_book[id].book_num_now--; //减少先存的图书
                ////////////////////////////////////////////////////////////////////////////////////
                cout<<"\t\t\t     借阅图书信息打印"<<endl;
//                Print_book_All_information(lib_book[id]);
                admin.now_book++;
                cout<<endl;
                cout<<"\t\t\t         借阅成功！ "<<endl;
            }
            else
            {
                cout<<endl;
                cout<<"\t\t\t     But，由于本书太过热门，被借光了"<<endl;
                cout<<endl;
                cout<<"\t\t\t     请问您是否要预约这本书？？"<<endl;
                cout<<endl;
                cout<<"\t\t\t           1.是    2.否"<<endl;
                cout<<"\t\t\t     ";
                char key ;
                while(cin>>key&&key=='\n'||key==' ');
                if (key=='1')
                    Pre_Brorrow_book(tree,id,admin);
                else
                    return ;
            }
        }
    }
    cout<<endl;
}
void Print_Linklist(node *head) //打印链表节点信息
{
    node *p=head->next;
    while (p->next)
    {
        p->userdata.show();
//            cout<<p->data<<",";
        p=p->next;
    }
    p->userdata.show();
//        cout<<p->data<<endl;
}
void Info_book(Btree &tree) //图书信息主页面
{
    cout<<"\n\n\n\t\t\t     Welcome,这里是图书信息区域！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     在这里，你可以查询任意一本图书的基本信息，也可以查看所有的图书信息!"<<endl;
    while (1)
    {
        info_book(tree);
        cout<<"\t\t\t     请问您是否要继续查询图书的相关信息？"<<endl;
        cout<<endl;
        cout<<"\t\t\t              1.是       2.否"<<endl;
        cout<<endl;
        cout<<"\t\t\t     请输入：";
        char key ;
        while (cin>>key)
        {
            if (key=='1'||key=='2')
                break;
        }
        if (key=='1');
        else
            break;
    }
    getchar();
    cout<<"\t\t\t     See You Later,即将返回操作界面！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     ";
    Goto_Other_window();
    system("cls");
    Entry_Menu(tree,admin);
}
void info_book(Btree &tree)//图书信息详情页       //需要添加功能         //****************************************************************
{
//    system("cls");
cout<<endl;
    cout<<"\t\t\t     1.检索图书信息"<<endl;
    cout<<endl;
    cout<<"\t\t\t     2.查看所有图书"<<endl;
    cout<<endl;
    cout<<"\t\t\t     ";
    char key;
    while (cin>>key)
    {
        if (key=='1'||key=='2')
            break;
        else
            continue ;
    };
    if (key=='1')
    {
        while(1)
        {
            Info_book_find(tree);//查阅图书的基本信息
            cout<<"\t\t\t     请问您是否要继续查询？"<<endl;
            cout<<endl;
            cout<<"\t\t\t        1.是     2.否"<<endl;
            cout<<endl;
            cout<<"\t\t\t     ";
            char Key;
         while (cin>>Key)
         {
             if(Key=='1'||Key=='2')
                break;
         }
        if (Key=='1')//继续 ， 1 继续，2 退出
            continue ;
        else
            break;
        }
        cout<<endl;
    }
    else
    {

        Info_book_styple(); //打印头部 就是打印书号，书名等
        Btree_Inorder_Info(tree);// 中序遍历，打印所有信息
        cout<<endl<<"\t\t\t     ******************************************************************************         "<<endl;
//        getchar();
    }
                   //*******************************************************************************

}

void Info_book_find(Btree &tree)//查询图书信息（单本）
{

        cout<<"\t\t\t     请输入您需要查询的图书书号"<<endl<<endl;
        cout<<"\t\t\t     ID：";
        int id;
        cin>>id;
        char key;
        Find_res ret=Btree_Search(tree,id);
        if (!ret.yes)
        {
            cout<<endl<<"\t\t\t     对不起，我们书库并没有这本书！"<<endl;
        }
        else
        {
            cout<<"\t\t\t     您所要查询的图书信息如下："<<endl;
            cout<<endl;
            Print_book_All_information(lib_book[id]);
//            Info_book_styple();
//            lib_book[id].show();        //////////*********************************************************
            cout<<endl;
        }
        cout<<endl;


}
void Btree_Inorder_Info(Btree &tree)//查询图书信息所有
{
    if (tree==NULL)
        return ;
    int n=tree->keynum;
    int i;
    for (i=1; i<=n; i++)
    {
        Btree_Inorder_Info(tree->child[i-1]);
        int x=tree->key[i];
        lib_book[x].show();        //**************************************显示图书的所有信息
        //缺少借阅信息，预约信息
    }
    Btree_Inorder_Info(tree->child[i-1]);
}
void Btree_INorder(Btree &tree) //B树中序遍历
{
    if (tree==NULL)
        return ;
    int n=tree->keynum;
    int i;
    for( i=1; i<=n; i++)
    {
        Btree_INorder(tree->child[i-1]);
        cout<<tree->key[i]<<",";

    }
    Btree_INorder(tree->child[i-1]);
}
void Reader_Push()//读者热推图书//************************************************************************************
{
    //cout<<"**********"<<endl;
}
void Author_Zone(Btree &tree)//作者专区     ***************************************************************************
/********************************
char key ;
    while (cin>>key &&key=='\n'||key==' '); //获取准确字符
********************************/
{
//     system("cls");
    cout<<"\n\n\n\t\t\t     欢迎来到作者专区，这是一个可以让你更进一步接触作者的地方！"<<endl;
    cout<<endl;
//    getchar();
    while (1)
    {
        author_book_num=0;
        char name[20];
        cout<<"\t\t\t     请输入您需要查询的作者姓名(20个字符以内)"<<endl;
        cout<<endl;
        cout<<"\t\t\t     作者姓名：";
        getchar();
        gets(name);
        cout<<endl<<endl;
        cout<<"\t\t\t     您要查询的作者姓名是："<<name<<endl<<endl;
        Btree_Inorder_Search(tree,name);
        if (author_book_num==0)
        {
            cout<<endl;
            cout<<"\t\t\t     十分抱歉，我们并没有任何关于这位作者的书籍，我们会尽快添加相关的书籍的！"<<endl;
            cout<<endl;
            cout<<"\t\t\t     您是否要登记相关信息，以便我们下次采购时购买!"<<endl<<endl;
            cout<<"\t\t\t     1.是   2.否"<<endl;
            cout<<endl;
            cout<<"\t\t\t     请输入：";
            char key ;
            while (cin>>key &&key=='\n'||key==' ');
//            printf("%d %c\n",key,key);
            cout<<endl;
//            getchar();
            if (key=='1')
            {
                Reader_Push();//读者热推图书          ****************************************************************未写
                    break;
            }
            else
            {
                cout<<"\t\t\t     请问您是否要退出作者专区？"<<endl<<endl;
                cout<<"\t\t\t     1.是   2.否"<<endl<<endl;
                char Key;
                cout<<"\t\t\t     请输入：";

                while (cin>>Key &&Key=='\n'||Key==' ');

//            printf("%d %c\n",Key,Key);
                if (Key=='1')
                    break;
                cout<<endl<<endl;
//                getchar();
            }
        }
        else
        {

            //cout<<author_book_num<<endl;
            Info_book_styple();
            for (int i=0; i<author_book_num; i++)
            {
                lib_book[author_book_id[i]].show();

            }
            cout<<endl<<"\t\t\t     ******************************************************************************         "<<endl;
            cout<<endl;

            cout<<"\t\t\t     请问您是否要退出作者专区？"<<endl<<endl;
                cout<<"\t\t\t     1.是   2.否"<<endl<<endl;
                char Key;
                cout<<"\t\t\t     请输入：";

            while (cin>>Key &&Key=='\n'||Key==' ');

//            printf("%d %c\n",Key,Key);
            if (Key=='1')
                break;
            cout<<endl<<endl;
//            getchar();
        }

    }
   //getchar();
    cout<<endl<<"\t\t\t     GoodBye(<-^_^->),即将返回主要操作页面！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     ";
    Goto_Other_window();    //转载页面动画
    system("cls");
    Entry_Menu(tree,admin);      ///主页面 ***************************************************************************************
    //退出接口
    //**********************************************************************************************************************
}

void Btree_Inorder_Search(Btree &tree,char name[]) //中序查找每一个节点信息
{
    if (tree==NULL)
        return ;
    int i;
    int n=tree->keynum;
    for (i=1; i<=n; i++)
    {
        Btree_Inorder_Search(tree->child[i-1],name);
        int  x=tree->key[i];
        if (strcmp(lib_book[x].book_author,name)==0)
        {
            author_book_id[author_book_num]=x;
            author_book_num++;
//                 cout<<x<<endl;
        }
    }
    Btree_Inorder_Search(tree->child[i-1],name);

}
inline void Init_lib() //存测试数据
{
    book a(1,"123  15","1 2","1","1",2,20);
    lib_book[1]=a;
//    Info_book_styple();
//    a.show();
    book b(2,"abcd","1 2","1","1",2,20);
    lib_book[2]=b;
//    b.show();
    book c(3,"stop!!","1 2","1","1",2,20);
    lib_book[3]=c;
//    c.show();
    book d(4,"***456","1 2","1","1",2,20);
    lib_book[4]=d;
//    d.show();
    book e(5,"***456","1 2 3","1","1",2,20);
    lib_book[5]=e;
//    e.show();
    book f(6,"地方","4 5","1","1",2,20);
    lib_book[6]=f;
//    f.show();
    book g(7,"1","4 5","1","1",2,20);
    lib_book[7]=g;
//    g.show();
    book h(8,"wangzhe","df","1","1",2,20);
    lib_book[8]=h;
//    h.show();
    book i(9,"666奈斯","xxx","1","1",2,20);
    lib_book[9]=i;
//    i.show();
    book j(10,"哇哦123!","1234","1","1",2,20);
    lib_book[10]=j;
//    j.show();


}
inline void Info_book_styple()// 打印图书的基本信息项
{
    cout<<"\t\t\t     ******************************此书的基本信息**********************************         "<<endl;
    cout<<"\t\t\t     "<<setiosflags(ios_base::right)<<setw(6)<<"书号"<<resetiosflags(ios_base::right)<<setw(10)<<"书名"<<setw(10)<<"作者"
        <<setw(10)<<"出版社"<<setw(12)<<"出版时间"<<setw(8)<<"定价"
        <<setw(10)<<"现存量"<<setw(10)<<"总存量"<<endl<<endl;
}
/*****
// test——data

*****/
inline void  Print_book(book &b) //定点打印图书信息
{
    //定位函数
    Info_book_styple();//书的基本 信息
    b.show();
    cout<<endl<<"\t\t\t     ******************************************************************************         "<<endl;
    cout<<endl;
}
void delete_book(Btree &tree) //删除图书详情页
{
    cout<<"\t\t\t     请输入你要删除的书号：";
    int tmp;
    cin>>tmp;
    cout<<endl;
    Find_res ret = Btree_Search(tree,tmp);
    if(!ret.yes)
    {
        cout<<"\t\t\t     Sorry, 我们的书库并没有这本书！"<<endl;
        cout<<endl;
    }
    else
    {
        cout<<"\t\t\t     您要删除的图书信息为："<<endl;
        Print_book(lib_book[tmp]);
        char key;
        cout<<"\t\t\t     确定要删除此书吗？"<<endl<<endl;
        cout<<"\t\t\t     1.是   2.否"<<endl<<endl;
        cout<<"\t\t\t     请输入：";
        cin>>key;
        if (key=='2')
        {
            return ;
        }
        bool yes=Btree_Delete(tree,tmp);
        lib_book[tmp].book_judge=0;
        cout<<endl;
        cout<<"\t\t\t     清除成功！-_-"<<endl<<endl;;
        cout<<"\t\t\t     删除后的B树如下："<<endl;
        Btree_show(tree);
        cout<<endl<<endl;
    }
}
void Delete_book( Btree &tree ) //添加图书页面
/****************************************
    缺少esc监控建，
    打印位置的确定
    监控物理按键
    Write By XPF
****************************************/
{

    char out_key;
    cout<<"\n\n\n\t\t\t     欢迎来到图书管理之清除库存系统 ^_^!"<<endl<<endl;
    while(1)
    {
        delete_book(tree);
        cout<<"\t\t\t     请问，您是否想继续删除图书?"<<endl<<endl;
        cout<<"\t\t\t     按2返回管理员界面，按其他键继续执行操作!"<<endl<<endl;
        cout<<"\t\t\t     ";
        cin>>out_key;
        if (out_key=='2')
            break;
        cout<<endl;
    }

    getchar();
    cout<<"\t\t\t     图书删除完毕，即将返回管理员界面！"<<endl;
    cout<<endl;
    cout<<"\t\t\t     ";
    Goto_Other_window();
    system("cls");
    Admin_Menu(tree);
//返回主菜单                                                                         *******************
//    Sleep(1000);
//    system("cls");
//     main_window();
}
void add_book(Btree &tree) //添加图书详情页
{
    cout<<endl;
    cout<<"\t\t\t     请输入书号： ";
    book tmp;
    cin>>tmp.book_id;
    int x =tmp.book_id;
    Find_res ret=Btree_Search(tree,x);
    if (ret.yes)
    {
        Print_book(lib_book[x]);
        cout<<"\t\t\t     本书已在书库！"<<endl<<endl;
        cout<<"\t\t\t     请输入新增图书数量：";
        //cout<<endl;
        int add;
        cin>>add;
        lib_book[x].book_num_all+=add;
        lib_book[x].book_num_now+=add;
        Print_book(lib_book[x]);

        return ;
    }
//    Insert(tree,tmp.book_id);
//    lib_book[book_num].book_id=tmp_id;
    while (getchar()!='\n');// 去掉无关输入
    cout<<"\t\t\t     请输入书名： ";
    gets(tmp.book_name);
//    gets(lib_book[book_num].book_name);
//    getchar();
    cout<<"\t\t\t     请输入作者： ";
    gets(tmp.book_author);
//    gets(lib_book[book_num].book_author);
//    getchar();
    cout<<"\t\t\t     请输入出版社： ";
    gets(tmp.book_pulish);
//    gets(lib_book[book_num].book_pulish);
//    getchar();
    cout<<"\t\t\t     请输入出版时间：";
    gets(tmp.book_time);
//    gets(lib_book[book_num].book_time);
//    getchar();
    cout<<"\t\t\t     请输入书的定价： ";
    cin>>tmp.book_values;
    while (getchar()!='\n');// 去掉无关输入
//    cin>>lib_book[book_num].book_values;
//    getchar();
    cout<<"\t\t\t     请输入书的数量： ";
    cin>>tmp.book_num_all;
    while (getchar()!='\n');// 去掉无关输入
//    cin>>lib_book[book_num].book_num_all;
//    getchar();
    lib_book[book_num].book_num_now=lib_book[book_num].book_num_all;
    cout<<endl;
    cout<<"\t\t\t     要添加的图书信息如下："<<endl;
    Info_book_styple();
    tmp.book_num_now=tmp.book_num_all;
    tmp.show();
    cout<<endl<<"\t\t\t     ******************************************************************************         "<<endl;
    cout<<endl;
    cout<<"\t\t\t     您确定要添加这本书吗？"<<endl<<endl;
    cout<<"\t\t\t     1.是   2.否"<<endl<<endl;
    char book_insuree;
    cout<<"\t\t\t     请输入：";
    cin>>book_insuree;   //确定是否添加图书
    if (book_insuree=='1')
    {
        tmp.book_judge=1;
//        lib_book[book_num++].show();
        lib_book[tmp.book_id]=tmp;
        book_num++;
        cout<<endl;
        Btree_Insert(tree,x);
        cout<<"\t\t\t     插入后的B树如下："<<endl;
//        Show_BTree(tree);
        Btree_show(tree);
        cout<<endl<<endl;
    }
    getchar();
}
void Add_Book( Btree &tree ) //添加图书页面   **************************************************************************
/****************************************
    缺少esc监控建，
    打印位置的确定
    监控物理按键
    Write By XPF
****************************************/
{

    char key;
    cout<<"\n\n\n\t\t\t     欢迎来到图书管理之采编入库系统 ^_^!"<<endl<<endl;
    cout<<"\t\t\t     欢迎您为我们的图书馆舔砖加瓦^_^"<<endl;
    cout<<endl;
    cout<<"\t\t\t     即将返回主页面"<<endl;
    while(1)
    {
        add_book(tree);
        cout<<"\t\t\t     请问，您是否想继续添加图书??"<<endl<<endl;
        cout<<"\t\t\t     1.是   2.否"<<endl<<endl;
        cout<<"\t\t\t     请输入：";
        while (cin>>key)
        {
            if (key=='1'||key=='2')
                break;
            else
                continue ;
        };
        if (key=='2')
            break;
        cout<<endl;
    }

    cout<<endl;
    getchar();
    cout<<"\t\t\t     图书录入完毕，即将返回管理员主页面！"<<endl<<endl;
    cout<<"\t\t\t     ";
        Goto_Other_window();// ----转载页面动画   *********************************************************************
        system("cls");
        Admin_Menu(tree);
//返回主菜单
//    Sleep(1000);
//    system("cls");
//     main_window();
}

