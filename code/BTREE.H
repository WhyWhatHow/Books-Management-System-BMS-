
/******************************
Write By XPF:
key:  1,2,3,...m
child:0,1,2,3..m
*******************************/
#include<bits/stdc++.h>
using namespace std;
const int mm= 3;
typedef int keytype;
typedef struct Bnode{

    int keynum;
    Bnode*parent;
    Bnode*child[mm+1];
    keytype key[mm+1];
    Bnode(int n,Bnode *p)
    {
        keynum=n;
        parent=p;
        for (int i=0;i<=mm;i++)
        {
            key[i]=0;
            child[i]=NULL;
        }
    }
}Tree,*Btree;
typedef struct Find_res{
    Btree node ;
    int i;
    bool  yes;
    Find_res(int pos,bool res,Btree p)
    {
        node =p;
        i=pos;
        yes=res;
    }
}Find_Result;
Find_res Btree_Search(Btree &root,keytype &x); //查找关键字 O(log2n)
bool Btree_Insert(Btree &root,keytype &x);  //插入的主函数  // 逻辑关系
bool Btree_Delete(Btree &root,keytype &x);// 删除的主函数   //逻辑关系
void Btree_Insert_key(Btree &root,Btree &a,keytype &x,int i);//插入关键字
void Btree_split(Btree &p, Btree &a,keytype &x,int i); //分成两部分 //插入的主要函数
void Btree_Right_Adjust(Btree &p,Btree &q,int i);    //右旋
void Btree_Left_Adjust(Btree &p,Btree &q,int i);//左旋
void Btree_Merge_Bnode(Btree &p,Btree &q,Btree &pr,int i);//合并 p，pr两个节点
void BTree_Right_Compress(Btree &p,int i); //从第 I 个位置向后移动   //即
void Btree_Left_Compress(Btree &q,int i);//从第I个位置向前移动       //即覆盖位置i
//void Btree_show(Btree &root,short x=34);// 凹入表打印B树

Find_res Btree_Search(Btree &root,keytype &x)
{
    Btree p=root;
    Btree q=NULL;
       int i;
    while (p!=NULL)
    {
        for (i=0;i<p->keynum&&x>=p->key[i+1];i++);

        if (x==p->key[i])
        {
            return Find_res(i,1,p);
        }
        else
            q=p;
        p=p->child[i];
    }
    return Find_res(i,0,q);
}

/****************************
插入的主题流程如下：
    1判断是否为空树.是否为空树
    2.树中是否存在此关键字信息
    3.叶子中插入
        a. 小于m-1 挖空，填空 i
        b:  先分裂节点，递归处理（插入后中间值上移（定义 parent指针用作上移））
****************************/
bool Btree_Insert(Btree &root,keytype &x)
{
    if(root==NULL)
    {
        root=new Bnode(1,NULL);
        root->key[1]=x;
        return 1;                                //已存在这本书
    }
    Find_res ret=Btree_Search(root,x);
    if (ret.yes==1)
        return 0;
    Btree p=ret.node;
    Btree q;
    Btree sub=NULL; // 若分，sub继承p的右半部分
    keytype tmp=x;
    int  i=ret.i;
    while (1)
    {
        if (p->keynum<(mm-1))
        {
            Btree_Insert_key(p,sub,tmp,i);
            return 1;
        }
        Btree_split(p,sub,tmp,i);
        if (p->parent!=NULL)
        {
            q=p->parent;
            for (i=0;i<q->keynum;i++) //找到该节点在父亲节点的位置
            {
                if (x<q->key[i+1])
                    break;
            }
            p=q;  //进入上一层
        }
        else
        {
            root= new Bnode(1,NULL);
            root->key[1]=tmp;
            root->child[0]=p;
            root->child[1]=sub;
            p->parent=sub->parent=root;
            return 1;
        }
    }
}

