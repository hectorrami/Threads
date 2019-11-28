#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stack>
using namespace std;

static pthread_mutex_t sem;
static pthread_cond_t myturn = PTHREAD_COND_INITIALIZER;

struct data{
    int n[100];
    char symbol;
    string message[100];
    string Decompressed;
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
    
    
    string temp = pos2->message[current];
    string msg;
    char symbol = temp[0];


    cout << "Symbol : " << symbol << endl;
    cout << "Binary : ";
    for(int i =2 ; i< temp.size(); i++)
        msg += temp[i];

    cout << msg << endl;

    /*If statement to insert if there is a 0 at that position*/
   

    pthread_mutex_unlock(&sem);



    pthread_mutex_lock(&sem);
    //cout << "Thread #" << turn << " ended" << endl;


    for(int i =0; i < msg.size(); i++){
        if(msg[i] == '1')
            msg[i] = symbol;
    }

    cout << "MSG: " << msg << endl;

    string str1 = pos2->message[current];
    string str2 = " Binary code = ";
    str1.insert(1,str2);


    //cout << str1 << endl;
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
   
    int NTHREADS = 5;
    pthread_t tid[NTHREADS];
    pthread_mutex_init(&sem, NULL); 

    for(int i =0; i < 5;++i)
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
    for(int i =0; i < 5; i++)
        pthread_join(tid[i], NULL);

    cout << "Decompressed file components: " << endl;
    //cout << pos.Decompressed << endl;

    return 0; 
}