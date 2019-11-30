#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stack>
using namespace std;

static pthread_mutex_t sem;
static pthread_cond_t myturn = PTHREAD_COND_INITIALIZER;

struct data{
    string str;
    string temporary;
    int n[100];
    string message[100];
};

void *child_thread(void *arg){

    pthread_mutex_lock(&sem);
    static int turn = 1;
    static int current = 0;
    struct data *pos2 = (struct data *)arg;
    static int x;
    x = pos2->n[current];
    while(x != turn)
        pthread_cond_wait(&myturn, &sem);
    //cout << "I am thread #" << x << endl;
    string temp;
    string msg;
    char symbol;
    //cout << "Message here: " << pos2->str << endl;
    temp = pos2->message[current];
    symbol = temp[0];
    //cout << "Symbol : " << symbol << endl;
    for(int i =2 ; i< temp.size(); i++)
        msg += temp[i];
    // cout << "Binary : ";
    // cout << msg << endl;
    pthread_mutex_unlock(&sem);

    pthread_mutex_lock(&sem);
    //cout << "Thread #" << turn << " ended" << endl;
    for(int i =0; i < msg.size(); i++){
        if(msg[i] == '1')
            msg[i] = symbol;
    }

   //cout << "MSG: " << msg << endl;
   for(int i =0 ; i < pos2->str.size(); i++){
       
       for(int j =0 ; j < msg.size(); j++)
            if(msg[j] == '0'){
               msg[j] = pos2->str[i];
               i++;
            }

   }
    pos2->str = msg;

    string str1 = pos2->message[current];
    string str2 = " Binary code = ";
    str1.insert(1,str2);
    cout << str1 << endl;

    current++;
    turn++;
    pthread_cond_broadcast(&myturn);
    pthread_mutex_unlock(&sem);
    return NULL;
}

int main(){

    struct data pos;
    stack<string> str_stack;
    string input;

    while(getline(cin, input)){
        str_stack.push(input);
    }
   
    int NTHREADS = str_stack.size();
    pthread_t tid[NTHREADS];
    pthread_mutex_init(&sem, NULL); 

    for(int i =0; i < NTHREADS;++i)
    {
        pos.n[i] = i+1;
        pos.message[i] = str_stack.top();
        if(pthread_create(&tid[i], NULL,child_thread,(void *)&pos))
        {   
            fprintf(stderr,"Error creating thread\n");
            return 1;
        }
        str_stack.pop();
    }

    /*Wait for the other threads to finish*/
    for(int i =0; i < NTHREADS; i++)
        pthread_join(tid[i], NULL);

    /*prinT out the final decompressed message*/
    cout << "Decompressed file contents:" << endl;
    cout << pos.str << endl;

    return 0; 
}