void Btree_Insert_key(Btree &p,Btree &a,keytype &x,int i) //应插入位置 i+1
{
    for (int j=p->keynum; j>i ; j--) //挖坑 i+1 ,keynum+1
    {
        p->key[j+1]=p->key[j];
        p->child[j+1]=p->child[j];
    }
    p->keynum++;
    p->key[i+1]=x;
    p->child[i+1]=a;
    if (a!=NULL)  //跟新父亲节点
        a->parent=p;
}
void Btree_split(Btree &p,Btree &a,keytype &x,int i)
{
    Btree_Insert_key(p,a,x,i);
    int mid =(mm+1)/2;
    int ssize=(mm&1)?mid:mid+1;
    Btree q=new Bnode(0,p->parent);
    q->child[0]=p->child[mid];
     for (int j=1;j<ssize;j++)
     {
         q->child[j]=p->child[mid+j];
         q->key[j]=p->key[mid+j];
     }
     if(q->child[0]!=NULL)  //更新父节点
     {
         for (int ii=0;ii<ssize;ii++)
            q->child[ii]->parent=q;
     }
     q->keynum=mm-mid;
     p->keynum=mid-1;
     //生成（x,a）传递
     x=p->key[mid];
     a=q;
}
/****************************************
删除的两大部分：删除（叶节点，非叶节点），调整
1.删除对应的节点信息
    如果在叶子上直接删除
    如果在内节点上，找到p最右边相邻叶子节点交换，叶子节点信息进行覆盖
2.调整（循环）
    如果是删除信息的所在的节点是富裕的叶子节点，或者说是树根的情况，退出循环
    否则，需要去找删除节点在其父亲节点中子树的位置，如果是位于0号位置，进行右调整，其他位置进行左调整
    左右调整大致分为两个情况：
        如果兄弟节点富裕的话，直接进行相应的节点信息调整即可，否则的话，合并两个节点信息，生成一个新节点
    节点上移,调整父亲节点信息
        合并两个节点信息采用大的给小的的形式，所以对于左调整，选择合适合并节点进行合并即可
3.退出循环后特殊判断一下空树的情况
****************************************/
bool Btree_Delete(Btree &root,keytype &x)
{
    Find_res ret=Btree_Search(root,x);
    if(ret.yes==0)                            //没有找到
        return 0;
    Btree p=ret.node ;
    Btree q;
    int i=ret.i;
    if (p->child[i]!=NULL)   //找到p最右边相邻的叶子节点交换并覆盖
    {
        q=p->child[i];
        while (q->child[0]!=NULL)
        {
            q=q->child[0];
        }
        p->key[i] = q->key[1];
        Btree_Left_Compress(q,1); // 覆盖位置1
        p=q;
    }
    else
    {
        Btree_Left_Compress(p,i); //叶节点直接删除
    }
    int mid =(mm+1)/2;
    while (1)
    {
        if (p==root||p->keynum>=mid-1)   //叶子节点富裕，可以直接删除
             break;//之前删了，所以退出
        else
        {
            q=p->parent;
            for (i=0;i<=q->keynum;i++)//找到p在其父亲节点的位置
                if(q->child[i]==p)
                break;
            if (i==0) //最左端 ，右调整
                Btree_Right_Adjust(p, q,i);
            else
                Btree_Left_Adjust(p,q,i);
            p=q;//节点上移
        }
    }
    if(root->keynum==0) //空树的情况
    {
        p=root->child[0];
        delete root;
           root =p;
        if(root!=NULL)
            root->parent=NULL;
    }
        return 1 ;
}
void Btree_Right_Adjust(Btree &p,Btree &q,int i)
{
    Btree pr=q->child[i+1];
    if(pr->keynum>=(mm+1)/2)  // 兄弟节点有足够多的节点
    {
        p->keynum++;
        p->key[p->keynum]=q->key[i+1];   // 父亲节点下移
        p->child[p->keynum]=pr->child[0];
        if (p->child[p->keynum]!=NULL)
        {
            p->child[p->keynum]->parent=p;  //修改子孩子的父节点原为 pr
        }
        q->key[i+1]=pr->key[1];
        pr->child[0]=pr->child[1];
        Btree_Left_Compress(pr,1); //pr 节点前移
    }
    else
        Btree_Merge_Bnode(p,q,pr,i+1); //没有节点可以借
}
void Btree_Left_Adjust(Btree &p,Btree &q,int i)  // -------------------------???
{
        Btree pl=q->child[i-1];
        if (pl->keynum>=(mm+1)/2)//节点满的情况
        {
            BTree_Right_Compress(p,1);
            p->child[1]=p->child[0];
            p->key[1]=q->key[i]; //父亲节点下移
            p->child[0]=pl->child[pl->keynum];
            if (p->child[0]!=NULL)
                p->child[0]->parent=p;
            q->key[i]=pl->key[pl->keynum];
            pl->keynum--;
        }
        else
        {
            // 左右交换，衣服合并函数的参数要求
            Btree pr=p;
            p=pl;
            Btree_Merge_Bnode(p,q,pr,i);    ///没有节点可以借
        }
}
void Btree_Left_Compress(Btree &p,int i) // 覆盖位置
{
    int n=p->keynum;
    for (int j=i;j<n;j++)
    {
        p->child[j]=p->child[j+1];
        p->key[j]=p->key[j+1];
    }
    p->keynum--;
}
void BTree_Right_Compress(Btree &p,int i)
{
    for(int j=p->keynum;j>=i;j--)
    {
        p->child[j+1]=p->child[j];
        p->key[j+1]=p->key[j];
    }
    p->keynum++;
}
void Btree_Merge_Bnode(Btree &p,Btree &q,Btree &pr,int i)
{
    int n=p->keynum+1;
    p->key[n]=q->key[i];
    p->child[n]=pr->child[0];
    for(int j=1;j<=pr->keynum;j++)
    {
        p->key[n+j]=pr->key[j];
        p->child[n+j]=pr->child[j];
    }
    if (p->child[0])
    {
        for (int j=0;j<=pr->keynum;j++)
            p->child[n+j]->parent=p;
    }
    Btree_Left_Compress(q,i);
    p->keynum=p->keynum+1+pr->keynum;
    delete pr;
    pr=NULL;
}
void Btree_show(Btree &root,short x=34)
{

     if (root)
     {
        int i;
         cout<<endl;
         for (i=0;i<x;i++)
            cout<<" ";
            cout<<"(";
         for (i=1;i<root->keynum;i++)
         {
            cout<<root->key[i]<<",";
         }
         cout<<root->key[i];
         cout<<")";
         for (i=0;i<=root->keynum;i++)
             Btree_show(root->child[i],x+8);
     }
}
