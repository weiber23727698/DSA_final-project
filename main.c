#include "api.h"
#include <stdio.h>
#define base 19997
// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.
FILE *fp;
typedef struct Node node;
struct Node{
    int l,h2;
    //char *s;
    node *n;
}***table;
int n_mails, n_queries,**size;
mail *mails;
query *queries;
int find(int mid,char *s,int l,int r);
void init(){
    int i;
    table=(node***)malloc(sizeof(node**)*(n_mails+1));
    size=(int**)malloc(sizeof(int*)*(n_mails+1));
    for(i=0;i<=n_mails;++i){
        table[i]=(node**)malloc(sizeof(node*)*(base+1));
        size[i]=(int*)malloc(sizeof(int)*(base+1));
        size[i]=calloc(base+1,sizeof(int));
        /*for(int j=0;j<=base;++j)
            size[i][j]=0;*/
    }
}
int answer_array[1000000],ans_length;
node* create_node(int h2){
    node *newnode=(node*)malloc(sizeof(node));
    newnode->n=NULL;
    //newnode->l=(r-l+1);
    //newnode->s=(char*)malloc(sizeof(char)*(newnode->l+1));
    newnode->h2=h2;
    /*for(int i=l;i<=r;++i)
        newnode->s[i-l]=s[i];
    newnode->s[newnode->l]='\0';*/
    return newnode;
}
int len(char *s){
    int l=0;
    while(s[l]!='\0')
        ++l;
    return l;
}
int hash_char(char ch){
    if('0'<=ch&&ch<='9')
        return ((int)(ch-48)*(int)(ch-48))%26;
    else if(65<=ch&&ch<=90)
        return ((int)(ch-55)*(int)(ch-55))%26;
    else
        return ((int)(ch-87)*(int)(ch-87))%26;
}
int hash2_char(char ch){
    if('0'<=ch&&ch<='9')
        return (int)(ch-22);
    else if(65<=ch&&ch<=90)
        return ((int)(ch-65)*(int)(ch-65)+(int)(ch-65))%26;
    else
        return ((int)(ch-97)*(int)(ch-97)+(int)(ch-97))%26;
}
int hash(char *s,int l,int r){
    int h=0,h2=0;
    for(int i=l;i<=r;++i){
        h*=36;
        h+=hash_char(s[i]);
        h%=base;
        h2*=36;
        h2+=hash2_char(s[i]);
        h2%=base;
    }
    return h*base+h2;
}
/*int hash2(char *s,int l,int r){
    int ans=0;
    for(int i=l;i<=r;++i){
        ans*=36;
        ans+=hash2_char(s[i]);
        ans%=base;
    }
    return ans;
}*/
/*int issame(char *s1,char*s2,int l1,int r1,int l2,int r2){
    if(r1-l1!=r2-l2)
        return 0;
    for(int i=0;i<=r1-l1;++i){
        if(hash_char(s1[i+l1])!=hash_char(s2[i+l2]))
            return 0;
    }
    return 1;
}*/
void ins(int mid,int h,node *newnode){
    if(size[mid][h]>=1)
        return;
    newnode->n=table[mid][h];
    table[mid][h]=newnode;
    ++size[mid][h];
}
void make_table(int mid,char *s){
    int c=0,i=0,h=0,h2=0;
    while(s[i]!='\0'){
        if((s[i]>='0'&&s[i]<='9')||(65<=s[i]&&s[i]<=90)||(97<=s[i]&&s[i]<=122)){
            h*=36;
            h+=hash_char(s[i]);
            h%=base;
            h2*=36;
            h2+=hash2_char(s[i]);
            h2%=base;
            ++c;
        }
        else if(c){
            ins(mid,h,create_node(h2));
            h2=h=c=0;
        }
        ++i;
    }
    if(c)
        ins(mid,h,create_node(h2));
}
void build(){
    for(int i=0;i<n_mails;++i){
        make_table(mails[i].id,mails[i].content);
        //return;
        make_table(mails[i].id,mails[i].subject);
    }
}
/*int find(int mid,char *s,int l,int r){//token有沒有在mails[id]裡面
    int h=hash(s,l,r);
    node *c=table[mid][h];
    for(int i=1;i<=size[mid][h];++i){
        if(issame(c->s,s,0,c->l-1,l,r))
            return 1;
        c=c->n;
    }
    return 0;
}*/
int find2(int mid,int h,int h2){
    node *c=table[mid][h];
    for(int i=1;i<=size[mid][h];++i){
        if(c->h2==h2){
            return 1;
        }
        c=c->n;
    }
    return 0;
}
int prior(char ch){
    if(ch=='&')
        return 2;
    else if(ch=='|')
        return 1;
    else if(ch=='!')
        return 3;
    else
        return 5;
}
int eval(int mid,int *stack2,int c2){
    char stack3[2050],c3=0;
    //return 1;
    for(int i=1;i<=c2;++i){
        switch(stack2[i]){
            case('!'):
                stack3[c3]=!stack3[c3];
                break;
            case('&'):
                stack3[c3-1]&=stack3[c3];
                --c3;
                break;
            case('|'):;
                stack3[c3-1]|=stack3[c3];
                --c3;
                break;
            default:
                stack3[++c3]=find2(mid,stack2[i]/base,stack2[i]%base);
        }
    }
    //fprintf(fp,"\n");
    return (int)(stack3[1]);
}
void ans_expr(char ex[3000]){
    int stack1[2050],stack2[2050],stack3[2050];
    int c1=0,i=0,c2=0,i2,c3=0;
    while(ex[i]!='\0'){
        switch(ex[i]){
            case('!'):
                while(c1>0&&stack1[c1]!='('&&prior(stack1[c1])>=3)
                    stack2[++c2]=stack1[c1--];
                stack1[++c1]=ex[i++];
                break;
            case('&'):
                while(c1>0&&stack1[c1]!='('&&prior(stack1[c1])>=2)
                    stack2[++c2]=stack1[c1--];
                stack1[++c1]=ex[i++];
                break;
            case('|'):
                while(c1>0&&stack1[c1]!='('&&prior(stack1[c1])>=1)
                    stack2[++c2]=stack1[c1--];
                stack1[++c1]=ex[i++];
                break;
            case(')'):
                while(stack1[c1]!='(')
                    stack2[++c2]=stack1[c1--];
                ++i;
                c1--;
                break;
            case('('):
                stack1[++c1]=ex[i++];
                break;
            default:
                i2=i;
                while(ex[i+1]!='\0'&&((ex[i+1]>='0'&&ex[i+1]<='9')||('a'<=ex[i+1]&&ex[i+1]<='z')||('A'<=ex[i+1]&&ex[i+1]<='Z')))
                    ++i;
                stack2[++c2]=hash(ex,i2,i++);

        }
    }
    while(c1>0)
        stack2[++c2]=stack1[c1--];
    ans_length=0;
    //fprintf(fp,"\n");
    for(int j=0;j<n_mails;++j){
        if(eval(j,stack2,c2))
            answer_array[ans_length++]=j;
    }
}
int main(void) {
    //fp=fopen("輸出.txt","w");
	api.init(&n_mails, &n_queries, &mails, &queries);
	init();
	//api.answer(0,NULL,0);
	build();
    for(int i = 0; i <n_queries; i++)
		if(queries[i].type == expression_match){
            ans_expr(queries[i].data.expression_match_data.expression);
            api.answer(queries[i].id, answer_array, ans_length);
		}

  return 0;
}
