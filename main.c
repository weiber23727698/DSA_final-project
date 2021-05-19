#include "api.h"
#include <stdio.h>
#define base 9997
// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.
typedef struct Node node;
struct Node{
    int l;
    char *s;
    node *n;
}***table;
int n_mails, n_queries,**size;
mail *mails;
query *queries;
void init(){
    int i;
    table=(node***)malloc(sizeof(node**)*(n_mails+1));
    size=(int**)malloc(sizeof(int*)*(n_mails+1));
    for(i=0;i<=n_mails;++i){
        table[i]=(node**)malloc(sizeof(node*)*10001);
        size[i]=(int*)malloc(sizeof(int)*10001);
        for(int j=0;j<=10000;++j)
            size[i][j]=0;
    }
}
int answer_array[1000000],ans_length;
node* create_node(char *s,int l,int r){
    node *newnode=(node*)malloc(sizeof(node));
    newnode->n=NULL;
    newnode->l=(r-l+1);
    newnode->s=(char*)malloc(sizeof(char)*(newnode->l+1));
    for(int i=l;i<=r;++i)
        newnode->s[i-l]=s[i];
    newnode->s[newnode->l]='\0';
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
        return (int)(ch-'0');
    else if(65<=ch&&ch<=90)
        return (int)(ch-55);
    else
        return (int)(ch-87);
}
int hash(char *s,int l,int r){
    int ans=0;
    for(int i=l;i<=r;++i){
        ans*=36;
        ans+=hash_char(s[i]);
        ans%=base;
    }
    /*if(ans<=0)
        system("pause");*/
    return ans;
}
int issame(char *s1,char*s2,int l1,int r1,int l2,int r2){
    if(r1-l1!=r2-l2)
        return 0;
    for(int i=0;i<=r1-l1;++i){
        if(hash_char(s1[i+l1])!=hash_char(s2[i+l2]))
            return 0;
    }
    return 1;
}
void ins(int mid,int h,node *newnode){
    /*if(!(0<=mid&&mid<=10000&))
       exit(0);*/
    if(!(0<=h))
        exit(0);
    table[mid][h]=NULL;
    if(size[mid][h]!=0)
        newnode->n=table[mid][h];
    table[mid][h]=newnode;
    ++size[mid][h];
}
void make_table(int mid,char *s){
    int c=0,i=0,h=0;
    while(s[i]!='\0'){
        if((s[i]>='0'&&s[i]<='9')||(65<=s[i]&&s[i]<=90)||(97<=s[i]&&s[i]<=122)){
            h*=36;
            h+=hash_char(s[i]);
            h%=base;
            ++c;
        }
        else if(c){
            ins(mid,h,create_node(s,i-c,i-1));
            h=c=0;
        }
        ++i;
    }
    if(c)
        ins(mid,h,create_node(s,i-c,i-1));
}
void build(){
    for(int i=0;i<n_mails;++i){
        make_table(mails[i].id,mails[i].content);
        //return;
        make_table(mails[i].id,mails[i].subject);
    }
}
int find(int mid,char *s,int l,int r){//token有沒有在mails[id]裡面
    int h=hash(s,l,r);
    node *c=table[mid][h];
    for(int i=1;i<=size[mid][h];++i){
        if(issame(c->s,s,0,c->l-1,l,r))
            return 1;
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
int eval(char *ex,int mid){
    /*If (encounter token)
        push to final result
    Else if (encounter operator)
        While(precedence of operator < top of stack)
            pop top of stack to final result
        push operator to stack
    Else if (encounter ‘(‘ )
        Push to stack
    Else if (encounter ‘)’)
        While(top of stack != ‘(‘)
            Pop top of stack to final result
                Discard both brackets*/
    char stack1[2050],stack2[2050],stack3[2050];
    int c1=0,i=0,c2=0,i2,c3=0;
    while(ex[i]!='\0'){
        if((ex[i]>='0'&&ex[i]<='9')||('a'<=ex[i]&&ex[i]<='z')||('A'<=ex[i]&&ex[i]<='Z')){
            i2=i;
            while(ex[i+1]!='\0'&&((ex[i+1]>='0'&&ex[i+1]<='9')||('a'<=ex[i+1]&&ex[i+1]<='z')||('A'<=ex[i+1]&&ex[i+1]<='Z')))
                ++i;
            stack2[++c2]=find(mid,ex,i2,i++);
        }
        else if(ex[i]=='!'){
            while(c1>0&&prior(stack1[c1])>=3&&stack1[c1]!='(')
                stack2[++c2]=stack1[c1--];
            stack1[++c1]=ex[i++];
        }
        else if(ex[i]=='&'){
            while(c1>0&&prior(stack1[c1])>=2&&stack1[c1]!='(')
                stack2[++c2]=stack1[c1--];
            stack1[++c1]=ex[i++];
        }
        else if(ex[i]=='|'){
            while(c1>0&&prior(stack1[c1])>=1&&stack1[c1]!='(')
                stack2[++c2]=stack1[c1--];
            stack1[++c1]=ex[i++];
        }
        else if(ex[i]==')'){
            while(stack1[c1]!='(')
                stack2[++c2]=stack1[c1--];
            ++i;
            c1--;
        }
        else{
            stack1[++c1]=ex[i++];
        }
    }
    while(c1>0)
        stack2[++c2]=stack1[c1--];
    //return 1;
    for(i=1;i<=c2;++i){
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
                stack3[++c3]=stack2[i];
        }
    }
    return (int)(stack3[1]);
}
void ans_expr(char s[3000]){
    ans_length=0;
    for(int i=0;i<n_mails;++i){
        if(eval(s,i))
            answer_array[ans_length++]=i;
    }
}
int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);
	init();
	//api.answer(0,NULL,0);
	build();
	//while(1);
	/*ans_length=2;
	answer_array[0]=38;
	answer_array[1]=3;*/
	//for(int i = 0; i < n_queries; i++)
    for(int i = 0; i < n_queries; i++)
        /*if(queries[i].type == expression_match)
            api.answer(i, NULL,0);*/
        //api.answer(queries[i].id, answer_array, ans_length);
		if(queries[i].type == expression_match){
            ans_expr(queries[i].data.expression_match_data.expression);
            api.answer(queries[i].id, answer_array, ans_length);
		}
  return 0;
}
