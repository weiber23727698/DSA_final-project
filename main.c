#include "api.h"
#include <stdio.h>
#define base 29999
// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.
//FILE *fp;
typedef struct Node node;
struct Node{
    long l,h2;
    //char *s;
    node *n;
}***table;
int n_mails, n_queries;
long **size;
mail *mails;
query *queries;
long find(long mid,char *s,long l,long r);
void init(){
    long i;
    table=(node***)malloc(sizeof(node**)*(n_mails+1));
    size=(long**)malloc(sizeof(long*)*(n_mails+1));
    for(i=0;i<=n_mails;++i){
        table[i]=(node**)malloc(sizeof(node*)*(base+1));
        size[i]=(long*)malloc(sizeof(long)*(base+1));
        for(long j=0;j<=base;++j)
            size[i][j]=0;
    }
}
int answer_array[1000000],ans_length;
node* create_node(long h2){
    node *newnode=(node*)malloc(sizeof(node));
    newnode->n=NULL;
    //newnode->l=(r-l+1);
    //newnode->s=(char*)malloc(sizeof(char)*(newnode->l+1));
    newnode->h2=h2;
    /*for(long i=l;i<=r;++i)
        newnode->s[i-l]=s[i];
    newnode->s[newnode->l]='\0';*/
    return newnode;
}
long len(char *s){
    long l=0;
    while(s[l]!='\0')
        ++l;
    return l;
}
long hash_char(char ch){
    /*if('0'<=ch&&ch<='9')
        return (long)(ch-'0');
    else if(65<=ch&&ch<=90)
        return (long)(ch-55);
    else
        return (long)(ch-87);*/
    if('0'<=ch&&ch<='9')
        return ((long)(ch-'0')*(long)(ch-'0'))%26;
    else if(65<=ch&&ch<=90)
        return ((long)(ch-55)*(long)(ch-55))%26;
    else
        return ((long)(ch-87)*(long)(ch-87))%26;
}
long hash2_char(char ch){
    if('0'<=ch&&ch<='9')
        return (long)(ch-22);
    else if(65<=ch&&ch<=90)
        return (((long)(ch-65))*((long)(ch-65))+(long)(ch-65))%26;
    else
        return (((long)(ch-97))*((long)(ch-97))+(long)(ch-97))%26;
}
long hash(char *s,long l,long r){
    long ans=0;
    for(long i=l;i<=r;++i){
        ans*=36;
        ans+=hash_char(s[i]);
        ans%=base;
    }
    /*if(ans<=0)
        system("pause");*/
    return ans;
}
long hash2(char *s,long l,long r){
    long ans=0;
    for(long i=l;i<=r;++i){
        ans*=36;
        ans+=hash2_char(s[i]);
        ans%=base;
    }
    /*if(ans<=0)
        system("pause");*/
    return ans;
}
/*long issame(char *s1,char*s2,long l1,long r1,long l2,long r2){
    if(r1-l1!=r2-l2)
        return 0;
    for(long i=0;i<=r1-l1;++i){
        if(hash_char(s1[i+l1])!=hash_char(s2[i+l2]))
            return 0;
    }
    return 1;
}*/
void ins(long mid,long h,node *newnode){
    /*if(!(0<=mid&&mid<=10000&))
       exit(0);*/
    //table[mid][h]=NULL;
    //if(size[mid][h]!=0)
        newnode->n=table[mid][h];
    table[mid][h]=newnode;
    ++size[mid][h];
}
void make_table(long mid,char *s){
    long c=0,i=0,h=0,h2=0;
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
            /*if(mid==4681&&issame(s,"Aira",i-c,i-1,0,3))
                fprlongf(fp,"here:%d\n",hash("Aira",0,3)==h);*/
            ins(mid,h,create_node(h2));
            /*f(mid==4681&&issame(s,"Aira",i-c,i-1,0,3)&&find(4681,"Aira",0,3))
                fprlongf(fp,"find:%d\n",find(4681,"Aira",0,3));*/
            h2=h=c=0;
        }
        ++i;
    }
    if(c)
        ins(mid,h,create_node(h2));
    /*if(mid>=4681&&!find(4681,"Aira",0,3))
            fprlongf(fp,"in make table here:%d\n",mid);*/
}
void build(){
    for(long i=0;i<n_mails;++i){
        make_table(mails[i].id,mails[i].content);
        //return;
        make_table(mails[i].id,mails[i].subject);
        /*if(i>=4681&&!find(4681,"Aira",0,3))
            fprlongf(fp,"here:%d\n",i);*/
    }
}
/*long find(long mid,char *s,long l,long r){//token有沒有在mails[id]裡面
    long h=hash(s,l,r);
    node *c=table[mid][h];
    for(long i=1;i<=size[mid][h];++i){
        if(issame(c->s,s,0,c->l-1,l,r))
            return 1;
        c=c->n;
    }
    return 0;
}*/
long find2(long mid,long h,long h2){
    node *c=table[mid][h];
    for(long i=1;i<=size[mid][h];++i){
        if(c->h2==h2){
            /*if(mid==781)
                fprlongf(fp,"%s %d\n",c->s,c->h2);*/
            return 1;
        }
        c=c->n;
    }
    return 0;
}
long prior(char ch){
    if(ch=='&')
        return 2;
    else if(ch=='|')
        return 1;
    else if(ch=='!')
        return 3;
    else
        return 5;
}
long eval(long mid,long *stack2,long c2){
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
    char stack3[2050],c3=0;
    //return 1;
    for(long i=1;i<=c2;++i){
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
        /*if(mid==781){
            for(long j=1;j<=c3;++j)
                fprlongf(fp,"%d",stack3[j]);
            fprlongf(fp,"\n");
        }*/
    }
    //fprlongf(fp,"\n");
    return (long)(stack3[1]);
}
void ans_expr(char ex[3000]){
    long stack1[2050],stack2[2050],stack3[2050];
    long c1=0,i=0,c2=0,i2,c3=0;
    while(ex[i]!='\0'){
        if((ex[i]>='0'&&ex[i]<='9')||('a'<=ex[i]&&ex[i]<='z')||('A'<=ex[i]&&ex[i]<='Z')){
            i2=i;
            while(ex[i+1]!='\0'&&((ex[i+1]>='0'&&ex[i+1]<='9')||('a'<=ex[i+1]&&ex[i+1]<='z')||('A'<=ex[i+1]&&ex[i+1]<='Z')))
                ++i;
            stack2[++c2]=hash(ex,i2,i)*base+hash2(ex,i2,i);
            /*for(long j=i2;j<=i;++j)
                fprlongf(fp,"%c",ex[j]);
            fprlongf(fp," ");
            fprlongf(fp,"%8d ",stack2[c2]);*/
            ++i;
        }
        else if(ex[i]=='!'){
            while(c1>0&&stack1[c1]!='('&&prior(stack1[c1])>=3)
                stack2[++c2]=stack1[c1--];
            stack1[++c1]=ex[i++];
        }
        else if(ex[i]=='&'){
            while(c1>0&&stack1[c1]!='('&&prior(stack1[c1])>=2)
                stack2[++c2]=stack1[c1--];
            stack1[++c1]=ex[i++];
        }
        else if(ex[i]=='|'){
            while(c1>0&&stack1[c1]!='('&&prior(stack1[c1])>=1)
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
    ans_length=0;
    //fprlongf(fp,"\n");
    for(long j=0;j<n_mails;++j){
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
	//while(1);
	/*ans_length=2;
	answer_array[0]=38;
	answer_array[1]=3;*/
	//for(long i = 0; i < n_queries; i++)
    for(long i = 0; i <n_queries; i++)
		if(queries[i].type == expression_match){
            ans_expr(queries[i].data.expression_match_data.expression);
            api.answer(queries[i].id, answer_array, ans_length);
		}
    /*ans_expr(queries[4].data.expression_match_data.expression);
            api.answer(queries[4].id, answer_array, ans_length);*/
    /*for(long i=0;i<ans_length;++i)
        fprlongf(fp,"%d ",answer_array[i]);*/
  return 0;
}
