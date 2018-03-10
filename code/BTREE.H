
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
Find_res Btree_Search(Btree &root,keytype &x); //���ҹؼ��� O(log2n)
bool Btree_Insert(Btree &root,keytype &x);  //�����������  // �߼���ϵ
bool Btree_Delete(Btree &root,keytype &x);// ɾ����������   //�߼���ϵ
void Btree_Insert_key(Btree &root,Btree &a,keytype &x,int i);//����ؼ���
void Btree_split(Btree &p, Btree &a,keytype &x,int i); //�ֳ������� //�������Ҫ����
void Btree_Right_Adjust(Btree &p,Btree &q,int i);    //����
void Btree_Left_Adjust(Btree &p,Btree &q,int i);//����
void Btree_Merge_Bnode(Btree &p,Btree &q,Btree &pr,int i);//�ϲ� p��pr�����ڵ�
void BTree_Right_Compress(Btree &p,int i); //�ӵ� I ��λ������ƶ�   //��
void Btree_Left_Compress(Btree &q,int i);//�ӵ�I��λ����ǰ�ƶ�       //������λ��i
//void Btree_show(Btree &root,short x=34);// �������ӡB��

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
����������������£�
    1�ж��Ƿ�Ϊ����.�Ƿ�Ϊ����
    2.�����Ƿ���ڴ˹ؼ�����Ϣ
    3.Ҷ���в���
        a. С��m-1 �ڿգ���� i
        b:  �ȷ��ѽڵ㣬�ݹ鴦����������м�ֵ���ƣ����� parentָ���������ƣ���
****************************/
bool Btree_Insert(Btree &root,keytype &x)
{
    if(root==NULL)
    {
        root=new Bnode(1,NULL);
        root->key[1]=x;
        return 1;                                //�Ѵ����Ȿ��
    }
    Find_res ret=Btree_Search(root,x);
    if (ret.yes==1)
        return 0;
    Btree p=ret.node;
    Btree q;
    Btree sub=NULL; // ���֣�sub�̳�p���Ұ벿��
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
            for (i=0;i<q->keynum;i++) //�ҵ��ýڵ��ڸ��׽ڵ��λ��
            {
                if (x<q->key[i+1])
                    break;
            }
            p=q;  //������һ��
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

void Btree_Insert_key(Btree &p,Btree &a,keytype &x,int i) //Ӧ����λ�� i+1
{
    for (int j=p->keynum; j>i ; j--) //�ڿ� i+1 ,keynum+1
    {
        p->key[j+1]=p->key[j];
        p->child[j+1]=p->child[j];
    }
    p->keynum++;
    p->key[i+1]=x;
    p->child[i+1]=a;
    if (a!=NULL)  //���¸��׽ڵ�
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
     if(q->child[0]!=NULL)  //���¸��ڵ�
     {
         for (int ii=0;ii<ssize;ii++)
            q->child[ii]->parent=q;
     }
     q->keynum=mm-mid;
     p->keynum=mid-1;
     //���ɣ�x,a������
     x=p->key[mid];
     a=q;
}
/****************************************
ɾ�������󲿷֣�ɾ����Ҷ�ڵ㣬��Ҷ�ڵ㣩������
1.ɾ����Ӧ�Ľڵ���Ϣ
    �����Ҷ����ֱ��ɾ��
    ������ڽڵ��ϣ��ҵ�p���ұ�����Ҷ�ӽڵ㽻����Ҷ�ӽڵ���Ϣ���и���
2.������ѭ����
    �����ɾ����Ϣ�����ڵĽڵ��Ǹ�ԣ��Ҷ�ӽڵ㣬����˵��������������˳�ѭ��
    ������Ҫȥ��ɾ���ڵ����丸�׽ڵ���������λ�ã������λ��0��λ�ã������ҵ���������λ�ý��������
    ���ҵ������·�Ϊ���������
        ����ֵܽڵ㸻ԣ�Ļ���ֱ�ӽ�����Ӧ�Ľڵ���Ϣ�������ɣ�����Ļ����ϲ������ڵ���Ϣ������һ���½ڵ�
    �ڵ�����,�������׽ڵ���Ϣ
        �ϲ������ڵ���Ϣ���ô�ĸ�С�ĵ���ʽ�����Զ����������ѡ����ʺϲ��ڵ���кϲ�����
3.�˳�ѭ���������ж�һ�¿��������
****************************************/
bool Btree_Delete(Btree &root,keytype &x)
{
    Find_res ret=Btree_Search(root,x);
    if(ret.yes==0)                            //û���ҵ�
        return 0;
    Btree p=ret.node ;
    Btree q;
    int i=ret.i;
    if (p->child[i]!=NULL)   //�ҵ�p���ұ����ڵ�Ҷ�ӽڵ㽻��������
    {
        q=p->child[i];
        while (q->child[0]!=NULL)
        {
            q=q->child[0];
        }
        p->key[i] = q->key[1];
        Btree_Left_Compress(q,1); // ����λ��1
        p=q;
    }
    else
    {
        Btree_Left_Compress(p,i); //Ҷ�ڵ�ֱ��ɾ��
    }
    int mid =(mm+1)/2;
    while (1)
    {
        if (p==root||p->keynum>=mid-1)   //Ҷ�ӽڵ㸻ԣ������ֱ��ɾ��
             break;//֮ǰɾ�ˣ������˳�
        else
        {
            q=p->parent;
            for (i=0;i<=q->keynum;i++)//�ҵ�p���丸�׽ڵ��λ��
                if(q->child[i]==p)
                break;
            if (i==0) //����� ���ҵ���
                Btree_Right_Adjust(p, q,i);
            else
                Btree_Left_Adjust(p,q,i);
            p=q;//�ڵ�����
        }
    }
    if(root->keynum==0) //���������
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
    if(pr->keynum>=(mm+1)/2)  // �ֵܽڵ����㹻��Ľڵ�
    {
        p->keynum++;
        p->key[p->keynum]=q->key[i+1];   // ���׽ڵ�����
        p->child[p->keynum]=pr->child[0];
        if (p->child[p->keynum]!=NULL)
        {
            p->child[p->keynum]->parent=p;  //�޸��Ӻ��ӵĸ��ڵ�ԭΪ pr
        }
        q->key[i+1]=pr->key[1];
        pr->child[0]=pr->child[1];
        Btree_Left_Compress(pr,1); //pr �ڵ�ǰ��
    }
    else
        Btree_Merge_Bnode(p,q,pr,i+1); //û�нڵ���Խ�
}
void Btree_Left_Adjust(Btree &p,Btree &q,int i)  // -------------------------???
{
        Btree pl=q->child[i-1];
        if (pl->keynum>=(mm+1)/2)//�ڵ��������
        {
            BTree_Right_Compress(p,1);
            p->child[1]=p->child[0];
            p->key[1]=q->key[i]; //���׽ڵ�����
            p->child[0]=pl->child[pl->keynum];
            if (p->child[0]!=NULL)
                p->child[0]->parent=p;
            q->key[i]=pl->key[pl->keynum];
            pl->keynum--;
        }
        else
        {
            // ���ҽ������·��ϲ������Ĳ���Ҫ��
            Btree pr=p;
            p=pl;
            Btree_Merge_Bnode(p,q,pr,i);    ///û�нڵ���Խ�
        }
}
void Btree_Left_Compress(Btree &p,int i) // ����λ��
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